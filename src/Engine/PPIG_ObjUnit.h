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

//-----------------------------------------------------------------------------
//  Unit              : Unit
//
//  Version           : 1.1
//
//  Description       :
//    * this class is base for ground, air and static units
//    * a unit should have a visual subsystem and scripting capabilities, may be
//      in a battle group
//    * as of 23.12.04, this class has been compacted with CIGObjGroundUnit and
//      the later removed from project
//    * also added tactical actions; these are used as high level directives to a
//      unit - you order a unit to do move to a point, but the unit will not
//      rush straight, but rather it will follow instructions given in its script
//      on how to perform the movement
//    * the tactical actions were added to the children of this class
//    * the tactical actions cannot be accessed by a unit script; i took this
//      decision for security reasons, as it will be extremely simple to create
//      a recursive call to tactical actions; only the mission control may ask
//      a unit to perform tactic actions
//
//  History           :
//    [24.05.2000] - [grabX] - unit created
//    [23.12.2004] - [Karg]  - class renamed to CIGUnit
//    [06.11.2005] - [Karg]  - added tactical actions
//-----------------------------------------------------------------------------

#ifndef PPIG_ObjUnitH
#define PPIG_ObjUnitH

#include "PPIG_ObjReal.h"
#include "ZCallback.h"
#include "PPIG_Doctrine.h"
#include "ZCsl.hpp"
#include <vector>
#include "PPIG_SubSysSensors.h"

// forward
class CIGSubSysBody;
class CIGSubSysVisual;
class CIGSubSysTactics;

class CIGUnit : public CIGRealObject  //## Inherits: <unnamed>%391723430302
{
  // friends
  friend class CIGSubSysTactics;

  public:
      virtual CPString ClassName () {return IGCN_UNIT;}
      virtual bool IsDerivedFrom (CPString className);
      virtual void Paint(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel = 1);
      bool IsWaiting ();

      virtual void NoteInLogicMap();
      virtual void NoteInLogicMap(CPPoint Point);
      virtual void RemoveFromLogicMap();

      //virtual CIGSubSysAction* GetActionInstance(TIGSubSysActionData actionData);

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  public: // get-set operations
      virtual CPGISprite* GetProjectileSprite();
      virtual void SetProjectileSprite(CPString fileName);

      virtual CPGISprite* GetEffectSprite();
      virtual void SetEffectSprite(CPString fileName);

      virtual CPGISprite* GetAfterEffectSprite();
      virtual void SetAfterEffectSprite(CPString fileName);

      virtual int GetRange();
      virtual void SetRange(int value);

      virtual int GetDamage();
      virtual void SetDamage(int value);

      virtual int GetWepType();
      virtual void SetWepType(int value);

      virtual int GetProjectilesPerShot();
      virtual void SetProjectilesPerShot(int value);

      virtual int GetSoundOnFire();
      virtual void SetSoundOnFire(CPString fileName);

      virtual int GetSoundOnHit();
      virtual void SetSoundOnHit(CPString fileName);

      virtual bool CanBePlaced(CPPoint _point);        // check the object mask against the map and
                                                       // return true if the object may be placed in the map

      virtual float GetMaxForce (void) { return mMaxForce; }
      virtual void SetMaxForce (float _v) { mMaxForce = _v; }

      virtual CPPoint PredictFuturePosition (const float predictionTime) { return CPPoint(0, 0); }

  public: // mediator (with subsystems) interface
    virtual CPList<CIGAuxAttacker>* GetAttackersList() { return mSensors ? mSensors->GetAttackersList() : 0; }

  public:
    void                  SetScriptObserver(CPPIG_ScriptObserver* _obs);
    CPPIG_ScriptObserver* GetScriptObserver() { return mScriptObserver; };

    virtual CPString GetScriptVar(CPString _name);
    virtual void     SetScriptVar(CPString _name, CPString _value);

  protected:
      CIGUnit();
      virtual ~CIGUnit();

  protected: // Karg - protected data, yuck
    CIGSubSysTactics             *mTactics;
    CIGSubSysBody                *mBody;
    CIGSubSysSensors             *mSensors;
    std::vector<CIGSubSysVisual*> mVisuals;

