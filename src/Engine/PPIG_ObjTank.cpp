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
#include <math.h>
#pragma hdrstop

//#if defined(__BORLANDC__) || defined(__SUNPRO_CC) || defined (__GNUG__)
//  #include <strstream>
//#else
//  #include "strstrea.h"	// TMN: Note use of obsolete header
//#endif

#ifdef PPT_COMPILER_MICROSOFT
#include <strstream>
#else
#include <sstream>
#endif
#include "config.h"
#include "PPIG_ObjTank.h"
#include "Equipment.h"
#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "PPIG_SubSysTurret.h"
#include "PPIG_SubSysVPropulsion.h"
#include "PPIG_SubSysBody.h"
#include "PPIG_SubSysSensors.h"
#include "PP_Stdlib.h"
#include "ZMathLib.h"
#include "PPIG_CScreen.h"
#include "PP_Timer.h"
#include "WeaponRulesLib.h"
#include "PPIG_SubSysTactics.h"
//---------------------------------------------------------------------------

// Descriere   :
// Param       :
// Rezultat    :
// Comentarii  : take care that modifying the counters means that you have to
//               (re)compile both the engine and the map editor and that
//               old maps (that have different values for the counters) need
//               to be re-saved in the map editor. Otherwise at deserialization
//               the counters will receive their older values
CIGTankUnit::CIGTankUnit()
{
  VLOG(9) << "CIGTankUnit::CIGTankUnit";

  SetSpeed(CPCounter(8));
  SetActionMask((1<<MA_STOP)|
                (1<<MA_MOVE)|
                (1<<MA_ATTACK)|
                (1<<MA_PATROL)|
                (1<<MA_GUARD)|
                (1<<MA_HOLD)|
                (1<<MA_SUPPRESS_FIRE));

  SetLogicDimX(1);
  SetLogicDimY(1);

  // subsystems
  mTurret     = new CIGSubSysTurret(this);
  mPropulsion = new CIGSubSysVPropulsion(this);
  mBody       = new CIGSubSysBody(this);
  mSensors    = new CIGSubSysSensors(this);
  //mVisual     = new CIGSubSysVisual(this);
  mOscilator  = new CIGSubSysOscilator(this);

  // oscilator setup
  mOscilator->SetUpdateEnabled(false);

  // visual setup
  //mVisual->SetMaxScanRange(8);
  //mVisual->SetFOV(80);

  SetUpdateTimePeriod(gfRandom(800) + 400);

  VLOG(9) << "CIGTankUnit::CIGTankUnit - over";
}
//---------------------------------------------------------------------------

CIGTankUnit::~CIGTankUnit()
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

  if (mTurret)
  {
    delete mTurret;
    mTurret = 0;
  }

  //if (mVisual)
  //{
  //  delete mVisual;
  //  mVisual = 0;
  //}
  ClearVisuals();

  if (mOscilator)
  {
    delete mOscilator;
    mOscilator = 0;
  }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if this class is derived from className
 parameters : className
---------------------------------------------------------------------------*/
bool CIGTankUnit::IsDerivedFrom (CPString className)
{
   return ((CIGVehicle::IsDerivedFrom(className)) || (className == CIGTankUnit::ClassName()));
}
//---------------------------------------------------------------------------

void CIGTankUnit::AddAction(TIGSubSysActionData actionData)
{
  switch (actionData.mType)
  {
    case OA_ATTACK:
    case OA_ATTACKGROUND:
      mTurret->AddAction(actionData);
      break;

    case OA_STOP:
      mTurret->AddAction(actionData);
      mPropulsion->AddAction(actionData);
      break;

    case OA_TACTIC_MOVE:
    case OA_TACTIC_ATTACK:
    case OA_TACTIC_ATTACKGROUND:
    case OA_TACTIC_PATROL:
      mTactics->AddAction(actionData);
      break;

    case OA_TACTIC_STOP:
      mTurret->CancelCurrentAction();
      mPropulsion->CancelCurrentAction();
      mTactics->AddAction(actionData);
      break;

    case OA_MOVE:
    case OA_PATROL:
      mPropulsion->AddAction(actionData);
      break;

    default:
      mTactics->CancelCurrentAction();
      mPropulsion->AddAction(actionData);
  }
}
//---------------------------------------------------------------------------

