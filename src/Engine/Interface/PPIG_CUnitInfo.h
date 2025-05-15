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
//  Unit              :  Unit Info (PPIG_CUnitInfo)
//
//  Version           : 1.0
//
//  Description       :
//    * depending on what is selected (GetMissionInstance()->GetObjects(MOL_SELECTEDXXX)),
//      sends update/paint/handle actions to the corresponding control
//    * there are controls for infantry info, vehicle info (artillery/tank/recon), transport
//      info, aircraft info and group info (when more than one unit is selected)
//    * if units are removed from the selected lists, it is the job of the controls
//      to verify their existence in the lists
//
//  History           :
//    [13.01.2003] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef PPIG_CUnitInfoH
#define PPIG_CUnitInfoH
//---------------------------------------------------------------------------

#include "PP_CPanel.h"

class CPIInfantryInfo;
class CPIVehicleInfo;
class CPIGroupInfo;
class CPITranInfo;
class CPIAircraftInfo;
class CPIDropshipInfo;
class CPIFactoryInfo;

class CPIUnitInfo : public CPIWinControl
{
  public: // c-d
    CPIUnitInfo(CPIWinControl *Parent = NULL);
    ~CPIUnitInfo();

  public: // ops
    virtual void Paint();
    //virtual void Update();

  public: // handlers
    //virtual void OnLeftMouseDown(int X, int Y);
    //virtual void OnLeftMouseUp(int X, int Y);
    //virtual void OnRightMouseClick();
    //virtual void OnMouseMove(int X, int Y);
    //virtual void OnMouseOn(int X, int Y);

  private: // ops
    CPIWinControl* ComputeControlType();

    CPIGroupInfo*    mGroupInfo;
    CPIVehicleInfo*  mGenericVehicleInfo;
    CPITranInfo*     mTranInfo;
    CPIInfantryInfo* mInfantryInfo;
    CPIAircraftInfo* mGenericAircraftInfo;
    CPIDropshipInfo* mDropshipInfo;
    CPIFactoryInfo*  mFactoryInfo;
};

#endif
