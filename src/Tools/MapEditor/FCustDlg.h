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
#ifndef FCustDlgH
#define FCustDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------

#define CUST_OK      0
#define CUST_CANCEL  1

class TFormDialog : public TForm
{
__published:	// IDE-managed Components
   TMemo *memoMsg;
   TBitBtn *btnOk;
   TBitBtn *btnCancel;
private:	// User declarations
public:		// User declarations
   __fastcall TFormDialog(TComponent* Owner);
};

TModalResult MEMsg(AnsiString caption,
                   int buttons,
                   AnsiString captionOk, AnsiString captionCancel);
TModalResult MEMsg(AnsiString msg, AnsiString caption,
                   int buttons,
                   AnsiString captionOk, AnsiString captionCancel);


//---------------------------------------------------------------------------
extern PACKAGE TFormDialog *FormDialog;
//---------------------------------------------------------------------------
#endif
