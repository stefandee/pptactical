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
//  Unit              :  Infantry Unit Info (PPIG_CInfantryInfo)
//
//  Version           : 1.0
//
//  Description       :
//    * shows info on the selected infantry unit
//    * it's not entirely aesthetic and maybe this will change to 1.0
//
//  History           :
//    [13.01.2003] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef PPIG_CInfantryInfoH
#define PPIG_CInfantryInfoH
//---------------------------------------------------------------------------

#include "PP_CWinControl.h"

class CPILabel;
class CPIImage;
class CIGInfantryUnit;

class CPIInfantryInfo : public CPIWinControl
{
  public: //c-d
      CPIInfantryInfo(CPIWinControl *Parent = NULL);
      virtual ~CPIInfantryInfo();

  public: // get-set
      void SetInfantryUnit(CIGInfantryUnit* _unit);

  private:
    CPILabel* mLabelName, *mLabelWeapon, *mLabelArmor;
    CPIImage* mPortrait, *mWeapon, *mArmor, *mSep1, *mSep2;

    CPILabel* mSniper, *mClose, *mAntitank, *mCamo, *mStealth, *mHack; // battle skills
    CPILabel* mLead, *mInfluence, *mBravery, *mWisdom;                 // psi skills

    CIGInfantryUnit* mInfantryUnit;
};

#endif
