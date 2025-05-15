//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998 - 2003 Stefan Dicu & Tudor Girba                       //
// Implemented by Florin B. & Stefan Dicu                                    // 
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

#ifndef __PPIG_SUBSYSFPROPULSION_H__
#define __PPIG_SUBSYSFPROPULSION_H__

#include "PPIG_SubSysAction.h"
#include "PPIG_SubSysBase.h"

class CIGFighterUnit;

#define HOVERING 1
#define MOVING   2
const int AIRCRAFT_PROPULSION_RADIUS = 8;

class CIGSubSysFPropulsion : public CIGSubSysBase
{
  public:
      CIGSubSysFPropulsion(CIGFighterUnit * parent);
      virtual ~CIGSubSysFPropulsion();

  public:
      virtual CIGSubSysAction *GetActionInstance(TIGSubSysActionData actionData);

      virtual void OnActionStop  ();
      virtual void OnActionMove  ();
      virtual void OnActionPatrol();
      virtual void OnActionHold  ();
      virtual void OnActionGuard ();
      virtual void OnActionDead  ();
      virtual void OnActionDie   ();

  protected:
      virtual void Move();
      virtual void ComputeMoveParameters();

  public: // overwritten get-set operations
    virtual void TurnToSense(int wantedSense);
    virtual void SetSenseCounter(int value);
    virtual void SetSenseCounter(CPCounter &value);

  private:
    CPPoint             m_StopPt;
    unsigned long       m_Flag;
    float               m_Angle;
    CPCounter           mTimer;      
};

#endif //__PPIG_SUBSYSFPROPULSION_H__
