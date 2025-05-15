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
//  Unit              :
//
//  Version           :
//
//  Description       :
//    *
//  History           :
//    [] - [] -
//-----------------------------------------------------------------------------

#ifndef PPIG_CDropshipInfoH
#define PPIG_CDropshipInfoH

#include "PP_CWinControl.h"

//class CIGVehicle;
class CIGDropship;

class CPIDropshipInfo : public CPIWinControl
{
  public: //c-d
    CPIDropshipInfo(CPIWinControl *Parent = NULL);
    virtual ~CPIDropshipInfo();

  public: // get-set
    void SetDropshipUnit(CIGDropship* _unit);

  public: // ops
    virtual void Paint();

  public: // handlers
    virtual void OnLeftMouseDown(int X, int Y);

  private:
    CIGDropship* mDropshipUnit;
    CPGISprite  *mSpriteInfantry, *mSpriteVehicle;
};

//---------------------------------------------------------------------------
#endif
