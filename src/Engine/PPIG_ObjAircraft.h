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

//-----------------------------------------------------------------------------
//  Unit              : Pure Power In-game Object Aircraft
//
//  Version           : 1.0
//
//  Description       :
//    * base class for all flying objects
//    * it has the same interface and members as a CIGObjVehicle (since vehicles and
//      aircrafts share the same struct for data)
//    * I have chosen this approach because the description struct will be separated
//      for vehicles and aircrafts
//
//  History           :
//    [22.02.2003] - [Karg] - unit&class created
//-----------------------------------------------------------------------------

#ifndef PPIG_ObjAircraftH
#define PPIG_ObjAircraftH
//---------------------------------------------------------------------------

#include "PPIG_ObjUnit.h"
#include "AIUnits.h"
#include "Equipment.h"
#include "ZCsl.hpp"
#include "PPIG_Doctrine.h"
#include "PPIG_SubSysBody.h"
#include "PPIG_SubSysSensors.h"
#include "PPIG_SubSysVisual.h"
#include <vector>

class CIGAircraft : public CIGUnit
{
  public: // get-set
    virtual void SetAircraftUnifiedData(TVehicleUnified& value);
    virtual TVehicleUnified& GetAircraftUnifiedData() { return mAircraftUnifiedData; }

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  public: // generic
    virtual ~CIGAircraft();
    virtual CPString ClassName () { return IGCN_AIRUNIT; }
    virtual bool IsDerivedFrom (CPString className);

  public: // paint details
    virtual void PaintSelection (CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel = 1);
    //virtual void PaintTeam (CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel = 1);
    //virtual void PaintHealthBar (CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevele = 1);

  public: // get-set, mediating with subsystems
    virtual int  GetSenseCounter() { return 0; }
    virtual void SetSenseCounter(int _v) {}

    virtual int GetScanAngle() { return 0; }
    virtual void SetScanAngle(int _v) {}

    //virtual void NoteInLogicMap();
    //virtual void NoteInLogicMap(CPPoint Point);

    virtual void ComputeWay() {};
    virtual bool CanBePlaced(CPPoint _point);        // check the object mask against the map and
                                                     // return true if the object may be placed in the map
    virtual bool CanLand(CPPoint _point);
    virtual void NoteStimul(CIGObjectStimul &stimul);
    virtual CPPoint GetClosestPointTo(CPPoint targetPoint);
    virtual void Die();

  protected: // c-d
    CIGAircraft();

  protected: // script
    virtual void InitScriptingInterface();
    virtual void InitHandlers();

  protected: // script handlers
    ZCsl_callback_t  mHandleUnload;
    ZCsl_callback_t  mHandleUnloadAll;
    ZCsl_callback_t  mHandleLand;

    // the unload actions are only specific to transporters, not to all
    // aircrafts. however, since only dropships can handle this action, there
    // is no problem making the scripting available for all aircrafts
    ZString mcUnload(ZCsl* csl);
    ZString mcUnloadAll(ZCsl* csl);
    ZString mcLand(ZCsl* csl);

  protected: // common subsystems
    //CIGSubSysBody           *mBody;
    //CIGSubSysSensors        *mSensors;
    //CIGSubSysVisual         *mVisual;
    //std::vector<CIGSubSysVisual*> mVisuals;

  private:
    TVehicleUnified     mAircraftUnifiedData;     //for map units
};

#endif
