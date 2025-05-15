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
//  Unit              : Group Info Control (PPIG_CGroupInfo.h)
//
//  Version           : 1.0
//
//  Description       :
//    * this control is used to show icons of selected units, when the selection
//      contains more than one unit
//    * when the user clicks on a unit icon, that unit will be selected
//
//  History           :
//    [18.01.2003] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef PPIG_CGroupInfoH
#define PPIG_CGroupInfoH
//---------------------------------------------------------------------------

#include "PP_CWinControl.h"

class CPILabel;
class CPIImage;
class CIGBaseObject;

class CPIGroupInfo : public CPIWinControl
{
  public: //c-d
    CPIGroupInfo(CPIWinControl *Parent = NULL);
    virtual ~CPIGroupInfo();

  public: // get-set
    //void SetUnitList(CIGBaseObjectList* _unitList);
    void RemoveAll();
    void AddUnit(CIGBaseObject* _unit);

  public: // ops
    virtual void Paint();
    //virtual void Update();

  public: // handlers
    virtual void OnLeftMouseDown(int X, int Y);

  private:
    //CIGBaseObjectList* mUnitList;
    CPList<CIGBaseObject> *mList;
    CPGISprite *mSpriteInfantry, *mSpriteVehicle, *mSpriteFighter,
               *mSpriteChopper, *mSpriteDropship;
};

#endif
