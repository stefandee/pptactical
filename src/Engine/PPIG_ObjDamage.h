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

 description: Class Damage Object
 last modify: 10 06 00
        by grabX

 karg - 26.08.04       
 introduced time to live for the object (the attribute existed but it was not used)       
---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
#ifndef PPIG_ObjDamageH
#define PPIG_ObjDamageH

#include "PPIG_ObjReal.h"
#include "PPIG_SubSysDPropulsion.h"
//class CIGSubSysDPropulsion;

const unsigned int DEFAULT_DAMAGE_TIME_TO_LIVE = 5000; // the default time to live,
                                                       // used unless a call to SetLife is made
                                                       // the value is expressed miliseconds

class CIGDamageObject: public CIGRealObject
{
   public:
     CIGDamageObject();
     virtual ~CIGDamageObject();

     virtual CPString ClassName () {return IGCN_DAMAGEOBJECT;}
     virtual bool IsDerivedFrom (CPString className);

     void Update();
     void Paint(CPPoint ptViewport, CPPoint ptWindow, int Width, int Height, int zoomLevel);
     virtual void Begin();

     virtual void Explode();
     virtual void CreateStimul();

   public: // action
     virtual void AddAction(TIGSubSysActionData actionData);
     virtual void AddAction (CIGSubSysAction *Action);
     virtual void AddActionWithPriority (CIGSubSysAction *Action);
     virtual void SetAction (TIGSubSysActionData actionData);

   public:
     CPSprite *GetEffectSprite();
     void SetEffectSprite(CPSprite *value);

     CPSprite *GetAfterEffectSprite();
     void SetAfterEffectSprite(CPSprite *value);

     CPSprite *GetTrackEffectSprite();
     void SetTrackEffectSprite(CPSprite *value);

     int  GetPower();
     void SetPower(int value);

     int  GetTrackPersistency();
     void SetTrackPersistency(int value);

     CIGBaseObject* GetSourceObject();
     void SetSourceObject(CIGBaseObject *value);

     CIGBaseObject *GetTargetObject();
     void SetTargetObject(CIGBaseObject *value);

     CPPoint GetTargetPoint();
     void SetTargetPoint(CPPoint value);

     int GetTargetCoordZ();
     void SetTargetCoordZ(int value);

     unsigned int GetLife();
     void SetLife(unsigned int value);

     virtual void SetSoundOnHit(int value);
     virtual int  GetSoundOnHit();

     int GetStimulType();
     void SetStimulType(int value);

     void SetSplashRadius(int _v) { mSplashRadius = _v; };
     int  GetSplashRadius() { return mSplashRadius; };

     // temporary
     virtual void SetSenseCounter(int value) { mPropulsion->SetSenseCounter(value); }
     virtual void SetSenseCounter(CPCounter& value) { mPropulsion->SetSenseCounter(value); }

   public: // serialization - deserialization
     virtual void Serialize(PP::Stream& _a);
     virtual unsigned __int32 DataSize();
     virtual void DeSerialize(PP::Stream& _a);

   protected:
     virtual bool CanBePlaced(CPPoint _point) { return true; }

     CIGSubSysDPropulsion *mPropulsion;

   private:
      CPSprite             *mEffectSprite;      // the effect sprite
      CPSprite             *mTrackEffectSprite; // the effect sprite of the 'trail smoke' anim
      CPSprite             *mAfterEffectSprite; // the effect sprite of the 'smoke' anim
      int                  mPower;
      CIGBaseObject        *mSourceObject;      // the object that created this object
      CIGBaseObject        *mTargetObject;      // the target object, if any
      CPPoint              mTargetPoint;        // the target point, if any (logic coord)
      unsigned int         mLife;               // the life remained (measured in miliseconds)
      int                  mSoundOnHit;         // the index of the sound
      int                  mStimulType;
      int                  mSplashRadius;
      int                  mTrackPersistency;
      int                  mCoordZ;
};

