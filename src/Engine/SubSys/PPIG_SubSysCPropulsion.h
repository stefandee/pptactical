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
//  Unit              : Subsystem Chopper Propulsion
//
//  Version           : 1.0
//
//  Description       :
//    * propulsion for chopper-type (helicopter) object
//    * the chopper must be at the right altitude before moving
//    * if the chopper is at lower altitude than expected, it will first
//      rise then it will move
//
//  History           :
//    [24.02.2003] - [Karg] - unit&class created
//-----------------------------------------------------------------------------

#ifndef PPIG_SubSysCPropulsionH
#define PPIG_SubSysCPropulsionH
//---------------------------------------------------------------------------

#include "PPIG_SubSysAction.h"
#include "PPIG_SubSysBase.h"
#include "basic3d.h"
//#include "PPIG_ObjChopper.h"
class CIGChopperUnit;

class CIGSubSysCPropulsion : public CIGSubSysBase
{
  public: // c-d
    CIGSubSysCPropulsion(CIGChopperUnit* _parent);
    virtual ~CIGSubSysCPropulsion();

  public: // ops
    virtual void Update();

  public: // actions
    virtual CIGSubSysAction* GetActionInstance(TIGSubSysActionData actionData);
    virtual void OnActionStop    ();
    virtual void OnActionMove    ();
    virtual void OnActionPatrol  ();
    virtual void OnActionHold    ();
    virtual void OnActionGuard   ();
    virtual void OnActionDead    ();
    virtual void OnActionLand    ();
    virtual void OnActionTakeoff ();
    virtual void OnActionDie     ();

    virtual void StartActionMove();

  public: // overwritten get-set operations
    virtual void TurnToSense(int wantedSense);
    virtual void SetSenseCounter(int value);
    virtual void SetSenseCounter(CPCounter &value);

  protected: // utility methods
    virtual void Move(CVector3D& dir);
    virtual void Stop();

  private:
    CPCounter           mTimer;

    // the current orientation of the object in radians
    float               mCurrentAngle;

    CVector3D mRealCoord;
    CVector3D mVelocity;

    double DistanceToTarget();
    bool SteeringArrive ();
    void TurnToAngle(float Angle);
    void ResetVisual();
};

#endif
