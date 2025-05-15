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
//#if defined(__BORLANDC__) || defined(__SUNPRO_CC) || defined (__GNUG__)
//  #include <strstream>
//#else
//  #include "strstrea.h"	// TMN: Note use of obsolete header
//#endif

#pragma hdrstop

#include "PPIG_ObjAircraft.h"

#if PPT_COMPILER_MICROSOFT
#include <strstream>
#else
#include <sstream>
#endif
#include "basic3d.h"
#include "basepath.h"
#include "PP_Graphic.h"
#include "Paths.h"
#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "PPIG_EngineDef.h"
#include "PP_Stdlib.h"
#include "ZMathLib.h"
#include "WeaponRulesLib.h"
#include "PPIG_CScreen.h"
#include "PPIG_ObjEffect.h"

CIGAircraft::CIGAircraft() : CIGUnit()
{
    VLOG(5) << "CIGAircraft::CIGAircraft - start";

    // i can see my house! :>
    SetLayer(MOL_AIROBJECTS);

    // script stuff
    mModuleName = "Air-C"; // aircraft compiler module name

    // default - derived classes will have to override this
    SetLogicDimX(1);
    SetLogicDimY(1);

    VLOG(5) << "CIGAircraft::CIGAircraft- over";
}
//---------------------------------------------------------------------------

CIGAircraft::~CIGAircraft()
{
    if (mValidCsl)
    {
        delete mCompiler;
    }

    ClearVisuals();
}
//---------------------------------------------------------------------------

bool CIGAircraft::IsDerivedFrom (CPString className)
{
    return ((CIGUnit::IsDerivedFrom(className)) || (className == CIGAircraft::ClassName()));
}
//---------------------------------------------------------------------------