  protected: // Karg - and more protected data
    ZCsl*                 mCompiler;
    ZString               mModuleName;
    bool                  mValidCsl;
    CPPIG_ScriptObserver* mScriptObserver;

  protected: // script compiler
    virtual void addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback);
    virtual void InitCsl();
    virtual void InitHandlers();
    virtual void InitScriptingInterface();
    virtual void InitScriptingLibs();

  protected:
    virtual void ClearVisuals();
    virtual bool InVisualRange(int _x, int _y);

  protected: // script handlers & script methods
      ZCsl_callback_t  mHandlePostMessage;
      ZCsl_callback_t  mHandleMove;
      ZCsl_callback_t  mHandleStop;
      ZCsl_callback_t  mHandleMyX;
      ZCsl_callback_t  mHandleMyY;
      ZCsl_callback_t  mHandleMyZ;
      ZCsl_callback_t  mHandlePlaySound;
      ZCsl_callback_t  mHandleSetScanAngle;
      ZCsl_callback_t  mHandleGetScanAngle;
      ZCsl_callback_t  mHandleLookAt;
      ZCsl_callback_t  mHandleLookTo;
      ZCsl_callback_t  mHandleSweeping;
      ZCsl_callback_t  mHandleMoving;

      ZCsl_callback_t  mHandleScan;
      ZCsl_callback_t  mHandleUnitInViewCount;
      ZCsl_callback_t  mHandleUnitInView;

      ZCsl_callback_t  mHandleWeaponRange;
      ZCsl_callback_t  mHandleWeaponDamage;

      ZCsl_callback_t  mHandleInVisual;
      ZCsl_callback_t  mHandleUnitId;
      ZCsl_callback_t  mHandleUnitDist;
      ZCsl_callback_t  mHandleUnitStatus;
      ZCsl_callback_t  mHandleUnitThreat;
      ZCsl_callback_t  mHandleUnitRelation;
      ZCsl_callback_t  mHandleUnitType;
      ZCsl_callback_t  mHandleUnitX;
      ZCsl_callback_t  mHandleUnitY;
      ZCsl_callback_t  mHandleUnitZ;

      // TEST CODE FOR SDJ
      ZCsl_callback_t  mHandleUnitHP;

      ZCsl_callback_t  mHandleFireTo;
      ZCsl_callback_t  mHandleFireAt;

      ZCsl_callback_t  mHandleNoiseType;
      ZCsl_callback_t  mHandleNoiseX;
      ZCsl_callback_t  mHandleNoiseY;
      ZCsl_callback_t  mHandleNoiseCount;
      ZCsl_callback_t  mHandleNoisePurge;

      ZCsl_callback_t  mHandleAttackerOf;
      ZCsl_callback_t  mHandleAttackerOfCount;

      ZCsl_callback_t  mHandleMyHp;
      ZCsl_callback_t  mHandleMyId;

      ZCsl_callback_t  mHandleGetGroundType;
      ZCsl_callback_t  mHandleGetMapVisibility;

      ZString mcPostMessage(ZCsl* csl);
      ZString mcMove(ZCsl* csl);
      ZString mcStop(ZCsl* csl);
      ZString mcMyX(ZCsl* csl);
      ZString mcMyY(ZCsl* csl);
      ZString mcMyZ(ZCsl* csl);
      ZString mcPlaySound(ZCsl* csl);
      ZString mcSetScanAngle(ZCsl* csl);
      ZString mcGetScanAngle(ZCsl* csl);
      ZString mcSweeping(ZCsl* csl);
      ZString mcLookAt(ZCsl* csl);
      ZString mcLookTo(ZCsl* csl);
      ZString mcMoving(ZCsl* csl);

      ZString mcScan(ZCsl* csl);
      ZString mcUnitInViewCount(ZCsl* csl);
      ZString mcUnitInView(ZCsl* csl);

      ZString mcWeaponRange(ZCsl* csl);
      ZString mcWeaponDamage(ZCsl* csl);
      ZString mcInVisual(ZCsl* csl);
      ZString mcUnitId(ZCsl* csl);
      ZString mcUnitDist(ZCsl* csl);
      ZString mcUnitStatus(ZCsl* csl);
      ZString mcUnitThreat(ZCsl* csl);
      ZString mcUnitRelation(ZCsl* csl);
      ZString mcUnitType(ZCsl* csl);
      ZString mcUnitX(ZCsl* csl);
      ZString mcUnitY(ZCsl* csl);
      ZString mcUnitZ(ZCsl* csl);

      // TEST CODE FOR SDJ
      ZString mcUnitHP(ZCsl* csl);

      ZString mcFireTo(ZCsl* csl);
      ZString mcFireAt(ZCsl* csl);

      ZString mcNoiseType(ZCsl* csl);
      ZString mcNoiseX(ZCsl* csl);
      ZString mcNoiseY(ZCsl* csl);
      ZString mcNoiseCount(ZCsl* csl);
      ZString mcNoisePurge(ZCsl* csl);

      ZString mcAttackerOf(ZCsl* csl);
      ZString mcAttackerOfCount(ZCsl* csl);

      ZString mcMyHp(ZCsl* csl);
      ZString mcMyId(ZCsl* csl);

      ZString  mcGetGroundType(ZCsl* csl);
      ZString  mcGetMapVisibility(ZCsl* csl);

  private: //## implementation

      // Data Members for Class Attributes
      int         mRange;
      int         mDamage;
      int         mWepType;
      int         mProjectilesPerShot;

      // maximum turning force that can be applied by this unit
      float       mMaxForce;

      CPGISprite *mProjectileSprite;
      CPGISprite *mEffectSprite;
      CPGISprite *mAfterEffectSprite;

      int         mSoundOnFire;
      int         mSoundOnHit;
};

