//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2004 Stefan Dicu & Tudor Girba                         //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License //
// as published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public    //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <sstream>
#include "FBGEdit.h"
#include "FMain.h"
#include "ZMathLib.h"
#include <string.h>
#include <fcntl.h>
#include <io.h>

#include "PPIG_Doctrine.h"
#include "PPIG_BattleGroup.h"
#include "PPIG_Map.h"
#include "PPIG_Mission.h"
#include "PPIG_BGManager.h"

#include "basepath.h"
#include "Paths.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ScriptPopupMenu"
#pragma resource "*.dfm"
TFormBGEdit *FormBGEdit;
//---------------------------------------------------------------------------

__fastcall TFormBGEdit::TFormBGEdit(TComponent* Owner)
        : TForm(Owner)
{
  mCurrentBg        = 0; // absolute value adds +2, because 0 and 1 index battle groups are reserved and cannot be edited
  gbData->Enabled   = false;
  gbScript->Enabled = false;

  // compiler
  mModuleName     = "BattleGroup";
  //mHandleDummy    = mcDummy;
  TScriptPopupMenu* lPopup = new TScriptPopupMenu(memoSrc);

  lPopup->SetInitialDir(AnsiString(getGamePath().c_str()) + AnsiString(PATH_SCRIPTS_BATTLEGROUP));
  lPopup->SetFilter("C Script Language files|*.csl|All files|*.*");
  lPopup->SetDefaultExt("csl");

  memoSrc->PopupMenu = lPopup;
}
//---------------------------------------------------------------------------

void __fastcall TFormBGEdit::FormShow(TObject *Sender)
{
  mCurrentBg = 0;
  ShowCurrentBGroup();
}
//---------------------------------------------------------------------------

void TFormBGEdit::ShowCurrentBGroup()
{
  CPPIG_BattleGroup* lBg;

  if (GetMissionInstance()->GetBGManager()->BGroupCount() > 2)
  {
    gbData->Enabled   = true;
    gbScript->Enabled = true;

    lBg = GetMissionInstance()->GetBGManager()->GetBGroupByIndex(mCurrentBg + 2);

    edName->Text   = lBg->GetName().c_str();
    edId->Text     = lBg->GetId();
    //memoDesc->Text = lBg->GetDescription().c_str();

    memoSrc->Clear();
    memoErrors->Clear();

    if (lBg->GetScriptObserver())
    {
      ScriptToMemo(lBg->GetScriptObserver()->GetScript().c_str()); // :) those interfaces!
    }
    else
    {
      memoSrc->Text = "";
    }
  }
  else
  {
    gbData->Enabled   = false;
    gbScript->Enabled = false;

    edName->Text     = "";
    edId->Text       = "";
    memoDesc->Text   = "";
    memoSrc->Text    = "";
    memoErrors->Text = "";
  }

  UpdateIndex();
}
//---------------------------------------------------------------------------

void TFormBGEdit::ScriptToMemo(const char* _str)
{
  AnsiString lStrLine = "";

  for(int i = 0; i < (int)strlen(_str); i++)
  {
    if (_str[i] != '\n')
    {
      // and ignore 10
      if (_str[i] != 10)
      {
        lStrLine += _str[i];
      }
    }
    else
    {
      memoSrc->Lines->Add(lStrLine);
      lStrLine = "";
    }
  }

  if (lStrLine != "")
  {
    memoSrc->Lines->Add(lStrLine);
  }  
}
//---------------------------------------------------------------------------

void __fastcall TFormBGEdit::edNameExit(TObject *Sender)
{
  // modify bg name
  CPPIG_BattleGroup* lBg = GetMissionInstance()->GetBGManager()->GetBGroupByIndex(mCurrentBg + 2);

  lBg->SetName(CPString(System::UTF8String(edName->Text).c_str()));
}
//---------------------------------------------------------------------------

