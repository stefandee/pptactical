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
//  Unit              : Class In Game turret subsystem
//
//  Version           : 1.0
//
//  Description       :
//    * implementation for object subsystem turret
//
//  History           :
//    [05.11.2002] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef PPIG_SubSysTurretH
#define PPIG_SubSysTurretH
//---------------------------------------------------------------------------

#include "PPIG_SubSysBase.h"
#include <stdlib.h>

class CIGTankUnit;

class CIGSubSysTurret : public CIGSubSysBase
{
  public: // c-d
    CIGSubSysTurret(CIGTankUnit* _parent);
    virtual ~CIGSubSysTurret();

    virtual void Update();

  public: // ops
    virtual CIGSubSysAction* GetActionInstance(TIGSubSysActionData actionData);

  public: // get-set
    virtual void SetSense(int _v) { mSense = abs(_v) % 8;  };
    virtual int  GetSense() { return mSense; };

    virtual void SetWantedSense(int _v) { mWantedSense = abs(_v) % 8;  };
    virtual int  GetWantedSense() { return mWantedSense; };

  public: // actions - are called by the SubSysTurretAction classes
    virtual void OnActionStop   ();
    virtual void OnActionRotate ();
    virtual void OnActionAttack ();
    virtual void OnActionAttackGround();

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  protected:
    CIGSubSysTurret();

    virtual void Turn();
    virtual void Fire();
    virtual void ComputeDir();

  private:
    int mSense, mWantedSense, mDir;
};

#endif