/////////////////////////////////////////////////////////////////////////////
//GET-SET (INLINE) SECTION
/////////////////////////////////////////////////////////////////////////////

inline int CIGUnit::GetRange()
{
   return mRange;
}
inline void CIGUnit::SetRange(int value)
{
   mRange = value;
}

/*---------------------------------------------------------------------------
 description: Damage
---------------------------------------------------------------------------*/
inline int CIGUnit::GetDamage()
{
   return mDamage;
}
inline void CIGUnit::SetDamage(int value)
{
   mDamage = value;
}

/*---------------------------------------------------------------------------
 description: Weapon Type
---------------------------------------------------------------------------*/
inline int CIGUnit::GetWepType()
{
   return mWepType;
}
inline void CIGUnit::SetWepType(int value)
{
   mWepType = value;
}

/*---------------------------------------------------------------------------
 description: Projectiles per shot
---------------------------------------------------------------------------*/
inline int CIGUnit::GetProjectilesPerShot()
{
  return mProjectilesPerShot;
}
inline void CIGUnit::SetProjectilesPerShot(int value)
{
  mProjectilesPerShot = value;
}

/*---------------------------------------------------------------------------
 description: Projectile Sprite
---------------------------------------------------------------------------*/
inline CPGISprite* CIGUnit::GetProjectileSprite()
{
   return mProjectileSprite;
}

/*---------------------------------------------------------------------------
 description: Effect Sprite
---------------------------------------------------------------------------*/
inline CPGISprite* CIGUnit::GetEffectSprite()
{
   return mEffectSprite;
}

/*---------------------------------------------------------------------------
 description: After Effect Sprite
---------------------------------------------------------------------------*/
inline CPGISprite* CIGUnit::GetAfterEffectSprite()
{
   return mAfterEffectSprite;
}

/*---------------------------------------------------------------------------
 description: Sound On Fire
---------------------------------------------------------------------------*/
inline int CIGUnit::GetSoundOnFire()
{
   return mSoundOnFire;
}

/*---------------------------------------------------------------------------
 description: Sound On Hit
---------------------------------------------------------------------------*/
inline int CIGUnit::GetSoundOnHit()
{
   return mSoundOnHit;
}
//---------------------------------------------------------------------------
#endif

