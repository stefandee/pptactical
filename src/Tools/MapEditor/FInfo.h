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

 descrition : diferite infpoormatii despre misiune 
 last modify: 16 11 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef FInfoH
#define FInfoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>

#include "PPIG_ObjBase.h"


//---------------------------------------------------------------------------
class TFormInfo : public TForm
{
__published:	// IDE-managed Components
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TLabel *Label4;
   TLabel *Label5;
   TLabel *Label6;
   TLabel *lbCoord;
   TLabel *lbID;
   TLabel *lbName;
   TLabel *lbType;
   TLabel *lbHealth;
   TLabel *lbSide;
   void __fastcall FormHide(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
   TMenuItem *itCheck;

   __fastcall TFormInfo(TComponent* Owner);
   void setInfo(CIGBaseObject *obj);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormInfo *FormInfo;
//---------------------------------------------------------------------------
#endif
