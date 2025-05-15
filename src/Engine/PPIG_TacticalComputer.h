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
//  Unit              : Tactical Computer
//
//  Version           : 1.0
//
//  Description       :
//    * contains code for combat rules (damage, hit percents, so on)
//
//  History           :
//    [12.06.2004] - [Karg] - unit&class created
//-----------------------------------------------------------------------------

#ifndef PPIG_TacticalComputerH
#define PPIG_TacticalComputerH
//---------------------------------------------------------------------------

#include "Serializable.h"

class CIGTacticalComputer : public CSerializable
{
  public:
    CIGTacticalComputer();
    ~CIGTacticalComputer();

  protected:

  private:
};


#endif
