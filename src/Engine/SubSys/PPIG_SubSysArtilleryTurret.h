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
//  Unit              : Artillery Unit Turret
//
//  Version           : 1.0
//
//  Description       :
//    * turret for artillery units; derived from normal tank turret, this one
//      fires only when the unit is not moving
//
//  History           :
//    [04.12.2002] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef PPIG_SubSysArtilleryTurretH
#define PPIG_SubSysArtilleryTurretH
//---------------------------------------------------------------------------

#include "PPIG_SubSysTurret.h"
class CIGArtilleryUnit;

class CIGSubSysArtilleryTurret : public CIGSubSysTurret
{
  public: // c'tor
    CIGSubSysArtilleryTurret(CIGArtilleryUnit* _parent);

  public: // actions
    virtual void OnActionAttack ();
    virtual void OnActionAttackGround();
};

#endif
