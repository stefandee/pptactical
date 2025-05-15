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
//  Unit              : Dropship Unit
//
//  Version           : 1.1
//
//  Description       :
//    * the dropship is a chopper that can carry infantry and vehicles
//    * the dropship is slower than a chopper
//    * bays (infantry/vehiles) should be one class actually, as they are just
//      containers and (what can be loaded into the bay) is decided outside
//      the container
//    * the dropship needs a new propulsion that implements the unloadall/unload
//      action
//
//  History           :
//    [13.07.2000] - [grabX] - unit created
//    [24.07.2004] - [Karg]  - dropship is now a chopper (no longer a static)
//-----------------------------------------------------------------------------

#ifndef PPIG_ObjDropshipH
#define PPIG_ObjDropshipH

#include "PPIG_ObjChopper.h"

class CIGSubSysBayManager;
class CIGInfantryUnit;
class CIGVehicle;

class CIGDropship: public CIGChopperUnit
{
  public: // ctor
    CIGDropship();
    virtual ~CIGDropship();

  public: // id
    virtual CPString ClassName () {return IGCN_DROPSHIPUNIT;}
    virtual bool IsDerivedFrom (CPString className);

  public: // ops
    bool            Load(CIGInfantryUnit* _v);
    bool            Load(CIGVehicle* _v);
    virtual bool    Can(int _capability);
    virtual void    Update();

  public: // get-set
    virtual void SetAircraftUnifiedData(TVehicleUnified& value);

  public: // facade ops
    virtual int GetCapacity();
    virtual CIGUnit* GetUnit(int _index);
    virtual void RemoveUnit(CIGUnit* _unit);

  public: // action
    virtual void AddAction(TIGSubSysActionData actionData);
    virtual void AddAction (CIGSubSysAction *Action);
    virtual void AddActionWithPriority (CIGSubSysAction *Action);
    virtual void SetAction (TIGSubSysActionData actionData);

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  protected: // script
    virtual void InitScriptingInterface();
    virtual void InitHandlers();

  protected: // script handlers
    ZCsl_callback_t  mHandleBayCount;

    ZString mcBayCount(ZCsl* csl);

  protected:
    virtual void CreatePropulsion();

  private:
    CIGSubSysBayManager* mBay;
};

//---------------------------------------------------------------------------
#endif
