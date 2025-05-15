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

#ifndef PPIG_SubSysPropulsionH
#define PPIG_SubSysPropulsionH
//---------------------------------------------------------------------------

#include "PPIG_SubSysAction.h"
#include "PPIG_SubSysBase.h"

class CIGInfantryUnit;

class CIGSubSysPropulsion : public CIGSubSysBase
{
  public: // c-d
    CIGSubSysPropulsion(CIGInfantryUnit* _parent);
    virtual ~CIGSubSysPropulsion();

  public: // actions
    virtual CIGSubSysAction* GetActionInstance(TIGSubSysActionData actionData);
    virtual void OnActionStop ();
    virtual void OnActionMove ();
    virtual void OnActionPatrol ();
    virtual void OnActionAttack ();
    virtual void OnActionAttackGround ();
    virtual void OnActionHold ();
    virtual void OnActionLoad ();
    virtual void OnActionGuard ();
    virtual void OnActionCapture ();
    virtual void OnActionDead();

  public: // ops
    virtual void Fire();
    virtual void Move();
    virtual void Stop();

  public: // get-set
    virtual void SetSenseCounter(int value);
    virtual void SetSenseCounter(CPCounter &value);

  protected: // utility methods
    virtual bool IsNextPlaceEmpty();
    virtual bool IsNextObjectMoving();

  protected: // overwritten get-set operations
    virtual void TurnToSense(int wantedSense);

  private:
};

#endif
