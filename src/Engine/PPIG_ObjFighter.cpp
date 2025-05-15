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

#include "PPIG_ObjFighter.h"
#include "PP_Stdlib.h"
#include "PP_Timer.h"
#include "PPIG_SubSysTactics.h"
#include "PP_Graphic.h"
//-------------------------------------------------------------------------------------------------

#ifdef __BORLANDC__
    #pragma package(smart_init)
#endif

CIGFighterUnit::CIGFighterUnit() : CIGAircraft()
{
    VLOG(9) << "CIGFighterUnit::CIGFighterUnit";
    SetSpeed(CPCounter(16));
    SetActionMask((1 << MA_STOP) | (1<<MA_MOVE) | (1<<MA_ATTACK) | (1<<MA_PATROL) |
                  (1 << MA_GUARD) | (1<<MA_HOLD) | (1<<MA_SUPPRESS_FIRE));

    SetLogicDimX(1);
    SetLogicDimY(1);

    m_Propulsion = new CIGSubSysFPropulsion(this);
    mBody       = new CIGSubSysBody(this);
    mSensors    = new CIGSubSysSensors(this);
    m_Weapons    = new CIGSubSysFighterWeapons(this);
    //mVisual     = new CIGSubSysVisual(this);

    //mVisual->SetMaxScanRange(20);
    //mVisual->SetFOV(120);

    SetUpdateTimePeriod(gfRandom(800) + 400);
    SetCoordZ(64);
    VLOG(9) << "CIGFighterUnit::CIGFighterUnit: constructed";
}
//-------------------------------------------------------------------------------------------------

CIGFighterUnit::~CIGFighterUnit()
{
    VLOG(9) << "CIGFighterUnit::~CIGFighterUnit";
    delete m_Propulsion; m_Propulsion   = 0;
    delete mBody;       mBody         = 0;
    //delete mVisual;     mVisual       = 0;
    delete mSensors;    mSensors      = 0;
    delete m_Weapons;    m_Weapons      = 0;
    VLOG(2) << "CIGFighterUnit::~CIGFighterUnit: destructed";
}
//-------------------------------------------------------------------------------------------------

bool CIGFighterUnit::IsDerivedFrom(CPString className)
{
    return (CIGAircraft::IsDerivedFrom(className) || (className == CIGFighterUnit::ClassName()));
}
//-------------------------------------------------------------------------------------------------

void CIGFighterUnit::InitCsl()
{
}
//-------------------------------------------------------------------------------------------------

void CIGFighterUnit::InitHandlers()
{
}
//-------------------------------------------------------------------------------------------------

void CIGFighterUnit::AddAction(TIGSubSysActionData actionData)
{
  switch (actionData.mType)
  {
    case OA_ATTACK:
    case OA_ATTACKGROUND:
      m_Weapons->AddAction(actionData);
      break;

    case OA_TACTIC_MOVE:
    case OA_TACTIC_ATTACK:
    case OA_TACTIC_ATTACKGROUND:
    case OA_TACTIC_PATROL:
      mTactics->AddAction(actionData);
      break;

    case OA_TACTIC_STOP:
      mTactics->AddAction(actionData);
      m_Weapons->CancelCurrentAction();
      break;

    case OA_MOVE:
    case OA_PATROL:
      m_Propulsion->AddAction(actionData);
      break;

    case OA_STOP:
      m_Weapons->AddAction(actionData);
      break;

    default:
      mTactics->CancelCurrentAction();
      m_Propulsion->AddAction(actionData);
  }
}
//-------------------------------------------------------------------------------------------------

void CIGFighterUnit::AddAction (CIGSubSysAction *Action)
{
  switch (Action->GetType())
  {
    case OA_ATTACK:
    case OA_ATTACKGROUND:
      m_Weapons->AddAction(Action);
      break;

    case OA_TACTIC_MOVE:
    case OA_TACTIC_ATTACK:
    case OA_TACTIC_ATTACKGROUND:
    case OA_TACTIC_PATROL:
      mTactics->AddAction(Action);
      break;

    case OA_TACTIC_STOP:
      mTactics->AddAction(Action);
      m_Weapons->CancelCurrentAction();
      break;

    case OA_MOVE:
    case OA_PATROL:
      m_Propulsion->AddAction(Action);
      break;

    case OA_STOP:
      m_Weapons->AddAction(Action);
      break;

    default:
      mTactics->CancelCurrentAction();
      m_Propulsion->AddAction(Action);
  }
}
//-------------------------------------------------------------------------------------------------

void CIGFighterUnit::AddActionWithPriority (CIGSubSysAction *Action)
{
  switch (Action->GetType())
  {
    case OA_ATTACK:
    case OA_ATTACKGROUND:
      m_Weapons->AddActionWithPriority(Action);
      break;

    case OA_TACTIC_MOVE:
    case OA_TACTIC_ATTACK:
    case OA_TACTIC_ATTACKGROUND:
    case OA_TACTIC_PATROL:
      mTactics->AddActionWithPriority(Action);
      break;

    case OA_TACTIC_STOP:
      mTactics->AddActionWithPriority(Action);
      m_Weapons->CancelCurrentAction();
      break;

    case OA_MOVE:
    case OA_PATROL:
      m_Propulsion->AddActionWithPriority(Action);
      break;

    case OA_STOP:
      m_Weapons->AddActionWithPriority(Action);
      break;

    default:
      mTactics->CancelCurrentAction();
      m_Propulsion->AddActionWithPriority(Action);
  }
}
//-------------------------------------------------------------------------------------------------

void CIGFighterUnit::SetAction (TIGSubSysActionData actionData)
{
  switch (actionData.mType)
  {
    case OA_ATTACK:
    case OA_ATTACKGROUND:
      m_Weapons->SetAction(actionData);
      break;

    case OA_TACTIC_MOVE:
    case OA_TACTIC_ATTACK:
    case OA_TACTIC_ATTACKGROUND:
    case OA_TACTIC_PATROL:
      mTactics->SetAction(actionData);
      break;

    case OA_TACTIC_STOP:
      mTactics->SetAction(actionData);
      m_Weapons->CancelCurrentAction();
      break;

    case OA_MOVE:
    case OA_PATROL:
      m_Propulsion->SetAction(actionData);
      break;

    case OA_STOP:
      m_Weapons->SetAction(actionData);
      break;

    default:
      mTactics->CancelCurrentAction();
      m_Propulsion->SetAction(actionData);
  }
}
//-------------------------------------------------------------------------------------------------

void CIGFighterUnit::SetSenseCounter(int _v)
{
  m_Propulsion->SetSenseCounter(_v);
}
//---------------------------------------------------------------------------

int CIGFighterUnit::GetSenseCounter()
{
  return m_Propulsion->GetSenseCounter();
}
//---------------------------------------------------------------------------

//TODO: Nu se poate muta Update() si Paint() in CIGAircraft() -- sau chiar mai sus in ierarhie ?
//m_* pot fi CIGSubSysBase
void CIGFighterUnit::Update()
{
  m_Propulsion->Update();
  mBody->Update();
  m_Weapons->Update();

  // solve the initial state where the aircraft has no action
  // the move action is set to the aircraft
  // this happens when dropped in mapeditor and map loaded in engine
  if (!m_Propulsion->GetCurrentAction())
  {
     TIGSubSysActionData lTmpActionData;
     lTmpActionData.mType        = OA_MOVE;
     lTmpActionData.mStartPoint  = GetCoord();
     lTmpActionData.mTargetPoint = GetCoord();

     SetAction(lTmpActionData);
  }

  //Unit Dead or Update Time is not elapsed
  if (GetIsDead() || (GetTimerInstance()->GetTime() - GetUpdateTime() < (unsigned)GetUpdateTimePeriod()))
      return;

  SetUpdateTime(GetTimerInstance()->GetTime());

  for(unsigned int i = 0 ; i < mVisuals.size(); i++)
  {
    mVisuals[i]->Update();
  }

  mTactics->Update();
  mSensors->Update();

  if (mValidCsl)
  {
      try
      {
          mCompiler->call(mModuleName, "main");
      }
      catch (const ZException &)
      {
          LOG(FATAL) << "CIGFighterUnit::Update(): failed to compile script";
      }
  }
  else
  {
    //logWriteLn("CIGFighterUnit::Update(): CSL NOT VALID");
  }

  // update the fog-of-war
  UpdateView(2);
}
//-------------------------------------------------------------------------------------------------

void CIGFighterUnit::Paint(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel)
{
    if (true == GetVisible() && (NULL != GetSprite()))
    {
        //logOut << m_Propulsion->GetAnimCounter() << " - " << m_Propulsion->GetActionAnimTypes(m_Propulsion->GetCurrentActionIndex() + m_Propulsion->GetActionStepCounters(m_Propulsion->GetCurrentActionIndex())) << "\n";

        // paint the shadow
        GetGraphicInstance()->PaintSprite((GetCoord().x + ptViewPort.x - ptWindow.x) / zoomLevel,
                                          (GetCoord().y + ptViewPort.y - ptWindow.y) / zoomLevel - GetCoordZ(),
                                           GetSprite(),
                                           m_Propulsion->GetAnimCounter()(),
                                           m_Propulsion->GetActionAnimTypes(m_Propulsion->GetCurrentActionIndex() + m_Propulsion->GetActionStepCounters(m_Propulsion->GetCurrentActionIndex()))
        );

        // paint the sprite
        GetGraphicInstance()->PaintSprite((GetCoord().x + ptViewPort.x - ptWindow.x) / zoomLevel,
                                          (GetCoord().y + ptViewPort.y - ptWindow.y) / zoomLevel,
                                           GetSprite(),
                                           m_Propulsion->GetAnimCounter()(),
                                           1
        );
    }

  // test code - draws the (view) scan
  /*
  int lColumn = m_Propulsion->GetAnimCounter()();

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
//-------------------------------------------------------------------------------------------------

CPRect CIGFighterUnit::GetPaintRect()
{
  if (!GetSprite())
  {
    return CPRect(-1, -1, -1, -1);
  }

  return GetSprite()->GetPaintRect
  (
    CPPoint(GetCoord().x, GetCoord().y - GetCoordZ()),
    m_Propulsion->GetAnimCounter()(),
    m_Propulsion->GetActionAnimTypes(m_Propulsion->GetCurrentActionIndex() + m_Propulsion->GetActionStepCounters(m_Propulsion->GetCurrentActionIndex()))
  );
}
//---------------------------------------------------------------------------


void CIGFighterUnit::Serialize(PP::Stream& _a)
{
  CIGAircraft::Serialize(_a);
  m_Propulsion->Serialize(_a);
}
//-------------------------------------------------------------------------------------------------

unsigned __int32 CIGFighterUnit::DataSize()
{
    return 0; //????
}
//-------------------------------------------------------------------------------------------------

void CIGFighterUnit::DeSerialize(PP::Stream& _a)
{
    CIGAircraft::DeSerialize(_a);
    m_Propulsion->DeSerialize(_a);

    // TODO: fix, see why these counters have wrong values at serialization...
    //m_Propulsion->GetSenseCounter().SetMaxValue(7);
    //m_Propulsion->GetAnimCounter().SetMaxValue(7);

    SetAircraftUnifiedData(GetAircraftUnifiedData());

    SetLogicDimX(1);
    SetLogicDimY(1);

    // temporary
    //m_Visual->SetMaxScanRange(20);
    //mVisual->SetScanAngle(gDirAngle[m_Propulsion->GetSenseCounter()] * 180.0 / PI);
}

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
