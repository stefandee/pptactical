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

 description: Class In Game - Ground Unit
 last modify: 24 05 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
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

#include "PPIG_ObjUnit.h"
#include "PP_Graphic.h"
#include "PPIG_Mission.h"
#include "PPIG_ObjRocket.h"
#include "PPIG_ObjBullet.h"
#include "PPIG_ObjLaser.h"
#include "PPIG_ObjPlasma.h"
#include "basepath.h"
#include "Paths.h"
#include "PP_Sound.h"
#include "PPIG_SpaceSound.h"
#include "Equipment.h"
#include "ZMathLib.h"
#include "WeaponRulesLib.h"
#include "PPIG_CScreen.h"
#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "PPIG_BGManager.h"

#include "PPIG_SubSysVisual.h"
#include "PPIG_SubSysBody.h"
#include "PPIG_SubSysTactics.h"

#include "PPIG_WeaponRules.h"

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CIGUnit::CIGUnit() : CIGRealObject()
{
  //GetActionStepCounter().SetMaxValue(0);
  SetLayer(MOL_GROUNDUNIT);

  mRange  = 0;
  mDamage = 0;
  mWepType= 0;

  mProjectileSprite  = 0;
  mEffectSprite      = 0;
  mAfterEffectSprite = 0;

  mSoundOnFire      = -1;
  mSoundOnHit       = -1;

  mCompiler       = 0;
  mValidCsl       = false;
  mScriptObserver = 0;
  mModuleName     = "UnInitialized-C"; // generic compiler module name (lack of compiler ;)

  mBody    = 0;
  mSensors = 0;

  mMaxForce = 1.0;

  mTactics    = new CIGSubSysTactics(this);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CIGUnit::~CIGUnit()
{
  delete mTactics;
  mTactics = 0;

  // remove the sprites
  //GetGraphicInstance()->ReleaseSprite(mEffectSprite,      SPRL_MISSION);
  //GetGraphicInstance()->ReleaseSprite(mProjectileSprite,  SPRL_MISSION);
  //GetGraphicInstance()->ReleaseSprite(mAfterEffectSprite, SPRL_MISSION);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if this class is derived from className
 parameters : className
---------------------------------------------------------------------------*/
bool CIGUnit::IsDerivedFrom (CPString className)
{
   return ((className == CIGUnit::ClassName()) || (CIGRealObject::IsDerivedFrom(className)));
}
//---------------------------------------------------------------------------

void CIGUnit::SetScriptObserver(CPPIG_ScriptObserver* _obs)
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

CPString CIGUnit::GetScriptVar(CPString _name)
{
  if (!mValidCsl)
  {
    return "";
  }

  if (!mCompiler->varExists(_name.c_str()))
  {
    return "";
  }

  return CPString((mCompiler->get(_name.c_str())).buffer());
}
//---------------------------------------------------------------------------

void     CIGUnit::SetScriptVar(CPString _name, CPString _value)
{
  if (!mValidCsl)
  {
    return;
  }

  if (mCompiler->varExists(_name.c_str()))
  {
    mCompiler->set(_name.c_str(), _value.c_str());
  }
}
//---------------------------------------------------------------------------

void CIGUnit::SetSoundOnHit(CPString fileName)
{
   if (!fileName.isEmpty())
   {
     mSoundOnHit = GetSoundInstance()->LoadNew((PATH_SFX + fileName.c_str()).c_str(), 5);
   }
}
//---------------------------------------------------------------------------

void CIGUnit::SetSoundOnFire(CPString fileName)
{
   if (!fileName.isEmpty())
   {
     mSoundOnFire = GetSoundInstance()->LoadNew((PATH_SFX + fileName.c_str()).c_str(), 5);
   }
}
//---------------------------------------------------------------------------

void CIGUnit::SetProjectileSprite(CPString fileName)
{
   if (fileName.isEmpty() == false)
   {
     mProjectileSprite = GetGraphicInstance()->AddSprite(CPString(PATH_EXPLODES_GFX) + fileName, 8, 2, SPRL_MISSION);
   }
}
//---------------------------------------------------------------------------

void CIGUnit::SetEffectSprite(CPString fileName)
{
   if (fileName.isEmpty() == false)
      mEffectSprite = GetGraphicInstance()->AddSprite(CPString(PATH_EXPLODES_GFX) + fileName, 8, 2, SPRL_MISSION);
}
//---------------------------------------------------------------------------

void CIGUnit::SetAfterEffectSprite(CPString fileName)
{
   if (fileName.isEmpty() == false)
   {
     mAfterEffectSprite = GetGraphicInstance()->AddSprite(CPString(PATH_EXPLODES_GFX) + fileName, 8, 2, SPRL_MISSION);
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the object sprite (animtype & animCount)
 parameters : ptViewport - the zone of the screen where the paint is wanted
              rcWindow - the visible portion of the map
              Width, Height - is probably redundant and represents the viewable area
              zoomLevel - is 1 by default
---------------------------------------------------------------------------*/
void CIGUnit::Paint(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel)
{
}

/*---------------------------------------------------------------------------
 description: returns true if the object is waiting for another object to move
---------------------------------------------------------------------------*/
bool CIGUnit::IsWaiting ()
{
   return (GetWaitingFor() != NULL);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: notes the object in the ground mask map
---------------------------------------------------------------------------*/
void CIGUnit::NoteInLogicMap()
{
   NoteInLogicMap(GetLogicCoord());
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: notes this object in the specified point
 parameters : Point - the beginning point (in logic coord)
---------------------------------------------------------------------------*/
void CIGUnit::NoteInLogicMap(CPPoint Point)
{
  if (!CanBePlaced(Point))
  {
    VLOG(5) << "CIGUnit::NoteInLogicMap - Cannot be placed.";
    return;
  }

  for (int i=Point.x; i<Point.x + GetLogicDimX(); i++)
  {
      for (int j=Point.y; j<Point.y + GetLogicDimY(); j++)
      {
        GetMissionInstance()->NoteInGroundLogicMap(this, i, j);
      }
  }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: notes the object in the ground mask map
---------------------------------------------------------------------------*/
void CIGUnit::RemoveFromLogicMap()
{
   CPPoint lCoord = GetLogicCoord();
   const int lMaxX = lCoord.x + GetLogicDimX();
   const int lMaxY = lCoord.y + GetLogicDimY();
   for (int i=lCoord.x; i<lMaxX; i++)
   {
      for (int j=lCoord.y; j<lMaxY; j++)
      {
         GetMissionInstance()->RemoveFromGroundLogicMap(this, i, j);
      }
   }
}
//---------------------------------------------------------------------------

bool CIGUnit::CanBePlaced(CPPoint _point)
{
  if (GetLogicCoord().x != (float)GetCoord().x / MAPCELLX)
  {
    return true;
  }

  if (GetLogicCoord().y != (float)GetCoord().y / MAPCELLY)
  {
    return true;
  }

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

void CIGUnit::ClearVisuals()
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
bool CIGUnit::InVisualRange(int _x, int _y)
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


//---------------------------------------------------------------------------
//
// Scripting section
//
//---------------------------------------------------------------------------

void CIGUnit::addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback)
{
  if (!mCompiler)
  {
    LOG(ERROR) << "CIGUnit::addModuleFunc - mCompiler is invalid";
    return;
  }

  mCompiler->addFunc(mModuleName, szMemberName, callback);
}
//---------------------------------------------------------------------------

void CIGUnit::InitCsl()
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
    LOG(ERROR) << "CIGUnit::InitCsl - errors while memory alloc for script compiler.";
    return;
  }

  try
  {
    InitScriptingLibs();
    InitScriptingInterface();
  }
  catch(...)
  {
    LOG(ERROR) << "CIGUnit::InitCsl - errors initializing scripting interface";
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
    LOG(ERROR) << "CIGUnit::InitCsl - errors compiling script";
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

void CIGUnit::InitScriptingLibs()
{
  if (!mCompiler)
  {
    LOG(ERROR) << "CIGUnit::InitScriptingLibs - mCompiler is invalid";
    return;
  }

  // load math library
  ZMathLib* lMathLib = new ZMathLib(mCompiler, "Math Library");
  lMathLib->InitLibrary();
  mCompiler->loadLibrary(lMathLib);

  // load weapon rules library
  WeaponRulesLib* lWeaponRulesLib = new WeaponRulesLib(mCompiler, "Weapon Rules Library");
  lWeaponRulesLib->InitLibrary();
  mCompiler->loadLibrary(lWeaponRulesLib);
}
//---------------------------------------------------------------------------

void CIGUnit::InitHandlers()
{
  mHandlePostMessage  .set(this, &CIGUnit:: mcPostMessage);
  mHandleMove         .set(this, &CIGUnit:: mcMove);
  mHandleStop         .set(this, &CIGUnit:: mcStop);
  mHandleMyX          .set(this, &CIGUnit:: mcMyX);
  mHandleMyY          .set(this, &CIGUnit:: mcMyY);
  mHandleMyZ          .set(this, &CIGUnit:: mcMyZ);
  mHandlePlaySound    .set(this, &CIGUnit:: mcPlaySound);
  mHandleSetScanAngle .set(this, &CIGUnit:: mcSetScanAngle);
  mHandleGetScanAngle .set(this, &CIGUnit:: mcGetScanAngle);
  mHandleLookAt       .set(this, &CIGUnit:: mcLookAt);
  mHandleLookTo       .set(this, &CIGUnit:: mcLookTo);
  mHandleSweeping     .set(this, &CIGUnit:: mcSweeping);
  mHandleMoving       .set(this, &CIGUnit:: mcMoving);

  mHandleScan         .set(this, &CIGUnit:: mcScan);
  mHandleUnitInViewCount .set(this, &CIGUnit:: mcUnitInViewCount);
  mHandleUnitInView   .set(this, &CIGUnit:: mcUnitInView);

  mHandleWeaponRange  .set(this, &CIGUnit:: mcWeaponRange);
  mHandleWeaponDamage .set(this, &CIGUnit:: mcWeaponDamage);

  mHandleInVisual     .set(this, &CIGUnit:: mcInVisual);
  mHandleUnitId       .set(this, &CIGUnit:: mcUnitId);
  mHandleUnitDist     .set(this, &CIGUnit:: mcUnitDist);
  mHandleUnitStatus   .set(this, &CIGUnit:: mcUnitStatus);
  mHandleUnitThreat   .set(this, &CIGUnit:: mcUnitThreat);
  mHandleUnitRelation .set(this, &CIGUnit:: mcUnitRelation);
  mHandleUnitType     .set(this, &CIGUnit:: mcUnitType);
  mHandleUnitX        .set(this, &CIGUnit:: mcUnitX);
  mHandleUnitY        .set(this, &CIGUnit:: mcUnitY);
  mHandleUnitZ        .set(this, &CIGUnit:: mcUnitZ);

  // TEST CODE FOR SDJ
  mHandleUnitHP        .set(this, &CIGUnit:: mcUnitHP);

  mHandleFireTo       .set(this, &CIGUnit:: mcFireTo);
  mHandleFireAt       .set(this, &CIGUnit:: mcFireAt);

  mHandleNoiseType    .set(this, &CIGUnit:: mcNoiseType);
  mHandleNoiseX       .set(this, &CIGUnit:: mcNoiseX);
  mHandleNoiseY       .set(this, &CIGUnit:: mcNoiseY);
  mHandleNoiseCount   .set(this, &CIGUnit:: mcNoiseCount);
  mHandleNoisePurge   .set(this, &CIGUnit:: mcNoisePurge);

  mHandleAttackerOf   .set(this, &CIGUnit:: mcAttackerOf);
  mHandleAttackerOfCount .set(this, &CIGUnit:: mcAttackerOfCount);

  mHandleMyHp         .set(this, &CIGUnit:: mcMyHp);
  mHandleMyId         .set(this, &CIGUnit:: mcMyId);

  mHandleGetGroundType   .set(this, &CIGUnit:: mcGetGroundType);
  mHandleGetMapVisibility.set(this, &CIGUnit:: mcGetMapVisibility);
}
//---------------------------------------------------------------------------

void CIGUnit::InitScriptingInterface()
{
  VLOG(9) << ("CIGUnit::InitScriptingInterface");

  // add functions
  addModuleFunc("PostMessage(const msg)", mHandlePostMessage);
  addModuleFunc("PlaySound(const no)", mHandlePlaySound);

  addModuleFunc("Move(const x, const y)", mHandleMove);
  addModuleFunc("Stop()", mHandleStop);
  addModuleFunc("MyX()", mHandleMyX);
  addModuleFunc("MyY()", mHandleMyY);
  addModuleFunc("MyZ()", mHandleMyZ);

  addModuleFunc("SetScanAngle(const index, const deg)", mHandleSetScanAngle);
  addModuleFunc("GetScanAngle(const index, )", mHandleGetScanAngle);
  addModuleFunc("LookTo(const index, const id)", mHandleLookTo);
  addModuleFunc("LookAt(const index, const x, const y)", mHandleLookAt);
  addModuleFunc("Sweeping(const index)", mHandleSweeping);
  addModuleFunc("Moving()", mHandleMoving);

  addModuleFunc("Scan(const index)", mHandleScan);
  addModuleFunc("UnitInViewCount(const index)", mHandleUnitInViewCount);
  addModuleFunc("UnitInView(const index, const unitindex)", mHandleUnitInView);

  addModuleFunc("WeaponRange(const no)", mHandleWeaponRange);
  addModuleFunc("WeaponDamage(const no, const dtype)", mHandleWeaponDamage);

  addModuleFunc("InVisual(const x, const y)", mHandleInVisual    );
  addModuleFunc("UnitId(const x, const y)",   mHandleUnitId      );
  addModuleFunc("UnitDist(const id)",         mHandleUnitDist    );
  addModuleFunc("UnitStatus(const id)",       mHandleUnitStatus  );
  addModuleFunc("UnitThreat(const id)",       mHandleUnitThreat  );
  addModuleFunc("UnitRelation(const id)",     mHandleUnitRelation);
  addModuleFunc("UnitType(const id)",         mHandleUnitType);
  addModuleFunc("UnitX(const id)",            mHandleUnitX);
  addModuleFunc("UnitY(const id)",            mHandleUnitY);
  addModuleFunc("UnitZ(const id)",            mHandleUnitY);

  // TEST CODE FOR SDJ
  addModuleFunc("UnitHP(const id)",           mHandleUnitHP);

  addModuleFunc("FireAt(const x, const y)", mHandleFireAt);
  addModuleFunc("FireTo(const id)", mHandleFireTo);

  addModuleFunc("NoiseType(const index)",  mHandleNoiseType);
  addModuleFunc("NoiseX(const index)",     mHandleNoiseX);
  addModuleFunc("NoiseY(const index)",     mHandleNoiseY);
  addModuleFunc("NoiseCount()",            mHandleNoiseCount);
  addModuleFunc("NoisePurge()",            mHandleNoisePurge);

  addModuleFunc("AttackerOf(const id, const index)",   mHandleAttackerOf);
  addModuleFunc("AttackerOfCount(const id)",           mHandleAttackerOfCount);

  addModuleFunc("MyHp()", mHandleMyHp);
  addModuleFunc("MyId()", mHandleMyId);

  addModuleFunc("GetGroundType(const x, const y)",    mHandleGetGroundType);
  addModuleFunc("GetMapVisibility(const x, const y)", mHandleGetMapVisibility);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
// Implementation of script methods follows
//
//---------------------------------------------------------------------------
ZString CIGUnit::mcPostMessage(ZCsl* csl)
{
  CPString lMsg = CPString(csl->get("msg").buffer());

  GetMainScreenInstance()->AddMessage(lMsg + "\n");

  return "";
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcMove(ZCsl* csl)
{
  TIGSubSysActionData lTmpActionData;

  lTmpActionData.mType          = OA_MOVE;
  lTmpActionData.mTargetPoint.x = csl->get("x").asInt() * MAPCELLX;
  lTmpActionData.mTargetPoint.y = csl->get("y").asInt() * MAPCELLY;

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

ZString CIGUnit::mcStop(ZCsl* csl)
{
  TIGSubSysActionData lTmpActionData;

  lTmpActionData.mType          = OA_STOP;
  SetAction(lTmpActionData);

  return "";
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcMyX(ZCsl* csl)
{
  return GetLogicCoord().x;
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcMyY(ZCsl* csl)
{
  return GetLogicCoord().y;
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcMyZ(ZCsl* csl)
{
  return GetCoordZ();
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcPlaySound(ZCsl* csl)
{
  return "";
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcSetScanAngle(ZCsl* csl)
{
  int lIndex = csl->get("index").asInt();
  int lDeg   = csl->get("deg").asInt();

  if (lIndex >= (int)mVisuals.size())
  {
    return "";
  }

  mVisuals[lIndex]->SetScanAngle(lDeg);
  //mVisuals[lIndex]->ResetScan();

  return "";
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcGetScanAngle(ZCsl* csl)
{
  int lIndex = csl->get("index").asInt();

  if (lIndex >= (int)mVisuals.size())
  {
    return "-1";
  }

  return ZString(mVisuals[lIndex]->GetScanAngle());
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcLookAt(ZCsl* csl)
{
  int lIndex = csl->get("index").asInt();

  if (lIndex >= (int)mVisuals.size())
  {
    return "";
  }

  mVisuals[lIndex]->SetLookAt(UL_POINT);
  mVisuals[lIndex]->SetLookPoint(CPPoint(csl->get("x").asInt(), csl->get("y").asInt()));
  //mVisuals[lIndex]->ResetScan();

  return "";
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcLookTo(ZCsl* csl)
{
  // check if unit in visual range
  int lIndex          = csl->get("index").asInt();
  CIGBaseObject* lObj = GetMissionInstance()->GetObjectById(csl->get("id").asInt());

  if (lIndex >= (int)mVisuals.size())
  {
    return "";
  }

  if (!lObj)
  {
    return 0;
  }

  // check if in visual range!
  //if (!mVisuals[lIndex]->InVisualRange(lObj->GetLogicCoord().x, lObj->GetLogicCoord().y))
  CPPoint closestPoint = lObj->GetClosestPointTo(GetLogicCoord());

  if (!mVisuals[lIndex]->InVisualRange(closestPoint.x, closestPoint.y))
  {
    return 0;
  }

  //mVisuals[lIndex]->ResetScan();
  mVisuals[lIndex]->SetLookAt(UL_OBJECT);
  mVisuals[lIndex]->SetLookObj(lObj);

  return 1;
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcSweeping(ZCsl* csl)
{
  int lIndex = csl->get("index").asInt();

  if (lIndex >= (int)mVisuals.size())
  {
    return "0";
  }

  return (int)(mVisuals[lIndex]->GetScanRange() < mVisuals[lIndex]->GetMaxScanRange());
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcMoving(ZCsl* csl)
{
  if (GetMoving())
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcScan(ZCsl* csl)
{
  int lIndex = csl->get("index").asInt();

  if (lIndex >= (int)mVisuals.size())
  {
    return "0";
  }

  mVisuals[lIndex]->UnitsInView();

  //logWrite("Scan - "); logWriteILn(mUnitsInView->Count());

  return mVisuals[lIndex]->GetUnitsInViewCount();
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcUnitInViewCount(ZCsl* csl)
{
  int lIndex = csl->get("index").asInt();

  if (lIndex >= (int)mVisuals.size())
  {
    return "0";
  }

  return mVisuals[lIndex]->GetUnitsInViewCount();
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcUnitInView(ZCsl* csl)
{
  int lIndex = csl->get("index").asInt();
  int lUnitIndex = csl->get("unitindex").asInt();

  if (lIndex >= (int)mVisuals.size())
  {
    return "-1";
  }

  //logWrite("Index to return - "); logWriteILn(lIndex);

  if (mVisuals[lIndex]->GetUnitsInViewCount() <= 0)
  {
    return -1;
  }

  if ((lUnitIndex < 0) || (lUnitIndex > mVisuals[lIndex]->GetUnitsInViewCount()))
  {
    return -1;
  }

  return mVisuals[lIndex]->GetUnitsInViewId(lUnitIndex);
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcWeaponRange(ZCsl* csl)
{
  // for the moment we ignore the parameter "no", since a soldier may have only one weapon
  return ZString(GetRange());
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcWeaponDamage(ZCsl* csl)
{
  // for the moment we ignore the parameter "no", since a soldier may have only one weapon
  return ZString(GetDamage());
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcInVisual(ZCsl* csl)
{
  int lX = csl->get("x").asInt();
  int lY = csl->get("y").asInt();

  return (int)InVisualRange(lX, lY);
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcUnitId(ZCsl* csl)
{
  int lX = csl->get("x").asInt();
  int lY = csl->get("y").asInt();

  // get unit id
  CIGBaseObject* lObj = GetMissionInstance()->GetMap()->GetGroundObject(lX, lY);

  // return -1 if there is no object there
  if (!lObj)
  {
    return -1;
  }

  // check to see if the (x, y) place is in visual range
  if (!InVisualRange(lX, lY))
  {
    return -1;
  }

  return lObj->GetId();
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcUnitDist(ZCsl* csl)
{
  int lId = csl->get("id").asInt();

  // get object
  CIGBaseObject* lObj = GetMissionInstance()->GetObjectById(lId);

  // return -1 if there is no object there
  if (!lObj)
  {
    return -1;
  }

  // check to see if the (x, y) place is in visual range
  //if (!InVisualRange(lObj->GetLogicCoord().x, lObj->GetLogicCoord().y))
  CPPoint coord = GetLogicCoord();

  if (!InVisualRange(lObj->GetClosestPointTo(coord).x, lObj->GetClosestPointTo(coord).y))
  {
    return -1;
  }

  return gfGetDist2D(lObj->GetClosestPointTo(coord), coord);
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcUnitStatus(ZCsl* csl)
{
  int lId = csl->get("id").asInt();

  // get object
  CIGBaseObject* lObj = GetMissionInstance()->GetObjectById(lId);

  // return -1 if there is no object there
  if (!lObj)
  {
    return -1;
  }

  // check to see if the (x, y) place is in visual range
  CPPoint coord = GetLogicCoord();

  if (!InVisualRange(lObj->GetClosestPointTo(coord).x, lObj->GetClosestPointTo(coord).y))
  //if (!InVisualRange(lObj->GetLogicCoord().x, lObj->GetLogicCoord().y))
  {
    return 0;
  }

  if (lObj->GetIsDead())
  {
    //logWriteLn("Object is DEAD!");
    return 1;
  }

  if (lObj->GetMoving())
  {
    return 2;
  }

  // standing or firing
  return 3;
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcUnitThreat(ZCsl* csl)
{
  // will be implemented
  return 80;
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcUnitRelation(ZCsl* csl)
{
  int lId = csl->get("id").asInt();

  // get object
  CIGBaseObject* lObj = GetMissionInstance()->GetObjectById(lId);

  // return -1 if there is no object there
  if (!lObj)
  {
    return -1;
  }

  // check to see if the (x, y) place is in visual range
  CPPoint coord = GetLogicCoord();

  if (!InVisualRange(lObj->GetClosestPointTo(coord).x, lObj->GetClosestPointTo(coord).y))
  //if (!InVisualRange(lObj->GetLogicCoord().x, lObj->GetLogicCoord().y))
  {
    return -1;
  }

  int lRelation = GetMissionInstance()->GetBGManager()->GetRelation(GetBattleGroupId(), lObj->GetBattleGroupId());

  switch(lRelation)
  {
    case REL_NEUTRAL  :
      return 0;

    case REL_FRIENDLY :
      return 1;

    case REL_OWNED    :
      return 2;

    case REL_ENEMY    :
      return 3;

    case REL_UNKNOWN  :
      return -1;
  }

  // to be sure
  return -1;
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcUnitType(ZCsl* csl)
{
  int lId = csl->get("id").asInt();

  // get object
  CIGBaseObject* lObj = GetMissionInstance()->GetObjectById(lId);

  // return -1 if there is no object there
  if (!lObj)
  {
    return -1;
  }

  // check to see if the (x, y) place is in visual range
  CPPoint coord = GetLogicCoord();

  if (!InVisualRange(lObj->GetClosestPointTo(coord).x, lObj->GetClosestPointTo(coord).y))
  //if (!InVisualRange(lObj->GetLogicCoord().x, lObj->GetLogicCoord().y))
  {
    return -1;
  }

  // now check the types and return the appropriate values
  if (lObj->ClassName() == CPString(IGCN_INFANTRYUNIT))
  {
    return 1;
  }

  if (lObj->ClassName() == CPString(IGCN_TANKUNIT))
  {
    return 2;
  }

  if (lObj->ClassName() == CPString(IGCN_RECONUNIT))
  {
    return 3;
  }

  if (lObj->ClassName() == CPString(IGCN_ARTILLERYUNIT))
  {
    return 4;
  }

  if (lObj->ClassName() == CPString(IGCN_TRANUNIT))
  {
    return 5;
  }

  if (lObj->ClassName() == CPString(IGCN_STATICOBJECT))
  {
    return 6;
  }

  if (lObj->ClassName() == CPString(IGCN_CHOPPERUNIT))
  {
    return 7;
  }

  if (lObj->ClassName() == CPString(IGCN_FIGHTERUNIT))
  {
    return 8;
  }

  if (lObj->ClassName() == CPString(IGCN_DROPSHIPUNIT))
  {
    return 9;
  }

  // unknown object type
  return 0;
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcUnitX(ZCsl* csl)
{
  int lId = csl->get("id").asInt();

  CIGBaseObject* lObj = GetMissionInstance()->GetObjectById(lId);

  // check if object exists
  if (!lObj)
  {
    return -1;
  }

  // check if in visual range!
  CPPoint coord = GetLogicCoord();

  if (!InVisualRange(lObj->GetClosestPointTo(coord).x, lObj->GetClosestPointTo(coord).y))
  //if (!InVisualRange(lObj->GetLogicCoord().x, lObj->GetLogicCoord().y))
  {
    //logWriteLn("fire - not in range");
    return -1;
  }

  return lObj->GetLogicCoord().x;
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcUnitY(ZCsl* csl)
{
  int lId = csl->get("id").asInt();

  CIGBaseObject* lObj = GetMissionInstance()->GetObjectById(lId);

  // check if object exists
  if (!lObj)
  {
    return -1;
  }

  // check if in visual range!
  CPPoint coord = GetLogicCoord();

  if (!InVisualRange(lObj->GetClosestPointTo(coord).x, lObj->GetClosestPointTo(coord).y))
  //if (!InVisualRange(lObj->GetLogicCoord().x, lObj->GetLogicCoord().y))
  {
    return -1;
  }

  return lObj->GetLogicCoord().y;
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcUnitZ(ZCsl* csl)
{
  int lId = csl->get("id").asInt();

  CIGBaseObject* lObj = GetMissionInstance()->GetObjectById(lId);

  // check if object exists
  if (!lObj)
  {
    return -1;
  }

  // check if in visual range!
  CPPoint coord = GetLogicCoord();

  if (!InVisualRange(lObj->GetClosestPointTo(coord).x, lObj->GetClosestPointTo(coord).y))
  //if (!InVisualRange(lObj->GetLogicCoord().x, lObj->GetLogicCoord().y))
  {
    return -1;
  }

  return lObj->GetCoordZ();
}
//---------------------------------------------------------------------------

// TEST CODE FOR SDJ
ZString CIGUnit::mcUnitHP(ZCsl* csl)
{
  int lId = csl->get("id").asInt();

  CIGBaseObject* lObj = GetMissionInstance()->GetObjectById(lId);

  // check if object exists
  if (!lObj)
  {
    return -1;
  }

  if (!lObj->IsDerivedFrom(IGCN_REALOBJECT))
  {
    return -1;
  }

  // check if in visual range!
  CPPoint coord = GetLogicCoord();

  if (!InVisualRange(lObj->GetClosestPointTo(coord).x, lObj->GetClosestPointTo(coord).y))
  //if (!InVisualRange(lObj->GetLogicCoord().x, lObj->GetLogicCoord().y))
  {
    return -1;
  }

  return (dynamic_cast<CIGRealObject*>(lObj))->GetCurrentHP();
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcFireTo(ZCsl* csl)
{
  CIGBaseObject* lObj = GetMissionInstance()->GetObjectById(csl->get("id").asInt());

  // check if object exists
  if (!lObj)
  {
    return 0;
  }

  // check if in visual range!
  //logWriteLn("Fire - check visual");
  CPPoint coord = GetLogicCoord();

  if (!InVisualRange(lObj->GetClosestPointTo(coord).x, lObj->GetClosestPointTo(coord).y))
  //if (!InVisualRange(lObj->GetLogicCoord().x, lObj->GetLogicCoord().y))
  {
    return 0;
  }

  //if (gfGetDist2D(GetLogicCoord(), lObj->GetLogicCoord()) > GetRange())
  if (gfGetDist2D(GetLogicCoord(), lObj->GetClosestPointTo(coord)) > GetRange())
  {
    return 0;
  }

  if (!CIGWeaponRules().CanFireObject(GetId(), GetWepType(), lObj->GetId()))
  {
    return 0;
  }

  TIGSubSysActionData lTmpActionData;

  lTmpActionData.mType          = OA_ATTACK;
  lTmpActionData.mTargetPoint   = CPPoint(lObj->GetLogicCoord().x * MAPCELLX, lObj->GetLogicCoord().y * MAPCELLY);
  lTmpActionData.mTargetObject  = lObj;
  SetAction(lTmpActionData);

  return 1;
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcFireAt(ZCsl* csl)
{
  TIGSubSysActionData lTmpActionData;

  lTmpActionData.mTargetPoint.x = csl->get("x").asInt() * MAPCELLX;
  lTmpActionData.mTargetPoint.y = csl->get("y").asInt() * MAPCELLY;

  if ((lTmpActionData.mTargetPoint.x < 0) || (lTmpActionData.mTargetPoint.x > GetMissionInstance()->GetMap()->GetLogicWidth()))
  {
    return 0;
  }

  if ((lTmpActionData.mTargetPoint.y < 0) || (lTmpActionData.mTargetPoint.y > GetMissionInstance()->GetMap()->GetLogicHeight()))
  {
    return 0;
  }

  if (gfGetDist2D(GetLogicCoord(), lTmpActionData.mTargetPoint) > GetRange())
  {
    return 0;
  }

  lTmpActionData.mType          = OA_ATTACKGROUND;
  lTmpActionData.mTargetObject  = NULL;
  SetAction(lTmpActionData);

  return 1;
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcNoiseType(ZCsl* csl)
{
  int lIndex = csl->get("index").asInt();

  if (lIndex < 0 || lIndex > mSensors->GetNoisesList()->Count())
  {
    return -1;
  }

  switch(mSensors->GetNoisesList()->Get(lIndex)->GetType())
  {
    case ST_NOISE_WEAPON_FIRE:
      return 3;

    case ST_NOISE_WEAPON_HIT :
      return 2;

    case ST_NOISE_MOVEMENT   :
      return 1;
  }

  // just to be sure
  VLOG(2) << "CIGInfantryUnit::mcNoiseType - unknown noise!";
  return -1;
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcNoiseX(ZCsl* csl)
{
  int lIndex = csl->get("index").asInt();

  if (lIndex < 0 || lIndex > mSensors->GetNoisesList()->Count())
  {
    return -1;
  }

  return mSensors->GetNoisesList()->Get(lIndex)->GetSourcePoint().x;
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcNoiseY(ZCsl* csl)
{
  int lIndex = csl->get("index").asInt();

  if (lIndex < 0 || lIndex > mSensors->GetNoisesList()->Count())
  {
    return -1;
  }

  return mSensors->GetNoisesList()->Get(lIndex)->GetSourcePoint().y;
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcNoiseCount(ZCsl* csl)
{
  return mSensors->GetNoisesList()->Count();
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcNoisePurge(ZCsl* csl)
{
  while(mSensors->GetNoisesList()->Count() > 0) { mSensors->GetNoisesList()->Delete(0); };

  return "";
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcAttackerOf(ZCsl* csl)
{
  int lId    = csl->get("id").asInt();
  int lIndex = csl->get("index").asInt();

  if (lIndex < 0)
  {
    return -1;
  }

  // is it me?
  if (lId == GetId())
  {
    // check for index
    if (lIndex >= mSensors->GetAttackersList()->Count())
    {
      return -1;
    }

    return mSensors->GetAttackersList()->Get(lIndex)->GetAttacker()->GetId();
  }

  // is it a friend?
  CIGBaseObject* lObj = GetMissionInstance()->GetObjectById(csl->get("id").asInt());

  // it must be a unit
  if (!lObj->IsDerivedFrom(IGCN_UNIT))
  {
    return -1;
  }

  if (!lObj)
  {
    return -1;
  }

  // check for index
  if (lIndex >= ((CIGUnit*)lObj)->GetAttackersList()->Count())
  {
    return -1;
  }

  TPoliticsRelations lRelation = GetMissionInstance()->GetBGManager()->GetRelation(GetBattleGroupId(), lObj->GetBattleGroupId());

  switch(lRelation)
  {
    case REL_OWNED:
    case REL_FRIENDLY:
      return ((CIGUnit*)lObj)->GetAttackersList()->Get(lIndex)->GetAttacker()->GetId();

    case REL_ENEMY:
    case REL_NEUTRAL:
      return -1;
  }

  return -1;
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcAttackerOfCount(ZCsl* csl)
{
  int lId    = csl->get("id").asInt();

  // is it me?
  if (lId == GetId())
  {
    //logWrite("Attackers of "); logWriteI(GetId()); logWrite(": "); logWriteILn(GetAttackersList()->Count());
    return mSensors->GetAttackersList()->Count();
  }

  // is it a friend?
  CIGBaseObject* lObj = GetMissionInstance()->GetObjectById(csl->get("id").asInt());

  if (!lObj)
  {
    return -1;
  }

  // it must be a unit
  if (!lObj->IsDerivedFrom(IGCN_UNIT))
  {
    return -1;
  }

  TPoliticsRelations lRelation = GetMissionInstance()->GetBGManager()->GetRelation(GetBattleGroupId(), lObj->GetBattleGroupId());

  switch(lRelation)
  {
    case REL_OWNED:
    case REL_FRIENDLY:
      //logWrite("Attackers of "); logWriteI(lObj->GetId()); logWrite(": "); logWriteILn(lObj->GetAttackersList()->Count());
      return ((CIGUnit*)lObj)->GetAttackersList()->Count();

    case REL_ENEMY:
    case REL_NEUTRAL:
      return -1;
  }

  return -1;
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcMyHp(ZCsl* csl)
{
  //logWriteILn(GetCurrentHP() * 100 / GetMaxHP());
  return ZString(GetCurrentHP() * 100 / GetMaxHP());
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcMyId(ZCsl* csl)
{
  return ZString(GetId());
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcGetGroundType(ZCsl* csl)
{
  int lX = csl->get("x").asInt();
  int lY = csl->get("y").asInt();

  return ZString(GetMissionInstance()->GetMap()->GetTileType(lX, lY));
}
//---------------------------------------------------------------------------

ZString CIGUnit::mcGetMapVisibility(ZCsl* csl)
{
  int lX = csl->get("x").asInt();
  int lY = csl->get("y").asInt();

  return ZString(GetMissionInstance()->GetMap()->GetVisibility(GetBattleGroupId(), lX, lY));
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
// Object serialize/deserialize section
//
//---------------------------------------------------------------------------

void CIGUnit::Serialize(PP::Stream& _a)
{
  int        lSprProjectile = -1, lSprEffect = -1, lSprAfterEffect = -1;
  int        lScriptId = -1;
  CSerString lModuleName((char*)mModuleName);

  // setup
  if (mProjectileSprite)
  {
    lSprProjectile = GetGraphicInstance()->GetSpriteIndex(mProjectileSprite, SPRL_MISSION);
  }

  if (mEffectSprite)
  {
    lSprEffect = GetGraphicInstance()->GetSpriteIndex(mEffectSprite, SPRL_MISSION);
  }

  if (mEffectSprite)
  {
    lSprAfterEffect = GetGraphicInstance()->GetSpriteIndex(mAfterEffectSprite, SPRL_MISSION);
  }

  if (mScriptObserver)
  {
    lScriptId    = mScriptObserver->GetScriptId();
  }

  // parent class serialize
  CIGRealObject::Serialize(_a);

  // serialize
  lModuleName.Serialize(_a);
  _a.writeBool(&mValidCsl);
  _a.writeInt(&lScriptId);

  _a.writeInt(&mRange);
  _a.writeInt(&mDamage);
  _a.writeInt(&mWepType);
  _a.writeInt(&mProjectilesPerShot);

  _a.writeInt(&lSprProjectile);
  _a.writeInt(&lSprEffect);
  _a.writeInt(&lSprAfterEffect);

  _a.writeInt(&mSoundOnFire);
  _a.writeInt(&mSoundOnHit);

  _a.writeFloat(&mMaxForce);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGUnit::DataSize()
{
  return 7 * sizeof(__int32);
}
//---------------------------------------------------------------------------

void CIGUnit::DeSerialize(PP::Stream& _a)
{
  int        lScriptId;
  int        lSprProjectile, lSprEffect, lSprAfterEffect;
  CSerString lModuleName;

  // parent class deserialize
  CIGRealObject::DeSerialize(_a);

  // deserialize
  lModuleName.DeSerialize(_a);
  _a.readBool(&mValidCsl);
  _a.readInt(&lScriptId);

  _a.readInt(&mRange);
  _a.readInt(&mDamage);
  _a.readInt(&mWepType);
  _a.readInt(&mProjectilesPerShot);

  _a.readInt(&lSprProjectile);
  _a.readInt(&lSprEffect);
  _a.readInt(&lSprAfterEffect);

  _a.readInt(&mSoundOnFire);
  _a.readInt(&mSoundOnHit);

  _a.readFloat(&mMaxForce);

  // setup
  mProjectileSprite  = GetGraphicInstance()->GetSpriteByIndex(lSprProjectile, SPRL_MISSION);
  mEffectSprite      = GetGraphicInstance()->GetSpriteByIndex(lSprEffect, SPRL_MISSION);
  mAfterEffectSprite = GetGraphicInstance()->GetSpriteByIndex(lSprAfterEffect, SPRL_MISSION);

  mValidCsl   = false; // lock the use of csl, maybe script get corrupted or something
  mModuleName = ZString(lModuleName.Get().c_str());

  SetScriptObserver(GetMissionInstance()->GetDoctrine()->GetScriptObserver(lScriptId));
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