void CIGTankUnit::AddAction (CIGSubSysAction *Action)
{
  switch (Action->GetType())
  {
    case OA_ATTACK:
    case OA_ATTACKGROUND:
      mTurret->AddAction(Action);
      break;

    case OA_STOP:
      mTurret->AddAction(Action);
      mPropulsion->AddAction(Action);
      break;

    case OA_TACTIC_MOVE:
    case OA_TACTIC_ATTACK:
    case OA_TACTIC_ATTACKGROUND:
    case OA_TACTIC_PATROL:
      mTactics->AddAction(Action);
      break;

    case OA_TACTIC_STOP:
      mTurret->CancelCurrentAction();
      mPropulsion->CancelCurrentAction();
      mTactics->AddAction(Action);
      break;

    case OA_MOVE:
    case OA_PATROL:
      mPropulsion->AddAction(Action);
      break;

    default:
      mTactics->CancelCurrentAction();
      mPropulsion->AddAction(Action);
  }
}
//---------------------------------------------------------------------------

void CIGTankUnit::AddActionWithPriority (CIGSubSysAction *Action)
{
  switch (Action->GetType())
  {
    case OA_ATTACK:
    case OA_ATTACKGROUND:
      mTurret->AddActionWithPriority(Action);
      break;

    case OA_STOP:
      mTurret->AddActionWithPriority(Action);
      mPropulsion->AddActionWithPriority(Action);
      break;

    case OA_TACTIC_MOVE:
    case OA_TACTIC_ATTACK:
    case OA_TACTIC_ATTACKGROUND:
    case OA_TACTIC_PATROL:
      mTactics->AddActionWithPriority(Action);
      break;

    case OA_TACTIC_STOP:
      mTurret->CancelCurrentAction();
      mPropulsion->CancelCurrentAction();
      mTactics->AddActionWithPriority(Action);
      break;

    case OA_MOVE:
    case OA_PATROL:
      mPropulsion->AddActionWithPriority(Action);
      break;

    default:
      mTactics->CancelCurrentAction();
      mPropulsion->AddActionWithPriority(Action);
  }
}
//---------------------------------------------------------------------------

void CIGTankUnit::SetAction (TIGSubSysActionData actionData)
{
  switch (actionData.mType)
  {
    case OA_ATTACK:
    case OA_ATTACKGROUND:
      mTurret->SetAction(actionData);
      break;

    case OA_STOP:
      mTurret->SetAction(actionData);
      mPropulsion->SetAction(actionData);
      break;

    case OA_TACTIC_MOVE:
    case OA_TACTIC_ATTACK:
    case OA_TACTIC_ATTACKGROUND:
    case OA_TACTIC_PATROL:
      mTactics->SetAction(actionData);
      break;

    case OA_TACTIC_STOP:
      mTurret->CancelCurrentAction();
      mPropulsion->CancelCurrentAction();
      mTactics->SetAction(actionData);
      break;

    case OA_MOVE:
    case OA_PATROL:
      mPropulsion->SetAction(actionData);
      break;

    default:
      mTactics->CancelCurrentAction();
      mPropulsion->SetAction(actionData);
  }
}
//---------------------------------------------------------------------------