void __fastcall TFormBGEdit::edIdExit(TObject *Sender)
{
  CPPIG_BattleGroup* lBg = GetMissionInstance()->GetBGManager()->GetBGroupByIndex(mCurrentBg + 2);

  // verify if the id is a number
  int lId = 0;
  try
  {
    lId = edId->Text.ToInt();
  }
  catch(const EConvertError &e)
  {
    lId = 0;
    edId->Text = "0";
  }

  // verify if the new id is unique
  if (!IdUnique(lId))
  {
    lId = GenerateUniqueId();
  }

  // change id in mission politics
  // GetMissionInstance()->GetBGManager()->ChangeId(lBg->GetId(), lId);

  // change bgroup id
  lBg->SetId(lId);

  // change script name
  /*
  if (lBg->GetScript())
  {
    lBg->GetScript()->SetName(CPString(lBg->GetName()) + CPString(lBg->GetId()));
  }
  */ 
}
//---------------------------------------------------------------------------

void __fastcall TFormBGEdit::memoDescExit(TObject *Sender)
{
  // modify bg description
  CPPIG_BattleGroup* lBg = GetMissionInstance()->GetBGManager()->GetBGroupByIndex(mCurrentBg + 2);

  lBg->SetDescription(CPString(System::UTF8String(memoDesc->Text).c_str()));
}
//---------------------------------------------------------------------------

void __fastcall TFormBGEdit::memoSrcExit(TObject *Sender)
{
  // modify bg script
  CPPIG_BattleGroup* lBg = GetMissionInstance()->GetBGManager()->GetBGroupByIndex(mCurrentBg + 2);
  AnsiString lStr = "";

  for(int i = 0; i < memoSrc->Lines->Count; i++)
  {
    lStr += memoSrc->Lines->Strings[i] + "\n";
  }

  // welcome to Obfuscated C++
  lBg->SetScriptObserver( GetMissionInstance()->GetDoctrine()->AddScript(lBg->GetScriptObserver(), CPString(lStr.c_str()) ) );
}
//---------------------------------------------------------------------------

bool TFormBGEdit::IdUnique(int _id)
{
  for(int i = 0; i < GetMissionInstance()->GetBGManager()->BGroupCount(); i++)
  {
    if (GetMissionInstance()->GetBGManager()->GetBGroupByIndex(i)->GetId() == _id)
    {
      return false;
    }
  }

  return true;
}
//---------------------------------------------------------------------------

int TFormBGEdit::GenerateUniqueId()
{
  int lId;

  do
  {
    lId = random(MaxInt);
  }
  while(!IdUnique(lId));

  return lId;
}
//---------------------------------------------------------------------------

