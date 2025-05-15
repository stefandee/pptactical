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

#include "PPIG_Mission.h"
#include "config.h"
#include "PPIG_ObjChopper.h"
#include "PP_Stdlib.h"
#include "PP_Timer.h"
#include "PPIG_SubSysOscilator.h"
#include "PPIG_SubSysTactics.h"
#include "PP_Graphic.h"
//---------------------------------------------------------------------------
#ifdef PPT_COMPILER_BORLAND
#pragma package(smart_init)
#endif


CIGChopperUnit::CIGChopperUnit() : CIGAircraft()
{
  VLOG(9) << "CIGChopperUnit::CIGChopperUnit";

  SetSpeed(CPCounter(12));

  SetActionMask((1<<MA_STOP)|
                (1<<MA_MOVE)|
                (1<<MA_ATTACK)|
                (1<<MA_PATROL)|
                (1<<MA_GUARD)|
                (1<<MA_SUPPRESS_FIRE)|
                (1<<MA_HOLD)|
                (1<<MA_LAND));

  SetLogicDimX(1);
  SetLogicDimY(1);

  // subsystems
  CreatePropulsion();
  mBody       = new CIGSubSysBody(this);
  mSensors    = new CIGSubSysSensors(this);
  //mVisual     = new CIGSubSysVisual(this);
  mWeapons    = new CIGSubSysChopperWeapons(this);
  mOscilator  = new CIGSubSysOscilator(this);

  // visual setup
  //mVisual->SetMaxScanRange(20);
  //mVisual->SetFOV(120);

  // oscilator setup
  mOscilator->SetUpdateEnabled(false);

  SetUpdateTimePeriod(gfRandom(800) + 400);

  SetCoordZ(64);

  VLOG(9) << "CIGChopperUnit::CIGChopperUnit - over";
}
//---------------------------------------------------------------------------

void CIGChopperUnit::CreatePropulsion()
{
  mPropulsion = new CIGSubSysCPropulsion(this);
}
//---------------------------------------------------------------------------

CIGChopperUnit::~CIGChopperUnit()
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

  //if (mVisual)
  //{
  //  delete mVisual;
  //  mVisual = 0;
  //}

  if (mOscilator)
  {
    delete mOscilator;
    mOscilator = 0;
  }
}
//---------------------------------------------------------------------------