void CIGTankUnit::SetVehicleUnifiedData(TVehicleUnified& value)
{
  CIGVehicle::SetVehicleUnifiedData(value);

  // check and enable the primary/secondary weapons abilities
  // check for movement type and set up the propulsion type accordingly

  // setting behavior for weapons
  SetProjectileSprite(value.wep1_projectile);
  SetEffectSprite(value.wep1_effectbitmap);
  SetAfterEffectSprite(value.wep1_aftereffectbitmap);
  SetProjectilesPerShot(value.wep1_projectiles_per_shot);
  SetSoundOnFire(value.wep1_onfire);
  SetSoundOnHit(value.wep1_onhit);
  SetRange(value.wep1_maxrange);
  //SetEffectSprite("explode001.png");//value.wep1_effectbitmap);

  // set the turning param for the turret
  mTurret->GetUpdateCounters(OA_ROTATE).SetMaxValue(value.mTurretTurnRate);
  mTurret->GetUpdateCounters(OA_ATTACK).SetMaxValue(value.wep1_firerate * 10);
  mTurret->GetUpdateCounters(OA_ATTACKGROUND).SetMaxValue(value.wep1_firerate * 10);

  // at startup, the weapons are ready to fire and turret ready to turn
  mTurret->GetUpdateCounters(OA_ATTACK).Set(value.wep1_firerate * 10);
  mTurret->GetUpdateCounters(OA_ATTACKGROUND).Set(value.wep1_firerate * 10);
  mTurret->GetUpdateCounters(OA_ROTATE).Set(value.mTurretTurnRate);

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

bool CIGTankUnit::CanBePlaced(CPPoint _point)
{
  if (GetLogicCoord().x != (float)GetCoord().x / MAPCELLX)
  {
    return true;
  }

  if (GetLogicCoord().y != (float)GetCoord().y / MAPCELLY)
  {
    return true;
  }

  int lReferenceAltitude = GetMissionInstance()->GetMap()->GetAltitude(_point.x, _point.y);

  for (int i = _point.x; i < _point.x + 1/*GetLogicDimX()*/; i++)
  {
      for (int j = _point.y; j < _point.y + 1/*GetLogicDimY()*/; j++)
      {
        if (!GetMissionInstance()->IsGroundPlaceEmpty(this, i, j))
        {
          return false;
        }

        // if the vehicle is not a hover and tile type is water
        if (GetVehicleUnifiedData().movement != 2 && GetMissionInstance()->GetMap()->GetTileType(i, j) == MTT_WATER)
        {
          return false;
        }

        // if the vehicle is not a hover and the altitude differnce is higher than 10 units (1/3 of a tile, somewhere around 0.66 meters)
        if (GetVehicleUnifiedData().movement != 2 && abs(GetMissionInstance()->GetMap()->GetAltitude(i, j) - lReferenceAltitude) > 10)
        {
          return false;
        }
      }
  }

  return true;
}
//---------------------------------------------------------------------------

void CIGTankUnit::SetSprite(CPSprite * value)
{
  CIGVehicle::SetSprite(value);
  mPropulsion->GetAnimCounter().GetThis()->SetMaxValue(GetSprite()->GetAnimCount()-1);
}
//---------------------------------------------------------------------------

void CIGTankUnit::Update()
{
  CIGVehicle::Update();

  // critical subsystems update
  mTurret->Update();
  mPropulsion->Update();
  mOscilator->Update();
  mTactics->Update();

  // if the unit is dead, there's nothing to update
  if (GetIsDead())
  {
    return;
  }

  if (GetTimerInstance()->GetTime() - GetUpdateTime() >= (unsigned)GetUpdateTimePeriod())
  {
      SetUpdateTime(GetTimerInstance()->GetTime());

      // non-critical subsystems update
      //mSensors->Update();
      mBody->Update();

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
        }
      }
  }

  // update the fog-of-war
  UpdateView(2);
}
//---------------------------------------------------------------------------

void CIGTankUnit::UpdateView(int _value)
{
    // these subsystems don't need fast update
    for(unsigned int i = 0, visualsSize =  mVisuals.size(); i < visualsSize; i++)
    {
        mVisuals[i]->Update();
        mVisuals[i]->UpdateView(_value);
    }
}
//---------------------------------------------------------------------------

