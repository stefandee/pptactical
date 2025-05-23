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
 application: PP Mission Editor

 descrition : Form Preferences 
 last modify: 21 11 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef FPrefH
#define FPrefH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFormPreferences : public TForm
{
__published:	// IDE-managed Components
   TPageControl *PageControl1;
   TTabSheet *sheetGrid;
   TTabSheet *sheetAltitude;
   TTabSheet *sheetEnvir;
   TBitBtn *btnSaveDesktop;
   TBitBtn *btnLoadDesktop;
   TBitBtn *btnOk;
   TBitBtn *btnCancel;
   TCheckBox *checkSaveDesktop;
   TComboBox *comboGridVert;
   TComboBox *comboGridHoriz;
   TLabel *lbGridHoriz;
   TLabel *lbGridVert;
   TShape *shapeGridColor;
   TLabel *lbGridColor;
   TColorDialog *dialogColor;
   TCheckBox *checkShowGrid;
   TLabel *Label1;
   TComboBox *comboAltitudeWidth;
   TComboBox *comboAltitudeHeight;
   TLabel *Label2;
   TLabel *Label3;
   TShape *shapeAltitudeBrushColor;
   TLabel *Label4;
   TLabel *lbAltitudeFont;
   TFontDialog *dialogFont;
   TTabSheet *sheetMask;
   TShape *shapeMaskColor;
   TLabel *Label5;
   TCheckBox *checkShowMask;
   void __fastcall shapeGridColorMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall lbAltitudeFontClick(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall shapeAltitudeBrushColorMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall btnLoadDesktopClick(TObject *Sender);
   void __fastcall btnSaveDesktopClick(TObject *Sender);
   void __fastcall shapeMaskColorMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// User declarations
public:		// User declarations
   __fastcall TFormPreferences(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormPreferences *FormPreferences;
//---------------------------------------------------------------------------
#endif
