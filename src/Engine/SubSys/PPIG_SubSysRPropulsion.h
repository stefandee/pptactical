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
//  Unit              : Rocket SubSys Propulsion
//
//  Version           : 1.0
//
//  Description       :
//    *
//
//  History           :
//    [12.11.2002] - [Karg] - unit/class created
//-----------------------------------------------------------------------------

#ifndef PPIG_SubSysRPropulsionH
#define PPIG_SubSysRPropulsionH
//---------------------------------------------------------------------------

#include "PPIG_SubSysDPropulsion.h"
class CIGRocketObject;

class CIGSubSysRPropulsion : public CIGSubSysDPropulsion
{
  public: // c-d
    CIGSubSysRPropulsion(CIGRocketObject* _parent);
    virtual ~CIGSubSysRPropulsion();

    virtual void CreateTrackEffect();

  public: // actions
    virtual void OnActionAttack();
    virtual void OnActionAttackGround();
    virtual void OnActionSelfDestruct();

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  private:
    int mOrientation;
};

#endif
