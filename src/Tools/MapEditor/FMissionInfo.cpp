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

#include <vcl.h>
#pragma hdrstop

#include "FMissionInfo.h"
#include "FMain.h"
#include "PPIG_Map.h"
#include "PPIG_Mission.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMissionInfo *FormMissionInfo;
//---------------------------------------------------------------------------
__fastcall TFormMissionInfo::TFormMissionInfo(TComponent* Owner)
        : TForm(Owner)
{
  mBkName = "";
  mBkDesc = "";
}
//---------------------------------------------------------------------------

void __fastcall TFormMissionInfo::FormShow(TObject *Sender)
{
  edName->Text   = GetMissionInstance()->GetMissionName().c_str();
  memoDesc->Text = GetMissionInstance()->GetMissionInfo().c_str();
}
//---------------------------------------------------------------------------

void __fastcall TFormMissionInfo::btnOkClick(TObject *Sender)
{
  GetMissionInstance()->SetMissionName(System::UTF8String(edName->Text).c_str());
  GetMissionInstance()->SetMissionInfo(System::UTF8String(memoDesc->Text).c_str());
}
//---------------------------------------------------------------------------

