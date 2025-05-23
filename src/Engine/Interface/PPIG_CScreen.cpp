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
// for more details.                                                     s    //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 application: Pure Power In Game

 description: Class Piron Component Interface - Pure Power Main Screen
              here are created the game controls

 last modify: 21 05 00
        by grabX
---------------------------------------------------------------------------*/
#include <ctime>
#include <iomanip>
#pragma hdrstop
//---------------------------------------------------------------------------

#include "physfs.h"
#include "PPIG_CScreen.h"
#include "PP_Graphic.h"
#include "basepath.h"
#include "Paths.h"
#include "PP_Sound.h"
#include "PPIG_SpaceSound.h"
//#include "PPIG_FMain.h"
//#include "dinput.h"  //for DIKs
#include "PP_KeyConvertor.h" // for PPKs
#include "PPIG_Mission.h"
#include "PPIG_EngineDef.h"
#include "PPIG_Exit.h"
#include "PPIG_Map.h"
#include "PPIG_MissionDiskOp.h"
#include "PlayerProfile.h"
#include "PP_VfsSystem.h"
#include "PP_Stdlib.h"

// gui
#include "PP_CObject.h"
#include "PP_CScreen.h"
#include "PP_CPanel.h"
#include "PP_CLabel.h"
#include "PP_CButton.h"
#include "PP_CCheckButton.h"
#include "PP_CCheckBox.h"
#include "PP_CTrackBar.h"
#include "PP_CSensitiveScroll.h"
#include "PPIG_CBattleControl.h"
#include "PPIG_CUnitInfo.h"
#include "PPIG_InterfaceDef.h"
#include "PP_CCommonDialog.h"
#include "PP_CAction.h"
#include "PPIG_CRadar.h"
#include "PPIG_CMessage.h"
#include "PP_CTab.h"
#include "PP_CSFormDialog.h"
#include "PPIG_CSaveGameDialog.h"
#include "PPIG_CLoadGameDialog.h"

//---------------------------------------------------------------------------

#define INDEX_AMBIENTALMUSIC 500

