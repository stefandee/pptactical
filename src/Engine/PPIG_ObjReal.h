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

 description: Class In Game Real Object
 last modify: 12 06 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PPIG_ObjRealH
#define PPIG_ObjRealH

#include "PPIG_ObjBase.h"
#include "PPIG_ObjList.h"

class CIGRealObject : public CIGBaseObject
{
  public:
      virtual CPString ClassName () {return IGCN_REALOBJECT;}
      virtual bool IsDerivedFrom (CPString className);
      virtual void PaintSelection (CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel = 1);
      virtual void PaintTeam (CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel = 1);
      virtual void PaintHealthBar (CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevele = 1);
      virtual void PaintMinimized (CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevele = 1);

      virtual bool IsEnemy(int X, int Y);
      virtual bool IsInTeam (int Team);
      virtual bool ContainsObjects();
      virtual bool CanContainObjects();

  public:
    //## Get and Set Operations for Class Attributes (generated)
      virtual bool GetSelected ();
      virtual void SetSelected (bool value);

      virtual int GetTeam ();
      virtual void SetTeam (int value);

      virtual int GetMaxHP ();
      virtual void SetMaxHP (int value);

      virtual int GetCurrentHP ();
      virtual void SetCurrentHP (int value);

      virtual CIGBaseObjectList * GetContainedObjects ();
      virtual void SetContainedObjects (CIGBaseObjectList * value);

      virtual int GetBulletRes();
      virtual void SetBulletRes(int value);

      virtual int GetEnergyRes();
      virtual void SetEnergyRes(int value);

      virtual int GetFireRes();
      virtual void SetFireRes(int value);

      virtual int GetPlasmaRes();
      virtual void SetPlasmaRes(int value);

      virtual int GetExplodeRes();
      virtual void SetExplodeRes(int value);

      virtual int  GetMass();
      virtual void SetMass(int value);

      virtual int GetBattleGroupId() { return mBattleGroupId; }
      virtual void SetBattleGroupId(int _v) { mBattleGroupId = _v; }

      virtual int GetId() { return mId; }
      virtual void SetId(int _v) { mId = _v; }

      virtual void SetUpdateTime(int _v) { mUpdateTime = _v; }
      virtual int  GetUpdateTime() { return mUpdateTime; }

      virtual void SetUpdateTimePeriod(int _v) { mUpdateTimePeriod = _v; }
      virtual int  GetUpdateTimePeriod() { return mUpdateTimePeriod; }

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  protected:
      CIGRealObject();
      virtual ~CIGRealObject();

      //virtual void TurnToSense(int wantedSense);

  private: //## implementation
      bool                 mSelected;           //shows if the object is selected
      int                  mTeam;               //the current team
      int                  mMaxHP;              //max of HP
      int                  mCurrentHP;          //current amount of HP
      CIGBaseObjectList   *mContainedObjects;   //the list of contained objects

      int                  mBattleGroupId;      //the battle group this object belongs to
      int                  mId;                 //the object id

      int                  mBulletRes;
      int                  mEnergyRes;
      int                  mFireRes;
      int                  mPlasmaRes;
      int                  mExplodeRes;
      int                  mMass;

      int                  mUpdateTime, mUpdateTimePeriod; // update parameters
};

/////////////////////////////////////////////////////////////////////////////
//GET-SET (INLINE) SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: Selected
---------------------------------------------------------------------------*/
inline bool CIGRealObject::GetSelected ()
{
  return mSelected;
}

inline void CIGRealObject::SetSelected (bool value)
{
  mSelected = value;
}

/*---------------------------------------------------------------------------
 description: Side
---------------------------------------------------------------------------*/
/*
inline int CIGRealObject::GetSide ()
{
  return mSide;
}

inline void CIGRealObject::SetSide (int value)
{
  mSide = value;
}
*/

/*---------------------------------------------------------------------------
 description: Team
---------------------------------------------------------------------------*/
inline int CIGRealObject::GetTeam ()
{
  return mTeam;
}

inline void CIGRealObject::SetTeam (int value)
{
  mTeam = value;
}

/*---------------------------------------------------------------------------
 description: MaxHP
---------------------------------------------------------------------------*/
inline int CIGRealObject::GetMaxHP ()
{
  return mMaxHP;
}

inline void CIGRealObject::SetMaxHP (int value)
{
  if (value > 0)
     mMaxHP = value;
}

/*---------------------------------------------------------------------------
 description: CurrentHP
---------------------------------------------------------------------------*/
inline int CIGRealObject::GetCurrentHP ()
{
  return mCurrentHP;
}

inline void CIGRealObject::SetCurrentHP (int value)
{
  mCurrentHP = value;
  if (value <= 0)
  {
     mCurrentHP = 0;
     Die();
   }
}

/*---------------------------------------------------------------------------
 description: ContainedObjects
---------------------------------------------------------------------------*/
inline CIGBaseObjectList * CIGRealObject::GetContainedObjects ()
{
  return mContainedObjects;
}

inline void CIGRealObject::SetContainedObjects (CIGBaseObjectList * value)
{
  mContainedObjects = value;
}

/*---------------------------------------------------------------------------
 description: Bullet Res
---------------------------------------------------------------------------*/
inline int CIGRealObject::GetBulletRes()
{
   return mBulletRes;
}
inline void CIGRealObject::SetBulletRes(int value)
{
   mBulletRes = value;
}

/*---------------------------------------------------------------------------
 description: Energy Res
---------------------------------------------------------------------------*/
inline int CIGRealObject::GetEnergyRes()
{
   return mEnergyRes;
}
inline void CIGRealObject::SetEnergyRes(int value)
{
   mEnergyRes = value;
}

/*---------------------------------------------------------------------------
 description: Fire Res
---------------------------------------------------------------------------*/
inline int CIGRealObject::GetFireRes()
{
   return mFireRes;
}
inline void CIGRealObject::SetFireRes(int value)
{
   mFireRes = value;
}

/*---------------------------------------------------------------------------
 description: Plasma Res
---------------------------------------------------------------------------*/
inline int CIGRealObject::GetPlasmaRes()
{
   return mPlasmaRes;
}
inline void CIGRealObject::SetPlasmaRes(int value)
{
   mPlasmaRes = value;
}

/*---------------------------------------------------------------------------
 description: Explode Res
---------------------------------------------------------------------------*/
inline int CIGRealObject::GetExplodeRes()
{
   return mExplodeRes;
}
inline void CIGRealObject::SetExplodeRes(int value)
{
   mExplodeRes = value;
}
//---------------------------------------------------------------------------

inline int CIGRealObject::GetMass()
{
  return mMass;
}
//---------------------------------------------------------------------------

inline void CIGRealObject::SetMass(int value)
{
  if (value >= 0)
  {
    mMass = value;
  }
}
//---------------------------------------------------------------------------

#endif

