//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
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
/*---------------------------------------------------------------------------
 application: Pure Power - in game

 description: Class Mission
              it manages all the objects and provides interface for user
              commands
 last modify: 10 06 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifdef __WIN32__
#include <io.h>
#endif

#if defined(__SUNPRO_CC) || defined(__GNUG__)
#include <sys/types.h>
#include <unistd.h>
#endif

#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <algorithm>
#pragma hdrstop

using namespace std;

//#include "pmvcpp.h"
//using namespace PureMVC;

#ifdef PP_FULL_VERSION
#include "Universe.h"
#endif

#include "config.h"
#include "logmethod.h"
#include "PPIG_Mission.h"
#include "PP_Stdlib.h"
#include "PPIG_MapData.h"
#include "PP_Graphic.h"
#include "PP_Sprite.h"
#include "PP_SpriteList.h"
//#include "PPIG_CScreen.h"
#include "PPIG_ObjEffect.h"
#include "PPIG_ObjInfantry.h"
#include "PPIG_ObjStatic.h"
#include "PPIG_ObjDropship.h"
#include "PPIG_ObjTank.h"
#include "PPIG_ObjRecon.h"
#include "PPIG_ObjTran.h"
#include "PPIG_ObjChopper.h"
#include "PPIG_ObjFighter.h"
#include "PPIG_ObjArtillery.h"
#include "PPIG_ObjRocket.h"
#include "PPIG_ObjLaser.h"
#include "PPIG_ObjBullet.h"
#include "PPIG_ObjBallistic.h"
#include "PPIG_ObjDamage.h"
#include "PPIG_ObjBoids.h"
#include "PPIG_MissionControl.h"
#include "PPIG_Map.h"
#include "AIUnits.h"       //unified infantry struct
#include "Statics.h"       //static struct
#include "Paths.h"
#include "basepath.h"
#include "PP_Timer.h"
#include "PPIG_ObjectCreator.h"
#include "PPIG_SerObjList.h"
#include "PPIG_BGManager.h"
#include "EngineConfig.h"
#include "PPIG_TransportRules.h"
#include "PP_Stdlib.h"

#ifdef _MSC_VER
#define for if(0) {} else for
#endif

CIGMission *CIGMission::mInstance = NULL;

/*---------------------------------------------------------------------------
 description: provides the single instance possible for the mission
 others     : it's a global function
---------------------------------------------------------------------------*/
CIGMission *GetMissionInstance()
{
    return CIGMission::Instance();
    /*
    CIGMission *lMI;
    return lMI->Instance();
    */
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: provides the single instance possible
---------------------------------------------------------------------------*/
CIGMission *CIGMission::Instance()
{
    if (mInstance == NULL)
    {
        mInstance = new CIGMission();
    }
    return mInstance;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: releases the instance
---------------------------------------------------------------------------*/
void CIGMission::Release()
{
    if (mInstance != NULL)
    {
        delete mInstance;
        mInstance = NULL;
    }
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CIGMission::CIGMission()
{
    try
    {
        mMap            = new CIGMap();
    }
    catch(...)
    {
        LOG(FATAL) << "UNABLE TO ALLOCATE Map !!!!!!!!!!";
        throw CErrorGeneric();
    }

    try
    {
        mMissionControl = new CPPIG_MissionControl();
    }
    catch(...)
    {
        LOG(FATAL) << "UNABLE TO ALLOCATE MissionControl !!!!!!!!!!";
        throw CErrorGeneric();
    }

    try
    {
        mDoctrine       = new CPPIG_Doctrine();
    }
    catch(...)
    {
        LOG(FATAL) << "UNABLE TO ALLOCATE Doctrine !!!!!!!!!!";
        throw CErrorGeneric();
    }

    try
    {
        mBGManager      = new CPPIG_BGManager();
    }
    catch(...)
    {
        LOG(FATAL) << "UNABLE TO ALLOCATE BattleGroup Manager !!!!!!!!!!";
        throw CErrorGeneric();
    }

    for (int i=0; i<MISSION_OBJECT_LISTS_COUNT; i++)
        mObjects[i] = new CIGBaseObjectList();
    mToDeleteObjects = new CIGBaseObjectList();
    for (int i=0; i<MISSION_FLAGS_COUNT; i++)
        mFlags[i] = false;
    mUpdateCounter  = 0;
    mUpdateCounter.SetMaxValue(0);
    mSelectionMask  = 0;
    mAction         = MA_DEFAULT;
    mCurrentAction  = MA_DEFAULT;
    mBeginPoint     = CPPoint(-1, -1);
    mCurrentPoint   = CPPoint(-1, -1);
    mEnabled        = true;

    // others init
    mMissionTime = 0;
    mTicks       = GetTimerInstance()->GetTime();
    mFocusObj    = 0;

    mNeutralUnitHint = false;
    mSelectNeutralUnits = false;

    mValidLayers[MOL_AIROBJECTS] = 1;
    mValidLayers[MOL_GROUNDUNIT] = 2;
    mValidLayers[MOL_GROUNDOBJECT] = 3;

    mTransportRules = new CIGTransportRules();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CIGMission::~CIGMission()
{
    VLOG(5) << "CIGMission::~CIGMission() - deleting the map";
    try
    {
        delete mMissionControl;
    }
    catch(...)
    {
        LOG(ERROR) << "CIGMission::~CIGMission() - cannot remove MissionControl object";
    }

    try
    {
        delete mBGManager;
    }
    catch(...)
    {
        LOG(ERROR) << "CIGMission::~CIGMission() - cannot remove Battle Group Manager object";
    }

    try
    {
        delete mDoctrine;
    }
    catch(...)
    {
        LOG(ERROR) << "CIGMission::~CIGMission() - cannot remove Doctrine object";
    }

    try
    {
        delete mMap;
    }
    catch(...)
    {
        LOG(ERROR) << "CIGMission::~CIGMission() - cannot remove map object";
    }

    VLOG(5) << "CIGMission::~CIGMission() - deleting objects";
    GetObjects(MOL_ALLOBJECTS)->FreeAll();
    for (int i=0; i<MISSION_OBJECT_LISTS_COUNT; i++)
    {
        delete mObjects[i];
    }

    GetToDeleteObjects()->FreeAll();
    delete mToDeleteObjects;

    delete mTransportRules;
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: updates the mission
 others     : it handles the timer event
---------------------------------------------------------------------------*/
void CIGMission::Update()
{
    if (GetEnabled() == false)
        return;

    GetUpdateCounter()++;

    if (GetUpdateCounter().MinReached() == true)
    {
        VLOG(9) << "CIGMission::Update";
//      GetToDeleteObjects()->FreeAll();
// Was modified by karg - changed from TList to KList GetToDeleteObjects()->ClearAll();
        //GetToDeleteObjects()->RemoveAll();

        if (GetToDeleteObjects()->Count() > 0)
        {
            VLOG(9) << "GetToDeleteObjects";
        }

        GetToDeleteObjects()->FreeAll();
        GetMap()->Update();
        GetObjects(MOL_ALLOBJECTS)->Update();

        VLOG(9) << "CIGMission::Update - over";
    }

    // mission control & mission control script update
    mMissionControl->Update();

    // battle groups update
    mBGManager->Update();

    // mission time update
    if (GetTimerInstance()->GetTime() - mTicks > 1000)
    {
        mTicks = GetTimerInstance()->GetTime();

        mMissionTime++;
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the viewable portion of the map
 parameters : ptViewport - the zone of the screen where the paint is wanted
              rcWindow - the visible portion of the map
              zoomLevel - is 1 by default
---------------------------------------------------------------------------*/
void CIGMission::Paint(CPPoint ptViewport, CPPoint ptWindow, int Width, int Height, int zoomLevel)
{
    GetMap()->Paint(ptViewport, ptWindow, Width, Height, zoomLevel);
    GetObjects(MOL_SELECTEDOWNEDUNITS)->PaintSelection(ptViewport, ptWindow, Width, Height, zoomLevel, NULL);
    GetObjects(MOL_SELECTEDOWNEDOTHERS)->PaintSelection(ptViewport, ptWindow, Width, Height, zoomLevel, NULL);
    GetObjects(MOL_SELECTEDOTHERS)->PaintSelection(ptViewport, ptWindow, Width, Height, zoomLevel, NULL);

    CPGIGraphicSystem *lGSI;   //graphic system interface
    try
    {
        lGSI = GetGraphicInstance();
    }
    catch(...)
    {
        LOG(FATAL) << "CIGMission::Paint - UNABLE TO QUERY GRAPHICS !!!!!!!!!!";
        throw;
    }

    if (GetCurrentAction() == MA_SELECTING)
    {
        //means a selecting rectangle should be painted
        lGSI->SetColor(0xFFFFFFFF);
        lGSI->Rectangle(CPRect(GetBeginPoint().x + ptViewport.x - ptWindow.x,
                               GetBeginPoint().y + ptViewport.y - ptWindow.y,
                               GetCurrentPoint().x + ptViewport.x - ptWindow.x,
                               GetCurrentPoint().y + ptViewport.y - ptWindow.y)
                       );
    }

    lGSI->SetColor(0xFFFFFFFF);
    lGSI->Rectangle(CPRect(GetCurrentPoint().x + ptViewport.x - ptWindow.x - 2,
                           GetCurrentPoint().y + ptViewport.y - ptWindow.y - 2,
                           GetCurrentPoint().x + ptViewport.x - ptWindow.x + 2,
                           GetCurrentPoint().y + ptViewport.y - ptWindow.y + 2)
                   );
}
//---------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 description: paints the viewable portion of the map (to the radar)
 parameters : ptViewport - the zone of the screen where the paint is wanted
              rcWindow - the visible portion of the map
              zoomLevel - is 1 by default
---------------------------------------------------------------------------*/
void CIGMission::PaintMinimized(CPPoint ptViewport, CPPoint ptWindow, int Width, int Height, int zoomLevel)
{
    GetObjects(MOL_ALLOBJECTS)->PaintMinimized(ptViewport, ptWindow, Width, Height, zoomLevel);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: makes the selection from begin point to end point
---------------------------------------------------------------------------*/
void CIGMission::MakeSelection()
{
    if (GetFlag(MF_ADDSELECTION))
    {
        AddSelection();
    }
    else if (GetFlag(MF_INVERTSELECTION))
    {
        AddRemoveSelection();
    }
    else
    {
        CancelSelection();
        AddSelection();
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: cancels the current selection
---------------------------------------------------------------------------*/
void CIGMission::CancelSelection()
{
    GetObjects(MOL_SELECTEDOWNEDUNITS)->SetSelected(false);
// was modified by Karg - changed TList to KList   GetObjects(MOL_SELECTEDOWNEDUNITS)->Clear();
    VLOG(9) << "Selected units owned before " << GetObjects(MOL_SELECTEDOWNEDUNITS)->Count();
    GetObjects(MOL_SELECTEDOWNEDUNITS)->RemoveAll();
    VLOG(9) << "Selected units owned after " << GetObjects(MOL_SELECTEDOWNEDUNITS)->Count();

    GetObjects(MOL_SELECTEDOTHERS)->SetSelected(false);
// was modified by Karg - changed TList to KList   GetObjects(MOL_SELECTEDOTHERS)->Clear();
    VLOG(9) << "Selected others before " << GetObjects(MOL_SELECTEDOTHERS)->Count();
    GetObjects(MOL_SELECTEDOTHERS)->RemoveAll();
    VLOG(9) << "Selected others after " << GetObjects(MOL_SELECTEDOTHERS)->Count();

    GetObjects(MOL_SELECTEDOWNEDOTHERS)->SetSelected(false);
// was modified by Karg - changed TList to KList   GetObjects(MOL_SELECTEDOWNEDOTHERS)->Clear();
    VLOG(9) << "Selected owned others before " << GetObjects(MOL_SELECTEDOWNEDOTHERS)->Count();
    GetObjects(MOL_SELECTEDOWNEDOTHERS)->RemoveAll();
    VLOG(9) << "Selected owned others after " << GetObjects(MOL_SELECTEDOWNEDOTHERS)->Count();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: adds the objects to selection (if possible)
---------------------------------------------------------------------------*/
void CIGMission::AddSelection()
{
    LogMethod("CIGMission::AddSelection");

    GetObjects(MOL_SELECTEDOTHERS)->RemoveAll();
    GetObjects(MOL_SELECTEDOWNEDOTHERS)->RemoveAll();
    GetObjects(MOL_SELECTEDOWNEDUNITS)->RemoveAll();

    CIGBaseObject *lTmpObj;

    int lLogicLeft, lLogicRight, lLogicTop, lLogicBottom;
    lLogicLeft  = min(GetBeginPoint().x, GetEndPoint().x);
    lLogicLeft  = max(0, lLogicLeft);
    lLogicRight = max(GetBeginPoint().x, GetEndPoint().x);
    lLogicRight = min(GetMap()->GetWidth()-1, lLogicRight);
    lLogicTop   = min(GetBeginPoint().y, GetEndPoint().y);
    lLogicTop   = max(0, lLogicTop);
    lLogicBottom= max(GetBeginPoint().y, GetEndPoint().y);
    lLogicBottom= min(GetMap()->GetHeight()-1, lLogicBottom);

    CPRect lSelectionRect = CPRect(lLogicLeft, lLogicTop, lLogicRight, lLogicBottom);
    //CPRect logicSelectionRect = CPRect(lLogicLeft / MAPCELLX, lLogicTop / MAPCELLY, lLogicRight / MAPCELLX, lLogicBottom / MAPCELLY);
    bool selectionIsPoint = (lSelectionRect.left == lSelectionRect.right && lSelectionRect.top == lSelectionRect.bottom);

    CIGBaseObjectList* selectedObjects;

    // point selection and rectangle selection work differently (point selection observes the layer order)
    if (selectionIsPoint)
    {
        selectedObjects = new CIGBaseObjectList();

        lTmpObj = GetObjectAtReal(lLogicLeft, lLogicTop);

        if (lTmpObj)
        {
            selectedObjects->Add(lTmpObj);
        }
    }
    else
    {
        selectedObjects = GetObjectInRectReal(lLogicLeft, lLogicTop, lLogicRight, lLogicBottom);
    }

    for(int index = 0; index < selectedObjects->Count(); index++)
    {
        lTmpObj = selectedObjects->GetObject(index);

        if (lTmpObj->GetIsDead())
        {
            continue;
        }

        CPRect objPaintRect = lTmpObj->GetPaintRect();

        // create the intersection between selection and object paint rect
        int startX = max(objPaintRect.left, lSelectionRect.left);
        int startY = max(objPaintRect.top, lSelectionRect.top);
        int endX = min(objPaintRect.right, lSelectionRect.right);
        int endY = min(objPaintRect.bottom, lSelectionRect.bottom);

        // check if there's at least one cell in the visibility map that is visible
        if (!mMap->AnyHasVisibility(OS_PLAYER, startX / MAPCELLX, startY / MAPCELLY, (endX - startX) / MAPCELLX, (endY - startY) / MAPCELLY, 2))
        {
            continue;
        }

        bool addedToSelection = false;

        if (lTmpObj->GetBattleGroupId() == OS_PLAYER)
        {
            if (lTmpObj->IsDerivedFrom(IGCN_UNIT))
            {
                GetObjects(MOL_SELECTEDOWNEDUNITS)->AddUnique(lTmpObj);
                addedToSelection = true;
            }
            else
            {
                GetObjects(MOL_SELECTEDOWNEDOTHERS)->AddUnique(lTmpObj);
                addedToSelection = true;
            }
        }
        else if (mBGManager->GetRelation(OS_PLAYER, lTmpObj->GetBattleGroupId()) != REL_NEUTRAL || GetSelectNeutralUnits())
        {
            GetObjects(MOL_SELECTEDOTHERS)->AddUnique(lTmpObj);
            addedToSelection = true;
        }

        // forbid multiple selection in "point" mode
        if (selectionIsPoint && addedToSelection)
        {
            break;
        }
    }

    GetObjects(MOL_SELECTEDOWNEDUNITS)->SetSelected(true);
    GetObjects(MOL_SELECTEDOWNEDOTHERS)->SetSelected(true);
    GetObjects(MOL_SELECTEDOTHERS)->SetSelected(true);

    delete selectedObjects;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: adds the objects to selection (if possible)

 TODO perhaps rewrite to address being a copy of AddSelection
---------------------------------------------------------------------------*/
void CIGMission::AddRemoveSelection()
{
    LogMethod("CIGMission::AddRemoveSelection");

    GetObjects(MOL_SELECTEDOTHERS)->SetSelected(false);
    GetObjects(MOL_SELECTEDOWNEDOTHERS)->SetSelected(false);
    GetObjects(MOL_SELECTEDOWNEDUNITS)->SetSelected(false);

    CIGBaseObject *lTmpObj;

    int lLogicLeft, lLogicRight, lLogicTop, lLogicBottom;
    lLogicLeft  = min(GetBeginPoint().x, GetEndPoint().x);
    lLogicLeft  = max(0, lLogicLeft);
    lLogicRight = max(GetBeginPoint().x, GetEndPoint().x);
    lLogicRight = min(GetMap()->GetWidth()-1, lLogicRight);
    lLogicTop   = min(GetBeginPoint().y, GetEndPoint().y);
    lLogicTop   = max(0, lLogicTop);
    lLogicBottom= max(GetBeginPoint().y, GetEndPoint().y);
    lLogicBottom= min(GetMap()->GetHeight()-1, lLogicBottom);

    CPRect lSelectionRect = CPRect(lLogicLeft, lLogicTop, lLogicRight, lLogicBottom);
    //CPRect logicSelectionRect = CPRect(lLogicLeft / MAPCELLX, lLogicTop / MAPCELLY, lLogicRight / MAPCELLX, lLogicBottom / MAPCELLY);
    bool selectionIsPoint = (lSelectionRect.left == lSelectionRect.right && lSelectionRect.top == lSelectionRect.bottom);

    CIGBaseObjectList* selectedObjects;

    // point selection and rectangle selection work differently (point selection observes the layer order)
    if (selectionIsPoint)
    {
        selectedObjects = new CIGBaseObjectList();

        lTmpObj = GetObjectAtReal(lLogicLeft, lLogicTop);

        if (lTmpObj && !lTmpObj->GetIsDead())
        {
            selectedObjects->Add(lTmpObj);
        }
    }
    else
    {
        selectedObjects = GetObjectInRectReal(lLogicLeft, lLogicTop, lLogicRight, lLogicBottom);
    }

    for(int index = 0; index < selectedObjects->Count(); index++)
    {
        lTmpObj = selectedObjects->GetObject(index);

        if (lTmpObj->GetIsDead())
        {
            continue;
        }

        CPRect objPaintRect = lTmpObj->GetPaintRect();

        // create the intersection between selection and object paint rect
        int startX = max(objPaintRect.left, lSelectionRect.left);
        int startY = max(objPaintRect.top, lSelectionRect.top);
        int endX = min(objPaintRect.right, lSelectionRect.right);
        int endY = min(objPaintRect.bottom, lSelectionRect.bottom);

        // check if there's at least one cell in the visibility map that is visible
        if (!mMap->AnyHasVisibility(OS_PLAYER, startX / MAPCELLX, startY / MAPCELLY, (endX - startX) / MAPCELLX, (endY - startY) / MAPCELLY, 2))
        {
            continue;
        }

        bool addedToSelection = false;

        if (lTmpObj->GetBattleGroupId() == OS_PLAYER)
        {
            if (lTmpObj->IsDerivedFrom(IGCN_UNIT))
            {
                GetObjects(MOL_SELECTEDOWNEDUNITS)->AddRemove(lTmpObj);
                addedToSelection = true;
            }
            else
            {
                GetObjects(MOL_SELECTEDOWNEDOTHERS)->AddRemove(lTmpObj);
                addedToSelection = true;
            }
        }
        else if (mBGManager->GetRelation(OS_PLAYER, lTmpObj->GetBattleGroupId()) != REL_NEUTRAL || GetSelectNeutralUnits())
        {
            GetObjects(MOL_SELECTEDOTHERS)->AddRemove(lTmpObj);
            addedToSelection = true;
        }

        // forbid multiple selection in "point" mode
        if (selectionIsPoint && addedToSelection)
        {
            break;
        }
    }

    GetObjects(MOL_SELECTEDOWNEDUNITS)->SetSelected(true);
    GetObjects(MOL_SELECTEDOWNEDOTHERS)->SetSelected(true);
    GetObjects(MOL_SELECTEDOTHERS)->SetSelected(true);

    delete selectedObjects;

    /*
    GetObjects(MOL_SELECTEDOTHERS)->RemoveAll();
    GetObjects(MOL_SELECTEDOWNEDOTHERS)->RemoveAll();

    int lLogicLeft, lLogicRight;
    int lLogicTop, lLogicBottom;

    CIGBaseObjectList* lTmpList = new CIGBaseObjectList();
    CIGBaseObject *lTmpObj;

    lLogicLeft  = min(GetBeginPoint().x, GetEndPoint().x);
    lLogicLeft  = max(0, lLogicLeft);
    lLogicRight = max(GetBeginPoint().x, GetEndPoint().x);
    lLogicRight = min(GetMap()->GetWidth()-1, lLogicRight);
    lLogicTop   = min(GetBeginPoint().y, GetEndPoint().y);
    lLogicTop   = max(0, lLogicTop);
    lLogicBottom= max(GetBeginPoint().y, GetEndPoint().y);
    lLogicBottom= min(GetMap()->GetHeight()-1, lLogicBottom);

    lLogicLeft  = lLogicLeft/MAPCELLX;
    lLogicRight = lLogicRight / MAPCELLX;
    lLogicTop   = lLogicTop / MAPCELLY;
    lLogicBottom= lLogicBottom / MAPCELLY;

    for (int i=lLogicLeft; i<=lLogicRight; i++)
    {
        for (int j=lLogicTop; j<=lLogicBottom; j++)
        {
            // visibility - dont select things that arent visible
            if (GetMap()->GetVisibility(OS_PLAYER, i, j) != 2)
            {
                continue;
            }

            // in a cell there may be one ground unit and several air units
            lTmpList->AddUnique(GetMap()->GetGroundObject(i, j));

            CIGBaseObjectList* lAirList = GetMap()->GetAirObjects(i, j);

            if (lAirList)
            {
                lTmpList->AddUnique(lAirList);
            }

            for(int k = 0;  k < lTmpList->Count(); k++)
            {
                lTmpObj = lTmpList->GetObject(k);

                if (lTmpObj != NULL)
                {

                    if (lTmpObj->GetBattleGroupId() == OS_PLAYER)
                        if (lTmpObj->IsDerivedFrom(IGCN_UNIT) == true)
                            GetObjects(MOL_SELECTEDOWNEDUNITS)->AddRemove(lTmpObj);
                        else
                            GetObjects(MOL_SELECTEDOWNEDOTHERS)->AddRemove(lTmpObj);
                    else
                        GetObjects(MOL_SELECTEDOTHERS)->AddRemove(lTmpObj);
                }
            }

            lTmpList->RemoveAll();
        }
    }

    delete lTmpList;

    GetObjects(MOL_SELECTEDOWNEDUNITS)->SetSelected(true);
    GetObjects(MOL_SELECTEDOWNEDOTHERS)->SetSelected(true);
    GetObjects(MOL_SELECTEDOTHERS)->SetSelected(true);
    */
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sets the team for all the selected owend units
 parameters : Team - the team number
---------------------------------------------------------------------------*/
void CIGMission::SetSelectionToTeam(int Team)
{
    CIGBaseObjectList* lOwnedUnits = GetObjects(MOL_OWNEDUNITS);
    const size_t       lCount      = lOwnedUnits->Count();

    for (int i=0; (unsigned)i < lCount; i++)
    {
        if (lOwnedUnits->GetObject(i)->GetTeam() == Team)
        {
            lOwnedUnits->GetObject(i)->SetTeam(-1); //no team
        }
    }

    GetObjects(MOL_SELECTEDOWNEDUNITS)->SetTeam(Team);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: selects the objects in the team Team
 parameters : Team - the team number
---------------------------------------------------------------------------*/
void CIGMission::GetSelectionFromTeam(int Team)
{
    if (GetFlag(MF_ADDSELECTION) == true)
        AddSelectionFromTeam(Team);
    else if (GetFlag(MF_INVERTSELECTION) == true)
        AddRemoveSelectionFromTeam(Team);
    else
    {
        CancelSelection();
        AddSelectionFromTeam(Team);
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: selects the objects in the team Team
 parameters : Team - the team number
---------------------------------------------------------------------------*/
void CIGMission::AddSelectionFromTeam(int Team)
{
    CIGBaseObjectList* lOwnedUnits = GetObjects(MOL_OWNEDUNITS);
    const size_t       lCount      = lOwnedUnits->Count();

    for (int i=0; (unsigned)i < lCount; i++)
    {
        if (lOwnedUnits->GetObject(i)->GetTeam() == Team)
        {
            GetObjects(MOL_SELECTEDOWNEDUNITS)->AddUnique(lOwnedUnits->GetObject(i));
        }
    }

    GetObjects(MOL_SELECTEDOWNEDUNITS)->SetSelected(true);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: selects the objects in the team Team
 parameters : Team - the team number
---------------------------------------------------------------------------*/
void CIGMission::AddRemoveSelectionFromTeam(int Team)
{
    CIGBaseObjectList* lOwnedUnits = GetObjects(MOL_OWNEDUNITS);
    const size_t       lCount      = lOwnedUnits->Count();

    for (int i=0; (unsigned)i < lCount; i++)
    {
        if (lOwnedUnits->GetObject(i)->GetTeam() == Team)
        {
            GetObjects(MOL_SELECTEDOWNEDUNITS)->AddRemove(lOwnedUnits->GetObject(i));
        }
    }

    GetObjects(MOL_SELECTEDOWNEDUNITS)->SetSelected(true);
}
//---------------------------------------------------------------------------

void CIGMission::RemoveFromSelection(CIGBaseObject *Object)
{
    GetObjects(MOL_SELECTEDOWNEDUNITS)->Remove(Object);
    GetObjects(MOL_SELECTEDOWNEDOTHERS)->Remove(Object);
    GetObjects(MOL_SELECTEDOTHERS)->Remove(Object);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: creates the mouse target effect
 others     : in fact, it creates an effect object
---------------------------------------------------------------------------*/
void CIGMission::CreateTargetEffect()
{
    VLOG(9) << "CIGMission::CreateTargetEffect";

    CPGIGraphicSystem *lGSI;   //graphic system interface
    try
    {
        lGSI = GetGraphicInstance();
    }
    catch(...)
    {
        LOG(FATAL) << "CIGMission::CreateTargetEffect - UNABLE TO QUERY GRAPHICS !!!!!!!!!!";
        throw CErrorGeneric();
    }

    CPGISprite *lSprite = lGSI->AddSprite(CPString(PATH_INGAME_GFX) + "target.png", 6, 1);
    CIGEffectObject *lTargetEffect = new CIGEffectObject();
    lTargetEffect->SetAnimCyclesCount(0);
    lTargetEffect->SetSprite(lSprite);
    lTargetEffect->CenterInPoint(GetCurrentPoint());
    lTargetEffect->NoteInLayerMap();
    GetObjects(MOL_ALLOBJECTS)->Add(lTargetEffect);

    VLOG(9) << "CIGMission::CreateTargetEffect - over";
}
//---------------------------------------------------------------------------

CIGBaseObject* CIGMission::GetObjectById(int _id)
{
    CIGBaseObjectList* lAllObjects  = GetObjects(MOL_ALLOBJECTS);
    const size_t       lObjectCount = lAllObjects->Count();

    for(int i = 0; (unsigned)i < lObjectCount; i++)
    {
        CIGBaseObject* lObject = lAllObjects->GetObject(i);
        if (lObject->GetId() == _id)
        {
            return lObject;
        }
    }

    return NULL;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: notes the object into the layer map
 parameters : Layer  - the layer where the notation is wanted
              Object - the object to be noted
              X, Y   - logic coord
---------------------------------------------------------------------------*/
void CIGMission::NoteInLayerMap(int Layer, CIGBaseObject *Object, int X, int Y)
{
    GetMap()->NoteInLayerMap(Layer, Object, X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: notes the object into the layer map
 parameters : Layer  - the layer where the notation is wanted
              Object - the object to be noted
              X, Y   - logic coord
---------------------------------------------------------------------------*/
void CIGMission::RemoveFromLayerMap(int Layer, CIGBaseObject *Object, int X, int Y)
{
    GetMap()->RemoveFromLayerMap(Layer, Object, X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: notes an object to the logic map
 parameters : Object - the object to be noted
              X, Y   - logic coordonates
---------------------------------------------------------------------------*/
void CIGMission::NoteInGroundLogicMap(CIGBaseObject *Object, int X, int Y)
{
    GetMap()->NoteInGroundLogicMap(Object, X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: removes an object from the logic map (if it exists)
 parameters : Object - the object to be noted
              X, Y   - logic coordonates
---------------------------------------------------------------------------*/
void CIGMission::RemoveFromGroundLogicMap(CIGBaseObject *Object, int X, int Y)
{
    GetMap()->RemoveFromGroundLogicMap(Object, X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the objects in X,Y is of Side
 parameters : Side - the queried side
              X, Y - the logic coord of the object
---------------------------------------------------------------------------*/
bool CIGMission::IsGroundObjectInBGroup(int _bgId, int X, int Y)
{
    return GetMap()->IsGroundObjectInBGroup(_bgId, X, Y);
}
//---------------------------------------------------------------------------

int  CIGMission::GetGroundObjectBGroup(int _x, int _y)
{
    return GetMap()->GetGroundObjectBGroup(_x, _y);
}
//---------------------------------------------------------------------------

TPoliticsRelations CIGMission::GetGroundObjectsRelation(int _bgId, int X, int Y)
{
    return mBGManager->GetRelation(_bgId, GetMap()->GetGroundObjectBGroup(X, Y));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the place is empty or is the specified object
 parameters : Object - the ignored object
              X, Y   - the logic coord of the queried place
---------------------------------------------------------------------------*/
bool CIGMission::IsGroundPlaceEmpty(CIGBaseObject* Object, int X, int Y)
{
    return GetMap()->IsGroundPlaceEmpty(Object, X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the object in X,Y is moving
 parameters : Object - the ignored object
              X, Y   - the logic coord of the queried place
---------------------------------------------------------------------------*/
bool CIGMission::IsGroundObjectMoving(CIGBaseObject* Object, int X, int Y)
{
    return GetMap()->IsGroundObjectMoving(Object, X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: notes the stimul in the point
 parameters : Sender - usually the damage object, but can be a unit as well
              Stimul - the stimul that must be sended
              X, Y   - the logic coord.
 other      : sends the stimul to the units in the point
---------------------------------------------------------------------------*/
void CIGMission::NoteGroundStimul(CIGBaseObject *Sender, CIGObjectStimul& Stimul, int X, int Y)
{
    if (GetMap()->GetGroundObject(X, Y) != NULL)
        GetMap()->GetGroundObject(X, Y)->NoteStimul(Stimul);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: notes the stimul in the point
 parameters : Sender - usually the damage object, but can be a unit as well
              Stimul - the stimul that must be sended
              X, Y   - the logic coord.
              Z - the altitude where the stimul was created (this param is useless,
                  as the weapons should check if they really hit the target)
 other      : sends the stimul to the units in the point
---------------------------------------------------------------------------*/
void CIGMission::NoteAirStimul(CIGBaseObject *Sender, CIGObjectStimul& Stimul, int X, int Y, int Z)
{
    CIGBaseObjectList* lAirList = GetMap()->GetAirObjects(X, Y);

    // do we have something to notify
    if (!lAirList)
    {
        return;
    }

    for(int i = 0;  i < lAirList->Count(); i++)
    {
        CIGBaseObject* lTmpObj = lAirList->GetObject(i);

        if (lTmpObj != 0)
        {
            lTmpObj->NoteStimul(Stimul);
        }
    }
}
//---------------------------------------------------------------------------

CIGBaseObject* CIGMission::GetObjectAtReal(int X, int Y)
{
    CIGBaseObjectList* objList = GetObjects(MOL_ALLOBJECTS);
    CIGBaseObject* obj = NULL;
    CIGBaseObject* result = NULL;
    int resultLayer = 999;

    // TODO very slow, implement a locality query system
    for(int i = 0, objListCount = objList->Count(); i < objListCount; i++)
    {
        obj = objList->GetObject(i);
        int objLayer = obj->GetLayer();

        // can select the object?
        if (mValidLayers.count(objLayer) == 0)
        {
            continue;
        }

        CPRect bounds = obj->GetPaintRect();

        if (!bounds.IsPointInside(X, Y))
        {
            continue;
        }

        // TODO add pixel perfect checks

        if (!result || mValidLayers[objLayer] < resultLayer || (mValidLayers[objLayer] == resultLayer && obj->GetCoord().y > result->GetCoord().y) )
        {
            result = obj;
            resultLayer = mValidLayers[result->GetLayer()];
        }
    }

    return result;
}
//---------------------------------------------------------------------------

CIGBaseObjectList* CIGMission::GetObjectInRectReal(int left, int top, int right, int bottom)
{
    CIGBaseObjectList* objList = GetObjects(MOL_ALLOBJECTS);
    CIGBaseObject* obj = NULL;
    CIGBaseObjectList* result = new CIGBaseObjectList();
    CPRect rect = CPRect(left, top, right, bottom);

    // TODO very slow, implement a locality query system
    for(int i = 0, objListCount = objList->Count(); i < objListCount; i++)
    {
        obj = objList->GetObject(i);
        int objLayer = obj->GetLayer();

        // can select the object?
        if (mValidLayers.count(objLayer) == 0)
        {
            continue;
        }

        if (obj->GetId() == 255)
        {
            int j = 10;
            j++;
        }

        CPRect bounds = obj->GetPaintRect();

        if (!bounds.Intersects(rect))
        {
            continue;
        }

        result->Add(obj);
    }

    return result;
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: Computes current action by checking Action and CurrentPoint
 TODO refactor this method to take into account that at a certain point there
 might be multiple units (e.g. an air and a ground/building)
---------------------------------------------------------------------------*/
void CIGMission::ComputeCurrentAction()
{
    int lBg;
    CIGBaseObject* lObject = NULL;
    bool lIsTran, lIsDropship;
    CPPoint lCurrentPoint = GetCurrentPoint();
    CPPoint lCurrentLogicPoint = CPPoint(lCurrentPoint.x/MAPCELLX, lCurrentPoint.y/MAPCELLY);

    if (GetFlag(MF_ADDSELECTION) == true || GetFlag(MF_INVERTSELECTION) == true)
    {
        if (GetCurrentAction() != MA_SELECTING)
        {
            SetCurrentAction(MA_SELECT);
        }

        return;
    }

    //here are only the actions that require mouse actions
    //e.g. stop doesn't require additional mouse actions
    if (GetObjects(MOL_SELECTEDOWNEDUNITS)->IsEmpty())
    {
        //no owned units selected
        if (GetCurrentAction() != MA_SELECTING)
            SetCurrentAction(MA_SELECT);
    }
    else
    {
        // owned units selected
        switch (GetAction())
        {
        case MA_LOAD:
        case MA_MOVE:
        case MA_ATTACK:
        case MA_DEFAULT:
        {
            //owned units selected
            lObject = GetObjectAtReal(lCurrentPoint.x, lCurrentPoint.y);
            lBg = lObject ? lObject->GetBattleGroupId() : OS_UNDEFINED;

            if (lBg == OS_PLAYER && mTransportRules->CanLoadAny(lObject, GetObjects(MOL_SELECTEDOWNEDUNITS)))
            {
                SetCurrentAction(MA_LOAD);
            }
            else
            {
                // check if any of the selected owned units can guard other objects
                //if ((lBg == OS_PLAYER || mBGManager->GetRelation(OS_PLAYER, lBg) == REL_FRIENDLY) &&
                //    !GetObjects(MOL_SELECTEDOWNEDUNITS)->ContainsOnly(lObject) &&
                //    GetObjects(MOL_SELECTEDOWNEDUNITS)->CanAny(CAN_GUARD))
                //{
                //    SetCurrentAction(MA_GUARD);
                //}
                //else
                //{
                    // if in (X, Y) is an enemy of player and player sees the tile then attack
                    if (mBGManager->GetRelation(OS_PLAYER, lBg) == REL_ENEMY && GetMap()->GetVisibility(OS_PLAYER, lCurrentLogicPoint.x, lCurrentLogicPoint.y) == 2)
                    {
                        SetCurrentAction(MA_ATTACK);
                    }
                    else
                    {
                        SetCurrentAction(MA_MOVE);
                    }
                //}
            }
        }
        break;

        case MA_PATROL:
            SetCurrentAction(MA_PATROL);
            break;

        case MA_SUPPRESS_FIRE:
            SetCurrentAction(MA_SUPPRESS_FIRE);
            break;

        case MA_LAND:
            SetCurrentAction(MA_LAND);
            break;

        case MA_TAKEOFF:
            //SetCurrentAction(MA_TAKEOFF);
            break;

        case MA_GUARD:
            SetCurrentAction(MA_GUARD);
            break;

        case MA_HOLD:
            break;

        case MA_CAPTURE:
            break;

        case MA_UNLOAD:
            break;

        case MA_UNLOAD_ALL:
            SetCurrentAction(MA_UNLOAD_ALL);
            break;

        case MA_TAKE_COVER:
            //SetCurrentAction(MA_TAKE_COVER);
            break;
        }
    }

    // TEST CODE FOR SDJ
    if (!GetObjects(MOL_SELECTEDOWNEDOTHERS)->IsEmpty())
    {
        switch (GetAction())
        {
            case MA_RALLY_POINT:
                SetCurrentAction(MA_RALLY_POINT);
                break;
        }
    }
}
//---------------------------------------------------------------------------

void CIGMission::SetAction(int value)
{
    if (!CheckOwnedActionMask(value))
    {
        ResetAction();
        return;
    }

    mAction = value;

    switch(mAction)
    {
        case MA_STOP:
            ActionStop();
            CancelAction();
            break;

        case MA_TAKE_COVER:
            ActionTakeCover();
            CancelAction();
            break;

        case MA_TAKEOFF:
            //ActionTakeoff();
            //CancelAction();
            break;

        //case MA_LAND:
        //   ActionLand();
        //   CancelAction();
        //   break;

        case MA_UNLOAD_ALL:
            //ActionUnloadAll();
            //CancelAction();
            break;

        case MA_HOLD:
            ActionHold();
            CancelAction();
            break;

        // TEST CODE FOR SDJ
        case MA_BUILD:
            ActionBuild();
            //ResetAction();
            //CancelAction();
            break;
    }
}
//---------------------------------------------------------------------------

void CIGMission::SetBeginPoint(CPPoint value)
{
    mBeginPoint = value;
    SetCurrentPoint(value);
    switch (GetCurrentAction())
    {
    case MA_SELECT:
        SetCurrentAction(MA_SELECTING);
        break;
    case MA_STOP:  // I don't think this is possible
        break;
    case MA_TAKE_COVER: // and neither is this
        break;
    case MA_MOVE:
        CreateTargetEffect();
        ActionMove();
        ResetAction();
        break;
    case MA_LAND:
        CreateTargetEffect();
        ActionLand();
        ResetAction();
        break;
    case MA_ATTACK:
        ActionAttack();
        ResetAction();
        break;
    case MA_PATROL:
        CreateTargetEffect();
        ActionPatrol();
        ResetAction();
        break;
    case MA_GUARD:
        ActionGuard();
        ResetAction();
        break;
    case MA_HOLD: // I don't think this is possible
        ResetAction();
        break;
    case MA_CAPTURE:
        ActionCapture();
        ResetAction();
        break;
    case MA_LOAD:
        ActionLoad();
        ResetAction();
        break;
    case MA_UNLOAD:
        ActionUnload();
        ResetAction();
        break;
    case MA_UNLOAD_ALL:
        CreateTargetEffect();
        ActionUnloadAll();
        ResetAction();
        break;
    case MA_SUPPRESS_FIRE:
        ActionSuppressFire();
        ResetAction();
        break;

    // TEST CODE FOR SDJ
    case MA_RALLY_POINT:
        ActionRallyPoint();
        ResetAction();
        break;
    }
}
//---------------------------------------------------------------------------

void CIGMission::SetFocusObject(CIGBaseObject* _obj)
{
    mFocusObj = _obj;
}
//---------------------------------------------------------------------------

// Description : Sets the focused object
// Param       :
// Result      :
// Comments    : Is not a real action method (like ActionMove and the rest)
void CIGMission::ActionFocusOnObject()
{
    // TODO: remake this algorithm
    CIGBaseObjectList* lSelectedOwnedUnits = GetObjects(MOL_SELECTEDOWNEDUNITS);

    if (mFocusObj)
    {
        mFocusObj = 0;
    }
    else if (lSelectedOwnedUnits->Count() > 0)
    {
        mFocusObj = lSelectedOwnedUnits->GetObject(0);
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the move action
---------------------------------------------------------------------------*/
void CIGMission::ActionMove()
{
    TIGSubSysActionData lTmpActionData;

    // TODO: by default, the tactic move order is issued to the units; however,
    // some applications may need to have a flexible control scheme over the units
    // (like a setting "follow strict orders" and "follow loose orders" - setting
    // that may be changed in the game GUI)
    lTmpActionData.mType = OA_TACTIC_MOVE;

    if (GetFlag(MF_PRESERVEFORM) == false)
    {
        CIGBaseObjectList* lSelectedOwnedUnits = GetObjects(MOL_SELECTEDOWNEDUNITS);
        const size_t       lObjectCount        = lSelectedOwnedUnits->Count();
        CPPoint            lCenterPoint        = lSelectedOwnedUnits->GetCenterPoint();
        CPPoint            lCurrentPoint       = GetCurrentPoint();

        for (int i=0; (unsigned)i < lObjectCount; i++)
        {
            CIGBaseObject* lObject      = lSelectedOwnedUnits->GetObject(i);
            CPPoint        lObjectCoord = lObject->GetLogicCoord();

            lTmpActionData.mTargetPoint.x = (lObjectCoord.x -
                                             lCenterPoint.x +
                                             lCurrentPoint.x/MAPCELLX) * MAPCELLX;
            lTmpActionData.mTargetPoint.y = (lObjectCoord.y -
                                             lCenterPoint.y +
                                             lCurrentPoint.y/MAPCELLY) * MAPCELLY;
            lObject->SetAction(lTmpActionData);
        }
    }
    else
    {
        lTmpActionData.mTargetPoint = GetCurrentPoint();
        GetObjects(MOL_SELECTEDOWNEDUNITS)->SetAction(lTmpActionData);
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the stop action
---------------------------------------------------------------------------*/
void CIGMission::ActionStop()
{
    TIGSubSysActionData lTmpActionData;
    lTmpActionData.mType = OA_TACTIC_STOP;
    GetObjects(MOL_SELECTEDOWNEDUNITS)->SetAction(lTmpActionData);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the attack action
---------------------------------------------------------------------------*/
void CIGMission::ActionAttack()
{
    CPPoint currentPoint = GetCurrentPoint();

    // TODO handle the case of an enemy unit hidden behind a neutral building/tree
    // (e.g. implement another method to get an enemy unit at current point)
    CIGBaseObject* lObject = GetObjectAtReal(currentPoint.x, currentPoint.y);

    if (!lObject)
    {
        return;
    }

    TIGSubSysActionData lTmpActionData;

    lTmpActionData.mTargetObject = lObject;

    // TODO: by default, the tactic attack order is issued to the units; however,
    // some applications may need to have a flexible control scheme over the units
    // (like a setting "follow strict orders" and "follow loose orders" - setting
    // that may be changed in the game GUI)
    lTmpActionData.mType = OA_TACTIC_ATTACK;
    lTmpActionData.mTargetPoint = currentPoint;
    GetObjects(MOL_SELECTEDOWNEDUNITS)->SetAction(lTmpActionData);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the patrol action
---------------------------------------------------------------------------*/
void CIGMission::ActionPatrol()
{
    TIGSubSysActionData lTmpActionData;
    lTmpActionData.mTargetPoint = GetCurrentPoint();

    // TODO: by default, the tactic patrol order is issued to the units; however,
    // some applications may need to have a flexible control scheme over the units
    // (like a setting "follow strict orders" and "follow loose orders" - setting
    // that may be changed in the game GUI)
    lTmpActionData.mType = OA_TACTIC_PATROL;
    GetObjects(MOL_SELECTEDOWNEDUNITS)->SetAction(lTmpActionData);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the guard action
---------------------------------------------------------------------------*/
void CIGMission::ActionGuard()
{
    // redundant check, a unit should not have MA_GUARD in the action mask anyway
    if (!GetObjects(MOL_SELECTEDOWNEDUNITS)->CanAny(CAN_GUARD))
    {
        return;
    }

    CIGBaseObject* lObject = GetObjectAtReal(GetCurrentPoint().x, GetCurrentPoint().y);

    if (!lObject || lObject->GetIsDead())
    {
        return;
    }

    // check relation with the object that will be guarded, only own and friendly units can be guarded
    int lBg = lObject->GetBattleGroupId();

    if (lBg != OS_PLAYER || mBGManager->GetRelation(OS_PLAYER, lBg) != REL_FRIENDLY)
    {
        return;
    }

    TIGSubSysActionData lTmpActionData;
    lTmpActionData.mType = OA_GUARD;
    lTmpActionData.mTargetObject = lObject;

    // TODO iterate over the selected own units, checking if they can guard and setting
    // the target position relative to the target unit mTargetPoint?
    GetObjects(MOL_SELECTEDOWNEDUNITS)->SetAction(lTmpActionData);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the hold action
---------------------------------------------------------------------------*/
void CIGMission::ActionHold()
{
    TIGSubSysActionData lTmpActionData;
    lTmpActionData.mType = OA_HOLD;
    GetObjects(MOL_SELECTEDOWNEDUNITS)->SetAction(lTmpActionData);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the capture action
---------------------------------------------------------------------------*/
void CIGMission::ActionCapture()
{
}
//---------------------------------------------------------------------------

// Description : handles the load action, also known as "get to the chopper" :P
// Param       : n/a
// Result      : n/a
// Comments    : sends onloadaction to the units selected, choosing between
//               units types
void CIGMission::ActionLoad()
{
    CIGBaseObject* target = GetObjectAtReal(GetCurrentPoint().x, GetCurrentPoint().y);

    if (!target)
    {
        return;
    }

    // iterate through the selected units, setting the load action to the
    // appropriate ones
    CIGBaseObjectList* lSelectedOwnedUnits = GetObjects(MOL_SELECTEDOWNEDUNITS);
    const int lObjectCount = lSelectedOwnedUnits->Count();
    bool loaded = false;

    for (int i = 0; (unsigned)i < lObjectCount; i++)
    {
        CIGBaseObject* lObject = lSelectedOwnedUnits->GetObject(i);

        if (mTransportRules->CanLoad(target, lObject))
        {
            TIGSubSysActionData lTmpActionData;

            lTmpActionData.mType         = OA_LOAD;
            lTmpActionData.mTargetObject = target;

            lObject->SetAction(lTmpActionData);

            loaded = true;
        }
    }

    if (!loaded)
    {
        // TODO give visual/audio feedback (e.g. play "denied" audio, show a floating message at current point)
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the unload action
---------------------------------------------------------------------------*/
void CIGMission::ActionUnload()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the unload all action
---------------------------------------------------------------------------*/
void CIGMission::ActionUnloadAll()
{
    TIGSubSysActionData lTmpActionData;

    lTmpActionData.mType         = OA_UNLOAD_ALL;
    lTmpActionData.mTargetObject = NULL;
    lTmpActionData.mTargetPoint  = GetCurrentPoint();

    CIGBaseObjectList* lSelectedOwnedUnits = GetObjects(MOL_SELECTEDOWNEDUNITS);
    const size_t       lObjectCount        = lSelectedOwnedUnits->Count();

    //
    // the unload all action can only be set for transporters and dropships
    // i should research a different approach to [who can do what action] policy
    //
    for (int i = 0; (unsigned)i < lObjectCount; i++)
    {
        CIGBaseObject* lObject = lSelectedOwnedUnits->GetObject(i);

        CPString lClassName = lObject->ClassName();

        if (lClassName == CPString(IGCN_TRANUNIT) ||
                lClassName == CPString(IGCN_DROPSHIPUNIT))
        {
            //lTmpActionData.mTargetPoint  = GetCurrentPoint();//lObject->GetLogicCoord();
            lObject->SetAction(lTmpActionData);
        }
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the suppress fire action - attack ground
---------------------------------------------------------------------------*/
void CIGMission::ActionSuppressFire()
{
    TIGSubSysActionData lTmpActionData;
    //lTmpActionData.mTargetObject = GetMap()->GetGroundObject(GetCurrentPoint().x/MAPCELLX,
    //                                                         GetCurrentPoint().y/MAPCELLY);

    // TODO: by default, the tactic attack ground order is issued to the units; however,
    // some applications may need to have a flexible control scheme over the units
    // (like a setting "follow strict orders" and "follow loose orders" - setting
    // that may be changed in the game GUI)
    lTmpActionData.mType         = OA_TACTIC_ATTACKGROUND;
    lTmpActionData.mTargetObject = NULL;

    lTmpActionData.mTargetPoint  = GetCurrentPoint();
    GetObjects(MOL_SELECTEDOWNEDUNITS)->SetAction(lTmpActionData);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the move action
---------------------------------------------------------------------------*/
void CIGMission::ActionTakeCover()
{
    TIGSubSysActionData lTmpActionData;
    lTmpActionData.mType = OA_MOVE;

    CIGBaseObjectList* lSelectedOwnedUnits = GetObjects(MOL_SELECTEDOWNEDUNITS);
    const size_t       lObjectCount        = lSelectedOwnedUnits->Count();

    for(int i = 0; (unsigned)i < lObjectCount; i++)
    {
        lTmpActionData.mTargetPoint   = lSelectedOwnedUnits->GetObject(i)->GetCoord();
        lTmpActionData.mTargetPoint.x = lTmpActionData.mTargetPoint.x + MAPCELLX * (gfRandom(10) - 5);
        lTmpActionData.mTargetPoint.y = lTmpActionData.mTargetPoint.y + MAPCELLY * (gfRandom(10) - 5);

        lSelectedOwnedUnits->GetObject(i)->SetAction(lTmpActionData);
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: like ActionMove, landing first requires movement to the desired
              point, then the altitude decrease
---------------------------------------------------------------------------*/
void CIGMission::ActionLand()
{
    TIGSubSysActionData lTmpActionData;
    lTmpActionData.mType = OA_LAND;

    if (GetFlag(MF_PRESERVEFORM) == false)
    {
        CIGBaseObjectList* lSelectedOwnedUnits = GetObjects(MOL_SELECTEDOWNEDUNITS);
        const size_t       lObjectCount        = lSelectedOwnedUnits->Count();
        CPPoint            lCenterPoint        = lSelectedOwnedUnits->GetCenterPoint();
        CPPoint            lCurrentPoint       = GetCurrentPoint();

        for (int i=0; (unsigned)i < lObjectCount; i++)
        {
            CIGBaseObject* lObject      = lSelectedOwnedUnits->GetObject(i);
            CPPoint        lObjectCoord = lObject->GetLogicCoord();

            lTmpActionData.mTargetPoint.x = (lObjectCoord.x -
                                             lCenterPoint.x +
                                             lCurrentPoint.x/MAPCELLX) * MAPCELLX;
            lTmpActionData.mTargetPoint.y = (lObjectCoord.y -
                                             lCenterPoint.y +
                                             lCurrentPoint.y/MAPCELLY) * MAPCELLY;
            lObject->SetAction(lTmpActionData);
        }
    }
    else
    {
        lTmpActionData.mTargetPoint = GetCurrentPoint();
        GetObjects(MOL_SELECTEDOWNEDUNITS)->SetAction(lTmpActionData);
    }
}
//---------------------------------------------------------------------------

void CIGMission::ActionTakeoff()
{
    //TIGSubSysActionData lTmpActionData;
    //lTmpActionData.mType = OA_TAKEOFF;
    //GetObjects(MOL_SELECTEDOWNEDUNITS)->SetAction(lTmpActionData);
}
//---------------------------------------------------------------------------

// TEST CODE FOR SDJ
void CIGMission::ActionBuild()
{
    TIGSubSysActionData lTmpActionData;

    lTmpActionData.mType = OA_BUILD;
    GetObjects(MOL_SELECTEDOWNEDOTHERS)->SetAction(lTmpActionData);
}
//---------------------------------------------------------------------------

// TEST CODE FOR SDJ
void CIGMission::ActionRallyPoint()
{
    TIGSubSysActionData lTmpActionData;

    lTmpActionData.mType        = OA_RALLY_POINT;
    lTmpActionData.mTargetPoint = GetCurrentPoint();
    GetObjects(MOL_SELECTEDOWNEDOTHERS)->SetAction(lTmpActionData);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets the action masks intersection of the selected owned objects
---------------------------------------------------------------------------*/
int CIGMission::GetObjectsActionMask()
{
    __int32  lMask1;
    __int32  lMask2;

    lMask1 = GetObjects(MOL_SELECTEDOWNEDUNITS)->GetActionMask();
    lMask2 = GetObjects(MOL_SELECTEDOWNEDOTHERS)->GetActionMask();

    if (GetObjects(MOL_SELECTEDOWNEDUNITS)->IsEmpty() == false)
    {
        if (GetObjects(MOL_SELECTEDOWNEDOTHERS)->IsEmpty() == false)
            return lMask1 & lMask2;
        else
            return lMask1;
    }
    else if (GetObjects(MOL_SELECTEDOWNEDOTHERS)->IsEmpty() == false)
        return lMask2;
    else
        return 0;
}
//---------------------------------------------------------------------------

bool CIGMission::CheckOwnedActionMask(int action)
{
    // ignore actions that cannot appear in an unit action mask
    if (action == MA_SELECT || action == MA_SELECTING || action == MA_DEFAULT)
    {
        return true;
    }

    return (GetObjectsActionMask() & (1 << action)) != 0;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets the name of the object in the wanted Point
 parameters : Point - real coord
 others     : for interface purpose
              return empty string ("") if no hint
---------------------------------------------------------------------------*/
CPString CIGMission::GetObjectHint()
{
    int mapVisibility = GetMap()->GetVisibility(OS_PLAYER, GetCurrentPoint().x / MAPCELLX, GetCurrentPoint().y / MAPCELLY);

    if (mapVisibility != 2)
    {
        return CPString("");
    }

    CIGBaseObject* object = GetObjectAtReal(GetCurrentPoint().x, GetCurrentPoint().y);

    // check the relation and setting to select/hover neutral elements
    return object && (mBGManager->GetRelation(OS_PLAYER, object->GetBattleGroupId()) != REL_NEUTRAL || GetNeutralUnitHint()) ? (object->GetName() + PP::toString(object->GetId()).c_str()) : CPString("");
}
//---------------------------------------------------------------------------

int CIGMission::GetUniqueUnitId()
{
    int  lId = -1;

    // parse the object lists, search for an unique id
    while(1)
    {
        lId++;

        if (UnitIdUnique(lId))
        {
            return lId;
        }
    }
}
//---------------------------------------------------------------------------

bool CIGMission::UnitIdUnique(int _id)
{
    bool lIsUnique = true;

    for(int i = 0; i < GetObjects(MOL_ALLOBJECTS)->Count(); i++)
    {
        if (GetObjects(MOL_ALLOBJECTS)->GetObject(i)->GetId() == _id)
        {
            lIsUnique = false;
            break;
        }
    }

    // also search MOL_TRANSPORTEDUNITS, since it is complimentary to MOL_ALLOBJECTS
    for(int i = 0; i < GetObjects(MOL_TRANSPORTEDUNITS)->Count(); i++)
    {
        if (GetObjects(MOL_TRANSPORTEDUNITS)->GetObject(i)->GetId() == _id)
        {
            lIsUnique = false;
            break;
        }
    }

    return lIsUnique;
}
//---------------------------------------------------------------------------

void CIGMission::Serialize(PP::Stream& _a)
{
    VLOG(5) << ".Writing mission data";

    // Mission Header
    VLOG(5) << ".Writing the mission headers...";
    CSerString lMissionName(mMissionName), lMissionInfo(mMissionInfo);

    lMissionName.Serialize(_a);
    lMissionInfo.Serialize(_a);
    // mission header - over

    // Load scripts from file
    mDoctrine->Serialize(_a);

    // Mission control serialize
    mMissionControl->Serialize(_a);


    // Read battle groups and politics
    mBGManager->Serialize(_a);

    // sprite descriptors
    CSerInt    lSprCount(GetGraphicInstance()->GetSpriteCount(SPRL_MISSION));

    VLOG(5) << ".Serializing sprites...";
    lSprCount.Serialize(_a);

    for (int i = 0; i < lSprCount.Get(); i++)
    {
        CPGISprite* lSprite = GetGraphicInstance()->GetSpriteByIndex(i, SPRL_MISSION);

        if (!lSprite)
        {
            LOG(FATAL) << "CIGMission::Serialize - Sprite is null, this will be a problem.";
            continue;
        }

        // path read from the file are relative to the game directory
        // they must be completed with the game path in order to read sprites
        CPString lFullPath = lSprite->GetFileName();
        //CSerString lSprFile(lFullPath.SubString(getGamePath().Length()/* - 1*/, lFullPath.Length() - getGamePath().Length()/* + 1*/));

        // when using the physfs filesystem, there's no need to worry about full path
        CSerString lSprFile(lFullPath);

        // sprite relative path and file name
        lSprFile.Serialize(_a);

        // writing sprite cells
        CSerInt lSprWCells(lSprite->GetAnimCount()), lSprHCells(lSprite->GetAnimTypes());
        lSprWCells.Serialize(_a);
        lSprHCells.Serialize(_a);
    }
    VLOG(5) << ".Sprites Serialized";
    // sprite descriptors - over

    // write the map
    mMap->Serialize(_a);

    // writing objects - delegating to a CIGSerBaseObjectList
    CIGSerBaseObjectList lObjList(GetObjects(MOL_ALLOBJECTS));

    lObjList.Serialize(_a);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGMission::DataSize()
{
    return 0;
}
//---------------------------------------------------------------------------

// Description : loads data from a stream into the structures
// Params      : Stream _a - the stream
// Result      : n/a
// Comments    : throws exceptions if error reading the stream
void CIGMission::DeSerialize(PP::Stream& _a)
{
    VLOG(5) << ".Reading mission data";

    // Mission Header
    VLOG(5) << ".Reading the mission headers...";
    CSerString lMissionName, lMissionInfo;

    lMissionName.DeSerialize(_a);
    lMissionInfo.DeSerialize(_a);

    mMissionName = lMissionName.Get();
    mMissionInfo = lMissionInfo.Get();
    // mission header - over

    // Load scripts from file
    mDoctrine->DeSerialize(_a);

    // Mission control serialize
    mMissionControl->DeSerialize(_a);

    // Read battle groups and politics
    mBGManager->DeSerialize(_a);

    // sprite descriptors
    CSerInt    lSprCount, lSprWCells, lSprHCells;
    CSerString lSprFile;

    // cleaning graphics
    GetGraphicInstance()->ReleaseAllSprites(SPRL_MISSION);

    VLOG(5) << ".Loading sprites...";
    lSprCount.DeSerialize(_a);

    for (int i = 0; i < lSprCount.Get(); i++)
    {
        lSprFile.DeSerialize(_a);

        // path read from the file are relative to the game directory, as the vfs needs
        CPString lFullPath = lSprFile.Get();

        // reading sprite cells
        lSprWCells.DeSerialize(_a);
        lSprHCells.DeSerialize(_a);

        // adding sprite ?!?
        GetGraphicInstance()->AddSprite(lFullPath, lSprWCells.Get(), lSprHCells.Get(), SPRL_MISSION);

        VLOG(5) << "..Sprite data loaded( " << i << "):" << lFullPath.c_str() << " (" << lSprWCells.Get() << " x " << lSprHCells.Get() << ")";

        VLOG(5) << "..Sprite added to list";
    }
    VLOG(5) << ".Sprites loaded";
    // sprite descriptors - over

    // dump sprites, see what we've got
    VLOG(5) << "Dumping sprites...";
    for(int i = 0; i < GetGraphicInstance()->GetSpriteCount(SPRL_MISSION); i++)
    {
        CPGISprite* lSprite = GetGraphicInstance()->GetSpriteByIndex(i, SPRL_MISSION);

        if (lSprite)
        {
            VLOG(5) << "..Sprite(" << i << "):" << lSprite->GetFileName() << " (" << lSprite->GetAnimCount() << " x " << lSprite->GetAnimTypes() << ")";
        }
    }

    // Read map
    mMap->DeSerialize(_a);

    // reading objects - the CIGMission behaves now as an abstract factory
    CSerInt         lObjCount, lObjType, lId;
    CIGBaseObject   *lObject;

    // object lists cleanup
    GetObjects(MOL_ALLOBJECTS)->FreeAll();
    for (int i=0; i<MISSION_OBJECT_LISTS_COUNT; i++)
    {
        mObjects[i]->RemoveAll();
    }
    GetToDeleteObjects()->FreeAll();
    // end object lists cleanup

    // deserialize objects
    CIGSerBaseObjectList lObjList(GetObjects(MOL_ALLOBJECTS));

    lObjList.DeSerialize(_a);

    // object list specific code
    for(int i = 0; i < GetObjects(MOL_ALLOBJECTS)->Count(); i++)
    {
        lObject = GetObjects(MOL_ALLOBJECTS)->GetObject(i);

        // just to make sure we didn't screw something up somewhere
        if (!lObject)
        {
            continue;
        }

        if (lObject->IsDerivedFrom(IGCN_UNIT))
        {
            // adding to battle group it belongs - only certain unit types
            // may reside in a battlegroup
            if (
                lObject->ClassName() == CPString(IGCN_STATICOBJECT) ||
                lObject->ClassName() == CPString(IGCN_INFANTRYUNIT) ||
                lObject->ClassName() == CPString(IGCN_TANKUNIT)     ||
                lObject->ClassName() == CPString(IGCN_RECONUNIT)    ||
                lObject->ClassName() == CPString(IGCN_ARTILLERYUNIT)||
                lObject->ClassName() == CPString(IGCN_TRANUNIT)     ||
                lObject->ClassName() == CPString(IGCN_CHOPPERUNIT)  ||
                lObject->ClassName() == CPString(IGCN_FIGHTERUNIT)  ||
                lObject->ClassName() == CPString(IGCN_DROPSHIPUNIT)
            )
            {
                mBGManager->AddUnit((CIGRealObject*)lObject);

                int relation = mBGManager->GetRelation(OS_PLAYER, lObject->GetBattleGroupId());

                switch (relation)
                {
                case REL_NEUTRAL:
                    GetObjects(MOL_NEUTRALUNITS)->Add(lObject);
                    break;
                case REL_OWNED:
                    GetObjects(MOL_OWNEDUNITS)->Add(lObject);
                    break;
                case REL_FRIENDLY:
                    GetObjects(MOL_FRIENDLYUNITS)->Add(lObject);
                    break;
                case REL_ENEMY:
                    GetObjects(MOL_ENEMYUNITS)->Add(lObject);
                    break;
                default:
                    // do nothing, continue loading
                    LOG(ERROR) << "Relation to player unknown, no list holding this unit. " << relation;
                    break;
                }
            }
        }

        lObject->NoteInLayerMap();
        lObject->NoteInLogicMap();
    }

    /*
    CIGBoidsObject* lObj = new CIGBoidsObject();
    lObj->SetLogicCoord(CPPoint(10, 10));
    lObj->SetCoord(CPPoint(160, 160));
    lObj->SetBattleGroupId(0);
    lObj->NoteInLayerMap();
    GetObjects(MOL_ALLOBJECTS)->Add(lObj);
    GetObjects(MOL_GROUNDEFFECTS)->Add(lObj);
    */

    VLOG(5) << ".Objects loaded";
    //Objects - over
}
//---------------------------------------------------------------------------

void CIGMission::Serialize(BaseSerializer& out)
{
    out.beginElement("mission");

    out.createAttribute("name", mMissionName.c_str());
    out.createAttribute("info", mMissionInfo.c_str());

    mDoctrine->Serialize(out);

    mMissionControl->Serialize(out);

    mBGManager->Serialize(out);

    out.rawWrite(GetGraphicInstance()->GetSpriteCount(SPRL_MISSION));

    out.beginElement("sprites");

    for (int i = 0, sprCount = GetGraphicInstance()->GetSpriteCount(SPRL_MISSION); i < sprCount; i++)
    {
        CPGISprite* lSprite = GetGraphicInstance()->GetSpriteByIndex(i, SPRL_MISSION);

        if (!lSprite)
        {
            LOG(FATAL) << "CIGMission::Serialize - Sprite is null, this will be a problem.";
            continue;
        }

        // path read from the file are relative to the game directory
        // they must be completed with the game path in order to read sprites
        // when using the physfs filesystem, there's no need to worry about full path
        CPString lFullPath = lSprite->GetFileName();

        out.beginElement("sprite");
        out.createAttribute("path", lFullPath.c_str());
        out.createAttribute("w", lSprite->GetAnimCount());
        out.createAttribute("h", lSprite->GetAnimTypes());
        out.endElement(); /**< sprite */
    }

    out.endElement(); /**< sprites */

    out.endElement(); /**< mission */

    // write the map
    mMap->Serialize(out);

    // writing objects - delegating to a CIGSerBaseObjectList
    //CIGSerBaseObjectList(GetObjects(MOL_ALLOBJECTS)).Serialize(out);

    VLOG(5) << ".Mission Serialize Done";
}
//---------------------------------------------------------------------------

void CIGMission::DeSerialize(BaseDeSerializer& in)
{
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

