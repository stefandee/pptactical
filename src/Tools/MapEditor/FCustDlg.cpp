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

 descrition : Custom Dialog 
 last modify: 18 11 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FCustDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDialog *FormDialog;
//---------------------------------------------------------------------------
__fastcall TFormDialog::TFormDialog(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: Mission Editor Message
---------------------------------------------------------------------------*/
TModalResult MEMsg(AnsiString msg, AnsiString caption,
                   int buttons,
                   AnsiString captionOk, AnsiString captionCancel)
{
   TModalResult result;
   FormDialog->memoMsg->Alignment = taCenter;
   FormDialog->Caption = caption;
   FormDialog->memoMsg->Clear();
   FormDialog->memoMsg->Text = msg;
   if (buttons == (CUST_OK|CUST_CANCEL))
   {
      FormDialog->btnCancel->Cancel = true;
      FormDialog->btnOk->Left = 16;
      FormDialog->btnOk->Visible = true;
      FormDialog->btnCancel->Visible = true;
   }
   if (buttons == CUST_OK)
   {
      FormDialog->btnOk->Cancel = true;
      FormDialog->btnOk->Left = (FormDialog->Width - FormDialog->btnOk->Width)/2;
      FormDialog->btnOk->Visible = true;
      FormDialog->btnCancel->Visible = false;
   }
   FormDialog->btnOk->Caption = captionOk;
   FormDialog->btnCancel->Caption = captionCancel;
   result = FormDialog->ShowModal();
   return result;
}

/*---------------------------------------------------------------------------
 descrition: Mission Editor Message
             inseamna ca in prealabil s-au pus in memo ce trebuia
---------------------------------------------------------------------------*/
TModalResult MEMsg(AnsiString caption,
                   int buttons,
                   AnsiString captionOk, AnsiString captionCancel)
{
   TModalResult result;
   FormDialog->memoMsg->Alignment = taLeftJustify;
   FormDialog->Caption = caption;
   if (buttons == (CUST_OK|CUST_CANCEL))
   {
      FormDialog->btnCancel->Cancel = true;
      FormDialog->btnOk->Left = 16;
      FormDialog->btnOk->Visible = true;
      FormDialog->btnCancel->Visible = true;
   }
   if (buttons == CUST_OK)
   {
      FormDialog->btnOk->Cancel = true;
      FormDialog->btnOk->Left = (FormDialog->Width - FormDialog->btnOk->Width)/2;
      FormDialog->btnOk->Visible = true;
      FormDialog->btnCancel->Visible = false;
   }
   FormDialog->btnOk->Caption = captionOk;
   FormDialog->btnCancel->Caption = captionCancel;
   result = FormDialog->ShowModal();
   return result;
}

//---------------------------------------------------------------------------
