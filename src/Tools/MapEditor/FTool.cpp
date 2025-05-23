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

 descrition : Form Tool
              tool bar-uri intr-o singura forma
              butoanele apeleaza actiuni din FormMain
 last modify: 21 11 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
 
#include "FTool.h"
#include "FMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormTool *FormTool;
//---------------------------------------------------------------------------
__fastcall TFormTool::TFormTool(TComponent* Owner)
   : TForm(Owner)
{
   btnInf->ImageIndex = 4;
   btnVehicle->ImageIndex = 5;
   btnStatic->ImageIndex = 6;
   btnAir->ImageIndex = 7;

   itCheck = NULL;
}
//---------------------------------------------------------------------------


void __fastcall TFormTool::FormShow(TObject *Sender)
{
   if (itCheck)
      itCheck->Checked = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormTool::FormHide(TObject *Sender)
{
   if (itCheck)
      itCheck->Checked = false;
}
//---------------------------------------------------------------------------

