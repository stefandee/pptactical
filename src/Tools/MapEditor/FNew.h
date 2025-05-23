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

 descrition : Form for new map
 last modify: 2 11 99 
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef FNewH
#define FNewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFormNewMap : public TForm
{
__published:	// IDE-managed Components
   TEdit *edMapHeight;
   TEdit *edMapWidth;
   TLabel *lbMapWidth;
   TLabel *lbMapHeight;
   TBitBtn *btnOk;
   TBitBtn *btnCancel;
   TComboBox *comboLandscape;
   TLabel *lbLandscape;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall btnOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TFormNewMap(TComponent* Owner);

   void buildLandscapeList();

};
//---------------------------------------------------------------------------
extern PACKAGE TFormNewMap *FormNewMap;
//---------------------------------------------------------------------------
#endif
