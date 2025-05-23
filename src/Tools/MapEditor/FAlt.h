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
#ifndef FAltH
#define FAltH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CSPIN.h"
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFormAltitude : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *groupboxSize;
   TCSpinEdit *editWidth;
   TCSpinEdit *editHeight;
   TCSpinEdit *editAltitude;
   TLabel *lbAltitude;
   TSpeedButton *btnAltitude;
   TLabel *Label1;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormHide(TObject *Sender);
   void __fastcall btnAltitudeClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
   TMenuItem *itCheck;

   __fastcall TFormAltitude(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAltitude *FormAltitude;
//---------------------------------------------------------------------------
#endif
 