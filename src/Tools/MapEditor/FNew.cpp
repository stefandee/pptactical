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
#include <vcl.h>
#include <stdlib.h>
#pragma hdrstop

#include "FNew.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormNewMap *FormNewMap;
//---------------------------------------------------------------------------

#include "PPME_Defines.h"
#include "basepath.h"
#include "Paths.h"

/////////////////////////////////////////////////////////////////////////////
//PUBLIC AREA
/////////////////////////////////////////////////////////////////////////////
__fastcall TFormNewMap::TFormNewMap(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: construieste lista pentru Landscape list
---------------------------------------------------------------------------*/
void TFormNewMap::buildLandscapeList()
{
   TSearchRec srDesc;      //descrierea fisierului
   int iAttributes = faDirectory;
   bool bFirst = true;     //arata daca este prima citire
   bool bEnd = false;      //arata daca s-a terminat citirea

   comboLandscape->Items->Clear();

   while (!bEnd)
   {
      if (bFirst)
      {
         bEnd = !(FindFirst(AnsiString(getGamePath().c_str()) + PATH_LANDSCAPE + "*.*", iAttributes, srDesc) == 0);
         bFirst = false;
      }
      else
         bEnd = !(FindNext(srDesc) == 0);
      if (!bEnd && srDesc.Name != "." && srDesc.Name != "..")
         comboLandscape->Items->Add(srDesc.Name);
   }
}

/////////////////////////////////////////////////////////////////////////////
//PUBLISHED AREA
/////////////////////////////////////////////////////////////////////////////

/*---------------------------------------------------------------------------
 descrition: cand fac show reconstruiesc lista
---------------------------------------------------------------------------*/
void __fastcall TFormNewMap::FormShow(TObject *Sender)
{
   buildLandscapeList();
   comboLandscape->ItemIndex = 0;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: verificarile de rigoare
---------------------------------------------------------------------------*/
void __fastcall TFormNewMap::btnOkClick(TObject *Sender)
{
   int width = atoi(AnsiString(edMapWidth->Text).c_str());
   int height = atoi(AnsiString(edMapHeight->Text).c_str());
   if (width <= 40)
      edMapWidth->SetFocus();
   else if (height <= 40)
      edMapHeight->SetFocus();
   else
      ModalResult = mrOk;
}
//---------------------------------------------------------------------------



