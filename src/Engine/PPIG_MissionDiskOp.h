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
//  Unit              : Mission Disk Operations
//
//  Version           : 1.0
//
//  Description       :
//    * contains an utility class used to perform disk operations for the mission
//      singleton
//
//  History           :
//    [25.09.2005] - [Karg] - created
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------

#ifndef PPIG_MissionDiskOpH
#define PPIG_MissionDiskOpH
//---------------------------------------------------------------------------

#include "PP_String.h"

class CIGMissionDiskOp
{
  public:
    static bool Save(CPString fileName);
    static bool Load(CPString fileName);
    static bool Delete(CPString fileName);
};

#endif
