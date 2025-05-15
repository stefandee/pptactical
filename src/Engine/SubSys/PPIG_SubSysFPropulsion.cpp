//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998 - 2003 Stefan Dicu & Tudor Girba                       //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License as     //
// published by the Free Software Foundation; either version 2.1 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License  //
// for more details.                                                         //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

#include <math.h>
#pragma hdrstop

#include "PPIG_SubSysFPropulsion.h"
#include "PPIG_ObjFighter.h"
#include "PPIG_SubSysAction.h"
#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "basepath.h"
#include "Paths.h"
#include "PPIG_SpaceSound.h"
#include "Equipment.h"
#include "PPIG_EngineDef.h"
#include "PPIG_SubSysVisual.h"
#include "basic3d.h"
#include "PPIG_BGManager.h"

CIGSubSysFPropulsion::CIGSubSysFPropulsion(CIGFighterUnit * parent) : CIGSubSysBase(parent)
{
    VLOG(9) << "CIGSubSysFPropulsion::CIGSubSysFPropulsion";
    //Action step counter
    GetActionStepCounters(OA_STOP)         .SetMaxValue(0);
    GetActionStepCounters(OA_MOVE)         .SetMaxValue(0);
    GetActionStepCounters(OA_ATTACKMOVE)   .SetMaxValue(0);
    GetActionStepCounters(OA_PATROL)       .SetMaxValue(0);
    GetActionStepCounters(OA_HOLD)         .SetMaxValue(0);
    GetActionStepCounters(OA_DIE)          .SetMaxValue(0);
    GetActionStepCounters(OA_DEAD)         .SetMaxValue(0);
    //------------------

    //Action update settings
    GetUpdateCounters(OA_MOVE)             .SetMaxValue(1);
    GetUpdateCounters(OA_ATTACKMOVE)       .SetMaxValue(1);
    GetUpdateCounters(OA_PATROL)           .SetMaxValue(1);
    GetUpdateCounters(OA_DIE)              .SetMaxValue(20);
    GetUpdateCounters(OA_DEAD)             .SetMaxValue(8192);

    //------------------
    //Action anim types
    SetActionAnimTypes(OA_STOP, 0);
    SetActionAnimTypes(OA_MOVE, 0);
    SetActionAnimTypes(OA_ATTACKMOVE, 0);
    SetActionAnimTypes(OA_PATROL, 0);
    SetActionAnimTypes(OA_HOLD, 0);
    SetActionAnimTypes(OA_GUARD, 0);
    SetActionAnimTypes(OA_DIE, 0);
    SetActionAnimTypes(OA_DEAD, 0);
    //------------------

    // default values (setting sprite for parrent may override these values)
    // 8 columns in sprite/8 directions
    GetSenseCounter().SetMaxValue(7);
    GetAnimCounter().SetMaxValue(7);
    mTimer.SetMaxValue(4);
    m_Flag = 0;
}
//-------------------------------------------------------------------------------------------------

CIGSubSysFPropulsion::~CIGSubSysFPropulsion()
{
}
//-------------------------------------------------------------------------------------------------

CIGSubSysAction* CIGSubSysFPropulsion::GetActionInstance(TIGSubSysActionData actionData)
{
    bool IsDead = GetParent()->GetIsDead();

    switch(actionData.mType)
    {
        case OA_STOP:
            if (!IsDead)
                return new CIGSubSysActionStop(this, actionData);
            break;

        case OA_MOVE:
            if (!IsDead)
            {
              m_Flag = MOVING;
              return new CIGSubSysActionMove(this, actionData);
            }
            break;

        case OA_PATROL:
            if (!IsDead)
                return new CIGSubSysActionPatrol(this, actionData);
            break;

        case OA_GUARD:
        case OA_HOLD:
           break;

        case OA_DIE:
            if (!IsDead)
                return new CIGSubSysActionDie(this, actionData);
            break;

        case OA_DEAD:
                return new CIGSubSysActionDead(this, actionData);
        default:
            break;
    }

    return NULL;
}
//-------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------
// description: stops the aircraft
// others     : the aircraft stops, then the current position becomes
//              the hovering point (the aircraft will rotate around this point)
//---------------------------------------------------------------------------
void CIGSubSysFPropulsion::OnActionStop()
{
}
//-------------------------------------------------------------------------------------------------

void CIGSubSysFPropulsion::OnActionMove()
{
  VLOG(9) << "CIGSubSysFPropulsion::OnActionMove";

  CIGAircraft *Parent     = static_cast<CIGAircraft*>(GetParent());
  CPPoint     TargetPoint = GetCurrentAction()->GetData().mTargetPoint;

  //int Sence = gfGetComplexDirFromPoints(Parent->GetCoord(), CPPoint(TargetPoint.x * MAPCELLX, TargetPoint.y * MAPCELLY));
  int Sence = gfGetDirFromPoints(Parent->GetCoord(), CPPoint(TargetPoint.x * MAPCELLX, TargetPoint.y * MAPCELLY));

  SetSenseCounter(Sence);
  //GetSenseCounter().GetThis()->SetCounter(Sence);

  mTimer.Inc();

  if (mTimer.MaxReached())
  {
    SetAnimCounter(Sence);
  }

  Move();

  //logOut << TargetPoint.x << "," << TargetPoint.y << "\n";

  TPos3D Pt1, Pt2;
  Pt1.x   = Parent->GetCoord().x;
  Pt1.y   = Parent->GetCoord().y;
  Pt1.z   = 0;

  Pt2.x   = TargetPoint.x * MAPCELLX;
  Pt2.y   = TargetPoint.y * MAPCELLY;
  Pt2.z   = 0;

  if (Dist3D(Pt1, Pt2) < Parent->GetSpeed())
  {
    //m_Flag = 0;
    ComputeMoveParameters();
  }
}
//-------------------------------------------------------------------------------------------------

