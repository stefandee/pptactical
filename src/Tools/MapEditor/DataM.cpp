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

 descrition : folosit pentru o chestie mai eleganta a managerului de meniu
 last modify: 29 10 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DataM.h"
#include "FMain.h"
#include "FPreview.h"
#include "FTool.h"
#include "FRadar.h"
#include "FTree.h"
#include "FInfo.h"
#include "FAlt.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDatas *Datas;
//---------------------------------------------------------------------------
__fastcall TDatas::TDatas(TComponent* Owner)
   : TDataModule(Owner)
{
   FormPreview->itCheck = FormMain->itPreview;
   FormTool->itCheck    = FormMain->itTool;
   FormRadar->itCheck   = FormMain->itRadar;
   FormTree->itCheck    = FormMain->itTree;
   FormInfo->itCheck    = FormMain->itInfo;
   FormAltitude->itCheck= FormMain->itAltitude;
}
//---------------------------------------------------------------------------
 