void TFormBGEdit::UpdateIndex()
{
  // the representation of progress must be 1 based, not 0 based
  // the maximum number must be the bg count, so that editing is intuitive
  if (GetMissionInstance()->GetBGManager()->BGroupCount() > 2)
  {
    lProgress->Caption = AnsiString(mCurrentBg + 1) + "/" + AnsiString(GetMissionInstance()->GetBGManager()->BGroupCount() - 2);
  }
  else
  {
    lProgress->Caption = "0/0";
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormBGEdit::btnPrevClick(TObject *Sender)
{
  mCurrentBg--;

  if (mCurrentBg < 0)
  {
    mCurrentBg = GetMissionInstance()->GetBGManager()->BGroupCount() - 3; // -2(reserved) and -1
  }

  ShowCurrentBGroup();
}
//---------------------------------------------------------------------------

void __fastcall TFormBGEdit::btnNextClick(TObject *Sender)
{
  mCurrentBg++;

  if (mCurrentBg > GetMissionInstance()->GetBGManager()->BGroupCount() - 3)
  {
    mCurrentBg = 0;
  }

  ShowCurrentBGroup();
}
//---------------------------------------------------------------------------

void __fastcall TFormBGEdit::btnAddClick(TObject *Sender)
{
  // create and add a new battle group
  CPPIG_BattleGroup* lBg = new CPPIG_BattleGroup();

  GetMissionInstance()->GetBGManager()->AddBGroup(lBg);

  if (!IdUnique(lBg->GetId()))
  {
    lBg->SetId(GenerateUniqueId());
  }

  // then set the index to point to it
  mCurrentBg = GetMissionInstance()->GetBGManager()->BGroupCount() - 3;

  // this kinda sux - update Politics
  // FormMain->theMission->mPolitics->AddBattleGroup(lBg->GetId());

  ShowCurrentBGroup();
}
//---------------------------------------------------------------------------

void __fastcall TFormBGEdit::btnDeleteClick(TObject *Sender)
{
  if (GetMissionInstance()->GetBGManager()->BGroupCount() <= 2)
  {
    // nothing to delete
    return;
  }

  CPPIG_BattleGroup* lBg = GetMissionInstance()->GetBGManager()->GetBGroupByIndex(mCurrentBg + 2);

  // remove battle group script observer
  GetMissionInstance()->GetDoctrine()->RemoveScriptObserver(lBg->GetScriptObserver());

  // remove the battlegroup now
  GetMissionInstance()->GetBGManager()->RemoveBGroup(lBg);

  mCurrentBg = 0;

  ShowCurrentBGroup();
}
//---------------------------------------------------------------------------

void TFormBGEdit::addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback)
{
  mCompiler->addFunc(mModuleName, szMemberName, callback);
}
//---------------------------------------------------------------------------

void __fastcall TFormBGEdit::btnCompileClick(TObject *Sender)
{
  CPPIG_BattleGroup* lBg = GetMissionInstance()->GetBGManager()->GetBGroupByIndex(mCurrentBg + 2);

  if (!lBg->GetScriptObserver())
  {
    memoErrors->Clear();
    memoErrors->Lines->Add("Nothing to compile.");
    return;
  }

  // do some compile
  // compile the script, put the errors on a memo
  //int ret = 0;
  bool lCslOk;
  AnsiString lStr = "";

  memoErrors->Clear();

  try
  {
    // new'g a compile object
    mCompiler = new ZCsl(ZCsl::disAll);
    lCslOk    = true;

    // load math library
    ZMathLib* lMathLib = new ZMathLib(mCompiler, "Math Library");
    lMathLib->InitLibrary();
    mCompiler->loadLibrary(lMathLib);

    // add functions
    addModuleFunc("MissionTime()", mHandleDummy);
    addModuleFunc("PostMessage(const msg)", mHandleDummy);

    addModuleFunc("UnitsCount()", mHandleDummy);
    addModuleFunc("UnitStatus(const index)",                mHandleDummy       );
    addModuleFunc("Move(const index, const x, const y)",    mHandleDummy       );
    addModuleFunc("UnitX(const index)",                     mHandleDummy       );
    addModuleFunc("UnitY(const index)",                     mHandleDummy       );
    addModuleFunc("UnitSense(const index)",                 mHandleDummy       );

    addModuleFunc("AttackRandomEnemy()", mHandleDummy);
    addModuleFunc("SeekEnemies()", mHandleDummy);
    addModuleFunc("SetTargets(const no)", mHandleDummy);
    addModuleFunc("ToAttackCount()", mHandleDummy);
    addModuleFunc("DirFromPoints(const x1, const y1, const x2, const y2)", mHandleDummy);

    // arrange the script source
    for(int i = 0; i < memoSrc->Lines->Count; i++)
    {
      lStr += memoSrc->Lines->Strings[i] + "\n";
    }

	std::stringstream str(lStr.c_str());

    // compile
    mCompiler->loadScript(mModuleName, &str);

    // delete compile object
    delete mCompiler;
  }
  catch(const ZException& err)
  {
    for (int i = 0; i < err.count(); i++)
    {
      memoErrors->Lines->Add((char*)err[i]);
    }

    if (lCslOk)
    {
      delete mCompiler;
    }

    return;
  }

  memoErrors->Lines->Add("Compile Ok.");
}
//---------------------------------------------------------------------------

ZString TFormBGEdit::mcDummy(ZCsl* csl)
{
  return "0";
}
//---------------------------------------------------------------------------