void CIGTankUnit::Paint(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel)
{
  //CIGUnit::Paint(ptViewPort, ptWindow, Width, Height, zoomLevel);
  if (GetVisible() == true)
  {
    if (GetSprite() != NULL)
    {
       // paint the tank body
       GetGraphicInstance()->PaintSprite(
                                  (GetCoord().x + ptViewPort.x - ptWindow.x)/zoomLevel,
                                  (GetCoord().y + ptViewPort.y - ptWindow.y)/zoomLevel + mOscilator->GetCurrentValue(),
                                  GetSprite(),
                                  mPropulsion->GetAnimCounter()(),
                                  mPropulsion->GetActionAnimTypes(mPropulsion->GetCurrentActionIndex()) + mPropulsion->GetActionStepCounters(mPropulsion->GetCurrentActionIndex())());

       // paint the turret only if the vehicle lives - the "dead" sprite needs no turret
       if (!GetIsDead())
       {
         GetGraphicInstance()->PaintSprite(
                                  (GetCoord().x + ptViewPort.x - ptWindow.x)/zoomLevel + GetVehicleUnifiedData().body[mPropulsion->GetAnimCounter()()].x - GetVehicleUnifiedData().turret[mTurret->GetSense()].x,
                                  (GetCoord().y + ptViewPort.y - ptWindow.y)/zoomLevel + GetVehicleUnifiedData().body[mPropulsion->GetAnimCounter()()].y - GetVehicleUnifiedData().turret[mTurret->GetSense()].y + mOscilator->GetCurrentValue(),
                                  GetSprite(),
                                  mTurret->GetSense(),
                                  1);
       }
    }
  }

  // test code - draws the (view) scan
  /*
  int lColumn = mPropulsion->GetAnimCounter()();

  GetGraphicInstance()->Line(NULL,
                             CPPoint(GetCoord().x + GetVehicleUnifiedData().mObsPoints[0].mMountPoints[lColumn].x + (ptViewPort.x - ptWindow.x)/zoomLevel,
                                     GetCoord().y + GetVehicleUnifiedData().mObsPoints[0].mMountPoints[lColumn].y + (ptViewPort.y - ptWindow.y)/zoomLevel),
                             CPPoint(GetCoord().x + GetVehicleUnifiedData().mObsPoints[0].mMountPoints[lColumn].x + (ptViewPort.x - ptWindow.x)/zoomLevel + mVisuals[0]->GetScanRange() * MAPCELLX * cos(mVisuals[0]->GetScanAngle() * PI / 180),
                                     GetCoord().y + GetVehicleUnifiedData().mObsPoints[0].mMountPoints[lColumn].y - mVisuals[0]->GetScanRange() * MAPCELLY * sin(mVisuals[0]->GetScanAngle() * PI / 180)  + (ptViewPort.y - ptWindow.y)/zoomLevel),
                             0xFFFFFF
                             );
  */

  /*
  GetGraphicInstance()->Line(NULL,
                             CPPoint(GetCoord().x + MAPCELLX/2 + (ptViewPort.x - ptWindow.x)/zoomLevel, GetCoord().y + MAPCELLY/2 + (ptViewPort.y - ptWindow.y)/zoomLevel),
                             CPPoint(GetCoord().x + MAPCELLX/2 + (ptViewPort.x - ptWindow.x)/zoomLevel + mVisual->GetScanRange() * MAPCELLX * cos(mVisual->GetScanAngle() * PI / 180),
                                     GetCoord().y + MAPCELLY/2 - mVisual->GetScanRange() * MAPCELLY * sin(mVisual->GetScanAngle() * PI / 180)  + (ptViewPort.y - ptWindow.y)/zoomLevel),
                             0xFFFFFF
                             );*/

}
//---------------------------------------------------------------------------

CPRect CIGTankUnit::GetPaintRect()
{
  if (!GetSprite())
  {
    return CPRect(-1, -1, -1, -1);
  }

  return GetSprite()->GetPaintRect
  (
    CPPoint(GetCoord().x, GetCoord().y - GetCoordZ() + mOscilator->GetCurrentValue()),
    mPropulsion->GetAnimCounter()(),
    mPropulsion->GetActionAnimTypes(mPropulsion->GetCurrentActionIndex()) + mPropulsion->GetActionStepCounters(mPropulsion->GetCurrentActionIndex())()
  );
}
//---------------------------------------------------------------------------

void CIGTankUnit::InitScriptingInterface()
{
  CIGUnit::InitScriptingInterface();

  addModuleFunc("Unload(const x, const y, const unitIndex)", mHandleUnload);
  addModuleFunc("UnloadAll(const x, const y)", mHandleUnloadAll);

  addModuleFunc("GetTurretSense()", mHandleGetTurretSense);
  addModuleFunc("SetTurretSense(const angle)", mHandleSetTurretSense);
}
//---------------------------------------------------------------------------

void CIGTankUnit::InitHandlers()
{
  CIGUnit::InitHandlers();

  mHandleUnload       .set(this, &CIGTankUnit:: mcUnload);
  mHandleUnloadAll    .set(this, &CIGTankUnit:: mcUnloadAll);

  mHandleGetTurretSense .set(this, &CIGTankUnit:: mcGetTurretSense);
  mHandleSetTurretSense .set(this, &CIGTankUnit:: mcSetTurretSense);
}
//---------------------------------------------------------------------------

/*
void CIGTankUnit::TurnToSense(int wantedSense)
{
  SetSenseCounter(GetSenseCounter().ToValue(wantedSense)());
}
//---------------------------------------------------------------------------

void CIGTankUnit::SetSenseCounter(int value)
{
  CIGUnit::SetSenseCounter(value);
  SetAnimCounter(value);
}
//---------------------------------------------------------------------------

void CIGTankUnit::SetSenseCounter(CPCounter &value)
{
  CIGUnit::SetSenseCounter(value);
  SetAnimCounter(value());
}
//---------------------------------------------------------------------------
*/