CPIMainScreen *CPIMainScreen::mInstance = NULL;

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: gets the main screen instance
---------------------------------------------------------------------------*/
CPIMainScreen* GetMainScreenInstance()
{
  return CPIMainScreen::Instance();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets the main screen instance
---------------------------------------------------------------------------*/
CPIMainScreen* CPIMainScreen::Instance()
{
   if (mInstance == NULL)
   {
      mInstance = new CPIMainScreen();
      mInstance->CreateUIElements();
      mInstance->InitEventHandlers();
   }
   return mInstance;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: releases the main screen instance
---------------------------------------------------------------------------*/
void CPIMainScreen::Release()
{
   if (mInstance != NULL)
   {
      delete mInstance;
      mInstance = NULL;
   }
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CPIMainScreen::CPIMainScreen() : CPIScreen()
{
   //custom main screen
   CPIPanel *lPanel = new CPIPanel(NULL);
   CPIScreen::SetDesktop(lPanel);  //desktop is a panel

   CPGIGraphicSystem *lGSI;   //local Graphic System Instance

   try
   {
      lGSI = lGSI->Instance();
   }
   catch(...)
   {
      LOG(FATAL) << "CPIMainScreen::CPIMainScreen - UNABLE TO QUERY GRAPHICS !!!!!!!!!!";
      throw;
   }

   GetDesktop()->SetWidth(lGSI->GetResX());
   GetDesktop()->SetHeight(lGSI->GetResY());
   GetDesktop()->SetMouseType(1);
}

void CPIMainScreen::CreateUIElements()
{
   CPGISprite *lSprite;
   int lSoundIndex;

   CPGIGraphicSystem *lGSI = lGSI = lGSI->Instance();

   CPSISoundSystem *lSSI;
   try
   {
      lSSI = GetSoundInstance();
   }
   catch(...)
   {
      LOG(FATAL) << "CPIMainScreen::CPIMainScreen - UNABLE TO QUERY SOUND !!!!!!!!!!";
      throw;
   }

   //PAUSE LABEL
   mPauseLabel = new CPILabel(GetDesktop());
   GetDesktop()->AddControl(mPauseLabel);
   mPauseLabel->SetText("PROTOCOL TRANSFER STALLED");
   mPauseLabel->SetAutoSize(true);
   mPauseLabel->SetLeft(GetDesktop()->GetWidth()/2 - 65);
   mPauseLabel->SetVisible(false);
   mPauseLabel->SetEnabled(false);
   //PAUSE LABEL - over

   StandardGUICallback scrollChangeHandler = std::bind(&CPIMainScreen::ScrollOnChange, this, std::placeholders::_1);

   //SCROLL CONTROLS
   mScrollNorth = new CPISensitiveScroll(GetDesktop());
   GetDesktop()->AddControl(mScrollNorth);
   mScrollNorth->SetWidth(GetDesktop()->GetWidth() - 50);
   mScrollNorth->SetHeight(1);
   mScrollNorth->SetLeft(25);
   mScrollNorth->SetTop(0);
   mScrollNorth->SetDimCount(2);
   mScrollNorth->SetDir(-1, 1);
   mScrollNorth->SetStep(4, 1);
   mScrollNorth->SetMouseType(MC_SCROLLNORTH);
   mScrollNorth->SetHandleOnChange(scrollChangeHandler);

   //scrollnortheast1
   mScrollNorthEast1 = new CPISensitiveScroll(GetDesktop());
   GetDesktop()->AddControl(mScrollNorthEast1);
   mScrollNorthEast1->SetWidth(25);
   mScrollNorthEast1->SetHeight(1);
   mScrollNorthEast1->SetLeft(GetDesktop()->GetWidth() - 25);
   mScrollNorthEast1->SetTop(0);
   mScrollNorthEast1->SetDimCount(2);
   mScrollNorthEast1->SetDir(1, 0);
   mScrollNorthEast1->SetStep(4, 0);
   mScrollNorthEast1->SetDir(-1, 1);
   mScrollNorthEast1->SetStep(4, 1);
   mScrollNorthEast1->SetMouseType(MC_SCROLLNORTHEAST);
   mScrollNorthEast1->SetHandleOnChange(scrollChangeHandler);

   //scrollnortheast2
   mScrollNorthEast2 = new CPISensitiveScroll(GetDesktop());
   GetDesktop()->AddControl(mScrollNorthEast2);
   mScrollNorthEast2->SetWidth(1);
   mScrollNorthEast2->SetHeight(25);
   mScrollNorthEast2->SetLeft(GetDesktop()->GetWidth() - 1);
   mScrollNorthEast2->SetTop(0);
   mScrollNorthEast2->SetDimCount(2);
   mScrollNorthEast2->SetDir(1, 0);
   mScrollNorthEast2->SetStep(4, 0);
   mScrollNorthEast2->SetDir(-1, 1);
   mScrollNorthEast2->SetStep(4, 1);
   mScrollNorthEast2->SetMouseType(MC_SCROLLNORTHEAST);
   mScrollNorthEast2->SetHandleOnChange(scrollChangeHandler);

   //scrolleast
   mScrollEast = new CPISensitiveScroll(GetDesktop());
   GetDesktop()->AddControl(mScrollEast);
   mScrollEast->SetWidth(1);
   mScrollEast->SetHeight(GetDesktop()->GetHeight() - 50);
   mScrollEast->SetLeft(GetDesktop()->GetWidth() - 1);
   mScrollEast->SetTop(25);
   mScrollEast->SetDir(1, 0);
   mScrollEast->SetStep(4, 0);
   mScrollEast->SetMouseType(MC_SCROLLEAST);
   mScrollEast->SetHandleOnChange(scrollChangeHandler);

   //scrollsoutheast1
   mScrollSouthEast1 = new CPISensitiveScroll(GetDesktop());
   GetDesktop()->AddControl(mScrollSouthEast1);
   mScrollSouthEast1->SetWidth(1);
   mScrollSouthEast1->SetHeight(25);
   mScrollSouthEast1->SetTop(GetDesktop()->GetHeight() - 25);
   mScrollSouthEast1->SetLeft(GetDesktop()->GetWidth() - 1);
   mScrollSouthEast1->SetDimCount(2);
   mScrollSouthEast1->SetDir(1, 0);
   mScrollSouthEast1->SetStep(4, 0);
   mScrollSouthEast1->SetDir(1, 1);
   mScrollSouthEast1->SetStep(4, 1);
   mScrollSouthEast1->SetMouseType(MC_SCROLLSOUTHEAST);
   mScrollSouthEast1->SetHandleOnChange(scrollChangeHandler);

   //scrollsoutheast2
   mScrollSouthEast2 = new CPISensitiveScroll(GetDesktop());
   GetDesktop()->AddControl(mScrollSouthEast2);
   mScrollSouthEast2->SetWidth(25);
   mScrollSouthEast2->SetHeight(1);
   mScrollSouthEast2->SetTop(GetDesktop()->GetHeight() - 1);
   mScrollSouthEast2->SetLeft(GetDesktop()->GetWidth() - 25);
   mScrollSouthEast2->SetDimCount(2);
   mScrollSouthEast2->SetDir(1, 0);
   mScrollSouthEast2->SetStep(4, 0);
   mScrollSouthEast2->SetDir(1, 1);
   mScrollSouthEast2->SetStep(4, 1);
   mScrollSouthEast2->SetMouseType(MC_SCROLLSOUTHEAST);
   mScrollSouthEast2->SetHandleOnChange(scrollChangeHandler);

   //scroll south
   mScrollSouth = new CPISensitiveScroll(GetDesktop());
   GetDesktop()->AddControl(mScrollSouth);
   mScrollSouth->SetWidth(GetDesktop()->GetWidth() - 50);
   mScrollSouth->SetHeight(10);
   mScrollSouth->SetLeft(25);
   mScrollSouth->SetTop(GetDesktop()->GetHeight() - 9);
   mScrollSouth->SetDimCount(2);
   mScrollSouth->SetDir(1, 1);
   mScrollSouth->SetStep(4, 1);
   mScrollSouth->SetMouseType(MC_SCROLLSOUTH);
   mScrollSouth->SetHandleOnChange(scrollChangeHandler);

   //scroll south west 1
   mScrollSouthWest1 = new CPISensitiveScroll(GetDesktop());
   GetDesktop()->AddControl(mScrollSouthWest1);
   mScrollSouthWest1->SetWidth(25);
   mScrollSouthWest1->SetHeight(1);
   mScrollSouthWest1->SetLeft(0);
   mScrollSouthWest1->SetTop(GetDesktop()->GetHeight() - 1);
   mScrollSouthWest1->SetDimCount(2);
   mScrollSouthWest1->SetDir(-1, 0);
   mScrollSouthWest1->SetStep(4, 0);
   mScrollSouthWest1->SetDir(1, 1);
   mScrollSouthWest1->SetStep(4, 1);
   mScrollSouthWest1->SetMouseType(MC_SCROLLSOUTHWEST);
   mScrollSouthWest1->SetHandleOnChange(scrollChangeHandler);

   //scroll south west 2
   mScrollSouthWest2 = new CPISensitiveScroll(GetDesktop());
   GetDesktop()->AddControl(mScrollSouthWest2);
   mScrollSouthWest2->SetWidth(1);
   mScrollSouthWest2->SetHeight(25);
   mScrollSouthWest2->SetLeft(0);
   mScrollSouthWest2->SetTop(GetDesktop()->GetHeight() - 25);
   mScrollSouthWest2->SetDimCount(2);
   mScrollSouthWest2->SetDir(-1, 0);
   mScrollSouthWest2->SetStep(4, 0);
   mScrollSouthWest2->SetDir(1, 1);
   mScrollSouthWest2->SetStep(4, 1);
   mScrollSouthWest2->SetMouseType(MC_SCROLLSOUTHWEST);
   mScrollSouthWest2->SetHandleOnChange(scrollChangeHandler);

   //scroll west
   mScrollWest = new CPISensitiveScroll(GetDesktop());
   GetDesktop()->AddControl(mScrollWest);
   mScrollWest->SetWidth(1);
   mScrollWest->SetHeight(GetDesktop()->GetHeight() - 50);
   mScrollWest->SetLeft(0);
   mScrollWest->SetTop(25);
   mScrollWest->SetDir(-1, 0);
   mScrollWest->SetStep(4, 0);
   mScrollWest->SetMouseType(MC_SCROLLWEST);
   mScrollWest->SetHandleOnChange(scrollChangeHandler);

   //scroll north west 1
   mScrollNorthWest1 = new CPISensitiveScroll(GetDesktop());
   GetDesktop()->AddControl(mScrollNorthWest1);
   mScrollNorthWest1->SetWidth(1);
   mScrollNorthWest1->SetHeight(25);
   mScrollNorthWest1->SetLeft(0);
   mScrollNorthWest1->SetTop(0);
   mScrollNorthWest1->SetDimCount(2);
   mScrollNorthWest1->SetDir(-1, 0);
   mScrollNorthWest1->SetStep(4, 0);
   mScrollNorthWest1->SetDir(-1, 1);
   mScrollNorthWest1->SetStep(4, 1);
   mScrollNorthWest1->SetMouseType(MC_SCROLLNORTHWEST);
   mScrollNorthWest1->SetHandleOnChange(scrollChangeHandler);

   //scroll north west 2
   mScrollNorthWest2 = new CPISensitiveScroll(GetDesktop());
   GetDesktop()->AddControl(mScrollNorthWest2);
   mScrollNorthWest2->SetWidth(25);
   mScrollNorthWest2->SetHeight(1);
   mScrollNorthWest2->SetLeft(0);
   mScrollNorthWest2->SetTop(0);
   mScrollNorthWest2->SetDimCount(2);
   mScrollNorthWest2->SetDir(-1, 0);
   mScrollNorthWest2->SetStep(4, 0);
   mScrollNorthWest2->SetDir(-1, 1);
   mScrollNorthWest2->SetStep(4, 1);
   mScrollNorthWest2->SetMouseType(MC_SCROLLNORTHWEST);
   mScrollNorthWest2->SetHandleOnChange(scrollChangeHandler);

   mScrolls[SS_NORTH]      = mScrollNorth;
   mScrolls[SS_NORTHEAST1] = mScrollNorthEast1;
   mScrolls[SS_NORTHEAST2] = mScrollNorthEast2;
   mScrolls[SS_EAST]       = mScrollEast;
   mScrolls[SS_SOUTHEAST1] = mScrollSouthEast1;
   mScrolls[SS_SOUTHEAST2] = mScrollSouthEast2;
   mScrolls[SS_SOUTH]      = mScrollSouth;
   mScrolls[SS_SOUTHWEST1] = mScrollSouthWest1;
   mScrolls[SS_SOUTHWEST2] = mScrollSouthWest2;
   mScrolls[SS_WEST]       = mScrollWest;
   mScrolls[SS_NORTHWEST1] = mScrollNorthWest1;
   mScrolls[SS_NORTHWEST2] = mScrollNorthWest2;
   //SCROLL CONTROLS - over

   //RADAR CONTROL
   mRadarControl = new CPIRadarControl(GetDesktop());
   GetDesktop()->AddControl(mRadarControl);
   mRadarControl->SetWidth(120);
   mRadarControl->SetHeight(120);
   mRadarControl->SetLeft(25);
   mRadarControl->SetTop(25);

   mRadarControl->SetHandleOnChange(std::bind(&CPIMainScreen::OnRadarChange, this, std::placeholders::_1));
   //mRadarControl->SetHandleOnChange(OnRadarChange);
   //RADAR CONTORL - over

   //COMMAND PANEL
   mPanelCommand = new CPIPanel(GetDesktop());
   GetDesktop()->AddControl(mPanelCommand);
   mPanelCommand->SetLeft(0);
   mPanelCommand->SetTop(0);
   mPanelCommand->SetWidth(170);
   mPanelCommand->SetHeight(GetDesktop()->GetHeight());

   //command buttons panel
   lSprite = lGSI->AddSprite(CPString(PATH_INGAME_GFX) + "hud_down.png", 1, 1);
   mPanelCmdBtns = new CPIPanel(mPanelCommand);
   mPanelCommand->AddControl(mPanelCmdBtns);
   mPanelCmdBtns->SetSprite(lSprite);
   mPanelCmdBtns->SetAutoSizeToSprite(true);
   mPanelCmdBtns->SetLeft(0);
   mPanelCmdBtns->SetTop(mPanelCommand->GetHeight() - 170);

   lSoundIndex = lSSI->LoadNew((PATH_INGAME_SFX + "snd_command.wav").c_str(), 2);
   for (int i=0; i<COMMAND_BUTTONS_COUNT; i++)
   {
      mBtnsCommand[i] = new CPIButton(mPanelCmdBtns);
      lSprite = lGSI->AddSprite(CPString(PATH_INGAME_GFX) + gCommandBtnsFiles[i], 1, 2);
      mBtnsCommand[i]->SetSprite(lSprite);
      mBtnsCommand[i]->SetAutoSizeToSprite(true);
      mBtnsCommand[i]->SetLeft(gCommandBtnsCoord[i].x);
      mBtnsCommand[i]->SetTop(gCommandBtnsCoord[i].y);
      mBtnsCommand[i]->SetHint(gCommandHints[i]);
      mPanelCmdBtns->AddControl(mBtnsCommand[i]);
      mBtnsCommand[i]->SetTag(gCommandMasks[i]); //in fact action ID
      mBtnsCommand[i]->SetHandleOnLeftMouseClick(std::bind(&CPIMainScreen::CommandBtnOnLeftMouseClick, this, std::placeholders::_1));
      //mBtnsCommand[i]->SetHandleOnLeftMouseClick(CommandBtnOnLeftMouseClick);
      mBtnsCommand[i]->SetVisible(false);
      mBtnsCommand[i]->SetOnLeftClickSound(lSoundIndex);
      //logWriteLn(gCommandBtnsFiles[i]);
   }
   //command buttons panel - over

   lSprite = lGSI->AddSprite(CPString(PATH_INGAME_GFX) + "hud_up.png", 1, 1);
   mPanelStatus = new CPIPanel(mPanelCommand);
   //mPanelStatus = new CPIUnitInfo(mPanelCommand);
   mPanelCommand->AddControl(mPanelStatus);
   mPanelStatus->SetLeft(0);
   mPanelStatus->SetTop(0);
   mPanelStatus->SetSprite(lSprite);
   mPanelStatus->SetAutoSizeToSprite(true);
   mPanelStatus->SetHeight(mPanelCommand->GetHeight());

   mUnitInfo = new CPIUnitInfo(mPanelStatus);
   mPanelStatus->AddControl(mUnitInfo);
   mUnitInfo->SetLeft(10);
   mUnitInfo->SetTop(190);
   mUnitInfo->SetVisible(true);
   mUnitInfo->SetEnabled(true);
   mUnitInfo->SetWidth(mPanelStatus->GetWidth());
   mUnitInfo->SetHeight(mPanelStatus->GetHeight());
   //mUnitInfo->SetHeight(400);

   //COMMAND PANEL - over

   //MENU PANEL
   lSprite = lGSI->AddSprite(CPString(PATH_INGAME_GFX) + "pl_menu.png", 1, 1);
   mPanelMenu = new CPIPanel(GetDesktop());
   GetDesktop()->AddControl(mPanelMenu);
   mPanelMenu->SetSprite(lSprite);
   mPanelMenu->SetAutoSizeToSprite(true);
   mPanelMenu->SetTop(- mPanelMenu->GetHeight() - 1);
   mPanelMenu->SetLeft(GetDesktop()->GetWidth() - mPanelMenu->GetWidth() + 2);
   mPanelMenu->SetAlpha(255);
   mPanelMenu->SetVisible(false);

   lSoundIndex = lSSI->LoadNew((PATH_INGAME_SFX + "snd_menu.wav").c_str(), 2);

   lSprite = lGSI->AddSprite(CPString(PATH_INGAME_GFX) + "btns_menu.png", 2, 2);
   mBtnMenu = new CPIButton(GetDesktop());
   GetDesktop()->AddControl(mBtnMenu);
   mBtnMenu->SetSprite(lSprite);
   mBtnMenu->SetAutoSizeToSprite(true);
   mBtnMenu->SetLeft(GetDesktop()->GetWidth() - mBtnMenu->GetWidth() + 2);
   mBtnMenu->SetTop(0);
   mBtnMenu->SetTag(0);
   mBtnMenu->SetOnLeftClickSound(lSoundIndex);
   mBtnMenu->SetAnimCounter(0);
   mBtnMenu->SetUpdateEnabled(false);
   mBtnMenu->SetHandleOnLeftMouseClick(std::bind(&CPIMainScreen::BtnMenuOnLeftMouseClick, this, std::placeholders::_1));
   //mBtnMenu->SetHandleOnLeftMouseClick(BtnMenuOnLeftMouseClick);
   mBtnMenu->SetHint("Options Menu");

   mTrackScrollSpeed = new CPITrackBar(mPanelMenu);
   lSprite = lGSI->AddSprite(CPString(PATH_INGAME_GFX) + "cell.png", 1, 2);
   mPanelMenu->AddControl(mTrackScrollSpeed);
   mTrackScrollSpeed->SetSprite(lSprite);
   mTrackScrollSpeed->SetMaxValue(8);
   mTrackScrollSpeed->SetCurrentValue(GetProfileInstance()->GetScrollSpeed());
   mTrackScrollSpeed->SetWidth(80);
   mTrackScrollSpeed->SetHeight(10);
   mTrackScrollSpeed->SetLeft(110);
   mTrackScrollSpeed->SetTop(42);
   mTrackScrollSpeed->SetHandleOnChange(std::bind(&CPIMainScreen::TrackScrollSpeedOnChange, this, std::placeholders::_1, std::placeholders::_2));
   //mTrackScrollSpeed->SetHandleOnChange(TrackScrollSpeedOnChange);

   mTrackGameSpeed = new CPITrackBar(mPanelMenu);
   mPanelMenu->AddControl(mTrackGameSpeed);
   mTrackGameSpeed->SetSprite(lSprite);
   mTrackGameSpeed->SetMaxValue(8);
   mTrackGameSpeed->SetCurrentValue(GetProfileInstance()->GetGameSpeed());
   mTrackGameSpeed->SetWidth(80);
   mTrackGameSpeed->SetHeight(10);
   mTrackGameSpeed->SetLeft(110);
   mTrackGameSpeed->SetTop(62);
   mTrackGameSpeed->SetHandleOnChange(std::bind(&CPIMainScreen::TrackGameSpeedOnChange, this, std::placeholders::_1, std::placeholders::_2));
   //mTrackGameSpeed->SetHandleOnChange(TrackGameSpeedOnChange);

   mTrackMusicVolume = new CPITrackBar(mPanelMenu);
   mPanelMenu->AddControl(mTrackMusicVolume);
   mTrackMusicVolume->SetSprite(lSprite);
   mTrackMusicVolume->SetMaxValue(8);
   mTrackMusicVolume->SetCurrentValue(GetProfileInstance()->GetMusicVolume() * 8 / 100);
   mTrackMusicVolume->SetWidth(80);
   mTrackMusicVolume->SetHeight(10);
   mTrackMusicVolume->SetLeft(110);
   mTrackMusicVolume->SetTop(92);
   mTrackMusicVolume->SetHandleOnChange(std::bind(&CPIMainScreen::TrackMusicVolumeOnChange, this, std::placeholders::_1, std::placeholders::_2));
   //mTrackMusicVolume->SetHandleOnChange(TrackMusicVolumeOnChange);

   mTrackSFXVolume = new CPITrackBar(mPanelMenu);
   mPanelMenu->AddControl(mTrackSFXVolume);
   mTrackSFXVolume->SetSprite(lSprite);
   mTrackSFXVolume->SetMaxValue(8);
   mTrackSFXVolume->SetCurrentValue(GetProfileInstance()->GetSFXVolume() * 8 / 100);
   mTrackSFXVolume->SetWidth(80);
   mTrackSFXVolume->SetHeight(10);
   mTrackSFXVolume->SetLeft(110);
   mTrackSFXVolume->SetTop(112);
   mTrackSFXVolume->SetHandleOnChange(std::bind(&CPIMainScreen::TrackSFXVolumeOnChange, this, std::placeholders::_1, std::placeholders::_2));

   //mTrackSFXVolume->SetHandleOnChange(TrackSFXVolumeOnChange);

   CPILabel *lLabel;
   lLabel = new CPILabel(mPanelMenu);
   mPanelMenu->AddControl(lLabel);
   lLabel->SetText("Scroll Speed");
   lLabel->SetLeft(30);
   lLabel->SetTop(37);
   lLabel->SetVisible(true);
   lLabel->SetEnabled(false);
   lLabel->SetColor(0xb0b0b0);

   lLabel = new CPILabel(mPanelMenu);
   mPanelMenu->AddControl(lLabel);
   lLabel->SetText("Game Speed");
   lLabel->SetLeft(30);
   lLabel->SetTop(57);
   lLabel->SetVisible(true);
   lLabel->SetEnabled(false);
   lLabel->SetColor(0xb0b0b0);

   lLabel = new CPILabel(mPanelMenu);
   mPanelMenu->AddControl(lLabel);
   lLabel->SetText("Music Volume");
   lLabel->SetLeft(30);
   lLabel->SetTop(87);
   lLabel->SetVisible(true);
   lLabel->SetEnabled(false);
   lLabel->SetColor(0xb0b0b0);

   lLabel = new CPILabel(mPanelMenu);
   mPanelMenu->AddControl(lLabel);
   lLabel->SetText("SFX Volume");
   lLabel->SetLeft(30);
   lLabel->SetTop(107);
   lLabel->SetVisible(true);
   lLabel->SetEnabled(false);
   lLabel->SetColor(0xb0b0b0);

   mShowViewCheck = new CPICheckBox(mPanelMenu);
   mPanelMenu->AddControl(mShowViewCheck);
   mShowViewCheck->SetText("Unit View Direction");
   mShowViewCheck->SetLeft(30);
   mShowViewCheck->SetTop(137);
   mShowViewCheck->SetWidth(160);
   mShowViewCheck->SetHeight(15);
   mShowViewCheck->SetVisible(true);
   mShowViewCheck->SetColor(0xb0b0b0);
   mShowViewCheck->SetChecked(GetProfileInstance()->GetShowViewDir());
   mShowViewCheck->SetHandleOnLeftMouseClick(std::bind(&CPIMainScreen::ViewCheckBoxOnLeftMouseClick, this, std::placeholders::_1));

   mShowUnitTooltipCheck = new CPICheckBox(mPanelMenu);
   mPanelMenu->AddControl(mShowUnitTooltipCheck);
   mShowUnitTooltipCheck->SetText("Unit Tooltip");
   mShowUnitTooltipCheck->SetLeft(30);
   mShowUnitTooltipCheck->SetTop(150);
   mShowUnitTooltipCheck->SetWidth(160);
   mShowUnitTooltipCheck->SetHeight(15);
   mShowUnitTooltipCheck->SetVisible(true);
   mShowUnitTooltipCheck->SetColor(0xb0b0b0);
   mShowUnitTooltipCheck->SetChecked(GetProfileInstance()->GetShowUnitTooltip());
   mShowUnitTooltipCheck->SetHandleOnLeftMouseClick(std::bind(&CPIMainScreen::UnitTooltipCheckBoxOnLeftMouseClick, this, std::placeholders::_1));

   //MENU PANEL - over

   // MESSAGE CONSOLE
   mPanelConsole = new CPIPanel(GetDesktop());
   GetDesktop()->AddControl(mPanelConsole);

   lSoundIndex = lSSI->LoadNew((PATH_INGAME_SFX + "snd_menu.wav").c_str(), 2);

   lSprite = lGSI->AddSprite(CPString(PATH_INGAME_GFX) + "btns_down.png", 2, 2);
   mBtnConsole = new CPIButton(mPanelConsole);
   mPanelConsole->AddControl(mBtnConsole);
   mBtnConsole->SetSprite(lSprite);
   mBtnConsole->SetAutoSizeToSprite(true);
   mBtnConsole->SetTop(0);
   mBtnConsole->SetTag(0);
   mBtnConsole->SetOnLeftClickSound(lSoundIndex);
   mBtnConsole->SetAnimCounter(0);
   mBtnConsole->SetUpdateEnabled(false);
   mBtnConsole->SetHandleOnLeftMouseClick(std::bind(&CPIMainScreen::BtnConsoleOnLeftMouseClick, this, std::placeholders::_1));
   //mBtnConsole->SetHandleOnLeftMouseClick(BtnConsoleOnLeftMouseClick);
   mBtnConsole->SetHint("Message Console");

   mConsole = new CPIMessageConsole(mPanelConsole);
   mPanelConsole->AddControl(mConsole);
   mConsole->SetWidth(360);
   mConsole->SetHeight(108);
   mConsole->SetLeft(0);
   mConsole->SetTop(mBtnConsole->GetHeight());
   //mConsole->SetVisible(true);

   mPanelConsole->SetWidth(mConsole->GetWidth() + 1);
   mBtnConsole->SetLeft(mPanelConsole->GetWidth() - mBtnConsole->GetWidth() + 1);
   mPanelConsole->SetHeight(mConsole->GetHeight() + mBtnConsole->GetHeight() + 1);

   mPanelConsole->SetTop(GetDesktop()->GetHeight() - mBtnConsole->GetHeight() + 1);
   mPanelConsole->SetLeft(GetDesktop()->GetWidth() - mPanelConsole->GetWidth() + 1);

   // mPanelConsole->SetTop(300);
   // mPanelConsole->SetLeft(400);
   // MESSAGE CONSOLE - OVER

   //BATTLE CONTROL
   mBattleControl = new CPIBattleControl(GetDesktop());
   GetDesktop()->AddControl(mBattleControl);
   mBattleControl->SetLeft(mPanelCommand->GetWidth() - 17);
   mBattleControl->SetTop(0);
   mBattleControl->SetHeight(GetDesktop()->GetHeight());
   mBattleControl->SetWidth(GetDesktop()->GetWidth() - mBattleControl->GetLeft());
   //BATTLE CONTROL - over

   mDialog = new CPP_CSFormDialog(GetDesktop());
   GetDesktop()->AddControl(mDialog);
   mDialog->SetLeft(GetDesktop()->GetWidth() / 2 - 358 / 2);
   //mDialog->SetHandleOnClose(std::bind(&CPIMainScreen::ExitDialogClosed, this, std::placeholders::_1));
   mDialog->SetEnabled(false);

   mDialogSave = new CPP_CSaveGameDialog(GetDesktop());
   GetDesktop()->AddControl(mDialogSave);
   mDialogSave->SetLeft(GetDesktop()->GetWidth() / 2 - 358 / 2);
   mDialogSave->SetHandleOnClose(std::bind(&CPIMainScreen::SaveDialogClosed, this, std::placeholders::_1));
   mDialogSave->SetEnabled(false);

   mDialogLoad = new CPP_CLoadGameDialog(GetDesktop());
   GetDesktop()->AddControl(mDialogLoad);
   mDialogLoad->SetLeft(GetDesktop()->GetWidth() / 2 - 358 / 2);
   mDialogLoad->SetHandleOnClose(std::bind(&CPIMainScreen::LoadDialogClosed, this, std::placeholders::_1));
   mDialogLoad->SetEnabled(false);

   //mDialog->SetTop(0);

   // test

   /*
   CPITab* tab = new CPITab(GetDesktop());
   GetDesktop()->AddControl(tab);
   tab->SetLeftDestination(200);
   tab->SetLeft(200);
   tab->SetTopDestination(200);
   tab->SetTop(200);
   tab->SetWidth(200);
   tab->SetHeight(150);

   GetDesktop()->BringControlToFront(tab);

   CPISheet* sheet = new CPISheet(tab);
   sheet->SetCaption("Sheet1");
   tab->AddPage(sheet);
   tab->SetTabIndex(0);
   */

   mObjectHintLabel = new CPILabel(GetDesktop());
   GetDesktop()->AddControl(mObjectHintLabel);
   mObjectHintLabel->SetVisible(false);
   mObjectHintLabel->SetEnabled(false);
   mObjectHintLabel->SetAutoSize(true);
   mObjectHintLabel->SetColor(0xb0b0b0);
   mObjectHintLabel->SetTextAllignment(PPDT_CENTER);
   mObjectHintLabel->BringToFront();

   //ACTIONS
   CreateActions();
   //ACTIONS - over

   //SOUND
   CreateMusic();
   //mTrackMusicVolume->SetCurrentValue(2);
   //mTrackSFXVolume->SetCurrentValue(2);
   //SOUND - over

   // force the player profile
   TrackScrollSpeedOnChange(0, 0);
   TrackGameSpeedOnChange(0, 0);
   TrackMusicVolumeOnChange(0, 0);
   TrackSFXVolumeOnChange(0, 0);
}
//---------------------------------------------------------------------------

void CPIMainScreen::AddMessage(CPString _msg)
{
  mConsole->AddText(_msg);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: creates the scrolls
---------------------------------------------------------------------------*/
void CPIMainScreen::CreateScrolls()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: creates the menu
---------------------------------------------------------------------------*/
void CPIMainScreen::CreateMenu()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: creates command panel
---------------------------------------------------------------------------*/
void CPIMainScreen::CreateCommandPanel()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: creates the keyboard acrtions
---------------------------------------------------------------------------*/
void CPIMainScreen::CreateActions()
{
   CPIActionList *lActionList;
   lActionList = new CPIActionList();
   SetActions(lActionList);

   CPIAction *mAction;

   //Exit action
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_X);
   mAction->SetKeysPressed(PPK_LALT);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionExit, this, std::placeholders::_1));

   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_F4);
   mAction->SetKeysPressed(PPK_LALT);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionExit, this, std::placeholders::_1));

   //scroll north action
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysPressed(PPK_UP);
   mAction->SetHandleOnActivate(std::bind(&CPISensitiveScroll::OnScroll, mScrolls[SS_NORTH], std::placeholders::_1));

   //scroll east action
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysPressed(PPK_RIGHT);
   mAction->SetHandleOnActivate(std::bind(&CPISensitiveScroll::OnScroll, mScrolls[SS_EAST], std::placeholders::_1));

   //scroll south action
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysPressed(PPK_DOWN);
   mAction->SetHandleOnActivate(std::bind(&CPISensitiveScroll::OnScroll, mScrolls[SS_SOUTH], std::placeholders::_1));

   //scroll east action
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysPressed(PPK_LEFT);
   mAction->SetHandleOnActivate(std::bind(&CPISensitiveScroll::OnScroll, mScrolls[SS_WEST], std::placeholders::_1));

   //menu action
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_F10);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::BtnMenuOnLeftMouseClick, this, std::placeholders::_1));
   //mAction->SetHandleOnActivate(BtnMenuOnLeftMouseClick);

   //hide command panel action
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_SPACE);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionHideCommandPanel, this, std::placeholders::_1));

   //take screen shot action
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_F11);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionTakeScreenShot, this, std::placeholders::_1));

   //add selection action
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_LSHIFT);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionAddSelection, this, std::placeholders::_1));

   //preserve formation
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_TAB);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionPreserveFormation, this, std::placeholders::_1));

   //normal selection action
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysUp(PPK_LSHIFT);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionNormalSelection, this, std::placeholders::_1));

   //cancel action
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_ESCAPE);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionCancelMissionAction, this, std::placeholders::_1));

   //pause mission action
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_P);
   mAction->SetKeysPressed(PPK_RCTRL);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionPauseMission, this, std::placeholders::_1));

   // quick save
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_F5);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionQuickSaveMission, this, std::placeholders::_1));

   // quick load
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_F6);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionQuickLoadMission, this, std::placeholders::_1));

   //save menu
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_F2);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionSaveMission, this, std::placeholders::_1));

   //load menu
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_F3);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionLoadMission, this, std::placeholders::_1));

   //focus on the selected unit
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_F);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionFocusOnObject, this, std::placeholders::_1));

   //OBJECT COMMAND ACTIONS
   //command stop action
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_S);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionMissionCommandStop, this, std::placeholders::_1));

   //command move action
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_M);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionMissionCommandMove, this, std::placeholders::_1));

   //command attack action
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_A);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionMissionCommandAttack, this, std::placeholders::_1));

   //command patrol action
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_P);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionMissionCommandPatrol, this, std::placeholders::_1));

   //command suppress fire action
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_Z);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionMissionCommandSuppressFire, this, std::placeholders::_1));

   //command take cover action
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_X);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionMissionCommandTakeCover, this, std::placeholders::_1));

   //command load action (for infantry & vehicles)
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_L);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionMissionCommandLoad, this, std::placeholders::_1));

   //command unload all action (for transporting vehicles)
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_U);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionMissionCommandUnloadAll, this, std::placeholders::_1));

   // guard
   mAction = new CPIAction();
   GetActions()->Add(mAction);
   mAction->SetKeysDown(PPK_G);
   mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionMissionCommandGuard, this, std::placeholders::_1));

   //OBJECT COMMAND ACTIONS - over

   //team actions
   for (int i=0; i<10; i++)
   {
      mAction = new CPIAction();
      GetActions()->Add(mAction);
      mAction->SetKeysPressed(PPK_LCTRL);
      mAction->SetKeysDown(PPK_1 + i);
      mAction->SetTag(i);
      mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionSetTeam, this, std::placeholders::_1));

      mAction = new CPIAction();
      GetActions()->Add(mAction);
      mAction->SetKeysPressed(PPK_RCTRL);
      mAction->SetKeysDown(PPK_1 + i);
      mAction->SetTag(i);
      mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionSetTeam, this, std::placeholders::_1));

      mAction = new CPIAction();
      GetActions()->Add(mAction);
      mAction->SetKeysPressed(PPK_RALT);
      mAction->SetKeysDown(PPK_1 + i);
      mAction->SetTag(i);
      mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionSelectTeam, this, std::placeholders::_1));

      mAction = new CPIAction();
      GetActions()->Add(mAction);
      mAction->SetKeysPressed(PPK_LALT);
      mAction->SetKeysDown(PPK_1 + i);
      mAction->SetTag(i);
      mAction->SetHandleOnActivate(std::bind(&CPIMainScreen::ActionSelectTeam, this, std::placeholders::_1));
   }
   //team actions - over
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: initializes the event handlers
---------------------------------------------------------------------------*/
void CPIMainScreen::InitEventHandlers()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: creates the music sound buffer and play with DSBPLAY_LOOPING
---------------------------------------------------------------------------*/
void CPIMainScreen::CreateMusic()
{
   try
   {
      CPSISoundSystem *lSSI;
      lSSI = GetSoundInstance();

      // TODO: uncomment this to have music
      //lSSI->Load(INDEX_AMBIENTALMUSIC, (getGamePath() + PATH_MUSIC + "music_demo.wav").c_str(), 1);
      //lSSI->Load(INDEX_AMBIENTALMUSIC, (getGamePath() + PATH_MUSIC + "alchemystudio.it.track6.wav").c_str(), 1);
      //lSSI->Play(INDEX_AMBIENTALMUSIC, 0, looped);
   }
   catch(...)
   {
      LOG(ERROR) << "UNABLE TO PLAY MUSIC !!!!!!!!!!!!";
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CPIMainScreen::~CPIMainScreen()
{
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PUBLIC USER METHODS
/////////////////////////////////////////////////////////////////////////////
void CPIMainScreen::Update()
{
   CPIScreen::Update();

   int actionMask = GetMissionInstance()->GetObjectsActionMask();

   for (int i=0; i<COMMAND_BUTTONS_COUNT; i++)
   {
      //mBtnsCommand[i]->SetVisible((1<<mBtnsCommand[i]->GetTag()) & GetMissionInstance()->GetObjectsActionMask());
      int buttonTag = mBtnsCommand[i]->GetTag();

      mBtnsCommand[i]->SetVisible(((1<<buttonTag) & actionMask) != 0);
   }

    /*
   if (GetMissionInstance()->GetObjectHint() != CPString("none"))
   {
      mObjectHintLabel->SetText(GetMissionInstance()->GetObjectHint());
      mObjectHintLabel->SetVisible(true);

      CPPoint pos = mBattleControl->CoordToScreen(mBattleControl->GetObjectHintPos());

      pos.x = clamp(pos.x, mObjectHintLabel->GetWidth() / 2, mObjectHintLabel->GetParent()->GetWidth() - mObjectHintLabel->GetWidth() / 2);
      pos.y = clamp(pos.y, 0, mObjectHintLabel->GetParent()->GetHeight() - mObjectHintLabel->GetHeight());

      mObjectHintLabel->SetLeft(pos.x);
      mObjectHintLabel->SetTop(pos.y);
   }
   else
   {
     mObjectHintLabel->SetVisible(false);
   }
   */

/*   GetGraphicInstance()->Rectangle(NULL, CPRect(0, 0,
                                              GetGraphicInstance()->GetResX(), GetGraphicInstance()->GetResX()),
                                   0);
*/   try
   {
      CIGSpaceSoundSystem *lSSSI;
      lSSSI = lSSSI->Instance();
      lSSSI->SetReferencePoint(CPPoint((mBattleControl->GetWindowLeft() + mBattleControl->GetWidth()/2)/MAPCELLX,
                                       (mBattleControl->GetWindowTop()+ mBattleControl->GetHeight()/2)/MAPCELLY));
   }
   catch(...)
   {
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sets the scroll bounds
 parameters : xMax, yMax - the map dims
---------------------------------------------------------------------------*/
void CPIMainScreen::SetScrollBounds(int xMax, int yMax)
{
   for (int i=0; i<SENSITIVESCROLLCOUNT; i++)
   {
      if (mScrolls[i]->GetDir(0) != 0)
         mScrolls[i]->SetMax(xMax - mBattleControl->GetWidth() - 1, 0);
      if (mScrolls[i]->GetDir(1) != 0)
         mScrolls[i]->SetMax(yMax - mBattleControl->GetHeight() - 1, 1);
   }
   mRadarControl->SetMapWidth(xMax);
   mRadarControl->SetMapHeight(yMax);
   mRadarControl->SetWindowWidth(mBattleControl->GetWidth());
   mRadarControl->SetWindowHeight(mBattleControl->GetHeight());
   mRadarControl->ComputeScale();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sets the current position of the viewport
 parameters : X, Y - real coord.
---------------------------------------------------------------------------*/
void CPIMainScreen::SetScrollPos(int X, int Y)
{
   for (int i=0; i<SENSITIVESCROLLCOUNT; i++)
   {
      if (mScrolls[i]->GetDir(0) != 0)
         mScrolls[i]->SetPosWithNoChange(0, X);
      if (mScrolls[i]->GetDir(1) != 0)
         mScrolls[i]->SetPosWithNoChange(1, Y);
   }
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PUBLIC (PUBLISHED) USER EVENT SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: menu btn shows and hides the menu
 others     : i use tag for simulating the second state
---------------------------------------------------------------------------*/
void CPIMainScreen::BtnMenuOnLeftMouseClick(CPIObject *Sender)
{
   if (mBtnMenu->GetTag() == 0)
   {
       // show
      mPanelMenu->SetTop(-2);
      mPanelMenu->SetVisible(true);
      mBtnMenu->SetTop(mPanelMenu->GetHeight() - 5);
      mBtnMenu->SetTag(1);
      mBtnMenu->SetAnimCounter(1);
   }
   else
   {
      // hide
      mPanelMenu->SetTop(- mPanelMenu->GetHeight() - 1);
      mPanelMenu->SetVisible(false);
      mBtnMenu->SetTop(0);
      mBtnMenu->SetTag(0);
      mBtnMenu->SetAnimCounter(0);
   }
}
//---------------------------------------------------------------------------

void CPIMainScreen::BtnConsoleOnLeftMouseClick(CPIObject *Sender)
{
   if (mBtnConsole->GetTag() == 0)
   { // console up
      mPanelConsole->SetTop(GetDesktop()->GetHeight() - mPanelConsole->GetHeight());
      mConsole->SetVisible(true);
      mBtnConsole->SetTag(1);
      mBtnConsole->SetAnimCounter(1);
   }
   else
   {  // console down
      mPanelConsole->SetTop(GetDesktop()->GetHeight() - mBtnConsole->GetHeight() + 1);
      mBtnConsole->SetTag(0);
      mBtnConsole->SetAnimCounter(0);
      mConsole->SetVisible(false);
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on scroll change event
 parameters : Sender - the scroll that sent the msg
---------------------------------------------------------------------------*/
void CPIMainScreen::ScrollOnChange(CPIObject *Sender)
{
    // reset the focus object when scrolling; alternatively, it's also possible
    // to interdict scrolling when in focus mode (really depends on the game)
    if (GetMissionInstance()->GetFocusObject())
    {
        GetMissionInstance()->SetFocusObject(NULL);
    }

    CPISensitiveScroll* senderScroll = dynamic_cast<CPISensitiveScroll*>(Sender);

   if (senderScroll == NULL)
   {
     LOG(ERROR) << "CPIMainScreen::ScrollOnChange - Sender is not sensitive scroll.";
     return;
   }

   if (senderScroll->GetDir(0) != 0)
   {//the x axis was scrolled
      int value = ((CPISensitiveScroll *)Sender)->GetPos(0);

      for (int i=0; i<SENSITIVESCROLLCOUNT; i++)
      {
         CPISensitiveScroll* scroll = mScrolls[i];

         if (scroll == NULL)
         {
           LOG(ERROR) << "CPIMainScreen::ScrollOnChange - mScrolls[i] is null.";
           continue;
         }

         scroll->SetPosWithNoChange(value, 0);
      }

      mBattleControl->SetWindowLeft(((CPISensitiveScroll *)Sender)->GetPos(0));
      mRadarControl->SetWindowLeft(((CPISensitiveScroll *)Sender)->GetPos(0));
   }
   if (senderScroll->GetDir(1) != 0)
   {//the y axis was scrolled
      for (int i=0; i<SENSITIVESCROLLCOUNT; i++)
      {
          if (mScrolls[i] != NULL)
          {
            mScrolls[i]->SetPosWithNoChange(((CPISensitiveScroll *)Sender)->GetPos(1), 1);
          }
      }
      mBattleControl->SetWindowTop(((CPISensitiveScroll *)Sender)->GetPos(1));
      mRadarControl->SetWindowTop(((CPISensitiveScroll *)Sender)->GetPos(1));
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: when the exit key is pressed
 others     : temporar action
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionExit(CPIObject *Sender)
{
    ShowDialog("Quit", "Return to Dev? :)", DB_OKCANCEL, std::bind(&CPIMainScreen::ExitDialogClosed, this, std::placeholders::_1));
}
//---------------------------------------------------------------------------

void CPIMainScreen::ExitDialogClosed(CPIObject *Sender)
{
  if (mDialog->GetModalResult() == PMR_OK)
  {
    GetExitInstance()->SetTerminated(true);
  }
}
//---------------------------------------------------------------------------

void CPIMainScreen::OverwriteDialogClosed(CPIObject *Sender)
{
  if (mDialog->GetModalResult() == PMR_YES)
  {
    // perform the save - assuming mDialogSave still contains the correct result ;)
    CIGMissionDiskOp::Save(PATH_SAVEDGAMES + mDialogSave->GetFileName());
  }
}
//---------------------------------------------------------------------------

void CPIMainScreen::DeleteDialogClosed(CPIObject *Sender)
{
  if (mDialog->GetModalResult() == PMR_YES)
  {
    // perform delete - assuming mDialogSave still contains the correct result ;)
    bool result = CIGMissionDiskOp::Delete(PATH_SAVEDGAMES + mDialogSave->GetFileName());

    if (!result)
    {
        StandardGUICallback empty;
        ShowDialog("Error", "Save game could not be deleted!", DB_OK, empty);
    }
  }
}
//---------------------------------------------------------------------------

void CPIMainScreen::SaveDialogClosed(CPIObject *Sender)
{
  // yes means save, no means delete :) (until we add custom button codes)
  if (mDialogSave->GetModalResult() == PMR_YES)
  {
    //CPString saveGameName = mDialogSave->GetFileName();

    // check if the file exists
    if (mDialogSave->FileExists())
    {
      // popup overwrite dialog
      ShowDialog("Confirm", "Overwrite this save game?", DB_YESNO, std::bind(&CPIMainScreen::OverwriteDialogClosed, this, std::placeholders::_1));
    }
    else
    {
      // save the game
      CIGMissionDiskOp::Save(PATH_SAVEDGAMES + mDialogSave->GetFileName());
    }
  }
  else if (mDialogSave->GetModalResult() == PMR_NO)
  {
    // popup another dialog, ask if the user is sure to delete
    ShowDialog("Confirm", "Delete this save game?", DB_YESNO, std::bind(&CPIMainScreen::DeleteDialogClosed, this, std::placeholders::_1));
  }
}
//---------------------------------------------------------------------------

void CPIMainScreen::LoadDialogClosed(CPIObject *Sender)
{
  // yes means load, no means cancel
  if (mDialogLoad->GetModalResult() == PMR_YES)
  {
    // perform the loading
    CIGMissionDiskOp::Load(PATH_OUT + PATH_SAVEDGAMES + mDialogLoad->GetFileName());
  }
}

/*---------------------------------------------------------------------------
 description: sets the scroll speed
 parameters : Sender - the track bar
---------------------------------------------------------------------------*/
void CPIMainScreen::TrackGameSpeedOnChange(CPIObject *Sender, int oldValue)
{
   GetMissionInstance()->GetUpdateCounter().SetMaxValue(7 - mTrackGameSpeed->GetCurrentValue());

   GetProfileInstance()->SetGameSpeed(mTrackGameSpeed->GetCurrentValue());
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sets the scroll speed
 parameters : Sender - the track bar
              oldValue - the old value of the track
---------------------------------------------------------------------------*/
void CPIMainScreen::TrackScrollSpeedOnChange(CPIObject *Sender, int oldValue)
{
   for (int i=0; i<SENSITIVESCROLLCOUNT; i++)
   {
      //logWrite("TrackScrollSpeedOnChange: ");logWriteILn(i);

      if (mScrolls[i]->GetDir(0) != 0)
           mScrolls[i]->SetStep(mTrackScrollSpeed->GetCurrentValue()*4+4, 0);
      if (mScrolls[i]->GetDir(1) != 0)
           mScrolls[i]->SetStep(mTrackScrollSpeed->GetCurrentValue()*4+4, 1);
   }

   GetProfileInstance()->SetScrollSpeed(mTrackScrollSpeed->GetCurrentValue());
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sets the music volume
 parameters : Sender - the track bar
              oldValue - the old value of the track
 others     : if 0 => Mute
---------------------------------------------------------------------------*/
void CPIMainScreen::TrackMusicVolumeOnChange(CPIObject *Sender, int oldValue)
{
   GetProfileInstance()->SetMusicVolume(mTrackMusicVolume->CurrentAsPercent());

   if (mTrackMusicVolume->GetCurrentValue() == 0)
   {
      GetSoundInstance()->Stop(INDEX_AMBIENTALMUSIC);
      GetSoundInstance()->SetEnabled(INDEX_AMBIENTALMUSIC, false);
   }
   else
   {
      GetSoundInstance()->Stop(INDEX_AMBIENTALMUSIC);

      GetSoundInstance()->SetEnabled(INDEX_AMBIENTALMUSIC, true);
      GetSoundInstance()->SetVolume(INDEX_AMBIENTALMUSIC, (100. * mTrackMusicVolume->GetCurrentValue()) / mTrackMusicVolume->GetMaxValue());

      GetSoundInstance()->Play(INDEX_AMBIENTALMUSIC, 0, looped);
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sets the sfx volume
 parameters : Sender - the track bar
              oldValue - the old value of the track
 others     : if 0 => Mute
---------------------------------------------------------------------------*/
void CPIMainScreen::TrackSFXVolumeOnChange(CPIObject *Sender, int oldValue)
{
   GetProfileInstance()->SetSFXVolume(mTrackSFXVolume->CurrentAsPercent());
   GetSoundInstance()->SetMasterVolume(mTrackSFXVolume->CurrentAsPercent());

   // crash on mingw and linux g++
   /*
   for (int i = 0; i < 500; i++)
   {
      // music volume is set elsewhere
      if (i == INDEX_AMBIENTALMUSIC)
      {
        continue;
      }

      if (mTrackSFXVolume->GetCurrentValue() == 0)
         GetSoundInstance()->SetEnabled(i, false);
      else
      {
         GetSoundInstance()->SetEnabled(i, true);
         GetSoundInstance()->SetVolume(i, (100. * mTrackSFXVolume->GetCurrentValue()) / mTrackSFXVolume->GetMaxValue());
      }
   }
   */

   //GetSoundInstance()->SetMasterVolume((100. * mTrackSFXVolume->GetCurrentValue()) / mTrackSFXVolume->GetMaxValue());
}
//---------------------------------------------------------------------------

void CPIMainScreen::ViewCheckBoxOnLeftMouseClick(CPIObject *Sender)
{
   GetProfileInstance()->SetShowViewDir(mShowViewCheck->GetChecked());
}
//---------------------------------------------------------------------------

void CPIMainScreen::UnitTooltipCheckBoxOnLeftMouseClick(CPIObject *Sender)
{
   GetProfileInstance()->SetShowUnitTooltip(mShowUnitTooltipCheck->GetChecked());
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handler called when the user change the position of the radar/mini-map
 parameters : Sender - the radar control
---------------------------------------------------------------------------*/
void CPIMainScreen::OnRadarChange(CPIObject *Sender)
{
    // reset the focus object when selecting in the mini-map; alternatively, it's also possible
    // to interdict changing the mini-map viewport when in focus mode (really depends on the game)
    if (GetMissionInstance()->GetFocusObject())
    {
        GetMissionInstance()->SetFocusObject(NULL);
    }

   for (int i=0; i<SENSITIVESCROLLCOUNT; i++)
   {
      if (mScrolls[i]->GetDir(0) != 0)
         mScrolls[i]->SetPosWithNoChange(mRadarControl->GetWindowLeft(), 0);
      if (mScrolls[i]->GetDir(1) != 0)
         mScrolls[i]->SetPosWithNoChange(mRadarControl->GetWindowTop(), 1);
   }
   mBattleControl->SetWindowLeft(mRadarControl->GetWindowLeft());
   mBattleControl->SetWindowTop(mRadarControl->GetWindowTop());
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: hides the command panel and enlarge the battle control
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionHideCommandPanel(CPIObject *Sender)
{
   if (mPanelCommand->GetVisible() == true)
   {
      mBattleControl->SetLeft(0);
      mPanelCommand->SetVisible(false);
      mRadarControl->SetColor(0);
   }
   else
   {
      mBattleControl->SetLeft(mPanelCommand->GetWidth() - 17);
      mPanelCommand->SetVisible(true);
      mRadarControl->SetColor(0xFF404040);
   }
   mBattleControl->SetWidth(GetDesktop()->GetWidth() - mBattleControl->GetLeft());
   SetScrollBounds(GetMissionInstance()->GetMap()->GetWidth(), GetMissionInstance()->GetMap()->GetHeight());
   mScrolls[SS_SOUTHEAST1]->Check();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: take a screen shot
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionTakeScreenShot(CPIObject *Sender)
{
    // make sure destination folder exists
    if (!PHYSFS_mkdir(PATH_SCREENSHOTS.c_str()))
    {
        LOG(ERROR) << "CPGIGraphicSystem::TakeScreenShot - cannot create output dir " << PATH_SCREENSHOTS.c_str();
        return;
    }

    /*
    std::time_t t = std::time(nullptr);

    char mbstr[200];
    if (std::strftime(mbstr, sizeof(mbstr), "PPTactical %Y%m%d %H%M%S.png", std::localtime(&t)))
    {
        GetGraphicInstance()->TakeScreenShot(Vfs()->GetWriteDir() + PATH_SCREENSHOTS + mbstr);
    }
    */

    int screenshotCount = PlayerProfile::Instance()->GetScreenshotCount();

    CPString fileName = "PPTactical_" + padZero(screenshotCount, 7) + ".png";
    GetGraphicInstance()->TakeScreenShot(PATH_SCREENSHOTS + fileName);

    PlayerProfile::Instance()->SetScreenshotCount(screenshotCount + 1);

    AddMessage(CPString("Screenshot ") + fileName + CPString(" saved!"));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sets the add selection flag
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionAddSelection(CPIObject *Sender)
{
   GetMissionInstance()->SetFlag(MF_INVERTSELECTION, true);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: resets the add selection flag
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionNormalSelection(CPIObject *Sender)
{
   GetMissionInstance()->SetFlag(MF_ADDSELECTION, false);
   GetMissionInstance()->SetFlag(MF_INVERTSELECTION, false);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: inverts the move in formation flag
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionPreserveFormation(CPIObject *Sender)
{
   GetMissionInstance()->InvertFlag(MF_PRESERVEFORM);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: cancel the current mission action
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionCancelMissionAction(CPIObject *Sender)
{
   GetMissionInstance()->CancelAction();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: cancel the current mission action
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionPauseMission(CPIObject *Sender)
{
  GetMissionInstance()->SetEnabled(!GetMissionInstance()->GetEnabled());
  mPauseLabel->SetVisible(!GetMissionInstance()->GetEnabled());
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: quick save - save the mission into a fixed file
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionQuickSaveMission(CPIObject *Sender)
{
  VLOG(5) << "CPIMainScreen::ActionQuickSaveMission";

  // TODO: check the result of save and popup an info box in case of save failure
  CIGMissionDiskOp::Save(PATH_SAVEDGAMES + FILE_QUICK_SAVE);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: quick save - load the mission from a fixed file
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionQuickLoadMission(CPIObject *Sender)
{
  // TODO should also confirm to quick load?
  if (CIGMissionDiskOp::Load(PATH_OUT + PATH_SAVEDGAMES + FILE_QUICK_SAVE))
  {
    mBattleControl->SetMapWidth(GetMissionInstance()->GetMap()->GetLogicWidth());
    mBattleControl->SetMapHeight(GetMissionInstance()->GetMap()->GetLogicHeight());
    SetScrollBounds(GetMissionInstance()->GetMap()->GetWidth(), GetMissionInstance()->GetMap()->GetHeight());
  }
  else
  {
    // TODO: failure - popup a dialog to inform the user
  }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: save - pops up a dialog for saving purposes
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionSaveMission(CPIObject *Sender)
{
  // popup a dialog
  mDialogSave->SetEnabled(true);
  GetDesktop()->BringControlToFront(mDialogSave);
  mDialogSave->SetTitle("Save");
  mDialogSave->ShowModal(GetDesktop());
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: load - pops up a dialog for loading purposes
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionLoadMission(CPIObject *Sender)
{
  // popup a dialog
  mDialogLoad->SetEnabled(true);
  GetDesktop()->BringControlToFront(mDialogLoad);
  mDialogLoad->SetTitle("Load");
  mDialogLoad->ShowModal(GetDesktop());
}
//---------------------------------------------------------------------------

void CPIMainScreen::ActionFocusOnObject(CPIObject *Sender)
{
   GetMissionInstance()->ActionFocusOnObject();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sends the stop command to the mission
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionMissionCommandStop(CPIObject *Sender)
{
   GetMissionInstance()->SetAction(MA_STOP);
}
//---------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 description: sends the stop command to the mission
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionMissionCommandMove(CPIObject *Sender)
{
   GetMissionInstance()->SetAction(MA_MOVE);
}
//---------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 description: sends the stop command to the mission
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionMissionCommandAttack(CPIObject *Sender)
{
   GetMissionInstance()->SetAction(MA_ATTACK);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sends the stop command to the mission
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionMissionCommandPatrol(CPIObject *Sender)
{
   GetMissionInstance()->SetAction(MA_PATROL);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sends the "suppress fire" command to the mission
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionMissionCommandSuppressFire(CPIObject *Sender)
{
   GetMissionInstance()->SetAction(MA_SUPPRESS_FIRE);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sends the "take cover" command to the mission
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionMissionCommandTakeCover(CPIObject *Sender)
{
   GetMissionInstance()->SetAction(MA_TAKE_COVER);
}

/*---------------------------------------------------------------------------
 description: sends the "load" command to the mission
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionMissionCommandLoad(CPIObject *Sender)
{
   GetMissionInstance()->SetAction(MA_LOAD);
}

/*---------------------------------------------------------------------------
 description: sends the "unload" command to the mission
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionMissionCommandUnloadAll(CPIObject *Sender)
{
   GetMissionInstance()->SetAction(MA_UNLOAD_ALL);
}

/*---------------------------------------------------------------------------
 description: sends the "guard" command to the mission
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionMissionCommandGuard(CPIObject *Sender)
{
   GetMissionInstance()->SetAction(MA_GUARD);
}

/*---------------------------------------------------------------------------
 description: sets the team
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionSetTeam(CPIObject *Sender)
{
   VLOG(9) << "Set Team " << Sender->GetTag();

   GetMissionInstance()->SetSelectionToTeam(Sender->GetTag());
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sets the team
---------------------------------------------------------------------------*/
void CPIMainScreen::ActionSelectTeam(CPIObject *Sender)
{
   GetMissionInstance()->GetSelectionFromTeam(Sender->GetTag());
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the command to the objects
---------------------------------------------------------------------------*/
void CPIMainScreen::CommandBtnOnLeftMouseClick(CPIObject *Sender)
{
   // in an older incarnation of the engine (0.7.0), there was only one unit
   // that had the takeoff action: the dropship
   // now, all chopper derived units have this action
   //if (Sender->GetTag() == MA_TAKEOFF)
   //{
   //   GetExitInstance()->SetTerminated(true);
      //FormInGame->SetTerminated(true);
   //}
   //else
   //{
      GetMissionInstance()->SetAction(Sender->GetTag());
   //}
}
//---------------------------------------------------------------------------

void CPIMainScreen::ShowDialog(const CPString& title, const CPString& message, TDialogButtons buttons, StandardGUICallback callback)
{
    mDialog->SetEnabled(true);
    GetDesktop()->BringControlToFront(mDialog);
    mDialog->SetText(message);
    mDialog->SetTitle(title);
    mDialog->SetButtons(buttons);
    mDialog->SetHandleOnClose(callback);
    mDialog->ShowModal(GetDesktop());
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

