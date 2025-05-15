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

#include "PPIG_CInfantryInfo.h"
#include "PP_CLabel.h"
#include "PP_CImage.h"
#include "basepath.h"
#include "Paths.h"
#include "PPIG_ObjInfantry.h"
//---------------------------------------------------------------------------
#ifdef PPT_COMPILER_BORLAND
#pragma package(smart_init)
#endif

CPIInfantryInfo::CPIInfantryInfo(CPIWinControl *Parent) : CPIWinControl(Parent)
{
  mInfantryUnit = 0;

  // setup the controls and fill them with dummy info at initialiation time
  mLabelName = new CPILabel(this);
  AddControl(mLabelName);
  mLabelName->SetText("INFANTRY UNIT NAME");
  mLabelName->SetLeft(68);
  mLabelName->SetTop(0);
  mLabelName->SetVisible(true);
  mLabelName->SetEnabled(false);
  mLabelName->SetColor(0xD0D0D0);

  mPortrait = new CPIImage(this);
  AddControl(mPortrait);
  mPortrait->SetLeft(0);
  mPortrait->SetTop(0);
  mPortrait->SetVisible(true);
  mPortrait->SetEnabled(false);
  mPortrait->LoadFromFile(CPString(PATH_PORTRAITS_GFX) + "rion.png");

  // separator
  mSep1 = new CPIImage(this);
  AddControl(mSep1);
  mSep1->SetLeft(68);
  mSep1->SetTop(12);
  mSep1->SetVisible(true);
  mSep1->SetEnabled(false);
  mSep1->LoadFromFile(CPString(PATH_INGAME_GFX) + "sep2.png");

  // separator
  /*
  mSep2 = new CPIImage(this);
  AddControl(mSep2);
  mSep2->SetLeft(0);
  mSep2->SetTop(128);
  mSep2->SetVisible(true);
  mSep2->SetEnabled(false);
  mSep2->LoadFromFile(getGamePath() + PATH_INGAME_GFX + "sep.bmp");
  */

  // weapon&armor thumbnails
  mWeapon = new CPIImage(this);
  AddControl(mWeapon);
  mWeapon->SetLeft(0);
  mWeapon->SetTop(105);
  mWeapon->SetVisible(true);
  mWeapon->SetEnabled(false);
  mWeapon->LoadFromFile(CPString(PATH_INGAME_GFX) + "weapon.png");

  mArmor = new CPIImage(this);
  AddControl(mArmor);
  mArmor->SetLeft(0);
  mArmor->SetTop(128);
  mArmor->SetVisible(true);
  mArmor->SetEnabled(false);
  mArmor->LoadFromFile(CPString(PATH_INGAME_GFX) + "armor.png");

  // weapon&armor name labels
  mLabelWeapon = new CPILabel(this);
  AddControl(mLabelWeapon);
  mLabelWeapon->SetText("WEAPON");
  mLabelWeapon->SetLeft(23);
  mLabelWeapon->SetTop(112);
  mLabelWeapon->SetVisible(true);
  mLabelWeapon->SetEnabled(false);
  mLabelWeapon->SetColor(0xB0B0B0);

  mLabelArmor = new CPILabel(this);
  AddControl(mLabelArmor);
  mLabelArmor->SetText("ARMOR");
  mLabelArmor->SetLeft(23);
  mLabelArmor->SetTop(136);
  mLabelArmor->SetVisible(true);
  mLabelArmor->SetEnabled(false);
  mLabelArmor->SetColor(0xB0B0B0);

  // first column - battle skills labels
  int lTop  = 14;
  int lLeft = 68;
  int dY = 10;

  mSniper = new CPILabel(this);
  AddControl(mSniper);
  mSniper->SetText("SNP: ??");
  mSniper->SetLeft(lLeft);
  mSniper->SetTop(lTop);
  mSniper->SetVisible(true);
  mSniper->SetEnabled(false);
  mSniper->SetColor(0xB0B0B0);
  lTop+=dY;

  mClose = new CPILabel(this);
  AddControl(mClose);
  mClose->SetText("CC : ??");
  mClose->SetLeft(lLeft);
  mClose->SetTop(lTop);
  mClose->SetVisible(true);
  mClose->SetEnabled(false);
  mClose->SetColor(0xB0B0B0);
  lTop+=dY;

  mAntitank = new CPILabel(this);
  AddControl(mAntitank);
  mAntitank->SetText("AT : ??");
  mAntitank->SetLeft(lLeft);
  mAntitank->SetTop(lTop);
  mAntitank->SetVisible(true);
  mAntitank->SetEnabled(false);
  mAntitank->SetColor(0xB0B0B0);
  lTop+=dY;

  mCamo = new CPILabel(this);
  AddControl(mCamo);
  mCamo->SetText("CAM: ??");
  mCamo->SetLeft(lLeft);
  mCamo->SetTop(lTop);
  mCamo->SetVisible(true);
  mCamo->SetEnabled(false);
  mCamo->SetColor(0xB0B0B0);
  lTop+=dY;

  mStealth = new CPILabel(this);
  AddControl(mStealth);
  mStealth->SetText("STH: ??");
  mStealth->SetLeft(lLeft);
  mStealth->SetTop(lTop);
  mStealth->SetVisible(true);
  mStealth->SetEnabled(false);
  mStealth->SetColor(0xB0B0B0);
  lTop+=dY;

  mHack = new CPILabel(this);
  AddControl(mHack);
  mHack->SetText("HAK: ??");
  mHack->SetLeft(lLeft);
  mHack->SetTop(lTop);
  mHack->SetVisible(true);
  mHack->SetEnabled(false);
  mHack->SetColor(0xB0B0B0);
  lTop+=dY;

  // second columns - psi skills labels
  mLead = new CPILabel(this);
  AddControl(mLead);
  mLead->SetText("LDR: ??");
  mLead->SetLeft(lLeft);
  mLead->SetTop(lTop);
  mLead->SetVisible(true);
  mLead->SetEnabled(false);
  mLead->SetColor(0xB0B0B0);
  lTop+=dY;

  mInfluence = new CPILabel(this);
  AddControl(mInfluence);
  mInfluence->SetText("INF: ??");
  mInfluence->SetLeft(lLeft);
  mInfluence->SetTop(lTop);
  mInfluence->SetVisible(true);
  mInfluence->SetEnabled(false);
  mInfluence->SetColor(0xB0B0B0);
  lTop+=dY;

  mBravery = new CPILabel(this);
  AddControl(mBravery);
  mBravery->SetText("BRV: ??");
  mBravery->SetLeft(lLeft);
  mBravery->SetTop(lTop);
  mBravery->SetVisible(true);
  mBravery->SetEnabled(false);
  mBravery->SetColor(0xB0B0B0);
  lTop+=dY;

  mWisdom = new CPILabel(this);
  AddControl(mWisdom);
  mWisdom->SetText("WIS: ??");
  mWisdom->SetLeft(lLeft);
  mWisdom->SetTop(lTop);
  mWisdom->SetVisible(true);
  mWisdom->SetEnabled(false);
  mWisdom->SetColor(0xB0B0B0);
}
//---------------------------------------------------------------------------

