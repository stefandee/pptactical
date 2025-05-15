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
 application: Pure Power In Game

 description: Class In Game Infantry Unit
 last modify: 13 06 00
        by grabX
---------------------------------------------------------------------------*/

//#include <string.h>
#include <cstring>
#include <math.h>

//#if defined(__BORLANDC__) || defined(__SUNPRO_CC) || defined (__GNUG__)
//  #include <strstream>
//#else
//  #include "strstrea.h"	// TMN: Note use of obsolete header
//#endif

#pragma hdrstop

#include "config.h"
#include "basic3d.h"
#include "PP_Graphic.h"
#include "Paths.h"
#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "PPIG_EngineDef.h"
#include "PP_Stdlib.h"
#include "PP_Timer.h"
#include "PlayerProfile.h"

#include "PPIG_SubSysPropulsion.h"
#include "PPIG_SubSysSensors.h"
//#include "PPIG_SubSysVisual.h"
#include "PPIG_SubSysBody.h"
#include "PPIG_SubSysTactics.h"

#include "PPIG_ObjInfantry.h"

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CIGInfantryUnit::CIGInfantryUnit() : CIGUnit()
{
    VLOG(9) << "CIGInfantryUnit::CIGInfantryUnit";
    SetSpeed(CPCounter(4));

    SetActionMask((1<<MA_STOP)|
                  (1<<MA_MOVE)|
                  (1<<MA_ATTACK)|
                  (1<<MA_PATROL)|
                  (1<<MA_GUARD)|
                  (1<<MA_HOLD)|
                  (1<<MA_CAPTURE)|
                  (1<<MA_SUPPRESS_FIRE)|
                  (1<<MA_TAKE_COVER));

    // script stuff
    mModuleName = "IU-C"; // infantry unit compiler module name

    SetLogicDimX(1);
    SetLogicDimY(1);

    SetUpdateTimePeriod(gfRandom(800) + 400);

    // subsystems init
    mPropulsion = new CIGSubSysPropulsion(this);
    mBody       = new CIGSubSysBody(this);
    mSensors    = new CIGSubSysSensors(this);

    /*
    mVisual     = new CIGSubSysVisual(this);
    // visual setup
    mVisual->SetMaxScanRange(10);
    mVisual->SetFOV(100);
    */

    VLOG(9) << "CIGInfantryUnit::CIGInfantryUnit - over";
}
//---------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CIGInfantryUnit::~CIGInfantryUnit()
{
    if (mPropulsion)
    {
        delete mPropulsion;
        mPropulsion = 0;
    }

    if (mBody)
    {
        delete mBody;
        mBody = 0;
    }

    if (mSensors)
    {
        delete mSensors;
        mSensors = 0;
    }

    ClearVisuals();

    if (mValidCsl)
    {
        delete mCompiler;
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: Inf Unified Data
---------------------------------------------------------------------------*/
inline void CIGInfantryUnit::SetInfUnifiedData(TInfantryUnified& value)
{
    mInfUnifiedData = value;

    if (value.type == 0)    // special
    {
        SetName(CPString(value.name) + " " + CPString(value.surname));
    }
    else                    // ordinary
    {
        SetName(CPString(value.kind));
    }

    //
    // Armor setup
    //
    SetMaxHP(value.hp);
    SetCurrentHP(value.hp);
    SetBulletRes(value.bullet_res);
    SetEnergyRes(value.nrg_res);
    SetFireRes(value.fire_res);
    SetPlasmaRes(value.plasma_res);
    SetExplodeRes(value.proj_res);

    //
    // Weapons setup
    //
    SetRange(value.range);
    SetDamage(value.damage);
    SetWepType(value.weptype);
    SetEffectSprite(value.projectile);
    SetProjectileSprite(value.effect_bitmap);
    SetAfterEffectSprite(value.aftereffect_bitmap);
    SetSoundOnFire(value.onfire);
    SetSoundOnHit(value.onhit);
    SetProjectilesPerShot(value.projectiles_per_shot);

    //
    // Propulsion setup
    //
    mPropulsion->GetUpdateCounters(OA_ATTACK).SetMaxValue(value.fire_rate * 10);
    mPropulsion->GetUpdateCounters(OA_ATTACKGROUND).SetMaxValue(value.fire_rate * 10);

    //
    // Observation Points setup
    //
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
 description: Inf Data - soldier
---------------------------------------------------------------------------*/
void CIGInfantryUnit::SetInfData(struct _Infantry& value)
{
    // by Karg: mInfData = value;
    memcpy(&mInfData, &value, sizeof(struct _Infantry));
    // SetName(CPString(value.name) + " " + CPString(value.surname));
    SetName(CPString(mInfData.name) + " " + CPString(mInfData.surname));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: Inf Weapon Data
---------------------------------------------------------------------------*/
inline void CIGInfantryUnit::SetInfWeaponData(struct _IWeapon& value)
{
    mInfWeaponData = value;
    SetRange(value.maxRange);
    SetDamage(value.damage);
    SetWepType(value.type);
    mPropulsion->GetUpdateCounters(OA_ATTACK).SetMaxValue(value.fire_rate * 10);
    mPropulsion->GetUpdateCounters(OA_ATTACKGROUND).SetMaxValue(value.fire_rate * 10);
    SetEffectSprite(value.effect_bitmap);
    SetProjectileSprite(value.projectile);
    SetSoundOnFire(value.onfire);
    SetSoundOnHit(value.onhit);
}
//---------------------------------------------------------------------------

void CIGInfantryUnit::SetSprite(CPSprite * value)
{
    CIGUnit::SetSprite(value);
    mPropulsion->GetAnimCounter().GetThis()->SetMaxValue(GetSprite()->GetAnimCount()-1);
}
//---------------------------------------------------------------------------

void CIGInfantryUnit::SetSenseCounter(int _v)
{
    mPropulsion->SetSenseCounter(_v);
}
//---------------------------------------------------------------------------

int CIGInfantryUnit::GetSenseCounter()
{
    return mPropulsion->GetSenseCounter();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: computes the way (probably posts a requests to path server)
---------------------------------------------------------------------------*/
void CIGInfantryUnit::ComputeWay()
{
    CIGUnit::ComputeWay();

    CPPoint lPoint = mPropulsion->GetCurrentAction()->GetData().mTargetPoint;

    GetPathServerInstance()->PostRequest(this, GetLogicCoord(), mPropulsion->GetCurrentAction()->GetData().mTargetPoint, GetLogicDimX(), GetInfUnifiedData().mMaxDownSlope, GetInfUnifiedData().mMaxUpSlope, TIGP_FOOT);
}
//---------------------------------------------------------------------------

void CIGInfantryUnit::UpdateView(int _value)
{
    // these subsystems don't need fast update
    for(unsigned int i = 0, visualsSize =  mVisuals.size(); i < visualsSize; i++)
    {
        mVisuals[i]->Update();
        mVisuals[i]->UpdateView(_value);
    }
}
//---------------------------------------------------------------------------

TUnitLook CIGInfantryUnit::GetLookAt()
{
    if (mVisuals.size() == 0)
    {
        return UL_NOTHING;
    }

    return mVisuals[0]->GetLookAt();
}
//---------------------------------------------------------------------------

int CIGInfantryUnit::GetScanAngle()
{
    if (mVisuals.size() == 0)
    {
        return -1;
    }

    return mVisuals[0]->GetScanAngle();
}
//---------------------------------------------------------------------------

void CIGInfantryUnit::SetScanAngle(int _v)
{
    if (mVisuals.size() == 0)
    {
        return;
    }

    mVisuals[0]->SetScanAngle(_v);
}
//---------------------------------------------------------------------------

int CIGInfantryUnit::GetScanRange()
{
    if (mVisuals.size() == 0)
    {
        return -1;
    }

    return mVisuals[0]->GetScanRange();
}
//---------------------------------------------------------------------------

void CIGInfantryUnit::SetScanRange(int _v)
{
    if (mVisuals.size() == 0)
    {
        return;
    }

    mVisuals[0]->SetScanRange(_v);
}
//---------------------------------------------------------------------------

void CIGInfantryUnit::AddAction(TIGSubSysActionData actionData)
{
    switch(actionData.mType)
    {
    case OA_TACTIC_MOVE:
    case OA_TACTIC_ATTACK:
    case OA_TACTIC_ATTACKGROUND:
    case OA_TACTIC_PATROL:
        mTactics->AddAction(actionData);
        break;

    case OA_MOVE:
    case OA_ATTACK:
    case OA_ATTACKGROUND:
    case OA_PATROL:
        mPropulsion->AddAction(actionData);
        break;

    case OA_STOP:
        mPropulsion->AddAction(actionData);
        break;

    case OA_TACTIC_STOP:
        mTactics->AddAction(actionData);
        //mPropulsion->AddAction(actionData);
        mPropulsion->CancelCurrentAction();
        break;

    default:
        // stop the tactics system while performing other actions
        // this may change, depending on the application
        mTactics->CancelCurrentAction();
        mPropulsion->AddAction(actionData);
        break;
    }
}
//---------------------------------------------------------------------------

void CIGInfantryUnit::AddAction (CIGSubSysAction *Action)
{
    switch(Action->GetType())
    {
    case OA_TACTIC_MOVE:
    case OA_TACTIC_ATTACK:
    case OA_TACTIC_ATTACKGROUND:
    case OA_TACTIC_PATROL:
        mTactics->AddAction(Action);
        break;

    case OA_MOVE:
    case OA_ATTACK:
    case OA_ATTACKGROUND:
    case OA_PATROL:
        mPropulsion->AddAction(Action);
        break;

    case OA_TACTIC_STOP:
        mTactics->AddAction(Action);
        mPropulsion->CancelCurrentAction();
        break;

    case OA_STOP:
        mPropulsion->AddAction(Action);
        break;

    default:
        // stop the tactics system while performing other actions
        // this may change, depending on the application
        mTactics->CancelCurrentAction();
        mPropulsion->AddAction(Action);
        break;
    }
}
//---------------------------------------------------------------------------

void CIGInfantryUnit::AddActionWithPriority (CIGSubSysAction *Action)
{
    switch(Action->GetType())
    {
    case OA_TACTIC_MOVE:
    case OA_TACTIC_ATTACK:
    case OA_TACTIC_ATTACKGROUND:
    case OA_TACTIC_PATROL:
        mTactics->AddActionWithPriority(Action);
        break;

    case OA_MOVE:
    case OA_ATTACK:
    case OA_ATTACKGROUND:
    case OA_PATROL:
        mPropulsion->AddActionWithPriority(Action);
        break;

    case OA_STOP:
        mPropulsion->AddActionWithPriority(Action);
        break;

    case OA_TACTIC_STOP:
        mTactics->AddActionWithPriority(Action);
        mPropulsion->CancelCurrentAction();
        break;

    default:
        // stop the tactics system while performing other actions
        // this may change, depending on the application
        mTactics->CancelCurrentAction();
        mPropulsion->AddActionWithPriority(Action);
        break;
    }
}
//---------------------------------------------------------------------------

void CIGInfantryUnit::SetAction (TIGSubSysActionData actionData)
{
    switch(actionData.mType)
    {
    case OA_TACTIC_MOVE:
    case OA_TACTIC_ATTACK:
    case OA_TACTIC_ATTACKGROUND:
    case OA_TACTIC_PATROL:
        mTactics->SetAction(actionData);
        break;

    case OA_MOVE:
    case OA_ATTACK:
    case OA_ATTACKGROUND:
    case OA_PATROL:
        mPropulsion->SetAction(actionData);
        break;

    case OA_STOP:
        mPropulsion->SetAction(actionData);
        break;

    case OA_TACTIC_STOP:
        mTactics->SetAction(actionData);
        mPropulsion->CancelCurrentAction();
        break;

    default:
        // stop the tactics system while performing other actions
        // this may change, depending on the application
        mTactics->CancelCurrentAction();
        mPropulsion->SetAction(actionData);
        break;
    }
}
//---------------------------------------------------------------------------

void CIGInfantryUnit::Update()
{
    //CIGUnit::Update();

    // update critical subsystems
    mPropulsion->Update();
    mBody->Update();
    mTactics->Update();

    // if the unit is dead, there's nothing to update
    if (GetIsDead())
    {
        return;
    }

    if (GetTimerInstance()->GetTime() - GetUpdateTime() >= (unsigned)GetUpdateTimePeriod())
    {
        SetUpdateTime(GetTimerInstance()->GetTime());

        mSensors->Update();

        // execute the script
        if (mValidCsl)
        {
            try
            {
                mCompiler->call(mModuleName, "main");
            }
            catch(const ZException& /*err*/)
            {
                //logWriteLn("CIGInfantryUnit::Update - error(s) calling main function");
                //for (int i = 0; i < err.count(); i++)
                //{
                //  logWriteLn(((char*)err[i]));
                //}
                LOG(FATAL) << "fail to compile script for infantry";
            }
        }
        else
        {
            //logWriteLn("CSL NOT VALID");
        }
    }

    // update the fog-of-war each frame
    UpdateView(2);
}
//---------------------------------------------------------------------------

void CIGInfantryUnit::Paint(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel)
{
    //CIGUnit::Paint(ptViewPort, ptWindow, Width, Height, zoomLevel);
    // the paint should go up to objbase with a subsys avatar...sometimes :)
    if (!GetVisible())
    {
        return;
    }

        if (GetSprite() != NULL)
        {
            GetGraphicInstance()->PaintSprite((GetCoord().x + ptViewPort.x - ptWindow.x)/zoomLevel,
                    (GetCoord().y + ptViewPort.y - ptWindow.y)/zoomLevel,
                    GetSprite(),
                    mPropulsion->GetAnimCounter()(), mPropulsion->GetActionAnimTypes(mPropulsion->GetCurrentActionIndex()) +
                    mPropulsion->GetActionStepCounters(mPropulsion->GetCurrentActionIndex())());
        }

    // test code - draws the (view) scan
    if (GetProfileInstance()->GetShowViewDir() && !GetIsDead())
    {
        int lColumn = mPropulsion->GetAnimCounter()();

        int scanRange = mVisuals[0]->GetScanRange();
        int scanAngle = mVisuals[0]->GetScanAngle();
        int fov = mVisuals[0]->GetFOV();

        if (CPString(GetInfUnifiedData().kind) == CPString("Rion Recon"))
        {
            VLOG(9) << "rion recon";
        }

        CPPoint dir = CPPoint(scanRange * MAPCELLX * cos(scanAngle * PI / 180),
                            - scanRange * MAPCELLY * sin(scanAngle * PI / 180));



        // test code to draw the view cone in a more realistic way. should be enhanced to support alpha blending
        GetGraphicInstance()->SetColor(0x40000000);
        GetGraphicInstance()->Pie(CPPoint(GetCoord().x + GetInfUnifiedData().mObsPoints[0].mMountPoints[lColumn].x + (ptViewPort.x - ptWindow.x)/zoomLevel,
                                          GetCoord().y + GetInfUnifiedData().mObsPoints[0].mMountPoints[lColumn].y + (ptViewPort.y - ptWindow.y)/zoomLevel),
                                  mVisuals[0]->GetScanRange() * MAPCELLX,
                                  CPPoint(mVisuals[0]->GetScanRange() * MAPCELLX * cos(mVisuals[0]->GetScanAngle() * PI / 180),
                                          mVisuals[0]->GetScanRange() * MAPCELLY * sin(mVisuals[0]->GetScanAngle() * PI / 180)),
                                  fov
                                 );

        GetGraphicInstance()->SetColor(0xFFFF0000);
        GetGraphicInstance()->Line(CPPoint(GetCoord().x + GetInfUnifiedData().mObsPoints[0].mMountPoints[lColumn].x + (ptViewPort.x - ptWindow.x)/zoomLevel,
                                           GetCoord().y + GetInfUnifiedData().mObsPoints[0].mMountPoints[lColumn].y + (ptViewPort.y - ptWindow.y)/zoomLevel),
                                   CPPoint(GetCoord().x + GetInfUnifiedData().mObsPoints[0].mMountPoints[lColumn].x + (ptViewPort.x - ptWindow.x)/zoomLevel + mVisuals[0]->GetScanRange() * MAPCELLX * cos(mVisuals[0]->GetScanAngle() * PI / 180),
                                           GetCoord().y + GetInfUnifiedData().mObsPoints[0].mMountPoints[lColumn].y + mVisuals[0]->GetScanRange() * MAPCELLY * sin(mVisuals[0]->GetScanAngle() * PI / 180)  + (ptViewPort.y - ptWindow.y)/zoomLevel)
                                   );
    }


    /*
    GetGraphicInstance()->TextOut(
      NULL,
      GetCoord().x + (ptViewPort.x - ptWindow.x)/zoomLevel,
      GetCoord().y + MAPCELLY/2 + (ptViewPort.y - ptWindow.y)/zoomLevel,
      CPString(GetLogicCoord().x) + "," + CPString(GetLogicCoord().y),
      TA_LEFT
      );
    */

    /*
    GetGraphicInstance()->TextOut(
      NULL,
      GetCoord().x + (ptViewPort.x - ptWindow.x)/zoomLevel,
      GetCoord().y + MAPCELLY/2 + (ptViewPort.y - ptWindow.y)/zoomLevel,
      CPString(GetCoord().x) + "," + CPString(GetCoord().y),
      TA_LEFT
      );
    */
}
//---------------------------------------------------------------------------

CPRect CIGInfantryUnit::GetPaintRect()
{
    if (!GetSprite())
    {
        return CPRect(-1, -1, -1, -1);
    }

    return GetSprite()->GetPaintRect
           (
               CPPoint(GetCoord().x, GetCoord().y),
               mPropulsion->GetAnimCounter()(),
               mPropulsion->GetActionAnimTypes(mPropulsion->GetCurrentActionIndex()) + mPropulsion->GetActionStepCounters(mPropulsion->GetCurrentActionIndex())()
           );
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if this class is derived from className
 parameters : className
---------------------------------------------------------------------------*/
bool CIGInfantryUnit::IsDerivedFrom (CPString className)
{
    return ((CIGUnit::IsDerivedFrom(className)) || (className == CIGInfantryUnit::ClassName()));
}
//---------------------------------------------------------------------------

void CIGInfantryUnit::NoteStimul(CIGObjectStimul &stimul)
{
    if (GetIsDead() == false)
    {
        mBody->OnStimul(stimul);
        mSensors->OnStimul(stimul);
    }
}
//---------------------------------------------------------------------------

bool CIGInfantryUnit::Can(int _capability)
{
    if (_capability == CAN_GUARD)
    {
        return true;
    }

    return CIGUnit::Can(_capability);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
// Class serialize/deserialize section
//
//---------------------------------------------------------------------------

void CIGInfantryUnit::Serialize(PP::Stream& _a)
{
    // parent class serialize
    CIGUnit::Serialize(_a);

    // subsys serialize
    mPropulsion->Serialize(_a);
    mBody->Serialize(_a);
    mSensors->Serialize(_a);

    int lVisualsCount = mVisuals.size();

    _a.writeInt(&lVisualsCount);

    for(unsigned int i = 0; i < mVisuals.size(); i++)
    {
        mVisuals[i]->Serialize(_a);
    }

    // serialize
    //_a.write(&mInfUnifiedData, sizeof(TInfantryUnified));
    mInfUnifiedData.Serialize(_a);

    //_a.write(&mInfData, sizeof(struct _Infantry));
    mInfData.Serialize(_a);

    //_a.write(&mInfArmorData, sizeof(struct _IArmor));
    mInfArmorData.Serialize(_a);

    //_a.write(&mInfWeaponData, sizeof(struct _IWeapon));
    mInfWeaponData.Serialize(_a);

    // TODO: serialize infantry ZCsl
}
//---------------------------------------------------------------------------

unsigned __int32 CIGInfantryUnit::DataSize()
{
    return 0;
}
//---------------------------------------------------------------------------

void CIGInfantryUnit::DeSerialize(PP::Stream& _a)
{
    // parent class deserialize
    CIGUnit::DeSerialize(_a);

    // subsys serialize
    mPropulsion->DeSerialize(_a);
    mBody->DeSerialize(_a);
    mSensors->DeSerialize(_a);

    // visuals subsystems
    ClearVisuals();

    int lVisualsCount;

    _a.readInt(&lVisualsCount);

    for(int i = 0; i < lVisualsCount; i++)
    {
        CIGSubSysVisual* lVisual = new CIGSubSysVisual(this);

        lVisual->DeSerialize(_a);

        mVisuals.push_back(lVisual);
    }

    // deserialize
    //_a.read(&mInfUnifiedData, sizeof(TInfantryUnified));
    mInfUnifiedData.DeSerialize(_a);

    //_a.read(&mInfData, sizeof(struct _Infantry));
    mInfData.DeSerialize(_a);

    //_a.read(&mInfArmorData, sizeof(struct _IArmor));
    mInfArmorData.DeSerialize(_a);

    //_a.read(&mInfWeaponData, sizeof(struct _IWeapon));
    mInfWeaponData.DeSerialize(_a);

    // TODO: deserialize infantry ZCsl

    // setup
    // TODO: we dont need these two atm; the "unified" model will be changed soon, in order to support inventory
    //SetInfWeaponData(mInfWeaponData);
    //SetInfArmorData(mInfArmorData);
    SetInfUnifiedData(mInfUnifiedData);

    // can be done, but it negates saving the scan angle to file, so might count as a bug
    // this should be done at editing time
    //SetScanAngle(gDirAngle[mPropulsion->GetSenseCounter()] * 180.0 / PI);
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif




