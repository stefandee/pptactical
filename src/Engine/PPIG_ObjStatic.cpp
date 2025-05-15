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

 description: Class In Game Static Object
 last modify: 30 05 00
        by grabX
---------------------------------------------------------------------------*/

//#if defined(__BORLANDC__) || defined(__SUNPRO_CC) || defined (__GNUG__)
//  #include <strstream>
//#else
//  #include "strstrea.h"	// TMN: Note use of obsolete header
//#endif

#pragma hdrstop

#if PPT_COMPILER_MICROSOFT
// keeping compatibilty with msvc 6...blindly; if this wont work on msvc 6 please inform me
#if (_MSC_VER >= 1400)
  #include <sstream>
#else
  #include <strstream>
#endif
#else
#include <sstream>
#endif
#include "PPIG_ObjStatic.h"
#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "PPIG_SubSysBody.h"
#include "PPIG_CScreen.h"
#include "PP_Timer.h"
#include "PP_Stdlib.h"
#include "ZMathLib.h"
#include "WeaponRulesLib.h"
//-----------------------------------------------------------------------------

CIGStaticLayer::CIGStaticLayer()
{
  mCoord  = CPPoint(0, 0);
  mSprite = 0;

  mAnimCounter = 0;
  mAnimCounter.SetMaxValue(7);

  mAlpha = 1.;

  mFrameUpdateTime  = GetTimerInstance()->GetTime();
  mFrameUpdateDelay = 5;

  mCycleUpdateTime  = GetTimerInstance()->GetTime();
  mCycleUpdateDelay = 5;

  mEnabled = true;
}
//-----------------------------------------------------------------------------

void CIGStaticLayer::Update()
{
  if (!mEnabled)
  {
    return;
  }

  if (mAnimCounter.MaxReached())
  {
    if (GetTimerInstance()->GetTime() - GetCycleUpdateTime() < (unsigned)GetCycleUpdateDelay())
    {
      return;
    }

    mAnimCounter = 0;
  }

  if (GetTimerInstance()->GetTime() - GetFrameUpdateTime() < (unsigned)GetFrameUpdateDelay())
  {
    return;
  }

  SetFrameUpdateTime(GetTimerInstance()->GetTime());

  mAnimCounter++;

  if (mAnimCounter.MaxReached())
  {
    SetCycleUpdateTime(GetTimerInstance()->GetTime());
  }
}
//-----------------------------------------------------------------------------

void CIGStaticLayer::Paint(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel)
{
   if (GetVisible() == true)
   {
      if (GetSprite() != NULL)
      {
         GetGraphicInstance()->PaintSprite((mCoord.x + ptViewPort.x - ptWindow.x)/zoomLevel,
                                    (mCoord.y + ptViewPort.y - ptWindow.y)/zoomLevel,
                                    GetSprite(), mAnimCounter, 0);
      }
   }
}
//-----------------------------------------------------------------------------

CPSprite * CIGStaticLayer::GetSprite ()
{
  return mSprite;
}
//-----------------------------------------------------------------------------

void CIGStaticLayer::SetSprite (CPSprite * value)
{
  mSprite = value;
}
//-----------------------------------------------------------------------------

bool CIGStaticLayer::GetVisible()
{
  return (mAlpha == 1.);
}
//-----------------------------------------------------------------------------

void CIGStaticLayer::SetAnimCounter(int _v)
{
  mAnimCounter.SetMaxValue(_v);
  mAnimCounter = 0;
}
//-----------------------------------------------------------------------------

void CIGStaticLayer::Serialize(PP::Stream& _a)
{
  CSerPoint(mCoord).Serialize(_a);
  CSerCounter(mAnimCounter).Serialize(_a);
  int lFrameUpdateTime = GetTimerInstance()->GetTime() - mFrameUpdateTime;
  int lCycleUpdateTime = GetTimerInstance()->GetTime() - mCycleUpdateTime;

  _a.writeFloat(&mAlpha);
  _a.writeInt(&lFrameUpdateTime);
  _a.writeInt(&mFrameUpdateDelay);
  _a.writeInt(&lCycleUpdateTime);
  _a.writeInt(&mCycleUpdateDelay);
  _a.writeBool(&mEnabled);

  int lSpriteIndex = -1;

  if (mSprite)
  {
    lSpriteIndex = GetGraphicInstance()->GetSpriteIndex(mSprite, SPRL_MISSION);
  }

  _a.writeInt(&lSpriteIndex);
}
//-----------------------------------------------------------------------------

unsigned __int32 CIGStaticLayer::DataSize()
{
  return 0;
}
//-----------------------------------------------------------------------------

void CIGStaticLayer::DeSerialize(PP::Stream& _a)
{
  CSerPoint   lSerPoint;
  CSerCounter lSerCounter;
  int lSpriteIndex = -1;
  int lFrameUpdateTime, lCycleUpdateTime;

  lSerPoint.DeSerialize(_a);
  lSerCounter.DeSerialize(_a);

  _a.readFloat(&mAlpha);
  _a.readInt(&lFrameUpdateTime);
  _a.readInt(&mFrameUpdateDelay);
  _a.readInt(&lCycleUpdateTime);
  _a.readInt(&mCycleUpdateDelay);
  _a.readBool(&mEnabled);

  _a.readInt(&lSpriteIndex);

  // setup
  mCoord           = lSerPoint.Get();
  mAnimCounter     = lSerCounter.Get();
  mSprite          = GetGraphicInstance()->GetSpriteByIndex(lSpriteIndex, SPRL_MISSION);
  mFrameUpdateTime = GetTimerInstance()->GetTime() - lFrameUpdateTime;
  mCycleUpdateTime = GetTimerInstance()->GetTime() - lCycleUpdateTime;
}
//-----------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CIGStaticObject::CIGStaticObject() : CIGRealObject()
{
  SetLayer(MOL_GROUNDOBJECT);

  // subsystems
  mBody = new CIGSubSysBody(this);

  // layers
  mLayers = new CPList<CIGStaticLayer>(4);

  // script stuff
  mCompiler       = 0;
  mValidCsl       = false;
  mScriptObserver = 0;
  mModuleName     = "St-C"; // static compiler module name

  SetUpdateTimePeriod(gfRandom(800) + 400);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CIGStaticObject::~CIGStaticObject()
{
  if (mBody)
  {
    delete mBody;
    mBody = 0;
  }

  // destroy the script compiler, if any
  if (mValidCsl)
  {
    delete mCompiler;
  }

  ClearVisuals();

  delete mLayers;
  mLayers = 0;
}
//---------------------------------------------------------------------------

/*
__int32 CIGStaticObject::GetActionMask ()
{
  return CIGRealObject::GetActionMask();
}
//---------------------------------------------------------------------------

void CIGStaticObject::SetActionMask (__int32 value)
{
  CIGRealObject::SetActionMask(value);
}
//---------------------------------------------------------------------------
*/

/*---------------------------------------------------------------------------
 description: returns true if the object is derived from the className
---------------------------------------------------------------------------*/
bool CIGStaticObject::IsDerivedFrom (CPString className)
{
   return (className == CIGStaticObject::ClassName() || CIGRealObject::IsDerivedFrom(className));
}
//---------------------------------------------------------------------------

void CIGStaticObject::SetScriptObserver(CPPIG_ScriptObserver* _obs)
{
  if (mValidCsl)
  {
    delete mCompiler;
  }

  mScriptObserver = _obs;
  mValidCsl       = false;

  InitHandlers();
  InitCsl();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the tick event
---------------------------------------------------------------------------*/
void CIGStaticObject::Update ()
{
  //logWrite("Static Action Mask: "); logWriteILn(GetActionMask());

  CIGRealObject::Update();

  // if the unit is dead, there's nothing to update
  if (GetIsDead())
  {
    return;
  }

  // delegate the update to layers
  for(int i = 0; i < mLayers->Count(); i++)
  {
    mLayers->Get(i)->Update();
  }

  if (GetTimerInstance()->GetTime() - GetUpdateTime() < (unsigned)GetUpdateTimePeriod())
  {
    return;
  }

  SetUpdateTime(GetTimerInstance()->GetTime());

  // delegate the update to visuals
  for(unsigned int i = 0 ; i < mVisuals.size(); i++)
  {
    mVisuals[i]->Update();
  }

  // update the fog-of-war
  UpdateView(2);

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
      LOG(ERROR) << "CIGStaticObject::Update - fail to compile script";
    }
  }
  else
  {
    //logWriteLn("CIGStaticObject::Update - NO SCRIPT SET");
  }
}
//---------------------------------------------------------------------------

void CIGStaticObject::Paint(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel)
{
  CIGRealObject::Paint(ptViewPort, ptWindow, Width, Height, zoomLevel);

  ptViewPort.x += GetCoord().x;
  ptViewPort.y += GetCoord().y;

  // delegate the paint to layers
  int lCount = mLayers->Count();

  if (mStaticData.isLandscape && CPString(mStaticData.objName) == CPString("road_diag_NW_SE"))
  {
    int a = 5;
    a++;
  }

  for(int i = 0; i < lCount; i++)
  {
    mLayers->Get(i)->Paint(ptViewPort, ptWindow, Width, Height, zoomLevel);
  }
}

/*---------------------------------------------------------------------------
 description: notes the object in the ground mask map
---------------------------------------------------------------------------*/
void CIGStaticObject::NoteInLogicMap()
{
   if (!CanBePlaced(GetLogicCoord()))
   {
     return;
   }

   for (int i=0; i<GetLogicDimX(); i++)
   {
      for (int j=0; j<GetLogicDimY(); j++)
      {
         if (GetStaticData().mask[j][i])
         {
           GetMissionInstance()->NoteInGroundLogicMap(this, GetLogicCoord().x+i, GetLogicCoord().y+j);
         }

         // update the altitude map, by adding the existing map altitudes with
         // the values stored in staticdata altitude
         GetMissionInstance()->GetMap()->SetAltitude(
           GetLogicCoord().x+i,
           GetLogicCoord().y+j,
           GetStaticData().altitude[j][i] + GetMissionInstance()->GetMap()->GetAltitude(GetLogicCoord().x+i, GetLogicCoord().y+j)
         );
      }
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: notes the object in the ground mask map
---------------------------------------------------------------------------*/
void CIGStaticObject::RemoveFromLogicMap()
{
   CPPoint lCoord = GetLogicCoord();
   const int lMaxX = lCoord.x + GetLogicDimX();
   const int lMaxY = lCoord.y + GetLogicDimY();
   for (int i=lCoord.x; i<lMaxX; i++)
   {
      for (int j=lCoord.y; j<lMaxY; j++)
      {
         //if (GetStaticData().mask[j][i])
         //{
           GetMissionInstance()->RemoveFromGroundLogicMap(this, i, j);
         //}

         // update the altitude map, by substracting from existing map altitudes
         // the value stored in staticdata altitude
         GetMissionInstance()->GetMap()->SetAltitude(
           i,
           j,
           -GetStaticData().altitude[j - lCoord.y][i - lCoord.x] + GetMissionInstance()->GetMap()->GetAltitude(i, j)
         );
      }
   }
}
//---------------------------------------------------------------------------

bool CIGStaticObject::CanBePlaced(CPPoint _point)
{
  for (int i = 0; i < GetLogicDimX(); i++)
  {
      for (int j = 0; j < GetLogicDimY(); j++)
      {
        //if (!GetMissionInstance()->IsGroundPlaceEmpty(this, _point.x + i, _point.y + j) && GetStaticData().mask[j][i])
        if (!GetMissionInstance()->IsGroundPlaceEmpty(this, _point.x + i, _point.y + j) && (GetStaticData().mask[j][i] == 1))
        {
          return false;
        }
      }
  }

  return true;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns the object logic point closest to the target
 parameters : targetPoint - the wanted point in logic coord
---------------------------------------------------------------------------*/
CPPoint CIGStaticObject::GetClosestPointTo(CPPoint targetPoint)
{
   CPPoint lRes = CPPoint(10000, 10000);
   for (int i=0; i<GetLogicDimX(); i++)
      for (int j=0; j<GetLogicDimY(); j++)
         if (GetStaticData().mask[j][i])
           if(gfGetDist2D(lRes, targetPoint) > gfGetDist2D(CPPoint(i + GetLogicCoord().x,j + GetLogicCoord().y), targetPoint))
            lRes = CPPoint(i + GetLogicCoord().x,j + GetLogicCoord().y);
   return lRes;
}
//---------------------------------------------------------------------------

void CIGStaticObject::NoteStimul(CIGObjectStimul &stimul)
{
  if (GetIsDead() == false)
  {
    mBody->OnStimul(stimul);
    //mSensors->OnStimul(stimul);
  }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: Static Data
---------------------------------------------------------------------------*/
void CIGStaticObject::SetStaticData(struct _StaticObject& value)
{
   mStaticData = value;
   VLOG(9) << "Static mask ";

   for (int i=0; i<mStaticData.maskX; i++)
   {
       std::string maskLine = "";
      for (int j=0; j<mStaticData.maskY; j++)
      {
          maskLine += (mStaticData.mask[i][j]==0) ? " 0" : " 1";
      }

      VLOG(9) << maskLine;
   }

   SetMaxHP(value.hp);
   SetCurrentHP(value.hp);
   SetName(CPString(value.objName));
   SetLogicDimX(mStaticData.maskY);
   SetLogicDimY(mStaticData.maskX);

   if (mStaticData.isLandscape)
   {
     SetLayer(MOL_GROUNDOVERLAY);
     //RemoveFromLogicMap();
   }
   else
   {
     SetLayer(MOL_GROUNDOBJECT);
   }

   //
   // Layers setup
   //
   for(int i = 0; i < mStaticData.mLayersCount; i++)
   {
     CIGStaticLayer* lLayer = new CIGStaticLayer();

     lLayer->SetSprite(GetGraphicInstance()->AddSprite(mStaticData.mLayers[i].mSprite, mStaticData.mLayers[i].mSpriteCols, mStaticData.mLayers[i].mSpriteRows, SPRL_MISSION));
     //lLayer->SetSprite(GetGraphicInstance()->AddSprite(mStaticData.mLayers[i].mSprite, 8, 1, SPRL_MISSION));

     lLayer->SetCoord(CPPoint(mStaticData.mLayers[i].mX, mStaticData.mLayers[i].mY));

     lLayer->SetAnimCounter(mStaticData.mLayers[i].mSpriteCols - 1);

     lLayer->SetFrameUpdateDelay(mStaticData.mLayers[i].mFrameDelay);
     lLayer->SetCycleUpdateDelay(mStaticData.mLayers[i].mCycleDelay);

     lLayer->SetVisible(mStaticData.mLayers[i].mAlpha);

     mLayers->Add(lLayer);
   }

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
     // do not provide a default visual subsystem, because statics
     // may be in large numbers in a map and doing visual update for too many objects
     // is very costly
   }
}
//---------------------------------------------------------------------------

struct _StaticObject& CIGStaticObject::GetStaticData()
{
   return mStaticData;
}
//---------------------------------------------------------------------------

void CIGStaticObject::UpdateView(int _value)
{
  if (mVisuals.size() == 0)
  {
    return;
  }

  mVisuals[0]->UpdateView(_value);
}
//---------------------------------------------------------------------------

void CIGStaticObject::ClearVisuals()
{
  for(unsigned int i = 0 ; i < mVisuals.size(); i++)
  {
    delete mVisuals[i];
  }

  mVisuals.clear();
}
//---------------------------------------------------------------------------

// Description : checks if a point is in visual range of all visual subsystems
// Param       : x, y - the point
// Result      : true if the point can be seen by at least one subsystem
// Comments    : common operation that involves scanning of all visual systems
bool CIGStaticObject::InVisualRange(int _x, int _y)
{
  for(unsigned int i = 0; i < mVisuals.size(); i++)
  {
    bool lResult = mVisuals[i]->InVisualRange(_x, _y);

    if (lResult)
    {
      return true;
    }
  }

  return false;
}
//---------------------------------------------------------------------------

void CIGStaticObject::addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback)
{
  mCompiler->addFunc(mModuleName, szMemberName, callback);
}
//---------------------------------------------------------------------------

void CIGStaticObject::InitHandlers()
{
  mHandlePostMessage  .set(this, &CIGStaticObject:: mcPostMessage);
  mHandleLayerSetEnabled.set(this, &CIGStaticObject::mcLayerSetEnabled);
  mHandleLayerGetEnabled.set(this, &CIGStaticObject::mcLayerGetEnabled);
  mHandleLayerSetVisible.set(this, &CIGStaticObject::mcLayerSetVisible);
  mHandleLayerGetVisible.set(this, &CIGStaticObject::mcLayerGetVisible);
  mHandleLayerGetX.set(this, &CIGStaticObject::mcLayerGetX);
  mHandleLayerGetY.set(this, &CIGStaticObject::mcLayerGetY);
  mHandleLayerSetX.set(this, &CIGStaticObject::mcLayerSetX);
  mHandleLayerSetY.set(this, &CIGStaticObject::mcLayerSetY);
  mHandleLayerSetCycleDelay.set(this, &CIGStaticObject::mcLayerSetCycleDelay);
  mHandleLayerGetCycleDelay.set(this, &CIGStaticObject::mcLayerGetCycleDelay);
  mHandleLayerSetFrameDelay.set(this, &CIGStaticObject::mcLayerSetFrameDelay);
  mHandleLayerGetFrameDelay.set(this, &CIGStaticObject::mcLayerGetFrameDelay);
  mHandleMyX.set(this, &CIGStaticObject::mcMyX);
  mHandleMyY.set(this, &CIGStaticObject::mcMyY);
  mHandleGetMask.set(this, &CIGStaticObject::mcGetMask);
  mHandleSetMask.set(this, &CIGStaticObject::mcSetMask);
  mHandleGetAltitude.set(this, &CIGStaticObject::mcGetAltitude);
  mHandleSetAltitude.set(this, &CIGStaticObject::mcSetAltitude);
  mHandleMyHp.set(this, &CIGStaticObject::mcMyHp);
  mHandleMyId.set(this, &CIGStaticObject::mcMyId);
  mHandleCanFire.set(this, &CIGStaticObject::mcCanFire);
}
//---------------------------------------------------------------------------

void CIGStaticObject::InitCsl()
{
  CPString lScript;

  if (mValidCsl)
  {
    delete mCompiler;
  }

  mValidCsl = false;

  if (!mScriptObserver)
  {
    return;
  }

  try
  {
    mCompiler = new ZCsl();
  }
  catch(...)
  {
    LOG(ERROR) << "CIGStaticObject::InitCsl - errors while memory alloc for script compiler.";
    return;
  }

  try
  {
    // load math library
    ZMathLib* lMathLib = new ZMathLib(mCompiler, "Math Library");
    lMathLib->InitLibrary();
    mCompiler->loadLibrary(lMathLib);

    // load weapon rules library
    WeaponRulesLib* lWeaponRulesLib = new WeaponRulesLib(mCompiler, "Weapon Rules Library");
    lWeaponRulesLib->InitLibrary();
    mCompiler->loadLibrary(lWeaponRulesLib);

    // add functions
    addModuleFunc("PostMessage(const msg)", mHandlePostMessage);
    addModuleFunc("LayerSetEnabled(const layer, const value)", mHandleLayerSetEnabled);
    addModuleFunc("LayerGetEnabled(const layer)", mHandleLayerGetEnabled);
    addModuleFunc("LayerSetVisible(const layer, const value)", mHandleLayerSetVisible);
    addModuleFunc("LayerGetVisible(const layer)", mHandleLayerGetVisible);
    addModuleFunc("LayerGetX(const layer)", mHandleLayerGetX);
    addModuleFunc("LayerGetY(const layer)", mHandleLayerGetY);
    addModuleFunc("LayerSetX(const layer, const value)", mHandleLayerSetX);
    addModuleFunc("LayerSetY(const layer, const value)", mHandleLayerSetY);
    addModuleFunc("LayerSetCycleDelay(const layer, const value)", mHandleLayerSetCycleDelay);
    addModuleFunc("LayerGetCycleDelay(const layer)", mHandleLayerGetCycleDelay);
    addModuleFunc("LayerSetFrameDelay(const layer, const value)", mHandleLayerSetFrameDelay);
    addModuleFunc("LayerGetFrameDelay(const layer)", mHandleLayerGetFrameDelay);
    addModuleFunc("MyX()", mHandleMyX);
    addModuleFunc("MyY()", mHandleMyY);
    addModuleFunc("GetMask(const x, const y)", mHandleGetMask);
    addModuleFunc("SetMask(const x, const y, const value)", mHandleSetMask);
    addModuleFunc("GetAltitude(const x, const y)", mHandleGetAltitude);
    addModuleFunc("SetAltitude(const x, const y, const value)", mHandleSetAltitude);
    addModuleFunc("MyHp()", mHandleMyHp);
    addModuleFunc("MyId()", mHandleMyId);
    addModuleFunc("CanFire()", mHandleCanFire);
  }
  catch(...)
  {
    LOG(ERROR) << "CIGStaticObject::InitCsl - errors initializing libraries";
    delete mCompiler;
    return;
  }

  lScript = mScriptObserver->GetScript();

  try
  {
    // The const_cast is a workaround for a Microsoft interface bug.
#if PPT_COMPILER_MICROSOFT
#if (_MSC_VER >= 1400)
    std::istringstream str(const_cast<char*>(lScript.c_str()));
#else
    std::istrstream str(const_cast<char*>(lScript.c_str()));
#endif
#else
	  std::istringstream str(const_cast<char*>(lScript.c_str()));
#endif
    // compile
    mCompiler->loadScript(mModuleName, &str);

    mValidCsl = true;
  }
  catch(const ZException& err)
  {
    LOG(ERROR) << "CIGStaticObject::InitCsl - errors compiling script";
    for (int i = 0; i < err.count(); i++)
    {
      LOG(ERROR) << (char*)err[i];
    }

    delete mCompiler;
    mValidCsl = false;
    mCompiler = 0;
  }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
// Implementation of script methods follows
//
//---------------------------------------------------------------------------
ZString CIGStaticObject::mcPostMessage(ZCsl* csl)
{
  CPString lMsg = CPString(csl->get("msg").buffer());

  GetMainScreenInstance()->AddMessage(lMsg + "\n");

  return "";
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcLayerSetEnabled(ZCsl* csl)
{
  int  lLayer = csl->get("layer").asInt();
  bool lValue = (bool)(csl->get("value").asInt());

  if (mLayers->Get(lLayer))
  {
    mLayers->Get(lLayer)->SetEnabled(lValue);
    return "1";
  }

  return "0";
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcLayerGetEnabled(ZCsl* csl)
{
  int  lLayer = csl->get("layer").asInt();

  if (mLayers->Get(lLayer))
  {
    return (int)(mLayers->Get(lLayer)->GetEnabled());
  }

  return "0";
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcLayerSetVisible(ZCsl* csl)
{
  int  lLayer = csl->get("layer").asInt();
  bool lValue = (bool)(csl->get("value").asInt());

  if (mLayers->Get(lLayer))
  {
    mLayers->Get(lLayer)->SetVisible(lValue);
    return "1";
  }

  return "0";
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcLayerGetVisible(ZCsl* csl)
{
  int  lLayer = csl->get("layer").asInt();

  if (mLayers->Get(lLayer))
  {
    return (int)(mLayers->Get(lLayer)->GetVisible());
  }

  return "0";
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcLayerGetX(ZCsl* csl)
{
  int  lLayer = csl->get("layer").asInt();

  if (mLayers->Get(lLayer))
  {
    return mLayers->Get(lLayer)->GetCoord().x;
  }

  return "-1";
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcLayerGetY(ZCsl* csl)
{
  int  lLayer = csl->get("layer").asInt();

  if (mLayers->Get(lLayer))
  {
    return mLayers->Get(lLayer)->GetCoord().y;
  }

  return "-1";
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcLayerSetX(ZCsl* csl)
{
  int lLayer = csl->get("layer").asInt();
  int lValue = csl->get("value").asInt();

  if (mLayers->Get(lLayer))
  {
    CPPoint lCoord = mLayers->Get(lLayer)->GetCoord();

    lCoord.x = lValue;

    mLayers->Get(lLayer)->SetCoord(lCoord);

    return "1";
  }

  return "0";
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcLayerSetY(ZCsl* csl)
{
  int lLayer = csl->get("layer").asInt();
  int lValue = csl->get("value").asInt();

  if (mLayers->Get(lLayer))
  {
    CPPoint lCoord = mLayers->Get(lLayer)->GetCoord();

    lCoord.y = lValue;

    mLayers->Get(lLayer)->SetCoord(lCoord);

    return "1";
  }

  return "0";
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcLayerSetCycleDelay(ZCsl* csl)
{
  int lLayer = csl->get("layer").asInt();
  int lValue = csl->get("value").asInt();

  if (mLayers->Get(lLayer))
  {
    mLayers->Get(lLayer)->SetCycleUpdateDelay(lValue);

    return "1";
  }

  return "0";
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcLayerGetCycleDelay(ZCsl* csl)
{
  int lLayer = csl->get("layer").asInt();

  if (mLayers->Get(lLayer))
  {
    return mLayers->Get(lLayer)->GetCycleUpdateDelay();
  }

  return "-1";
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcLayerSetFrameDelay(ZCsl* csl)
{
  int lLayer = csl->get("layer").asInt();
  int lValue = csl->get("value").asInt();

  if (mLayers->Get(lLayer))
  {
    mLayers->Get(lLayer)->SetFrameUpdateDelay(lValue);

    return "1";
  }

  return "0";
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcLayerGetFrameDelay(ZCsl* csl)
{
  int lLayer = csl->get("layer").asInt();

  if (mLayers->Get(lLayer))
  {
    return mLayers->Get(lLayer)->GetFrameUpdateDelay();
  }

  return "-1";
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcMyX(ZCsl* csl)
{
  return GetLogicCoord().x;
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcMyY(ZCsl* csl)
{
  return GetLogicCoord().y;
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcGetMask(ZCsl* csl)
{
  int lMaskX = csl->get("x").asInt();
  int lMaskY = csl->get("y").asInt();

  if (lMaskX > 0 &&
      lMaskX < GetStaticData().maskX &&
      lMaskY > 0 &&
      lMaskY < GetStaticData().maskY)
  {
    return GetStaticData().mask[lMaskX][lMaskY];
  }

  return -1;
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcSetMask(ZCsl* csl)
{
  int lMaskX = csl->get("x").asInt();
  int lMaskY = csl->get("y").asInt();
  int lValue = csl->get("value").asInt();

  if (lMaskX > 0 &&
      lMaskX < GetStaticData().maskX &&
      lMaskY > 0 &&
      lMaskY < GetStaticData().maskY)
  {
    GetStaticData().mask[lMaskX][lMaskY] = lValue;

    return "1";
  }

  return "0";
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcGetAltitude(ZCsl* csl)
{
  int lMaskX = csl->get("x").asInt();
  int lMaskY = csl->get("y").asInt();

  if (lMaskX > 0 &&
      lMaskX < GetStaticData().maskX &&
      lMaskY > 0 &&
      lMaskY < GetStaticData().maskY)
  {
    return GetStaticData().altitude[lMaskX][lMaskY];
  }

  return -1;
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcSetAltitude(ZCsl* csl)
{
  int lMaskX = csl->get("x").asInt();
  int lMaskY = csl->get("y").asInt();
  int lValue = csl->get("value").asInt();

  if (lMaskX > 0 &&
      lMaskX < GetStaticData().maskX &&
      lMaskY > 0 &&
      lMaskY < GetStaticData().maskY)
  {
    GetStaticData().altitude[lMaskX][lMaskY] = lValue;

    return "1";
  }

  return "0";
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcMyHp(ZCsl* csl)
{
  return ZString(GetCurrentHP() * 100 / GetMaxHP());
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcMyId(ZCsl* csl)
{
  return ZString(GetId());
}
//---------------------------------------------------------------------------

ZString CIGStaticObject::mcCanFire(ZCsl* csl)
{
  if (GetStaticData().canFire)
  {
    return "1";
  }

  return "0";
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
// Serialization/Deserialization
//
//---------------------------------------------------------------------------
void CIGStaticObject::Serialize(PP::Stream& _a)
{
  CSerString lModuleName((char*)mModuleName);
  int        lScriptId = -1;

  // setup
  if (mScriptObserver)
  {
    lScriptId    = mScriptObserver->GetScriptId();
  }

  // parent serialize
  CIGRealObject::Serialize(_a);

  // this serialize
  lModuleName.Serialize(_a);
  _a.writeBool(&mValidCsl);
  _a.writeInt(&lScriptId);

  mBody->Serialize(_a);

  //_a.write(&mStaticData, sizeof(struct _StaticObject));
  mStaticData.Serialize(_a);

  // serialize layers
  int lLayersCount = mLayers->Count();

  _a.writeInt(&lLayersCount);

  for(int i = 0; i < lLayersCount; i++)
  {
    mLayers->Get(i)->Serialize(_a);
  }

  int lVisualsCount = mVisuals.size();

  _a.writeInt(&lVisualsCount);

  for(unsigned int i = 0; i < mVisuals.size(); i++)
  {
    mVisuals[i]->Serialize(_a);
  }
}
//---------------------------------------------------------------------------

unsigned __int32 CIGStaticObject::DataSize()
{
  return CIGRealObject::DataSize() + sizeof(struct _StaticObject);
}
//---------------------------------------------------------------------------

void CIGStaticObject::DeSerialize(PP::Stream& _a)
{
  CSerString lModuleName;
  int        lScriptId;

  // parent serialize
  CIGRealObject::DeSerialize(_a);

  // this serialize
  lModuleName.DeSerialize(_a);
  _a.readBool(&mValidCsl);
  _a.readInt(&lScriptId);

  mBody->DeSerialize(_a);

  //_a.read(&mStaticData, sizeof(struct _StaticObject));
  struct _StaticObject staticObject;
  staticObject.DeSerialize(_a);
  SetStaticData(staticObject);

  //SetStaticData(mStaticData);

  // clear the layers
  while(mLayers->Count() > 0)
  {
    mLayers->Delete(0);
  }

  // deserialize layers
  int lLayersCount;

  _a.readInt(&lLayersCount);

  for(int i = 0; i < lLayersCount; i++)
  {
    CIGStaticLayer* lLayer = new CIGStaticLayer();

    lLayer->DeSerialize(_a);

    mLayers->Add(lLayer);
  }

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

  // setup
  mValidCsl   = false; // lock the use of csl, maybe script get corrupted or something
  mModuleName = ZString(lModuleName.Get().c_str());
  SetScriptObserver(GetMissionInstance()->GetDoctrine()->GetScriptObserver(lScriptId));
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

