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
//---------------------------------------------------------------------------
#ifndef m_slessH
#define m_slessH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include "Statics.h"
#include "ImageEx.h"
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu;
        TMenuItem *File1;
        TMenuItem *About1;
        TMenuItem *New1;
        TMenuItem *Load1;
        TMenuItem *Saveas1;
        TMenuItem *N1;
        TMenuItem *Exit1;
        TMenuItem *About2;
        TBevel *Bevel1;
        TGroupBox *gb_Generals;
        TGroupBox *gb_weapon;
        TStatusBar *StatusBar;
        TLabel *l_name;
        TEdit *ed_name;
        TLabel *l_desc;
        TMemo *m_desc;
        TCheckBox *chb_canfire;
        TCheckBox *chb_soldiers;
        TCheckBox *chb_vehicle;
        TGroupBox *gb_armor;
        TEdit *ed_hp;
        TLabel *l_hp;
        TButton *b_mask;
        TLabel *l_sprite;
        TEdit *ed_sprite;
        TSpeedButton *sb_sprite;
        TLabel *l_anim;
        TComboBox *cb_type;
        TLabel *l_pres;
        TLabel *l_nres;
        TLabel *l_plres;
        TLabel *l_bres;
        TLabel *l_wname;
        TEdit *ed_wname;
        TLabel *l_range;
        TLabel *l_precision;
        TLabel *l_radius;
        TLabel *l_damage;
        TLabel *l_fr;
        TLabel *l_wtype;
        TComboBox *cb_wtype;
        TLabel *l_projb;
        TLabel *l_effb;
        TEdit *ed_proj;
        TEdit *ed_effb;
        TSpeedButton *sb_projb;
        TSpeedButton *sb_effb;
        TEdit *ed_range;
        TEdit *ed_prec;
        TEdit *ed_radius;
        TEdit *ed_damage;
        TEdit *ed_fr;
        TEdit *ed_pres;
        TEdit *ed_nres;
        TEdit *ed_plres;
        TEdit *ed_bres;
        TMenuItem *Save1;
        TOpenDialog *OpenDialog;
        TSaveDialog *SaveDialog;
        TLabel *Label1;
        TEdit *ed_fres;
        TCheckBox *chb_indestr;
        TCheckBox *chb_sfire;
        TLabel *Label2;
        TEdit *ed_onFire;
        TSpeedButton *sb_onFire;
        TLabel *Label3;
        TEdit *ed_onHit;
        TSpeedButton *sb_onHit;
        TOpenDialog *OpenSoundDialog;
        TOpenImageEx *OpenPictureDialog;
        TCheckBox *chb_sLand;
        TButton *btnLights;
   TLabel *Label4;
        void __fastcall sb_spriteClick(TObject *Sender);
        void __fastcall sb_projbClick(TObject *Sender);
        void __fastcall sb_effbClick(TObject *Sender);
        void __fastcall Load1Click(TObject *Sender);
        
        void __fastcall New1Click(TObject *Sender);
        void __fastcall Saveas1Click(TObject *Sender);
        void __fastcall Save1Click(TObject *Sender);
        void __fastcall ed_nameExit(TObject *Sender);
        void __fastcall m_descExit(TObject *Sender);
        void __fastcall cb_typeExit(TObject *Sender);
        void __fastcall ed_spriteExit(TObject *Sender);
        void __fastcall chb_vehicleClick(TObject *Sender);
        void __fastcall chb_soldiersClick(TObject *Sender);
        void __fastcall chb_canfireClick(TObject *Sender);
        void __fastcall ed_hpExit(TObject *Sender);
        void __fastcall ed_presExit(TObject *Sender);
        void __fastcall ed_nresExit(TObject *Sender);
        void __fastcall ed_plresExit(TObject *Sender);
        void __fastcall ed_bresExit(TObject *Sender);
        void __fastcall ed_wnameExit(TObject *Sender);
        void __fastcall cb_wtypeExit(TObject *Sender);
        void __fastcall ed_projExit(TObject *Sender);
        void __fastcall ed_effbExit(TObject *Sender);
        void __fastcall ed_rangeExit(TObject *Sender);
        void __fastcall ed_precExit(TObject *Sender);
        void __fastcall ed_radiusExit(TObject *Sender);
        void __fastcall ed_damageExit(TObject *Sender);
        void __fastcall ed_frExit(TObject *Sender);
        void __fastcall Exit1Click(TObject *Sender);
        void __fastcall About2Click(TObject *Sender);
        void __fastcall b_maskClick(TObject *Sender);
        void __fastcall chb_indestrClick(TObject *Sender);
        void __fastcall chb_sfireClick(TObject *Sender);
        void __fastcall ed_fresExit(TObject *Sender);
        void __fastcall ed_onFireExit(TObject *Sender);
        void __fastcall ed_onHitExit(TObject *Sender);
        void __fastcall sb_onFireClick(TObject *Sender);
        void __fastcall sb_onHitClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall chb_sLandClick(TObject *Sender);
private:	// User declarations
        void SRec2Edit();
        void Test();

public:		// User declarations
        AnsiString fileName;
        hStaticObject tempS;

        __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
