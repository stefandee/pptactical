//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    // 
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU General Public License as     // 
// published by the Free Software Foundation; either version 2 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //  
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY// 
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License  //
// for more details.                                                         //
//                                                                           // 
// You should have received a copy of the GNU General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   // 
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 application: PP Units Editor

 descrition : AI Units
 last modify: 24 11 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef m_aiunitsH
#define m_aiunitsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ImgList.hpp>
#include <Mask.hpp>


#include "AIUnits.h"
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include "CSPIN.h"
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
   TPageControl *pagecontrolMain;
   TTabSheet *sheetInf;
   TTabSheet *sheetVeh;
   TLabel *lbCurrentInfUnit;
   TUpDown *updownInf;
   TLabel *lbRecordPos;
   TGroupBox *groupInfData;
    TLabel *lbInfLastName;
    TLabel *lbInfFirstName;
   TLabel *lbInfDescription;
    TComboBox *comboInfSurname;
    TComboBox *comboInfFirstName;
   TMemo *memoInfStory;
   TBitBtn *btnInfDeleteAll;
   TBitBtn *btnInfNew;
   TBitBtn *btnInfSave;
   TBitBtn *btnInfDelete;
   TLabel *lbInfFileName;
   TImageList *imglistMain;
   TGroupBox *groupInfKind;
   TRadioGroup *radiogroupInfType;
   TComboBox *comboInfKind;
   TPageControl *pagecontrolInf;
   TTabSheet *sheetInfSkiils;
   TTabSheet *sheetInfWep;
   TTabSheet *sheetInfArmor;
   TGroupBox *grouInfSkills;
   TLabel *lbInfExp;
   TLabel *lbInfLeader;
   TLabel *lbInfBavery;
   TLabel *lbInfCamo;
   TLabel *lbInfSnipery;
   TLabel *lbInfSR;
   TLabel *lbInfAT;
   TLabel *lbInfHigh;
   TMaskEdit *editInfExp;
   TMaskEdit *editInfBravery;
   TMaskEdit *editInfSnipery;
   TMaskEdit *editInfAT;
   TMaskEdit *editInfLeader;
   TMaskEdit *editInfCamo;
   TMaskEdit *editInfSR;
   TMaskEdit *editInfHigh;
   TGroupBox *goupInfWep;
   TLabel *Label2;
   TLabel *Label3;
   TLabel *Label4;
   TLabel *Label12;
   TLabel *Label13;
   TLabel *Label14;
   TSpeedButton *btnLoadInfFxBmp;
   TLabel *Label15;
   TSpeedButton *btnLoadInfProj;
   TSpeedButton *btnLoadInfOnHit;
   TLabel *Label16;
   TLabel *Label17;
   TSpeedButton *btnLoadInfOnFire;
   TEdit *editInfFxBmp;
   TEdit *editInfProjBmp;
   TEdit *editInfOnHit;
   TEdit *editInfOnFire;
   TTabSheet *sheetInfComp;
   TMaskEdit *editInfPrec;
   TMaskEdit *editInfRad;
   TMaskEdit *editInfDamage;
   TMaskEdit *editInfRange;
   TMaskEdit *editInfRate;
   TBitBtn *btnLoadInfWep;
   TBitBtn *btnLoadInfArmor;
   TGroupBox *Sprite;
   TEdit *editInfSprite;
   TLabel *lbInfSprite;
   TSpeedButton *btnInfLoadSprite;
   TComboBox *comboInfWepType;
   TLabel *l_iwType;
   TLabel *lbInfKind;
   TEdit *editCurrentInf;
   TComboBox *comboCurrentInf;
   TStatusBar *statusMain;
   TLabel *lbCurrentVehUnit;
   TLabel *lbVehFileName;
   TLabel *lbVehRecordPos;
   TEdit *editCurrentVeh;
   TUpDown *updownVeh;
   TComboBox *comboCurrentVeh;
   TGroupBox *groupVehSprite;
   TLabel *l_vBmp;
   TSpeedButton *btnVehLoadSprite;
   TEdit *editVehSprite;
   TButton *btnVehSkeleton;
   TGroupBox *groupVehData;
   TLabel *l_vName;
   TLabel *l_vDesc;
   TComboBox *comboVehName;
   TMemo *memoVehStory;
   TGroupBox *groupVehType;
   TLabel *Label1;
   TLabel *Label5;
   TComboBox *comboVehType;
   TComboBox *comboVehMove;
   TGroupBox *gropVehHP;
   TLabel *Label6;
   TMaskEdit *editVehHP;
   TBitBtn *btnVehDeleteAll;
   TBitBtn *btnVehNew;
   TBitBtn *btnVehSave;
   TBitBtn *btnVehDelete;
   TPageControl *pageVeh;
   TTabSheet *sheetVehCrew;
   TTabSheet *sheetVehWeapon1;
   TTabSheet *sheetVehArmor;
   TBitBtn *btnInfLoadComp;
   TTabSheet *sheetVehComp;
   TBitBtn *BitBtn1;
   TBitBtn *btnVehLoadCrew;
   TTabSheet *sheetVehWeapon2;
   TBitBtn *btnVehLoadWeapon1;
   TBitBtn *btnVehLoadWeapon2;
   TGroupBox *groupVehWeapon2;
   TLabel *Label22;
   TLabel *Label23;
   TLabel *Label24;
   TLabel *Label25;
   TLabel *Label26;
   TLabel *Label27;
   TSpeedButton *btnVehLoadW2Fx;
   TLabel *Label28;
   TSpeedButton *btnVehLoadW2Proj;
   TSpeedButton *btnVehLoadW2Hit;
   TLabel *Label29;
   TLabel *Label30;
   TSpeedButton *btnVehLoadW2Fire;
   TLabel *Label31;
   TEdit *editVehW2Fx;
   TEdit *editVehW2Proj;
   TEdit *editVehW2Hit;
   TEdit *editVehW2Fire;
   TMaskEdit *editVehW2Prec;
   TMaskEdit *editVehW2Rad;
   TMaskEdit *editVehW2Damage;
        TMaskEdit *editVehW2MinRange;
   TMaskEdit *editVehW2Rate;
   TComboBox *comboVehW2Type;
   TGroupBox *groupVehWeapon1;
   TLabel *Label32;
   TLabel *Label33;
   TLabel *Label34;
   TLabel *Label35;
   TLabel *Label36;
   TLabel *Label37;
   TSpeedButton *btnVehLoadW1Fx;
   TLabel *Label38;
   TSpeedButton *btnVehLoadW1Proj;
   TSpeedButton *btnVehLoadW1Hit;
   TLabel *Label39;
   TLabel *Label40;
   TSpeedButton *btnVehLoadW1Fire;
   TLabel *Label41;
   TEdit *editVehW1Fx;
   TEdit *editVehW1Proj;
   TEdit *editVehW1Hit;
   TEdit *editVehW1Fire;
   TMaskEdit *editVehW1Prec;
   TMaskEdit *editVehW1Rad;
   TMaskEdit *editVehW1Damage;
        TMaskEdit *editVehW1MinRange;
   TMaskEdit *editVehW1Rate;
   TComboBox *comboVehW1Type;
   TGroupBox *groupInfArmor;
   TLabel *l_iaPRes;
   TLabel *l_iaLRes;
   TLabel *l_iaXRes;
   TLabel *l_iaBRes;
   TMaskEdit *editVehPRes;
   TMaskEdit *editVehLRes;
   TMaskEdit *editVehXRes;
   TMaskEdit *editVehBRes;
   TGroupBox *GroupBox1;
   TLabel *Label42;
   TLabel *Label43;
   TLabel *Label44;
   TLabel *Label45;
   TMaskEdit *editInfPRes;
   TMaskEdit *editInfLRes;
   TMaskEdit *editInfXRes;
   TMaskEdit *editInfBRes;
   TGroupBox *groupVehComp;
   TLabel *lbVehAck;
   TLabel *lbVehResp;
   TLabel *lbVehHit;
   TLabel *lbVehAnnoyed;
   TLabel *lbVehDie;
   TLabel *lbVehIdle;
   TLabel *lbVehKill;
   TListBox *listVehAck;
   TListBox *listVehResp;
   TListBox *listVehHit;
   TListBox *listVehDie;
   TListBox *listVehAnn;
   TListBox *listVehIdle;
   TListBox *listVehKill;
   TGroupBox *groupInfComp;
   TLabel *lAck;
   TLabel *lResp;
   TLabel *lHit;
   TLabel *lAnnoyed;
   TLabel *lDie;
   TLabel *lIdle;
   TLabel *lKill;
   TListBox *listInfAck;
   TListBox *listInfResp;
   TListBox *listInfHit;
   TListBox *listInfDie;
   TListBox *listInfAnn;
   TListBox *listInfIdle;
   TListBox *listInfKill;
   TBitBtn *BitBtn2;
   TOpenDialog *dialogOpen;
   TGroupBox *groupVehCrewDatas;
   TComboBox *comboVehCallsign;
   TLabel *lCNick;
        TMemo *memoVehCrewInfo;
   TLabel *Label9;
   TMainMenu *menuMain;
   TMenuItem *mitFile;
   TMenuItem *itExit;
   TMenuItem *mitAbout;
   TMenuItem *itAbout;
        TLabel *Label10;
        TMaskEdit *editVehFRes;
        TLabel *Label11;
        TMaskEdit *editInfFRes;
        TMaskEdit *editVehW1MaxRange;
        TLabel *Label18;
        TLabel *Label19;
        TMaskEdit *editVehW2MaxRange;
        TCheckBox *checkVehW1Enabled;
        TCheckBox *checkVehW2Enabled;
        TCheckBox *ch_vSelf;
        TMaskEdit *editVehTurretTurn;
        TLabel *Label20;
        TLabel *Label21;
        TMaskEdit *editInfPShot;
        TLabel *Label46;
        TEdit *editInfAFxBmp;
        TSpeedButton *btnLoadInfAFx;
        TLabel *Label47;
        TEdit *editVehW1AFx;
        TSpeedButton *btnLoadVehW1AFx;
        TLabel *Label48;
        TMaskEdit *editVehW1PShot;
        TLabel *Label49;
        TEdit *editVehW2AFx;
        TSpeedButton *btnLoadVehW2AFx;
        TLabel *Label50;
        TMaskEdit *editVehW2PShot;
        TLabel *Label51;
        TEdit *editVehW1TrackP;
        TLabel *Label52;
        TEdit *editVehW2TrackP;
        TLabel *Label53;
        TEdit *editInfTrackP;
        TLabel *Label54;
        TEdit *editInfPortrait;
        TSpeedButton *btnInfLoadPortrait;
        TLabel *Label55;
        TEdit *editVehPortrait;
        TSpeedButton *btnVehLoadPortrait;
        TLabel *Label56;
        TMaskEdit *editVehBayCapacity;
        TMaskEdit *editVehUpSlope;
        TLabel *Label57;
        TLabel *Label58;
        TMaskEdit *editVehDownSlope;
        TLabel *lbInfStealth;
        TMaskEdit *editInfStealth;
        TLabel *lbInfInfl;
        TMaskEdit *editInfInfl;
        TLabel *lbInfWis;
        TMaskEdit *editInfWis;
        TPageControl *PageControl1;
        TTabSheet *sheetSkills1;
        TTabSheet *TabSheet2;
        TLabel *lCDrive;
        TMaskEdit *editVehDrive;
        TMaskEdit *editVehAntiInf;
        TLabel *lCAntiInf;
        TLabel *lbVehAntiVeh;
        TMaskEdit *editVehAntiVeh;
        TLabel *lbVehECM;
        TMaskEdit *editVehECM;
        TLabel *lCBravery;
        TMaskEdit *editVehBrave;
        TLabel *lCExp;
        TMaskEdit *editVehExp;
        TLabel *Label7;
        TMaskEdit *editVehMorale;
        TLabel *Label8;
        TMaskEdit *editVehLead;
        TLabel *lbVTeam;
        TMaskEdit *editVehTeam;
        TLabel *lbVehAntiAir;
        TMaskEdit *editVehAntiAir;
        TTabSheet *TabSheet1;
        TGroupBox *GroupBox2;
        TLabel *Label59;
        TEdit *edVExplodeSprite;
        TSpeedButton *btnVLoadExplode;
        TCSpinEdit *edVExplodeCount;
        TLabel *Label60;
        TMaskEdit *editVehW1TTL;
        TLabel *Label61;
        TLabel *Label62;
        TMaskEdit *editVehW2TTL;
        TLabel *Label63;
        TMaskEdit *editInfTTL;
        TGroupBox *groupHP;
        TLabel *lbInfHP;
        TMaskEdit *editInfHP;
        TButton *btnInfSkel;
        TLabel *Label64;
        TEdit *editInfMiniPortrait;
        TSpeedButton *btnInfLoadMiniPortrait;
        TLabel *Label65;
        TEdit *editVehMiniPortrait;
        TSpeedButton *btnVehLoadMiniPortrait;
    TComboBox *comboGender;
    TStaticText *StaticText1;
   void __fastcall itExitClick(TObject *Sender);
   void __fastcall itPreviewClick(TObject *Sender);
   void __fastcall radiogroupInfTypeClick(TObject *Sender);
   void __fastcall itAboutClick(TObject *Sender);
   void __fastcall btnInfDeleteAllClick(TObject *Sender);
   void __fastcall btnInfNewClick(TObject *Sender);
   void __fastcall btnInfSaveClick(TObject *Sender);
   void __fastcall btnInfDeleteClick(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall editCurrentInfChange(TObject *Sender);
   void __fastcall comboCurrentInfChange(TObject *Sender);
   void __fastcall btnInfLoadSpriteClick(TObject *Sender);
   void __fastcall btnLoadInfFxBmpClick(TObject *Sender);
   void __fastcall btnLoadInfProjClick(TObject *Sender);
   void __fastcall btnLoadInfOnHitClick(TObject *Sender);
   void __fastcall btnLoadInfOnFireClick(TObject *Sender);
   void __fastcall listInfAckDblClick(TObject *Sender);
   void __fastcall listInfAnnDblClick(TObject *Sender);
   void __fastcall listInfRespDblClick(TObject *Sender);
   void __fastcall listInfIdleDblClick(TObject *Sender);
   void __fastcall listInfHitDblClick(TObject *Sender);
   void __fastcall listInfKillDblClick(TObject *Sender);
   void __fastcall listInfDieDblClick(TObject *Sender);
   void __fastcall btnVehNewClick(TObject *Sender);
   void __fastcall btnVehSaveClick(TObject *Sender);
   void __fastcall btnVehDeleteClick(TObject *Sender);
   void __fastcall btnVehDeleteAllClick(TObject *Sender);
   void __fastcall editCurrentVehChange(TObject *Sender);
   void __fastcall comboCurrentVehChange(TObject *Sender);
        void __fastcall listVehAckDblClick(TObject *Sender);
        void __fastcall btnVehLoadSpriteClick(TObject *Sender);
        void __fastcall btnVehSkeletonClick(TObject *Sender);
        void __fastcall listVehAnnDblClick(TObject *Sender);
        void __fastcall listVehRespDblClick(TObject *Sender);
        void __fastcall listVehIdleDblClick(TObject *Sender);
        void __fastcall listVehHitDblClick(TObject *Sender);
        void __fastcall listVehKillDblClick(TObject *Sender);
        void __fastcall listVehDieDblClick(TObject *Sender);
        void __fastcall listVehAckKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall listVehAnnKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall listVehRespKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall listVehIdleKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall listVehHitKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall listVehKillKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall listVehDieKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall comboVehTypeChange(TObject *Sender);
        void __fastcall btnVehLoadW1FxClick(TObject *Sender);
        void __fastcall btnVehLoadW1ProjClick(TObject *Sender);
        void __fastcall btnVehLoadW1HitClick(TObject *Sender);
        void __fastcall btnVehLoadW1FireClick(TObject *Sender);
        void __fastcall btnLoadVehW1AFxClick(TObject *Sender);
        void __fastcall btnLoadInfAFxClick(TObject *Sender);
        void __fastcall btnVehLoadPortraitClick(TObject *Sender);
        void __fastcall btnInfLoadPortraitClick(TObject *Sender);
        void __fastcall btnVLoadExplodeClick(TObject *Sender);
        void __fastcall btnInfSkelClick(TObject *Sender);
        void __fastcall btnInfLoadMiniPortraitClick(TObject *Sender);
private:	// User declarations
  AnsiString FixPath(AnsiString _value);

public:		// User declarations
   __fastcall TFormMain(TComponent* Owner);

   //Infantry
   int handleInf;             //handle de fisier
   int sizeInf;               //aici tin minte sizeof(TInfantryUnified)
   hInfantryUnified recInf;   //record infantry

   void openInf();
   void closeInf();

   bool newInf();
   bool loadInf(int index);
   bool saveInf(int index);
   bool deleteInf(int index);
   bool deleteAllInf();

   void setInfDataToRecord();          //din edit-uri in recInf
   void setInfRecordToData();         //din recInf in edit-uri

   void updateInfRecords();
   void updateInfRecordsCaption();

   void loadInfNames();
   //Infantry - over

   //Vehicles
   int handleVeh;             //handle de fisier
   int sizeVeh;               //aici tin minte sizeof(TVehicleUnified)
   hVehicleUnified recVeh;    //record infantry

   void openVeh();
   void closeVeh();

   bool newVeh();
   bool loadVeh(int index);
   bool saveVeh(int index);
   bool deleteVeh(int index);
   bool deleteAllVeh();

   void setVehDataToRecord();          //din edit-uri in recVeh
   void setVehRecordToData();         //din recVeh in edit-uri

   void updateVehRecords();
   void updateVehRecordsCaption();

   void loadVehNames();
   //Vehicles - over

   //Air
   //Air - over

   void __fastcall DisplayHint(TObject *Sender);

};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
