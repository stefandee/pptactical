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
#include <vcl.h>
#pragma hdrstop

#include "FPref.h"
#include "FAlt.h"
#include "FMain.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPreferences *FormPreferences;
//---------------------------------------------------------------------------
__fastcall TFormPreferences::TFormPreferences(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFormPreferences::shapeGridColorMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   dialogColor->Color = shapeGridColor->Brush->Color;
   if (dialogColor->Execute())
      shapeGridColor->Brush->Color = dialogColor->Color;

}
//---------------------------------------------------------------------------

void __fastcall TFormPreferences::lbAltitudeFontClick(TObject *Sender)
{
   if (dialogFont->Execute())
      lbAltitudeFont->Font = dialogFont->Font;
}
//---------------------------------------------------------------------------

void __fastcall TFormPreferences::shapeAltitudeBrushColorMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
   if (dialogColor->Execute())
      shapeAltitudeBrushColor->Pen->Color = dialogColor->Color;
}
//---------------------------------------------------------------------------


void __fastcall TFormPreferences::shapeMaskColorMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (dialogColor->Execute())
      shapeMaskColor->Brush->Color = dialogColor->Color;
}
//---------------------------------------------------------------------------


void __fastcall TFormPreferences::FormCreate(TObject *Sender)
{
   comboGridHoriz->ItemIndex = 0;
   comboGridVert->ItemIndex  = 0;
   comboAltitudeWidth->ItemIndex  = 0;
   comboAltitudeHeight->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormPreferences::FormShow(TObject *Sender)
{
   FormPreferences->comboAltitudeWidth->ItemIndex =
                                                FormAltitude->editWidth->Value-1;
   FormPreferences->comboAltitudeHeight->ItemIndex =
                                                FormAltitude->editHeight->Value-1;
}
//---------------------------------------------------------------------------

void __fastcall TFormPreferences::btnSaveDesktopClick(TObject *Sender)
{
   FormMain->SaveDesktop();
}
//---------------------------------------------------------------------------

void __fastcall TFormPreferences::btnLoadDesktopClick(TObject *Sender)
{
   FormMain->LoadDesktop();
}
//---------------------------------------------------------------------------





