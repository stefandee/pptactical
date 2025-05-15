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
//  Unit              : Class In Game Infantry Unit
//
//  Version           : 1.0
//
//  Description       :
//    * implementation for infantry unit
//    * objbase classes (including CIGInfantryUnit) are adapters - adapt user
//      actions (from CIGMission) to subsystems actions - maybe not an adapter
//      in the classic Fantastic Four style :)
//    * from other point of view, objects are mediators between subsystems; it's
//      somehow ok because the subsystems are to be reused, while
//      ojects like tanks and infantry are leafs in the obj hierarchy, with well
//      defined behaviour
//
//  History           :
//    [13.06.2000] - [grabX] - initial implementation
//    [15.01.2002] - [Karg]  - added script support
//    [15.02.2002] - [Karg]  - added basic functions to script lib
//    [19.02.2002] - [Karg]  - must obtain observers to certain units (target, attackers) from mission instance!!!
//    [10.11.2002] - [Karg]  - refactored for subsystems organization
//    [11.11.2002] - [Karg]  - added add/set action methods
//-----------------------------------------------------------------------------

#ifndef PPIG_ObjInfantryH
#define PPIG_ObjInfantryH

#include "PPIG_ObjUnit.h"
#include "AIUnits.h"
#include "Soldier.h"
#include "Equipment.h"
#include "PPIG_SubSysVisual.h"
#include <vector>

class CIGSubSysPropulsion;

class CIGInfantryUnit : public CIGUnit
{
  public:
      CIGInfantryUnit();
      virtual ~CIGInfantryUnit();

      virtual CPString ClassName () {return IGCN_INFANTRYUNIT;}
      virtual bool IsDerivedFrom (CPString className);

      virtual void SetInfUnifiedData(TInfantryUnified& value);
      virtual TInfantryUnified& GetInfUnifiedData();

      virtual void SetInfData(struct _Infantry& value);
      virtual struct _Infantry& GetInfData();

      virtual void SetInfWeaponData(struct _IWeapon& value);
      virtual struct _IWeapon& GetWeaponData();

      virtual void SetInfArmorData(struct _IArmor& value);
      virtual struct _IArmor& GetInfArmorData();

      virtual void SetSprite(CPSprite * value);

      virtual void NoteStimul(CIGObjectStimul &stimul);

      virtual void Update();
      virtual void Paint(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel);
      virtual CPRect GetPaintRect();

      virtual void ComputeWay();

      // temporary get-set section
      void        UpdateView(int _value);
      TUnitLook   GetLookAt();

      int         GetScanAngle();
      void        SetScanAngle(int _v);

      int         GetScanRange();
      void        SetScanRange(int _v);

      void SetSenseCounter(int _v);
      int  GetSenseCounter();

      virtual bool Can(int _capability);

  public: // action
    virtual void AddAction(TIGSubSysActionData actionData);
    virtual void AddAction (CIGSubSysAction *Action);
    virtual void AddActionWithPriority (CIGSubSysAction *Action);
    virtual void SetAction (TIGSubSysActionData actionData);

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  private:
      TInfantryUnified     mInfUnifiedData;     //for map units
      struct _Infantry     mInfData;            //for owned units
      struct _IArmor       mInfArmorData;       //for owned units
      struct _IWeapon      mInfWeaponData;      //for owned units

      CIGSubSysPropulsion    *mPropulsion;
};

/////////////////////////////////////////////////////////////////////////////
//GET-SET (INLINE) SECTION
/////////////////////////////////////////////////////////////////////////////
inline TInfantryUnified& CIGInfantryUnit::GetInfUnifiedData()
{
   return mInfUnifiedData;
}

inline struct _Infantry& CIGInfantryUnit::GetInfData()
{
   return mInfData;
}

inline struct _IWeapon& CIGInfantryUnit::GetWeaponData()
{
   return mInfWeaponData;
}

/*---------------------------------------------------------------------------
 description: Infantry Armor Data
---------------------------------------------------------------------------*/
inline void CIGInfantryUnit::SetInfArmorData(struct _IArmor& value)
{
   mInfArmorData = value;
   SetBulletRes(value.bullet_res);
   SetEnergyRes(value.nrg_res);
   SetFireRes(value.fire_res);
   SetPlasmaRes(value.plasma_res);
   SetExplodeRes(value.proj_res);
}

inline struct _IArmor& CIGInfantryUnit::GetInfArmorData()
{
   return mInfArmorData;
}

//---------------------------------------------------------------------------
#endif