void CIGChopperUnit::Update()
{
  // update critical subsystems
  mPropulsion->Update();
  mBody->Update();
  mWeapons->Update();
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
          LOG(ERROR) << "CIGChopperUnit::Update - fail to compile script";
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

void CIGChopperUnit::Paint(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel)
{
  if (GetVisible() == true)
  {
    if (GetSprite() != NULL)
    {
       // paint the shadow first
       GetGraphicInstance()->PaintSprite((GetCoord().x + ptViewPort.x - ptWindow.x)/zoomLevel,
                                  (GetCoord().y + ptViewPort.y - ptWindow.y)/zoomLevel,
                                  GetSprite(),
                                  mPropulsion->GetAnimCounter()(),
                                  1);

       // paint the body
       GetGraphicInstance()->PaintSprite((GetCoord().x + ptViewPort.x - ptWindow.x)/zoomLevel,
                                  (GetCoord().y + ptViewPort.y - ptWindow.y)/zoomLevel - GetCoordZ() + mOscilator->GetCurrentValue(),
                                  GetSprite(),
                                  mPropulsion->GetAnimCounter()(),
                                  mPropulsion->GetActionAnimTypes(mPropulsion->GetCurrentActionIndex()) + mPropulsion->GetActionStepCounters(mPropulsion->GetCurrentActionIndex())());
    }
  }



  /*
       // test code for pixel map
       CPRect animRect = GetSprite()->GetAnimRect(mPropulsion->GetAnimCounter()(), mPropulsion->GetActionAnimTypes(mPropulsion->GetCurrentActionIndex()) + mPropulsion->GetActionStepCounters(mPropulsion->GetCurrentActionIndex())());

       for(int x = animRect.left; x < animRect.right; x++)
       {
         for(int y = animRect.top; y < animRect.bottom; y++)
         {
           int pixel = GetGraphicInstance()->GetPixel(GetSprite()->GetSurface(), CPPoint(x, y));

           if (pixel != 0)
           {
             GetGraphicInstance()->Rectangle(NULL,
                                        CPRect(
                                          (GetCoord().x + x - animRect.left + ptViewPort.x - ptWindow.x),
                                          (GetCoord().y + y - animRect.top + ptViewPort.y - ptWindow.y) - GetCoordZ() + mOscilator->GetCurrentValue(),
                                          (GetCoord().x + x - animRect.left + ptViewPort.x - ptWindow.x) + 1,
                                          (GetCoord().y + y - animRect.top + ptViewPort.y - ptWindow.y) - GetCoordZ() + mOscilator->GetCurrentValue() + 1
                                          ),
                                        0xFFFFFF);
           }
         }
       }

  */

  // test code for drawing the boundaries (aka paint rect)
  /*
  CPRect animRect = GetSprite()->GetAnimRect(mPropulsion->GetAnimCounter()(), mPropulsion->GetActionAnimTypes(mPropulsion->GetCurrentActionIndex()) + mPropulsion->GetActionStepCounters(mPropulsion->GetCurrentActionIndex())());

  GetGraphicInstance()->SetColor(0xFF00FF00);
  GetGraphicInstance()->Rectangle(
    CPRect(
      GetCoord().x + ptViewPort.x - ptWindow.x,
      GetCoord().y + ptViewPort.y - ptWindow.y - GetCoordZ(),
      GetCoord().x + animRect.right - animRect.left + ptViewPort.x - ptWindow.x,
      GetCoord().y + animRect.bottom - animRect.top + ptViewPort.y - ptWindow.y - GetCoordZ()
    )
  );
  */

  CPRect r = GetPaintRect();
  r.left += ptViewPort.x - ptWindow.x;
  r.right += ptViewPort.x - ptWindow.x;
  r.top += ptViewPort.y - ptWindow.y;
  r.bottom += ptViewPort.y - ptWindow.y;

  GetGraphicInstance()->SetColor(0xFFFFFFFF);
  GetGraphicInstance()->Rectangle(r);

  // test code - draws the (view) scan
  /*
  int lColumn = mPropulsion->GetAnimCounter()();

  GetGraphicInstance()->Line(
    0,
    CPPoint(GetCoord().x + GetAircraftUnifiedData().mObsPoints[0].mMountPoints[lColumn].x + (ptViewPort.x - ptWindow.x)/zoomLevel,
            GetCoord().y + GetAircraftUnifiedData().mObsPoints[0].mMountPoints[lColumn].y - GetCoordZ() + (ptViewPort.y - ptWindow.y)/zoomLevel),
    CPPoint(GetCoord().x + GetAircraftUnifiedData().mObsPoints[0].mMountPoints[lColumn].x + (ptViewPort.x - ptWindow.x)/zoomLevel + mVisuals[0]->GetScanRange() * MAPCELLX * cos(mVisuals[0]->GetScanAngle() * PI / 180),
            GetCoord().y + GetAircraftUnifiedData().mObsPoints[0].mMountPoints[lColumn].y - GetCoordZ() - mVisuals[0]->GetScanRange() * MAPCELLY * sin(mVisuals[0]->GetScanAngle() * PI / 180)  + (ptViewPort.y - ptWindow.y)/zoomLevel),
    0xFFFFFF
  );
  */
}
//---------------------------------------------------------------------------

CPRect CIGChopperUnit::GetPaintRect()
{
  if (!GetSprite())
  {
    return CPRect(-1, -1, -1, -1);
  }

  /*CPRect animRect = GetSprite()->GetAnimRect(mPropulsion->GetAnimCounter()(),
                                             mPropulsion->GetActionAnimTypes(mPropulsion->GetCurrentActionIndex()) + mPropulsion->GetActionStepCounters(mPropulsion->GetCurrentActionIndex())());
                                             */

  return GetSprite()->GetPaintRect
  (
    CPPoint(GetCoord().x, GetCoord().y - GetCoordZ() + mOscilator->GetCurrentValue()),
    mPropulsion->GetAnimCounter()(),
    mPropulsion->GetActionAnimTypes(mPropulsion->GetCurrentActionIndex()) + mPropulsion->GetActionStepCounters(mPropulsion->GetCurrentActionIndex())()
  );
}
//---------------------------------------------------------------------------

bool CIGChopperUnit::IsDerivedFrom (CPString className)
{
  return ((CIGAircraft::IsDerivedFrom(className)) || (className == CIGChopperUnit::ClassName()));
}
//---------------------------------------------------------------------------

void CIGChopperUnit::NoteInLogicMap()
{
  NoteInLogicMap(GetLogicCoord());
}
//---------------------------------------------------------------------------

void CIGChopperUnit::NoteInLogicMap(CPPoint Point)
{
  // we may notify on the ground only if the z coord is 0
  // otherwise, the chopper is in mid-air and we need not
  // to do the notification
  if (GetCoordZ() == 0)
  {
    CIGAircraft::NoteInLogicMap(Point);
  }
}
//---------------------------------------------------------------------------

bool CIGChopperUnit::CanBePlaced(CPPoint _point)
{
  if (GetCoordZ() > 0)
  {
    return true;
  }

  return CanLand(_point);
}
//---------------------------------------------------------------------------

// _point is in real and map coordinates
// picking makes use of the sprite information
bool CIGChopperUnit::Pick(CPPoint _point)
{
  CPRect paintRect = GetPaintRect();

  // check if _point is outside the unit
  if (paintRect.left > _point.x ||
      _point.x > paintRect.right ||
      _point.y < paintRect.top ||
      _point.y > paintRect.bottom)
  {
    return false;
  }

  // no sprite - no pixels -> bail out
  if (!GetSprite())
  {
    return false;
  }

  // now check the pixels
  CPRect animRect = GetSprite()->GetAnimRect(mPropulsion->GetAnimCounter()(), mPropulsion->GetActionAnimTypes(mPropulsion->GetCurrentActionIndex()) + mPropulsion->GetActionStepCounters(mPropulsion->GetCurrentActionIndex())());

  // TODO query the sprite collision map
  TColor32 pixel = 0xFFFFFFFF; //GetGraphicInstance()->GetPixel(GetSprite()->GetSurface(), CPPoint(animRect.left + _point.x - paintRect.left, animRect.top + _point.y - paintRect.top));

  return (pixel != 0);
}
//---------------------------------------------------------------------------

void CIGChopperUnit::AddAction(TIGSubSysActionData actionData)
{
  switch (actionData.mType)
  {
    case OA_ATTACK:
    case OA_ATTACKGROUND:
      mWeapons->AddAction(actionData);
      break;

    case OA_STOP:
      mPropulsion->AddAction(actionData);
      break;

    case OA_TACTIC_MOVE:
    case OA_TACTIC_ATTACK:
    case OA_TACTIC_ATTACKGROUND:
    case OA_TACTIC_PATROL:
      mTactics->AddAction(actionData);
      break;

    case OA_TACTIC_STOP:
      mPropulsion->CancelCurrentAction();
      mWeapons->CancelCurrentAction();
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

void CIGChopperUnit::AddAction (CIGSubSysAction *Action)
{
  switch (Action->GetType())
  {
    case OA_ATTACK:
    case OA_ATTACKGROUND:
      mWeapons->AddAction(Action);
      break;

    case OA_STOP:
      mPropulsion->AddAction(Action);
      break;

    case OA_TACTIC_MOVE:
    case OA_TACTIC_ATTACK:
    case OA_TACTIC_ATTACKGROUND:
    case OA_TACTIC_PATROL:
      mTactics->AddAction(Action);
      break;

    case OA_TACTIC_STOP:
      mPropulsion->CancelCurrentAction();
      mWeapons->CancelCurrentAction();
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

void CIGChopperUnit::AddActionWithPriority (CIGSubSysAction *Action)
{
  switch (Action->GetType())
  {
    case OA_ATTACK:
    case OA_ATTACKGROUND:
      mWeapons->AddActionWithPriority(Action);
      break;

    case OA_STOP:
      mPropulsion->AddActionWithPriority(Action);
      break;

    case OA_TACTIC_MOVE:
    case OA_TACTIC_ATTACK:
    case OA_TACTIC_ATTACKGROUND:
    case OA_TACTIC_PATROL:
      mTactics->AddActionWithPriority(Action);
      break;

    case OA_TACTIC_STOP:
      mPropulsion->CancelCurrentAction();
      mWeapons->CancelCurrentAction();
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

void CIGChopperUnit::SetAction (TIGSubSysActionData actionData)
{
  switch (actionData.mType)
  {
    case OA_ATTACK:
    case OA_ATTACKGROUND:
      mWeapons->SetAction(actionData);
      break;

    case OA_STOP:
      mPropulsion->SetAction(actionData);
      break;

    case OA_TACTIC_MOVE:
    case OA_TACTIC_ATTACK:
    case OA_TACTIC_ATTACKGROUND:
    case OA_TACTIC_PATROL:
      mTactics->SetAction(actionData);
      break;

    case OA_TACTIC_STOP:
      mPropulsion->CancelCurrentAction();
      mWeapons->CancelCurrentAction();
      mTactics->SetAction(actionData);
      break;

    case OA_MOVE:
    case OA_PATROL:
      mWeapons->CancelCurrentAction();
      mPropulsion->SetAction(actionData);
      break;

    default:
      mTactics->CancelCurrentAction();
      mPropulsion->SetAction(actionData);
  }
}
//---------------------------------------------------------------------------

void CIGChopperUnit::SetSenseCounter(int _v)
{
  mPropulsion->SetSenseCounter(_v);
}
//---------------------------------------------------------------------------

int CIGChopperUnit::GetSenseCounter()
{
  return mPropulsion->GetSenseCounter();
}
//---------------------------------------------------------------------------

void CIGChopperUnit::SetOscilatorUpdateEnabled(bool _v)
{
  mOscilator->SetUpdateEnabled(_v);
}
//---------------------------------------------------------------------------

void CIGChopperUnit::UpdateView(int _value)
{
    for(unsigned int i = 0, visualsSize =  mVisuals.size(); i < visualsSize; i++)
    {
        mVisuals[i]->Update();
        mVisuals[i]->UpdateView(_value);
    }
}
//---------------------------------------------------------------------------

void CIGChopperUnit::SetAircraftUnifiedData(TVehicleUnified& value)
{
  CIGAircraft::SetAircraftUnifiedData(value);

  mWeapons->GetUpdateCounters(OA_ATTACK).SetMaxValue(value.wep1_firerate * 10);
  mWeapons->GetUpdateCounters(OA_ATTACKGROUND).SetMaxValue(value.wep1_firerate * 10);

  // at startup, the weapons are ready to fire
  mWeapons->GetUpdateCounters(OA_ATTACK).Set(value.wep1_firerate * 10);
  mWeapons->GetUpdateCounters(OA_ATTACKGROUND).Set(value.wep1_firerate * 10);

  // setup the oscillator
  mOscilator->SetUpdateEnabled(GetCoordZ() > 0);
  mOscilator->SetAmplitude(2.0);
  mOscilator->SetStep(1.0);
  mOscilator->SetCurrentValue(gfRandom(2) - 1);
}
//---------------------------------------------------------------------------

bool CIGChopperUnit::Can(int _capability)
{
    if (_capability == CAN_GUARD)
    {
        return true;
    }

    return CIGAircraft::Can(_capability);
}
//---------------------------------------------------------------------------

float CIGChopperUnit::GetMaxSpeed (void)
{
    // TODO max speed is hard-coded for now, when the editors can be compiled again, re-do this part
    //return GetAircraftUnifiedData().maxSpeed;
    return 8;
}

//---------------------------------------------------------------------------
//
// Class serialize/deserialize section
//
//---------------------------------------------------------------------------
void CIGChopperUnit::Serialize(PP::Stream& _a)
{
  // parent class serialize
  CIGAircraft::Serialize(_a);

  // subsys serialize
  mPropulsion->Serialize(_a);
  //mVisual->Serialize(_a);
  mWeapons->Serialize(_a);
  //mTurret->Serialize(_a);
  //mBody->Serialize(_a);
  //mSensors->Serialize(_a);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGChopperUnit::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void CIGChopperUnit::DeSerialize(PP::Stream& _a)
{
  // parent class deserialize
  CIGAircraft::DeSerialize(_a);

  // subsys serialize
  mPropulsion->DeSerialize(_a);
  //mVisual->DeSerialize(_a);
  mWeapons->DeSerialize(_a);
  //mTurret->DeSerialize(_a);
  //mBody->DeSerialize(_a);
  //mSensors->DeSerialize(_a);

  // TODO: fix, see why these counters have wrong values at serialization...
  //mPropulsion->GetSenseCounter().SetMaxValue(7);
  //mPropulsion->GetAnimCounter().SetMaxValue(7);

  // just temporary here
  // TODO: find a way to improve this set - the counters
  // will get reset after this and after loading, things look pretty bad
  //SetAircraftUnifiedData(GetAircraftUnifiedData());

  // only temporary
  //mVisual->SetScanAngle(gDirAngle[mPropulsion->GetSenseCounter()] * 180.0 / PI);

  //SetLogicDimX(4);
  //SetLogicDimY(4);

  // test code crash
  // SetCurrentHP(1);
}
//---------------------------------------------------------------------------