void CIGTankUnit::NoteStimul(CIGObjectStimul &stimul)
{
  if (GetIsDead() == false)
  {
    mBody->OnStimul(stimul);
    mSensors->OnStimul(stimul);
  }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: computes the way (probably posts a requests to path server)
---------------------------------------------------------------------------*/
void CIGTankUnit::ComputeWay()
{
  TIGPropulsion lPropulsion = TIGP_TRACKED;

  CIGVehicle::ComputeWay();

  switch(GetVehicleUnifiedData().movement)
  {
    case 1:
      lPropulsion = TIGP_TRACKED;
      break;

    case 2:
    case 3:
      lPropulsion = TIGP_HOVER;
      break;

    case 6:
      lPropulsion = TIGP_WHEEL;
      break;
  }

  GetPathServerInstance()->PostRequest(
    this,
    GetLogicCoord(),
    mPropulsion->GetCurrentAction()->GetData().mTargetPoint,
    GetLogicDimX(), // TODO: update the path algorithm to take into account units larger than 1x1 tiles - GetLogicDimX(),
    GetVehicleUnifiedData().mMaxDownSlope,
    GetVehicleUnifiedData().mMaxUpSlope,
    lPropulsion
  );
}
//---------------------------------------------------------------------------

void CIGTankUnit::Die()
{
  TIGSubSysActionData lTmpActionData;
  lTmpActionData.mType = OA_STOP;

  // if the turret is firing and we don't stop it
  // it will continue to fire even if the object is dead
  // so make sure to stop the turret before dying :)
  mTurret->SetAction(lTmpActionData);

  // perform the default dying
  CIGVehicle::Die();
}
//---------------------------------------------------------------------------


ZString CIGTankUnit::mcUnload(ZCsl* csl)
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

ZString CIGTankUnit::mcUnloadAll(ZCsl* csl)
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

ZString CIGTankUnit::mcGetTurretSense(ZCsl* csl)
{
  return gDirAngle[mTurret->GetSense()] * 180 / PI;
}
//---------------------------------------------------------------------------

ZString CIGTankUnit::mcSetTurretSense(ZCsl* csl)
{
  int lDegreeAngle = csl->get("angle").asInt(), lSense = 0;

  // clamp to range
  if (lDegreeAngle < 0)
  {
    lDegreeAngle = 0;
  }

  lDegreeAngle = lDegreeAngle % 360;

  // convert the angle to frame-based sense (0..7)
  float lMin = (float)(2*PI);
  float lAngle = PI * (lDegreeAngle / 180.0);

  for (int i=0; i<8; i++)
  {
    if (fabs(lAngle - gDirAngle[i]) < lMin)
    {
      lMin = fabs(lAngle - gDirAngle[i]);
      lSense = i;
    }
  }

  // make the turret turn to the wanted sense (this will change, the sense
  // will be an action parameter (mData[0] most certainly)
  mTurret->SetWantedSense(lSense);

  TIGSubSysActionData lTmpActionData;

  lTmpActionData.mType          = OA_ROTATE;
  mTurret->SetAction(lTmpActionData);

  return "";
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
// Class serialize/deserialize section
//
//---------------------------------------------------------------------------

void CIGTankUnit::Serialize(PP::Stream& _a)
{
  // parent class serialize
  CIGVehicle::Serialize(_a);

  // subsys serialize
  mPropulsion->Serialize(_a);
  mTurret->Serialize(_a);
  mBody->Serialize(_a);
  mSensors->Serialize(_a);

  // visuals serialize
  int lVisualsCount = mVisuals.size();

  _a.writeInt(&lVisualsCount);

  for(unsigned int i = 0; i < mVisuals.size(); i++)
  {
    mVisuals[i]->Serialize(_a);
  }
}
//---------------------------------------------------------------------------

unsigned __int32 CIGTankUnit::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void CIGTankUnit::DeSerialize(PP::Stream& _a)
{
  // parent class deserialize
  CIGVehicle::DeSerialize(_a);

  // subsys serialize
  mPropulsion->DeSerialize(_a);
  mTurret->DeSerialize(_a);
  mBody->DeSerialize(_a);
  mSensors->DeSerialize(_a);

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

  // just temporary here
  SetVehicleUnifiedData(GetVehicleUnifiedData());

  //SetLogicDimX(1);
  //SetLogicDimY(1);

  // only temporary
  //mVisual->SetScanAngle(gDirAngle[mPropulsion->GetSenseCounter()] * 180.0 / PI);
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