/////////////////////////////////////////////////////////////////////////////
//GET-SET (INLINE) SECTION
/////////////////////////////////////////////////////////////////////////////

/*---------------------------------------------------------------------------
 description: Effect Sprite
---------------------------------------------------------------------------*/
inline CPSprite *CIGDamageObject::GetEffectSprite()
{
   return mEffectSprite;
}

inline void CIGDamageObject::SetEffectSprite(CPSprite *value)
{
   mEffectSprite = value;
}

/*---------------------------------------------------------------------------
 description: Track Effect Sprite
---------------------------------------------------------------------------*/
inline CPSprite *CIGDamageObject::GetTrackEffectSprite()
{
   return mTrackEffectSprite;
}

inline void CIGDamageObject::SetTrackEffectSprite(CPSprite *value)
{
   mTrackEffectSprite = value;
}

/*---------------------------------------------------------------------------
 description: After Effect Sprite - the effect after the explosion
---------------------------------------------------------------------------*/
inline CPSprite *CIGDamageObject::GetAfterEffectSprite()
{
   return mAfterEffectSprite;
}

inline void CIGDamageObject::SetAfterEffectSprite(CPSprite *value)
{
   mAfterEffectSprite = value;
}

/*---------------------------------------------------------------------------
 description: Power
---------------------------------------------------------------------------*/
inline int CIGDamageObject::GetPower()
{
   return mPower;
}

inline void CIGDamageObject::SetPower(int value)
{
   mPower = value;
}

/*---------------------------------------------------------------------------
 description: Track Persistency
---------------------------------------------------------------------------*/
inline int  CIGDamageObject::GetTrackPersistency()
{
  return mTrackPersistency;
}

inline void CIGDamageObject::SetTrackPersistency(int value)
{
  mTrackPersistency = value;
}

/*---------------------------------------------------------------------------
 description: Source Object
---------------------------------------------------------------------------*/
inline CIGBaseObject* CIGDamageObject::GetSourceObject()
{
   return mSourceObject;
}
inline void CIGDamageObject::SetSourceObject(CIGBaseObject *value)
{
   mSourceObject = value;
}

/*---------------------------------------------------------------------------
 description: Target Object
---------------------------------------------------------------------------*/
inline CIGBaseObject* CIGDamageObject::GetTargetObject()
{
   return mTargetObject;
}
inline void CIGDamageObject::SetTargetObject(CIGBaseObject *value)
{
   mTargetObject = value;
}
/*---------------------------------------------------------------------------
 description: Target Point
---------------------------------------------------------------------------*/
inline CPPoint CIGDamageObject::GetTargetPoint()
{
   return mTargetPoint;
}
inline void CIGDamageObject::SetTargetPoint(CPPoint value)
{
   mTargetPoint = value;
}

inline int CIGDamageObject::GetTargetCoordZ()
{
  return mCoordZ;
}

inline void CIGDamageObject::SetTargetCoordZ(int value)
{
  mCoordZ = value;
}

/*---------------------------------------------------------------------------
 description: Life
---------------------------------------------------------------------------*/
inline unsigned int CIGDamageObject::GetLife()
{
   return mLife;
}
inline void CIGDamageObject::SetLife(unsigned int value)
{
   mLife = value;
}

/*---------------------------------------------------------------------------
 description: Sound On Hit
---------------------------------------------------------------------------*/
inline void CIGDamageObject::SetSoundOnHit(int value)
{
   mSoundOnHit = value;
}
inline int CIGDamageObject::GetSoundOnHit()
{
   return mSoundOnHit;
}

/*---------------------------------------------------------------------------
 description: Stimul Type
---------------------------------------------------------------------------*/
inline int CIGDamageObject::GetStimulType()
{
   return mStimulType;
}

inline void CIGDamageObject::SetStimulType(int value)
{
   mStimulType = value;
}

//---------------------------------------------------------------------------
#endif
