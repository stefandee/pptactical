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
#ifndef FMissionH
#define FMissionH
//---------------------------------------------------------------------------
#include <Classes.hpp> 
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormMission : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *gb_mName;
        TEdit *ed_mName;
        TGroupBox *gb_mBrief;
        TMemo *m_mBrief;
        TGroupBox *gb_location;
        TComboBox *cb_ssys;
        TLabel *l_ssys;
        TLabel *Label1;
        TComboBox *cb_planet;
        TButton *b_Ok;
        TButton *b_Cancel;
        TLabel *Label2;
        TEdit *ed_Start;
        TLabel *Label3;
        TEdit *ed_End;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall cb_ssysChange(TObject *Sender);
        void __fastcall b_OkClick(TObject *Sender);
        void __fastcall ed_StartExit(TObject *Sender);
        void __fastcall ed_EndExit(TObject *Sender);
private:	// User declarations
        bool ExtractDate(AnsiString date, int* day, int* month, int* year);
public:		// User declarations
        __fastcall TFormMission(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMission *FormMission;
//---------------------------------------------------------------------------
#endif
