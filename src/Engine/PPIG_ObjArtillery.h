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
//  Unit              : Object InGame Artillery
//
//  Version           : 1.0
//
//  Description       :
//    * artillery vehicle is a tank with: low speed, low view range,
//      low fov, may only climb at small angles
//    * an artillery may not fire when moving
//
//  History           :
//    [03.12.2002] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef PPIG_ObjArtilleryH
#define PPIG_ObjArtilleryH

#include "PPIG_ObjTank.h"

class CIGArtilleryUnit : public CIGTankUnit
{
  public:
    CIGArtilleryUnit();
    virtual ~CIGArtilleryUnit();

    virtual CPString ClassName     () { return IGCN_ARTILLERYUNIT; }
    virtual bool     IsDerivedFrom (CPString className);

  public: // action
    virtual void AddAction             (TIGSubSysActionData actionData);
    virtual void AddAction             (CIGSubSysAction *Action);
    virtual void AddActionWithPriority (CIGSubSysAction *Action);
    virtual void SetAction             (TIGSubSysActionData actionData);

  protected:

  private:
};
//---------------------------------------------------------------------------

#endif