void CIGSubSysFPropulsion::OnActionPatrol()
{
    //OnActionMove();
}
//-------------------------------------------------------------------------------------------------

void CIGSubSysFPropulsion::OnActionHold()
{
    //TODO
}
//-------------------------------------------------------------------------------------------------

void CIGSubSysFPropulsion::OnActionGuard()
{
    //TODO
}
//-------------------------------------------------------------------------------------------------

void CIGSubSysFPropulsion::OnActionDead()
{
    CIGFighterUnit * Parent = static_cast<CIGFighterUnit*>(GetParent());
    if (GetActionStepCounters(OA_DEAD).MaxReached())
    {
        GetMissionInstance()->AddToDeleteObjects(Parent);
        GetMissionInstance()->GetBGManager()->RemoveUnit(Parent);
    }
}
//-------------------------------------------------------------------------------------------------

void CIGSubSysFPropulsion::OnActionDie()
{
  GetParent()->RemoveFromLayerMap();
}
//-------------------------------------------------------------------------------------------------

void CIGSubSysFPropulsion::Move()
{

   CIGFighterUnit * Parent = static_cast<CIGFighterUnit*>(GetParent());
   CPPoint ParentPt        = Parent->GetCoord();

   Parent->RemoveFromLayerMap();

   Parent->SetCoord(CPPoint(ParentPt.x + Parent->GetSpeed() * gDir[GetSenseCounter()()].x, ParentPt.y + Parent->GetSpeed()*gDir[GetSenseCounter()()].y));
   //Parent->SetCoord(CPPoint(ParentPt.x + (float)Parent->GetSpeed() * cos(gComplexDirAngle[GetSenseCounter()]),
   //                         ParentPt.y + (float)Parent->GetSpeed() * sin(gComplexDirAngle[GetSenseCounter()])
   //                         ));

   Parent->NoteInLayerMap();
}
//-------------------------------------------------------------------------------------------------

void CIGSubSysFPropulsion::ComputeMoveParameters()
{
    // fighter can't stop in the air, it has to hover around in circles, otherwise
    // it falls on the ground and the pilot dies...
    const float xRadius = MAPCELLX * AIRCRAFT_PROPULSION_RADIUS;
    const float yRadius = MAPCELLY * AIRCRAFT_PROPULSION_RADIUS;

    CIGFighterUnit *Parent = static_cast<CIGFighterUnit*>(GetParent());
    if (m_Flag != HOVERING)
    {
        m_StopPt = Parent->GetCoord();
        m_Flag = HOVERING;
        //m_Angle = 0;
        //decrease speed to cruise speed.
        //TODO: Must increase it when we move again
        //Parent->SetSpeed(Parent->GetSpeed().Dec(4));
    }
    else
    {
      // the angle increase should be dynamic
      m_Angle = m_Angle + PI / 8;
    }

    CPPoint NewTarget;

    NewTarget.x = (m_StopPt.x + xRadius * cos(m_Angle)) / MAPCELLX;
    NewTarget.y = (m_StopPt.y + yRadius * sin(m_Angle)) / MAPCELLY;

    GetCurrentAction()->SetTargetPoint(NewTarget);
}
//-------------------------------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: modify the sense to the wanted sense
 parameters : wantedSense - the wanted sense
 others     : this should be overwritten
---------------------------------------------------------------------------*/
void CIGSubSysFPropulsion::TurnToSense(int wantedSense)
{
  SetSenseCounter(GetSenseCounter().ToValue(wantedSense)());
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: SenseCounter
---------------------------------------------------------------------------*/
void CIGSubSysFPropulsion::SetSenseCounter(int value)
{
  CIGFighterUnit* lParent = (CIGFighterUnit*)GetParent();

  CIGSubSysBase::SetSenseCounter(value);

  if (mTimer.MaxReached())
  {
    SetAnimCounter(value);
  }

  int lTmpDir = gDirAngle[GetSenseCounter()] * 180.0 / PI;

  if (lParent->GetLookAt() == UL_NOTHING && !(lTmpDir > lParent->GetScanAngle() - 4 && lTmpDir < lParent->GetScanAngle() + 4))
  {
    lParent->SetScanRange(20);
    lParent->SetScanAngle(lTmpDir);
  }
}
//---------------------------------------------------------------------------

void CIGSubSysFPropulsion::SetSenseCounter(CPCounter &value)
{
  CIGFighterUnit* lParent = (CIGFighterUnit*)GetParent();

  CIGSubSysBase::SetSenseCounter(value);

  if (mTimer.MaxReached())
  {
    SetAnimCounter(value);
  }

  int lTmpDir = gDirAngle[GetSenseCounter()] * 180.0 / PI;

  if (lParent->GetLookAt() == UL_NOTHING && !(lTmpDir > lParent->GetScanAngle() - 4 && lTmpDir < lParent->GetScanAngle() + 4))
  {
    lParent->SetScanRange(20);
    lParent->SetScanAngle(lTmpDir);
  }
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
