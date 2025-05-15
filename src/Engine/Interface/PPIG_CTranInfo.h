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

#ifndef PPIG_CTranInfoH
#define PPIG_CTranInfoH

#include "PP_CWinControl.h"

//class CIGVehicle;
class CIGTranUnit;

class CPITranInfo : public CPIWinControl
{
  public: //c-d
    CPITranInfo(CPIWinControl *Parent = NULL);
    virtual ~CPITranInfo();

  public: // get-set
    void SetTranUnit(CIGTranUnit* _unit);

  public: // ops
    virtual void Paint();

  public: // handlers
    virtual void OnLeftMouseDown(int X, int Y);

  private:
    CIGTranUnit* mTranUnit;
    CPGISprite *mSpriteInfantry;
};

//---------------------------------------------------------------------------
#endif
