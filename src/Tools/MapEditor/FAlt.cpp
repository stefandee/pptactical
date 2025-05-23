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
 
 description: Form Altitude
 last modify: 02 03 00
        by grabX
---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FAlt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"

#include "FMain.h"

TFormAltitude *FormAltitude;

//---------------------------------------------------------------------------
__fastcall TFormAltitude::TFormAltitude(TComponent* Owner)
   : TForm(Owner)
{
   itCheck = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TFormAltitude::FormShow(TObject *Sender)
{
   if (itCheck)
      itCheck->Checked = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormAltitude::FormHide(TObject *Sender)
{
   if (itCheck)
      itCheck->Checked = false;
   if (btnAltitude->Down)
   {
      btnAltitude->Down = false;
      FormMain->endAltitude();
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormAltitude::btnAltitudeClick(TObject *Sender)
{
   if (btnAltitude->Down)
      FormMain->beginAltitude();
   else
      FormMain->endAltitude();
}
//---------------------------------------------------------------------------


 