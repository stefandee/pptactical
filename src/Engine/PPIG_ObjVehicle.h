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
//  Unit              : Class In Game Vehicle Unit
//
//  Version           : 1.0
//
//  Description       :
//    * abstract base for vehicles unit
//    * contains common behavior of all vehicles: scripting, propulsion, drawing, equipment managing
//    * also, a vehicle may be able to carry infantry units
//    * all vehicles share: armor, weapons, equipment, crew (from tanks to transporters)
//
//  History           :
//    [25.10.2002] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef PPIG_ObjVehicleH
#define PPIG_ObjVehicleH
//---------------------------------------------------------------------------

#include "AIUnits.h"
#include "Equipment.h"
#include "PPIG_ObjUnit.h"

class CIGVehicle : public CIGUnit
{
  public: // get-set
    virtual void SetVehicleUnifiedData(TVehicleUnified& value);
    virtual TVehicleUnified& GetVehicleUnifiedData() { return mVehicleUnifiedData; }

    //virtual void Move();
    virtual void SetSprite (CPSprite * value);

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  public: // generic
    virtual ~CIGVehicle();
    virtual CPString ClassName () { return IGCN_VEHICLEUNIT; }
    virtual bool IsDerivedFrom (CPString className);

  public: // get-set, mediating with subsystems
    virtual int  GetBodySenseCounter() { return 0; }
    virtual void SetBodySenseCounter(int _v) {}

    virtual int GetTurretSenseCounter() { return 0; }
    virtual void SetTurretSenseCounter(int _v) {}

    virtual int GetScanAngle() { return 0; }
    virtual void SetScanAngle(int _v) {}

  protected: // c-d
    CIGVehicle();

  private:
    TVehicleUnified     mVehicleUnifiedData;     //for map units
};

#endif
