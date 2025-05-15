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
//  Unit              : In-Game Chopper Object
//
//  Version           : 1.0
//
//  Description       :
//    * a chopper has two propulsions - one to move while airborne, another
//      to land/takeoff
//
//  History           :
//    [24.02.2003] - [Karg] - unit&class created
//-----------------------------------------------------------------------------

#ifndef PPIG_ObjChopperH
#define PPIG_ObjChopperH
//---------------------------------------------------------------------------

#include "PPIG_ObjAircraft.h"
#include "PPIG_SubSysCPropulsion.h"
#include "PPIG_SubSysBody.h"
#include "PPIG_SubSysSensors.h"
#include "PPIG_SubSysVisual.h"
#include "PPIG_SubSysChopperWeapons.h"

class CIGSubSysOscilator;

class CIGChopperUnit : public CIGAircraft
{
    friend class CIGSubSysChopperWeapons;

  public: // c-d
    CIGChopperUnit();
    virtual ~CIGChopperUnit();

  public: // ops
    virtual void Update();
    virtual void Paint(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel);
    virtual CPRect GetPaintRect();

  public: // generic
    virtual CPString ClassName () { return IGCN_CHOPPERUNIT; }
    virtual bool IsDerivedFrom (CPString className);

    virtual void NoteInLogicMap();
    virtual void NoteInLogicMap(CPPoint Point);
    virtual bool CanBePlaced(CPPoint _point);
    virtual bool Pick(CPPoint _point);

  public: // action
    virtual void AddAction(TIGSubSysActionData actionData);
    virtual void AddAction (CIGSubSysAction *Action);
    virtual void AddActionWithPriority (CIGSubSysAction *Action);
    virtual void SetAction (TIGSubSysActionData actionData);

  public: // should be refactored
    //void        UpdateView(int _value) { mVisuals[0]->UpdateView(_value); }
    TUnitLook GetLookAt() { return mVisuals[0]->GetLookAt(); }

    int GetScanAngle() { return mVisuals[0]->GetScanAngle(); }
    void SetScanAngle(int _v) { mVisuals[0]->SetScanAngle(_v); }

    int GetScanRange() { return mVisuals[0]->GetScanRange(); }
    void SetScanRange(int _v) { mVisuals[0]->SetScanRange(_v); }

    void SetSenseCounter(int _v);
    int  GetSenseCounter();

    void SetOscilatorUpdateEnabled(bool _v);

    void UpdateView(int _value);

    virtual bool Can(int _capability);

    virtual float GetMaxSpeed (void);

  public: // get-set
    virtual void SetAircraftUnifiedData(TVehicleUnified& value);

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  protected:
    virtual void CreatePropulsion();

    CIGSubSysCPropulsion    *mPropulsion;
    CIGSubSysChopperWeapons *mWeapons;
    CIGSubSysOscilator     *mOscilator;

  private:
};
//---------------------------------------------------------------------------

#endif
