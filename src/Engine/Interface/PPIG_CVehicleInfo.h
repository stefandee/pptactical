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
//  Unit              : Vehicle Info In-game Control
//
//  Version           : 1.0
//
//  Description       :
//    * shows information about generic vehicle (tank, artillery, recon)
//    * a separate controls exist for aircrafts and transporters, although they
//      are based on the same data structures (TVehicleUnified, namely)
//
//  History           :
//    [17.01.2003] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef PPIG_CVehicleInfoH
#define PPIG_CVehicleInfoH
//---------------------------------------------------------------------------

#include "PP_CWinControl.h"

class CPILabel;
class CPIImage;
class CIGVehicle;

class CPIVehicleInfo : public CPIWinControl
{
  public: //c-d
    CPIVehicleInfo(CPIWinControl *Parent = NULL);
    virtual ~CPIVehicleInfo();

  public: // get-set
    void SetVehicleUnit(CIGVehicle* _unit);
    CPString GetWeaponName(int _type);

  private:
    CPILabel* mLabelCrew, *mLabelVehicle;
    CPILabel* mLabelWep1, *mLabelWep2, *mLabelWep1Info, *mLabelWep2Info, *mLabelArmor, *mLabelArmorInfo;
    CPIImage* mPortrait, /**mVehicle, */*mSep1;

    CPILabel* mDrive, *mLead, *mBravery, *mTeamwork, *mECM, *mAntiInf, *mAntiTank, *mAntiAir; // crew skills

    CIGVehicle* mVehicleUnit;
};

#endif
