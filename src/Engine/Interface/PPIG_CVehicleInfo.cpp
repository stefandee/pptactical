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

#pragma hdrstop

#include "PPIG_CVehicleInfo.h"
#include "PP_CLabel.h"
#include "PP_CImage.h"
#include "basepath.h"
#include "Paths.h"
#include "PPIG_ObjVehicle.h"
//---------------------------------------------------------------------------

CPIVehicleInfo::CPIVehicleInfo(CPIWinControl *Parent) : CPIWinControl(Parent)
{
  CPFont* lFont;

  mVehicleUnit = 0;

  // setup the controls and fill them with dummy info at initialiation time
  mPortrait = new CPIImage(this);
  AddControl(mPortrait);
  mPortrait->SetLeft(GetWidth() / 2 - 32);
  mPortrait->SetTop(0);
  mPortrait->SetVisible(true);
  mPortrait->SetEnabled(false);
  mPortrait->LoadFromFile(CPString(PATH_PORTRAITS_GFX) + "rion.png");

  /*
  mVehicle = new CPIImage(this);
  AddControl(mVehicle);
  mVehicle->SetLeft(74);
  mVehicle->SetTop(0);
  mVehicle->SetVisible(true);
  mVehicle->SetEnabled(false);
  mVehicle->LoadFromFile(CPString(PATH_PORTRAITS_GFX) + "rion.png");
  */

  mLabelCrew = new CPILabel(this);
  AddControl(mLabelCrew);
  mLabelCrew->SetText("VEHICLE CREW NAME");
  //mLabelCrew->SetText("");
  mLabelCrew->SetLeft(30);
  mLabelCrew->SetTop(67);
  mLabelCrew->SetVisible(true);
  mLabelCrew->SetEnabled(false);
  mLabelCrew->SetColor(0xB0B0B0);

  lFont = new CPFont(PATH_FONTS + FILE_FONT_SMALL);
  lFont->SetSize(7);
  lFont->SetBold(true);
  mLabelCrew->SetFont(lFont);

  // crew combat skills
  mAntiInf = new CPILabel(this);
  AddControl(mAntiInf);
  mAntiInf->SetText("A-I: ??");
  //mAntiInf->SetText("");
  mAntiInf->SetLeft(20);
  mAntiInf->SetTop(77);
  mAntiInf->SetVisible(true);
  mAntiInf->SetEnabled(false);
  mAntiInf->SetColor(0xB0B0B0);

  mAntiTank = new CPILabel(this);
  AddControl(mAntiTank);
  mAntiTank->SetText("A-T: ??");
  //mAntitank->SetText("");
  mAntiTank->SetLeft(50);
  mAntiTank->SetTop(77);
  mAntiTank->SetVisible(true);
  mAntiTank->SetEnabled(false);
  mAntiTank->SetColor(0xB0B0B0);

  mAntiAir = new CPILabel(this);
  AddControl(mAntiAir);
  mAntiAir->SetText("A-A: ??");
  mAntiAir->SetLeft(80);
  mAntiAir->SetTop(77);
  mAntiAir->SetVisible(true);
  mAntiAir->SetEnabled(false);
  mAntiAir->SetColor(0xB0B0B0);

  mECM = new CPILabel(this);
  AddControl(mECM);
  mECM->SetText("ECM: ??");
  mECM->SetLeft(110);
  mECM->SetTop(77);
  mECM->SetVisible(true);
  mECM->SetEnabled(false);
  mECM->SetColor(0xB0B0B0);

  // crew psi skills
  mBravery = new CPILabel(this);
  AddControl(mBravery);
  mBravery->SetText("BRV: ??");
  //mBravery->SetText("");
  mBravery->SetLeft(20);
  mBravery->SetTop(87);
  mBravery->SetVisible(true);
  mBravery->SetEnabled(false);
  mBravery->SetColor(0xB0B0B0);

  mLead = new CPILabel(this);
  AddControl(mLead);
  mLead->SetText("LDR: ??");
  //mLead->SetText("");
  mLead->SetLeft(50);
  mLead->SetTop(87);
  mLead->SetVisible(true);
  mLead->SetEnabled(false);
  mLead->SetColor(0xB0B0B0);

  mTeamwork = new CPILabel(this);
  AddControl(mTeamwork);
  mTeamwork->SetText("TW: ??");
  //mTeamwork->SetText("");
  mTeamwork->SetLeft(80);
  mTeamwork->SetTop(87);
  mTeamwork->SetVisible(true);
  mTeamwork->SetEnabled(false);
  mTeamwork->SetColor(0xB0B0B0);

  mDrive = new CPILabel(this);
  AddControl(mDrive);
  mDrive->SetText("DRV: ??");
  //mDrive->SetText("");
  mDrive->SetLeft(110);
  mDrive->SetTop(87);
  mDrive->SetVisible(true);
  mDrive->SetEnabled(false);
  mDrive->SetColor(0xB0B0B0);

  // vehicle data
  mSep1 = new CPIImage(this);
  AddControl(mSep1);
  mSep1->SetLeft(GetWidth() / 2 - 40);
  mSep1->SetTop(100);
  mSep1->SetVisible(true);
  mSep1->SetEnabled(false);
  mSep1->LoadFromFile(CPString(PATH_INGAME_GFX) + "sep.png");

  mLabelVehicle = new CPILabel(this);
  AddControl(mLabelVehicle);
  mLabelVehicle->SetText("VEHICLE NAME");
  mLabelVehicle->SetLeft(30);
  mLabelVehicle->SetTop(103);
  mLabelVehicle->SetVisible(true);
  mLabelVehicle->SetEnabled(false);
  mLabelVehicle->SetColor(0xB0B0B0);

  lFont = new CPFont(PATH_FONTS + FILE_FONT_SMALL);
  lFont->SetSize(7);
  lFont->SetBold(true);
  mLabelVehicle->SetFont(lFont);

  mLabelWep1 = new CPILabel(this);
  AddControl(mLabelWep1);
  mLabelWep1->SetText("WEAPON1");
  mLabelWep1->SetLeft(30);
  mLabelWep1->SetTop(110);
  mLabelWep1->SetVisible(true);
  mLabelWep1->SetEnabled(false);
  mLabelWep1->SetColor(0xB0B0B0);

  mLabelWep1Info = new CPILabel(this);
  AddControl(mLabelWep1Info);
  mLabelWep1Info->SetText("WEAPON1 INFO");
  mLabelWep1Info->SetLeft(30);
  mLabelWep1Info->SetTop(120);
  mLabelWep1Info->SetVisible(true);
  mLabelWep1Info->SetEnabled(false);
  mLabelWep1Info->SetColor(0xB0B0B0);

  mLabelWep2 = new CPILabel(this);
  AddControl(mLabelWep2);
  mLabelWep2->SetText("WEAPON2");
  mLabelWep2->SetLeft(30);
  mLabelWep2->SetTop(130);
  mLabelWep2->SetVisible(true);
  mLabelWep2->SetEnabled(false);
  mLabelWep2->SetColor(0xB0B0B0);

  mLabelWep2Info = new CPILabel(this);
  AddControl(mLabelWep2Info);
  mLabelWep2Info->SetText("WEAPON2 INFO");
  mLabelWep2Info->SetLeft(30);
  mLabelWep2Info->SetTop(140);
  mLabelWep2Info->SetVisible(true);
  mLabelWep2Info->SetEnabled(false);
  mLabelWep2Info->SetColor(0xB0B0B0);

  mLabelArmor = new CPILabel(this);
  AddControl(mLabelArmor);
  mLabelArmor->SetText("ARMOR");
  mLabelArmor->SetLeft(30);
  mLabelArmor->SetTop(150);
  mLabelArmor->SetVisible(true);
  mLabelArmor->SetEnabled(false);
  mLabelArmor->SetColor(0xB0B0B0);

  mLabelArmorInfo = new CPILabel(this);
  AddControl(mLabelArmorInfo);
  mLabelArmorInfo->SetText("ARMOR INFO");
  mLabelArmorInfo->SetLeft(30);
  mLabelArmorInfo->SetTop(160);
  mLabelArmorInfo->SetVisible(true);
  mLabelArmorInfo->SetEnabled(false);
  mLabelArmorInfo->SetColor(0xB0B0B0);
}
//---------------------------------------------------------------------------

