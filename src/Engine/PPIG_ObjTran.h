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
//  Unit              : Transporter Unit
//
//  Version           : 1.0
//
//  Description       :
//    * the transporter is a tank unit that has an infantry bay subsystem - that is
//      it can carry a number of infantry units
//    * otherwise it behaves as a normal tank; it has the speed of a recon; the
//      fov is 180, may climb steeps between a tank and a recon, view range of
//      an infantry unit
//    * the tran needs a new propulsion that implements the unloadall/unload action
//
//  History           :
//    [04.12.2002] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef PPIG_ObjTranH
#define PPIG_ObjTranH

#include "PPIG_ObjTank.h"
//---------------------------------------------------------------------------

class CIGSubSysBayManager;
class CIGInfantryUnit;

class CIGTranUnit : public CIGTankUnit
{
  public: // c-d
    CIGTranUnit();
    virtual ~CIGTranUnit();

  public: // class
    virtual CPString ClassName () {return IGCN_TRANUNIT;}
    virtual bool IsDerivedFrom (CPString className);


  public: // ops
    virtual bool    Can(int _capability);
    bool            Load(CIGInfantryUnit* _v);
    virtual void    SetVehicleUnifiedData(TVehicleUnified& value);
    virtual void    Update();

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

  protected:

  private:
    CIGSubSysBayManager* mInfantryBay;
};
//---------------------------------------------------------------------------

#endif
