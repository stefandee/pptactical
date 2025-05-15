//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2004 Stefan Dicu & Tudor Girba                         //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License //
// as published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public    //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Unit              : SubSystem Tactics
//
//  Version           : 1.0
//
//  Description       :
//    * subsystem that handles several actions in a highlevel manner
//    * it handles move, patrol and attack actions by calling a predefined function
//      within the script
//    * it helps to separate between goal of an action (high level move e.g. reaching a position)
//      and actual implementation (low level move - pathfinding/tile-by-tile movement)
//    * an order issued through UI is a high level order
//    * by default, the implementation falls back to common behavior as defined
//      by each unit (eg: move will actually call propulsion move)
//    * one has to provide a script to implement custom behavior
//
//  History           :
//    [28.09.2005] - [Karg] - unit created
//-----------------------------------------------------------------------------


#ifndef PPIG_SubSysTacticsH
#define PPIG_SubSysTacticsH
//---------------------------------------------------------------------------

#include "PPIG_SubSysBase.h"

class CIGUnit;

class CIGSubSysTactics : public CIGSubSysBase
{
  public: // c-d
    CIGSubSysTactics(CIGUnit* _parent);
    virtual ~CIGSubSysTactics();

  public: // actions
    virtual CIGSubSysAction* GetActionInstance(TIGSubSysActionData actionData);
    virtual void OnActionTacticMove   ();
    virtual void OnActionTacticPatrol ();
    virtual void OnActionTacticAttack ();
    virtual void OnActionTacticAttackGround ();
    virtual void OnActionTacticStop   ();

  protected:
  private:
};

#endif