void CIGAircraft::SetAircraftUnifiedData(TVehicleUnified& value)
{
    mAircraftUnifiedData = value;

    // setting behavior for weapons
    SetProjectileSprite(value.wep1_projectile);
    SetEffectSprite(value.wep1_effectbitmap);
    SetAfterEffectSprite(value.wep1_aftereffectbitmap);
    SetProjectilesPerShot(value.wep1_projectiles_per_shot);
    SetSoundOnFire(value.wep1_onfire);
    SetSoundOnHit(value.wep1_onhit);
    SetRange(value.wep1_maxrange);

    // set the turning param for the turret
    /*
    mTurret->GetUpdateCounters(OA_ROTATE).SetMaxValue(value.mTurretTurnRate);
    mTurret->GetUpdateCounters(OA_ATTACK).SetMaxValue(value.wep1_firerate * 10);
    mTurret->GetUpdateCounters(OA_ATTACKGROUND).SetMaxValue(value.wep1_firerate * 10);

    // setup the oscilator
    if (value.movement == 2)
    {
      mOscilator->SetUpdateEnabled(true);
      mOscilator->SetAmplitude(2.0);
      mOscilator->SetStep(1.0);
      mOscilator->SetCurrentValue(gfRandom(2) - 1);
    }
    else
    {
      mOscilator->SetUpdateEnabled(false);
    }
    */

    // setup basic variables
    SetMaxHP(value.hp);
    SetCurrentHP(value.hp);
    SetName(value.name);

    // setup the visual subsystems from observation points
    ClearVisuals();

    if (value.mObsPointsCount > 0)
    {
        for(int i = 0; i < value.mObsPointsCount; i++)
        {
            CIGSubSysVisual* lVisual = new CIGSubSysVisual(this);

            lVisual->SetMinScanRange(value.mObsPoints[i].mMinScanRange);
            lVisual->SetMaxScanRange(value.mObsPoints[i].mMaxScanRange);
            lVisual->SetFOV(value.mObsPoints[i].mFOV);
            lVisual->SetScanAngle(value.mObsPoints[i].mScanAngle);
            lVisual->SetUpdateEnabled(value.mObsPoints[i].mActive);

            for(int j = 0; j < 8; j++)
            {
                lVisual->SetMountPoint(j, value.mObsPoints[i].mMountPoints[j]);
            }

            mVisuals.push_back(lVisual);
        }
    }
    else
    {
        // there was no attempt to edit any observation point for this unit
        // simply provide a default one
        mVisuals.push_back(new CIGSubSysVisual(this));
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: since an aircraft cant be on the ground, do nothing
---------------------------------------------------------------------------*/
//void CIGAircraft::NoteInLogicMap()
//{
//}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: notes this object in the specified point
 parameters : Point - the beginning point (in logic coord)
---------------------------------------------------------------------------*/
//void CIGAircraft::NoteInLogicMap(CPPoint Point)
//{
//}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: an aircraft may always be placed in its layer (the air layer)
 parameters :
---------------------------------------------------------------------------*/
bool CIGAircraft::CanBePlaced(CPPoint _point)
{
    return true;
}
//---------------------------------------------------------------------------

bool CIGAircraft::CanLand(CPPoint _point)
{
    for (int i = _point.x; i < _point.x + GetLogicDimX(); i++)
    {
        for (int j = _point.y; j < _point.y + GetLogicDimY(); j++)
        {
            if (!GetMissionInstance()->IsGroundPlaceEmpty(this, i, j))
            {
                return false;
            }
        }
    }

    return true;
}
//---------------------------------------------------------------------------

void CIGAircraft::NoteStimul(CIGObjectStimul &stimul)
{
    if (GetIsDead() == false)
    {
        mBody->OnStimul(stimul);
        mSensors->OnStimul(stimul);
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns the object logic point closest to the target
 parameters : targetPoint - the wanted point in logic coord
 comment    : should be remade, it has a certain bug because the air unit
              has Z coord
---------------------------------------------------------------------------*/
CPPoint CIGAircraft::GetClosestPointTo(CPPoint targetPoint)
{
    /*
    CPPoint lRes = CPPoint(1000, 1000);

    for (int i=0; i < GetLogicDimX(); i++)
    {
       for (int j=0; j<GetLogicDimY(); j++)
            if(gfGetDist2D(lRes, targetPoint) > gfGetDist2D(CPPoint(i + GetLogicCoord().x, j + GetLogicCoord().y), targetPoint))
             lRes = CPPoint(i + GetLogicCoord().x,j + GetLogicCoord().y);
    }

    return lRes;
    */

    return CPPoint(GetLogicCoord().x + GetLogicDimX() / 2, GetLogicCoord().y + GetLogicDimY() / 2);
}
//---------------------------------------------------------------------------

void CIGAircraft::Die()
{
    CPString fxFileName = CPString(GetAircraftUnifiedData().mExplodeSprite);

    if (!fxFileName.isEmpty())
    {
        CPGISprite* lExplodeSprite = GetGraphicInstance()->AddSprite(CPString(PATH_EXPLODES_GFX) + fxFileName, 8, 2, SPRL_MISSION);

        if (!lExplodeSprite)
        {
            return;
        }

        // create dying effect
        for(int i = 0; i < GetAircraftUnifiedData().mExplodeCount; i++)
        {
            CIGEffectObject *lTargetEffect;
            lTargetEffect = new CIGEffectObject();
            lTargetEffect->SetAnimCyclesCount(1);
            lTargetEffect->SetSprite(lExplodeSprite);

            int lX = GetLogicCoord().x * MAPCELLX + gfRandom(GetDimX());
            int lY = GetLogicCoord().y * MAPCELLY + gfRandom(GetDimY());

            lTargetEffect->CenterInPoint(CPPoint(lX, lY));
            lTargetEffect->SetCoordZ(GetCoordZ() + gfRandom(16) - 8);

            (lTargetEffect->GetUpdateCounter().GetThis())->SetMaxValue(gfRandom(3) + 3);

            lTargetEffect->NoteInLayerMap();
            GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Add(lTargetEffect);  //???????
        }
    }

    // perform the default dying
    CIGUnit::Die();
}
//---------------------------------------------------------------------------

void CIGAircraft::InitScriptingInterface()
{
    VLOG(5) << "CIGAircraft::InitScriptingInterface";

    CIGUnit::InitScriptingInterface();

    addModuleFunc("Unload(const x, const y, const unitIndex)", mHandleUnload);
    addModuleFunc("UnloadAll(const x, const y)", mHandleUnloadAll);
    addModuleFunc("Land(const x, const y)", mHandleLand);
}
//---------------------------------------------------------------------------

void CIGAircraft::InitHandlers()
{
    CIGUnit::InitHandlers();

    mHandleUnload       .set(this, &CIGAircraft:: mcUnload);
    mHandleUnloadAll    .set(this, &CIGAircraft:: mcUnloadAll);
    mHandleLand         .set(this, &CIGAircraft:: mcLand);
}
//---------------------------------------------------------------------------

/*
void CIGAircraft::TurnToSense(int wantedSense)
{
  SetSenseCounter(GetSenseCounter().ToValue(wantedSense)());
}
//---------------------------------------------------------------------------

void CIGAircraft::SetSenseCounter(int value)
{
  CIGUnit::SetSenseCounter(value);
  SetAnimCounter(value);
}
//---------------------------------------------------------------------------

void CIGAircraft::SetSenseCounter(CPCounter &value)
{
  CIGUnit::SetSenseCounter(value);
  SetAnimCounter(value());
}
//---------------------------------------------------------------------------
*/

//---------------------------------------------------------------------------
//
// Implementation of script methods follows
//
//---------------------------------------------------------------------------
ZString CIGAircraft::mcUnload(ZCsl* csl)
{
    TIGSubSysActionData lTmpActionData;

    lTmpActionData.mType          = OA_UNLOAD;
    lTmpActionData.mTargetPoint.x = csl->get("x").asInt() * MAPCELLX;
    lTmpActionData.mTargetPoint.y = csl->get("y").asInt() * MAPCELLY;
    lTmpActionData.mData[0]       = csl->get("unitIndex").asInt();
    lTmpActionData.mTargetObject  = 0;

    if (lTmpActionData.mTargetPoint.x < 0)
    {
        return -1;
    }

    if (lTmpActionData.mTargetPoint.y < 0)
    {
        return -1;
    }

    if (lTmpActionData.mData[0] < 0)
    {
        return -1;
    }

    SetAction(lTmpActionData);

    return 1;
}
//---------------------------------------------------------------------------

ZString CIGAircraft::mcUnloadAll(ZCsl* csl)
{
    TIGSubSysActionData lTmpActionData;

    lTmpActionData.mType          = OA_UNLOAD_ALL;
    lTmpActionData.mTargetPoint.x = csl->get("x").asInt() * MAPCELLX;
    lTmpActionData.mTargetPoint.y = csl->get("y").asInt() * MAPCELLY;
    lTmpActionData.mTargetObject  = 0;

    if (lTmpActionData.mTargetPoint.x < 0)
    {
        return -1;
    }

    if (lTmpActionData.mTargetPoint.y < 0)
    {
        return -1;
    }

    SetAction(lTmpActionData);

    return 1;
}
//---------------------------------------------------------------------------

ZString CIGAircraft::mcLand(ZCsl* csl)
{
    TIGSubSysActionData lTmpActionData;

    lTmpActionData.mType          = OA_LAND;
    lTmpActionData.mTargetPoint.x = csl->get("x").asInt() * MAPCELLX;
    lTmpActionData.mTargetPoint.y = csl->get("y").asInt() * MAPCELLY;
    lTmpActionData.mTargetObject  = 0;

    if (lTmpActionData.mTargetPoint.x < 0)
    {
        return -1;
    }

    if (lTmpActionData.mTargetPoint.y < 0)
    {
        return -1;
    }

    SetAction(lTmpActionData);

    return 1;
}
//---------------------------------------------------------------------------

void CIGAircraft::PaintSelection (CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel)
{
    ptWindow.y += GetCoordZ();

    CIGUnit::PaintSelection(ptViewPort, ptWindow, Width, Height, zoomLevel);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
// Class serialize/deserialize section
//
//---------------------------------------------------------------------------
void CIGAircraft::Serialize(PP::Stream& _a)
{
    // parent class serialize
    CIGUnit::Serialize(_a);

    // write data
    //_a.write(&mAircraftUnifiedData, sizeof(TVehicleUnified));
    mAircraftUnifiedData.Serialize(_a);

    // visuals serialize
    int lVisualsCount = mVisuals.size();

    _a.writeInt(&lVisualsCount);

    for(unsigned int i = 0; i < mVisuals.size(); i++)
    {
        mVisuals[i]->Serialize(_a);
    }
}
//---------------------------------------------------------------------------

unsigned __int32 CIGAircraft::DataSize()
{
    return 0;
}
//---------------------------------------------------------------------------

void CIGAircraft::DeSerialize(PP::Stream& _a)
{
    // parent class deserialize
    CIGUnit::DeSerialize(_a);

    // read data
    //_a.read(&mAircraftUnifiedData, sizeof(TVehicleUnified));
    mAircraftUnifiedData.DeSerialize(_a);

    // visuals deserialize
    ClearVisuals();

    int lVisualsCount;

    _a.readInt(&lVisualsCount);

    for(int i = 0; i < lVisualsCount; i++)
    {
        CIGSubSysVisual* lVisual = new CIGSubSysVisual(this);

        lVisual->DeSerialize(_a);

        mVisuals.push_back(lVisual);
    }

    // setup
    SetAircraftUnifiedData(GetAircraftUnifiedData());
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

