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
//  Unit              :
//
//  Version           :
//
//  Description       :
//    *
//  History           :
//    [] - [] -
//-----------------------------------------------------------------------------

#ifndef PPIG_SubSysVPropulsionH
#define PPIG_SubSysVPropulsionH
//---------------------------------------------------------------------------

#include "PPIG_SubSysAction.h"
#include "PPIG_SubSysBase.h"

class CIGVehicle;

class CIGSubSysVPropulsion : public CIGSubSysBase
{
  public: // c-d
    CIGSubSysVPropulsion(CIGVehicle* _parent);
    virtual ~CIGSubSysVPropulsion();

  public: // ops
    //virtual void Update();

  public: // actions
    virtual CIGSubSysAction* GetActionInstance(TIGSubSysActionData actionData);
    virtual void OnActionStop ();
    virtual void OnActionMove ();
    virtual void OnActionPatrol ();
    virtual void OnActionHold ();
    virtual void OnActionGuard ();
    virtual void OnActionDead();
    virtual void OnActionLoad ();

  protected: // utility methods
    virtual bool IsNextPlaceEmpty();
    virtual bool IsNextObjectMoving();

    virtual void Move();
    virtual void Stop();

  protected: // overwritten get-set operations
      virtual void TurnToSense(int wantedSense);
      virtual void SetSenseCounter(int value);
      virtual void SetSenseCounter(CPCounter &value);
};

#endif
