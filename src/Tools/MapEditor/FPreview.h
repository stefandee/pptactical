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

 descrition : preview pentru obiectul transmis 
 last modify: 2 11 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef FPreviewH
#define FPreviewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>

#include "PPME_Defines.h"
#include "Tile.h"
#include "AIUnits.h"
#include "Statics.h"

//---------------------------------------------------------------------------
class TFormPreview : public TForm
{
__published:	// IDE-managed Components
   TStatusBar *statusPreview;
   TPopupMenu *popupPreview;
   TMenuItem *mitAuto;
   TMenuItem *mitUser;
   TScrollBox *scrollPreview;
   TImage *imgPreview;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormHide(TObject *Sender);
   void __fastcall mitUserClick(TObject *Sender);
   void __fastcall mitAutoClick(TObject *Sender);

private:	// User declarations

public:		// User declarations
   TMenuItem *itCheck;
   TTreeItem *selectedItem;
   struct _StaticObject staticObj;
   CAuxMapChunk* tileObj;
   TInfantryUnified recInf;
   TVehicleUnified recVeh;

   __fastcall TFormPreview(TComponent* Owner);

   void setSelectedItem(TTreeItem *selecteditem);
   bool readStatic();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormPreview *FormPreview;
//---------------------------------------------------------------------------
#endif
