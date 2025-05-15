//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2005 Stefan Dicu & Tudor Girba                         //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License as     //
// published by the Free Software Foundation; either version 2.1 of the      //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License  //
// for more details.                                                         //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

#pragma hdrstop

#include <algorithm>
using namespace std;

#include "PPIG_SubSysBuilder.h"
#include "PPIG_ObjFactory.h"
#include "PPIG_ObjInfantry.h"
#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "PPIG_BGManager.h"
#include "basepath.h"
//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CIGSubSysBuilder::CIGSubSysBuilder(CIGFactoryObject* _parent) : CIGSubSysBase(_parent)
{
  // builder specific vaiables
  mPercentComplete = 0;

  // set update counters
  GetUpdateCounters(OA_BUILD)           .GetThis()->SetMaxValue(2); // this will be updated by the turret turn rate

  // action step counters
  GetActionStepCounters(OA_BUILD)         .SetMaxValue(1);

  // action anim types (the line in sprite) - not necessary
  SetActionAnimTypes(OA_BUILD, 0);
}
//---------------------------------------------------------------------------

CIGSubSysBuilder::~CIGSubSysBuilder()
{
}
//---------------------------------------------------------------------------

void CIGSubSysBuilder::Update()
{
  CIGSubSysBase::Update();
}
//---------------------------------------------------------------------------

CIGSubSysAction* CIGSubSysBuilder::GetActionInstance(TIGSubSysActionData actionData)
{
   CIGSubSysAction *lTmpAction = NULL;
   CIGFactoryObject* lParent = (CIGFactoryObject*)GetParent();
   bool lIsDead = lParent->GetIsDead();

   switch(actionData.mType)
   {
      case OA_BUILD:
         if (lIsDead == false)
         {
           lTmpAction = new CIGSubSysActionBuild(this, actionData);
         }
         break;
   }

   return lTmpAction;
}
//---------------------------------------------------------------------------

void CIGSubSysBuilder::OnActionBuild()
{
  mPercentComplete++;

  if (mPercentComplete > 100)
  {
    mPercentComplete = 0;

    // drop a soldier in the map

    // check if the index in the file exists and load the record
    TInfantryUnified lInfantryData;

    if (!getAIInfRecord((getGamePath() + CPString(PATH_AIINFANTRY) + CPString(FILE_AIINFANTRY)).c_str(), 0, &lInfantryData))
    {
      return;
    }

    // now create the infantry unit
    CIGInfantryUnit* lUnit = 0;

    try
    {
      lUnit = new CIGInfantryUnit();
    }
    catch(...)
    {
      LOG(FATAL) << "CIGSubSysBuilder::OnActionBuild - cannot create unit.";
      return;
    }


    int lX, lY;
    bool lPlaced = false;

    int lStartX = max(GetParent()->GetLogicCoord().x - 1, 0);
    int lStartY = max(GetParent()->GetLogicCoord().y - 1, 0);
    int lEndX   = min(lStartX + GetParent()->GetLogicDimX() + 1, GetMissionInstance()->GetMap()->GetLogicWidth());
    int lEndY   = min(lStartY + GetParent()->GetLogicDimY() + 1, GetMissionInstance()->GetMap()->GetLogicHeight());

    for (int i = lStartX; i < lEndX; i++)
    {
        for (int j = lStartY; j < lEndY; j++)
        {
          if (lUnit->CanBePlaced(CPPoint(i, j)))
          {
            lX = i;
            lY = j;
            lPlaced = true;
            break;
          }
        }
    }

    // the unit cannot be placed, return
    if (!lPlaced)
    {
      delete lUnit;
      return;
    }

    int lUnitId = GetMissionInstance()->GetUniqueUnitId();

    lUnit->SetCoord(CPPoint(lX * MAPCELLX, lY * MAPCELLY));
    lUnit->SetInfUnifiedData(lInfantryData);
    lUnit->SetId(lUnitId);
    lUnit->SetBattleGroupId(GetParent()->GetBattleGroupId());
    lUnit->SetSelected(false);
    //lUnit->SetScriptObserver( GetMissionInstance()->GetDoctrine()->AddScriptFromFile(lUnit->GetScriptObserver(), "" ) );

    CPSprite* lSprite = GetGraphicInstance()->AddSprite(CPString(PATH_INF_GFX) + lInfantryData.sprite, 8, 14, SPRL_MISSION);
    lUnit->SetSprite(lSprite);

    // now add the unit in the mission structures
    GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Add(lUnit);
    GetMissionInstance()->GetBGManager()->AddUnit(lUnit);

    switch (GetMissionInstance()->GetBGManager()->GetRelation(OS_PLAYER, lUnit->GetBattleGroupId()))
    {
      case REL_NEUTRAL:
        GetMissionInstance()->GetObjects(MOL_NEUTRALUNITS)->Add(lUnit);
        break;
      case REL_OWNED:
        GetMissionInstance()->GetObjects(MOL_OWNEDUNITS)->Add(lUnit);
        break;
      case REL_FRIENDLY:
        GetMissionInstance()->GetObjects(MOL_FRIENDLYUNITS)->Add(lUnit);
        break;
      case REL_ENEMY:
        GetMissionInstance()->GetObjects(MOL_ENEMYUNITS)->Add(lUnit);
        break;
      default:
        // do nothing
        LOG(ERROR) << "CPPIG_MissionControl::mcInsertInfantryAtPoint - Infantry relation to player unknown.";
        break;
    }

    // notify the unit in the maps
    lUnit->NoteInLayerMap();
    lUnit->NoteInLogicMap();

    // issue a move order
    TIGSubSysActionData lTmpActionData;

    lTmpActionData.mType        = OA_MOVE;
    lTmpActionData.mTargetPoint = (dynamic_cast<CIGFactoryObject*>(GetParent()))->GetRallyPoint();

    lUnit->SetAction(lTmpActionData);

    // end the action
    SetCurrentActionIndex(OA_STOP);
    GetCurrentAction()->End();
  }
}
//---------------------------------------------------------------------------

void CIGSubSysBuilder::Serialize(PP::Stream& _a)
{
}
//---------------------------------------------------------------------------

unsigned __int32 CIGSubSysBuilder::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void CIGSubSysBuilder::DeSerialize(PP::Stream& _a)
{
}
//---------------------------------------------------------------------------

