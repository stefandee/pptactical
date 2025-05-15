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
//  Unit              : Bay Manager (former Infantry Bay)
//
//  Version           : 1.0
//
//  Description       :
//    * subsystem that can store infantry units; it is mainly used by transporters
//      units (cigtranunit, dropships)
//    * class renamed
//    * this class is responsible for the units it keeps; that is, it won't serialize
//      only the unit id's, but the whole unit, since that unit does not exists in
//      mission and map anymore
//
//  History           :
//    [04.12.2002] - [Karg] - unit created
//    [24.07.2004] - [Karg] - class/unit renamed to Bay Manager
//-----------------------------------------------------------------------------

#ifndef PPIG_SubSysBayManagerH
#define PPIG_SubSysBayManagerH
//---------------------------------------------------------------------------

#include "PPIG_SubSysBase.h"
#include "PPIG_ObjList.h"

class CIGInfantryUnit;
class CIGVehicle;
class CIGUnit;

class CIGSubSysBayManager : public CIGSubSysBase
{
  public: // c-d
    CIGSubSysBayManager(CIGUnit* _parent);
    virtual ~CIGSubSysBayManager();

  public: // ops
    bool LoadUnit(CIGInfantryUnit* _unit);
    bool LoadUnit(CIGVehicle* _unit);
    //bool UnLoadUnit(CIGInfantryUnit* _unit);
    //bool UnLoadAll();

  public: // actions
    virtual CIGSubSysAction* GetActionInstance(TIGSubSysActionData actionData);

    virtual void OnActionUnload();
    virtual void OnActionUnloadAll();

  public: // get-set
    int GetCapacity() { return mUnitList->Count(); }
    void SetCapacity(int _v);
    CIGUnit* GetUnit(int _index) { return (CIGUnit*)(mUnitList->GetObject(_index)); }
    void RemoveUnit(CIGUnit* _unit);

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  protected:
    virtual CPPoint GetDropPoint(CIGUnit* _unit);

  private:
    //CPList<CIGUnit>*     mUnitList;
    CIGBaseObjectList*   mUnitList;
    int                  mCapacity;
};

//---------------------------------------------------------------------------

#endif