CPIInfantryInfo::~CPIInfantryInfo()
{
  mInfantryUnit = 0;
}
//---------------------------------------------------------------------------

void CPIInfantryInfo::SetInfantryUnit(CIGInfantryUnit* _unit)
{
  if (!_unit)
  {
    mInfantryUnit = 0;
    SetVisible(false);
    return;
  }

  SetVisible(true);

  mInfantryUnit = _unit;

  // fill label with info
  TInfantryUnified lData = _unit->GetInfUnifiedData();

  if (lData.type == 0)
  {
    mLabelName->SetText(CPString(lData.name) + " " + CPString(lData.surname));
  }
  else
  {
    mLabelName->SetText(CPString(lData.kind));
  }

  // battle skills
  mSniper->SetText(CPString("SNP: ") + CPString(lData.snipery));
  mClose->SetText(CPString("CC : ") + CPString(lData.sr_combat));
  mAntitank->SetText(CPString("AT : ") + CPString(lData.antitank));
  mCamo->SetText(CPString("CAM: ") + CPString(lData.camo));
  mStealth->SetText(CPString("STH: ") + CPString("xx"));
  mHack->SetText(CPString("HAK: ") + CPString(lData.high_tech));

  // psi skills
  mLead->SetText(CPString("LDR: ") + CPString(lData.leadership));
  mInfluence->SetText(CPString("INF: ") + CPString("xx"));
  mBravery->SetText(CPString("BRV: ") + CPString(lData.bravery));
  mWisdom->SetText(CPString("WIS: ") + CPString("xx"));

  // armor&weapons
  mLabelArmor->SetText(CPString(lData.proj_res) + "/" + CPString(lData.nrg_res) + "/" + CPString(lData.plasma_res) + "/" + CPString(lData.bullet_res) + "/" + CPString(lData.fire_res));

  switch(lData.weptype)
  {
    case IWT_RIFLE:
      mLabelWeapon->SetText("Rifle");
      break;
    case IWT_MACHINE_GUN:
      mLabelWeapon->SetText("Machine Gun");
      break;
    case IWT_DOUBLE_GUN:
      mLabelWeapon->SetText("Double gun");
      break;

    case IWT_LASER_GUN:
      mLabelWeapon->SetText("Laser rifle");
      break;

    case IWT_PLASMA_THROWER:
      mLabelWeapon->SetText("Plasma rifle");
      break;

    case IWT_FLAME_THROWER:
      mLabelWeapon->SetText("Flamethrower");
      break;

    case IWT_GRENADE_LAUNCHER:
      mLabelWeapon->SetText("Grenade Launcher");
      break;

    case IWT_ROCKET_LAUNCHER:
      mLabelWeapon->SetText("Rocket Launcher");
      break;
  }

  // portraits
  mPortrait->LoadFromFile(CPString(PATH_PORTRAITS_GFX) + lData.mPortrait);
}
//---------------------------------------------------------------------------

