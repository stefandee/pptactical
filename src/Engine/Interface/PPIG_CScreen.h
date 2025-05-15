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
// for more details.                                                         //
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
//---------------------------------------------------------------------------
#ifndef PPIG_CScreenH
#define PPIG_CScreenH

#include "PP_CScreen.h"
#include "PPIG_InterfaceDef.h"
#include "PP_GuiDef.h"

// fwd. decl.
class CPIPanel;
class CPIButton;
class CPITrackBar;
class CPISensitiveScroll;
class CPIBattleControl;
class CPILabel;
class CPIRadarControl;
class CPIMessageConsole;
class CPIUnitInfo;
class CPICommonDialog;
class CPP_CSFormDialog;
class CPP_CSaveGameDialog;
class CPP_CLoadGameDialog;
class CPICheckBox;

class CPIMainScreen: public CPIScreen
{
   public:
      static CPIMainScreen* Instance();
      static void Release();

   protected:
      static CPIMainScreen *mInstance;

      CPIMainScreen();
      virtual ~CPIMainScreen();
      void CreateScrolls();
      void CreateMenu();
      void CreateCommandPanel();
      void CreateActions();
      void InitEventHandlers();
      void CreateMusic();
      void CreateUIElements();

   public:  //user methods
      void Update();

      void SetScrollBounds(int xMax, int yMax);
      void SetScrollPos(int X, int Y);
      void AddMessage(CPString _msg);

   public:  //user event handlers
      void BtnMenuOnLeftMouseClick(CPIObject *Sender);
      void BtnConsoleOnLeftMouseClick(CPIObject *Sender);
      void ScrollOnChange(CPIObject *Sender);
      void ActionExit(CPIObject *Sender);
      void TrackGameSpeedOnChange(CPIObject *Sender, int oldValue);
      void TrackScrollSpeedOnChange(CPIObject *Sender, int oldValue);
      void TrackMusicVolumeOnChange(CPIObject *Sender, int oldValue);
      void TrackSFXVolumeOnChange(CPIObject *Sender, int oldValue);
      void ViewCheckBoxOnLeftMouseClick(CPIObject *Sender);
      void UnitTooltipCheckBoxOnLeftMouseClick(CPIObject* Sender);
      void OnRadarChange(CPIObject *Sender);
      void ActionHideCommandPanel(CPIObject *Sender);
      void ActionTakeScreenShot(CPIObject *Sender);
      void ActionAddSelection(CPIObject *Sender);
      void ActionNormalSelection(CPIObject *Sender);
      void ActionPreserveFormation(CPIObject *Sender);
      void ActionCancelMissionAction(CPIObject *Sender);
      void ActionPauseMission(CPIObject *Sender);

      // load and save
      void ActionQuickSaveMission(CPIObject *Sender);
      void ActionQuickLoadMission(CPIObject *Sender);
      void ActionSaveMission(CPIObject *Sender);
      void ActionLoadMission(CPIObject *Sender);

      void ActionFocusOnObject(CPIObject *Sender);

      void ActionMissionCommandStop(CPIObject *Sender);
      void ActionMissionCommandMove(CPIObject *Sender);
      void ActionMissionCommandAttack(CPIObject *Sender);
      void ActionMissionCommandPatrol(CPIObject *Sender);
      void ActionMissionCommandSuppressFire(CPIObject *Sender);
      void ActionMissionCommandTakeCover(CPIObject *Sender);
      void ActionMissionCommandLoad(CPIObject *Sender);
      void ActionMissionCommandUnloadAll(CPIObject *Sender);
      void ActionMissionCommandGuard(CPIObject *Sender);
      void ActionSetTeam(CPIObject *Sender);
      void ActionSelectTeam(CPIObject *Sender);
      void CommandBtnOnLeftMouseClick(CPIObject *Sender);

      void ExitDialogClosed(CPIObject *Sender);
      void OverwriteDialogClosed(CPIObject *Sender);
      void DeleteDialogClosed(CPIObject *Sender);
      void SaveDialogClosed(CPIObject *Sender);
      void LoadDialogClosed(CPIObject *Sender);

   public:  //user attributes
      //battle controls
      CPIBattleControl     *mBattleControl;
      CPIRadarControl      *mRadarControl;

      //command controls
      CPIPanel             *mPanelCommand;
      CPIPanel             *mPanelStatus;
      CPIUnitInfo          *mUnitInfo;
      CPIPanel             *mPanelCmdBtns;
      CPIButton            *mBtnsCommand[COMMAND_BUTTONS_COUNT];

      //menu controls
      CPIPanel             *mPanelMenu;
      CPIButton            *mBtnMenu;
      CPITrackBar          *mTrackScrollSpeed;
      CPITrackBar          *mTrackGameSpeed;
      CPITrackBar          *mTrackMusicVolume;
      CPITrackBar          *mTrackSFXVolume;

      //scroll controls
      CPISensitiveScroll   *mScrollNorth;
      CPISensitiveScroll   *mScrollNorthEast1;
      CPISensitiveScroll   *mScrollNorthEast2;
      CPISensitiveScroll   *mScrollEast;
      CPISensitiveScroll   *mScrollSouthEast1;
      CPISensitiveScroll   *mScrollSouthEast2;
      CPISensitiveScroll   *mScrollSouth;
      CPISensitiveScroll   *mScrollSouthWest1;
      CPISensitiveScroll   *mScrollSouthWest2;
      CPISensitiveScroll   *mScrollWest;
      CPISensitiveScroll   *mScrollNorthWest1;
      CPISensitiveScroll   *mScrollNorthWest2;
      CPISensitiveScroll   *mScrolls[SENSITIVESCROLLCOUNT];

      //misc
      CPILabel             *mPauseLabel;
      CPILabel             *mObjectHintLabel;
      CPP_CSFormDialog     *mDialog;
      CPP_CSaveGameDialog  *mDialogSave;
      CPP_CLoadGameDialog  *mDialogLoad;

      // messages window
      CPIMessageConsole    *mConsole;
      CPIPanel             *mPanelConsole;
      CPIButton            *mBtnConsole;

      CPICheckBox*         mShowViewCheck;
      CPICheckBox*         mShowUnitTooltipCheck;

   private:
        void ShowDialog(const CPString& title, const CPString& message, TDialogButtons buttons, StandardGUICallback callback = NULL);
};

CPIMainScreen* GetMainScreenInstance();


//---------------------------------------------------------------------------
#endif