CPIVehicleInfo::~CPIVehicleInfo()
{
  mVehicleUnit = 0;
}
//---------------------------------------------------------------------------

void CPIVehicleInfo::SetVehicleUnit(CIGVehicle* _unit)
{
  CPString lClassName = _unit->ClassName();

  if (!_unit)
  {
    mVehicleUnit = 0;
    SetVisible(false);
    return;
  }

  if (_unit->ClassName() != CPString(IGCN_ARTILLERYUNIT) &&
      _unit->ClassName() != CPString(IGCN_RECONUNIT) &&
      _unit->ClassName() != CPString(IGCN_TANKUNIT))
  {
    mVehicleUnit = 0;
    SetVisible(false);
    return;
  }

  SetVisible(true);

  // unit is valid and it will be showed
  mVehicleUnit = _unit;

  TVehicleUnified lData = mVehicleUnit->GetVehicleUnifiedData();

  // dont try this at home :D
  CPString lCrewCallSign(lData.callsign);

  // center the unit in control
  mLabelCrew->SetLeft(GetWidth() / 2 - mLabelCrew->GetFont()->GetSize() * lCrewCallSign.Length() / 2);
  mLabelCrew->SetText(lCrewCallSign);

  // should be set in SetWidth instead
  mSep1->SetLeft(GetWidth() / 2 - 60);

  // set the label for vehicle name and type
  CPString lVehicleName(lData.name);

  switch(lData.vType)
  {
    case 0:
      lVehicleName = lVehicleName + " (tank)";
      break;
    case 1:
      lVehicleName = lVehicleName + " (artillery)";
      break;
    case 3:
      lVehicleName = lVehicleName + " (recon)";
      break;
  }

  mLabelVehicle->SetLeft(GetWidth() / 2 - mLabelVehicle->GetFont()->GetSize() * lVehicleName.Length() / 2);
  mLabelVehicle->SetText(lVehicleName);

  // weapon1 name
  CPString lWeapon1 = GetWeaponName(lData.wep1_type);

  mLabelWep1->SetLeft(GetWidth() / 2 - mLabelWep1->GetFont()->GetSize() * lWeapon1.Length() / 2);
  mLabelWep1->SetText(lWeapon1);

  // weapon1 data
  CPString lWeapon1Data = CPString("R: ") + CPString(lData.wep1_minrange) + "-" + CPString(lData.wep1_maxrange) + "  " + CPString("D: ") + CPString(lData.wep1_damage);

  mLabelWep1Info->SetLeft(GetWidth() / 2 - mLabelWep1Info->GetFont()->GetSize() * lWeapon1Data.Length() / 2);
  mLabelWep1Info->SetText(lWeapon1Data);

  // weapon2 name
  CPString lWeapon2 = GetWeaponName(lData.wep2_type);

  mLabelWep2->SetLeft(GetWidth() / 2 - mLabelWep2->GetFont()->GetSize() * lWeapon2.Length() / 2);
  mLabelWep2->SetText(lWeapon2);

  // weapon2 data
  CPString lWeapon2Data = CPString("R: ") + CPString(lData.wep2_minrange) + "-" + CPString(lData.wep2_maxrange) + "  " + CPString("D: ") + CPString(lData.wep2_damage);

  mLabelWep2Info->SetLeft(GetWidth() / 2 - mLabelWep2Info->GetFont()->GetSize() * lWeapon2Data.Length() / 2);
  mLabelWep2Info->SetText(lWeapon2Data);

   //int  exp_res;           // explosion resistance
   //int  nrg_res;           // energy resistance
   //int  plasma_res;        // plasma resistance
   //int  bullet_res;        // bullet resistance
   //int  fire_res;          // fire resistance

  // armor name
  CPString lArmor = "Armor";
  mLabelArmor->SetLeft(GetWidth() / 2 - mLabelArmor->GetFont()->GetSize() * lArmor.Length() / 2);
  mLabelArmor->SetText(lArmor);

  // armor data
  CPString lArmorData = CPString(lData.exp_res) + "/" + CPString(lData.nrg_res) + "/" + CPString(lData.plasma_res) + "/" + CPString(lData.bullet_res) + "/" + CPString(lData.fire_res);
  mLabelArmorInfo->SetLeft(GetWidth() / 2 - mLabelArmorInfo->GetFont()->GetSize() * lArmorData.Length() / 2);
  mLabelArmorInfo->SetText(lArmorData);

  // crew battle skills
  mAntiInf->SetText(CPString("A-I:") + CPString("??"));
  mAntiTank->SetText(CPString("A-T:") + CPString("??"));
  mAntiAir->SetText(CPString("A-A:") + CPString("??"));
  mECM->SetText(CPString("ECM:") + CPString("??"));

  // crew psi skills
  mDrive->SetText(CPString("DRV: ") + CPString(lData.driveskill));
  mLead->SetText(CPString("LDR: ") + CPString(lData.leadership));
  mBravery->SetText(CPString("BRV: ") + CPString(lData.bravery));
  mTeamwork->SetText(CPString("TW: ") + CPString("xx"));

  // portraits
  mPortrait->LoadFromFile(CPString(PATH_PORTRAITS_GFX) + lData.mPortrait);
}
//---------------------------------------------------------------------------

CPString CPIVehicleInfo::GetWeaponName(int _type)
{
  switch(_type)
  {
    case VWT_ARTILLERY_SHELL:
      return "Artillery Shell";

    case VWT_MORTAR:
      return "Mortar";

    case VWT_SHELL_CANNON:
      return "Cannon";

    case VWT_VEHICLE_MACHINE_GUN:
      return "Machine gun";

    case VWT_ROCKET:
      return "Rocket";

    case VWT_LASER_CANNON:
      return "Laser cannon";

    case VWT_SAM:
      return "SA Missile";

    case VWT_SSM:
      return "SS Missile";

    case VWT_FLAMER:
      return "Flame Thrower";

    case VWT_PLASMA_FLAMER:
      return "Plasma rifle";
  }

  return "No weapon";
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

