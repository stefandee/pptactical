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

#include "FMissionScript.h"
#include "FMain.h"
#include <string.h>
#include <fcntl.h>
#include <io.h>
#include <sstream>
#include "ZMathLib.h"
#include "PPIG_Map.h"
#include "PPIG_Mission.h"
#include "PPIG_MissionControl.h"
#include "Paths.h"
#include "basepath.h"
#include "Stream.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ScriptPopupMenu"
#pragma resource "*.dfm"
TFormMissionScript *FormMissionScript;
//---------------------------------------------------------------------------

__fastcall TFormMissionScript::TFormMissionScript(TComponent* Owner)
        : TForm(Owner)
{
  mModuleName = "MissionControl";

  TScriptPopupMenu* lPopup = new TScriptPopupMenu(memoSrc);

  CPString scriptPath = getGamePath() + CPString(PATH_SCRIPTS_MISSION);
  lPopup->SetInitialDir(PP::Stream::FixPath(scriptPath.c_str()).c_str());

  lPopup->SetFilter("C Script Language files|*.csl|All files|*.*");
  lPopup->SetDefaultExt("csl");

  memoSrc->PopupMenu = lPopup;
}
//---------------------------------------------------------------------------

void __fastcall TFormMissionScript::FormShow(TObject *Sender)
{
  // must parse the missionScript for "\n" and add lines in memo accordingly
  memoSrc->Clear();
  memoErrors->Clear();

  if (GetMissionInstance()->GetMissionControl()->GetScriptObserver())
  {
    StringToMemo(GetMissionInstance()->GetMissionControl()->GetScriptObserver()->GetScript().c_str());
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMissionScript::btnOkClick(TObject *Sender)
{
  AnsiString lStr = "";

  for(int i = 0; i < memoSrc->Lines->Count; i++)
  {
    lStr += memoSrc->Lines->Strings[i] + "\n";
  }

  // OMG!!! :P
  GetMissionInstance()->GetMissionControl()->SetScriptObserver(GetMissionInstance()->GetDoctrine()->AddScript(GetMissionInstance()->GetMissionControl()->GetScriptObserver(), CPString(lStr.c_str())));
}
//---------------------------------------------------------------------------

void TFormMissionScript::addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback)
{
  mCompiler->addFunc(mModuleName, szMemberName, callback);
}
//---------------------------------------------------------------------------

void __fastcall TFormMissionScript::btnCompileClick(TObject *Sender)
{
  // compile the script, put the errors on a memo
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

    // add insertion functions
    addModuleFunc("InsertInfantryAtPoint(const x, const y, const bgId, const index, const scriptFile)", mHandleDummy);
    addModuleFunc("InsertVehicleAtPoint(const x, const y, const bgId, const index, const scriptFile)",  mHandleDummy);
    addModuleFunc("InsertAircraftAtPoint(const x, const y, const bgId, const index, const scriptFile)", mHandleDummy);

    // add modifications functions

    // add deletion functions
    addModuleFunc("KillUnit(const unitId)",   mHandleDummy);
    addModuleFunc("RemoveUnit(const unitId)", mHandleDummy);

    // add battlegroup functions
    addModuleFunc("GetBattleGroupCount()",                                    mHandleDummy);
    addModuleFunc("GetBattleGroup(const index)",                              mHandleDummy);
    addModuleFunc("GetBattleGroupUnitCount(const bgId)",                      mHandleDummy);
    addModuleFunc("GetBattleGroupUnit(const bgId, const index)",              mHandleDummy);
    addModuleFunc("IsUnitInBattleGroup(const bgId, const id)",                mHandleDummy);
    addModuleFunc("ChangePolitics(const bgId1, const bgId2, const relation)", mHandleDummy);
    addModuleFunc("GetPolitics(const bgId1, const bgId2)",                    mHandleDummy);
    addModuleFunc("CreateBattleGroup(const name, const info)",                mHandleDummy);
    addModuleFunc("RemoveBattleGroup(const bgId)",                            mHandleDummy);
    addModuleFunc("RelinquishCommand(const bgIdReceiver, const bgIdAborter)", mHandleDummy);

    // add map functions
    addModuleFunc("GetWidth()",                                                     mHandleDummy);
    addModuleFunc("GetHeight()",                                                    mHandleDummy);
    addModuleFunc("GetUnitAtPoint(const x, const y)",                               mHandleDummy);
    addModuleFunc("GetAltitude(const x, const y)",                                  mHandleDummy);
    addModuleFunc("SetAltitude(const x, const y, const value)",                     mHandleDummy);
    addModuleFunc("GetTileType(const x, const y)",                                  mHandleDummy);
    addModuleFunc("GetVisibility(const bgId, const x, const y)",                    mHandleDummy);
    addModuleFunc("CanBePlaced(const x, const y)",                                  mHandleDummy);
    addModuleFunc("NotifyVisibilityMap(const x, const y, const bgId, const value)", mHandleDummy);
    addModuleFunc("NotifyCircleVisibilityMap(const x, const y, const radius, const bgId, const value)", mHandleDummy);
    addModuleFunc("MoveCamera(const x, const y)",                                   mHandleDummy);

    // add unit functions
    addModuleFunc("ChangeUnitBattleGroup(const unitId, const newBg)", mHandleDummy);
    addModuleFunc("IsUnitDead(const id)",                             mHandleDummy);
    addModuleFunc("UnitExists(const id)",                             mHandleDummy);
    addModuleFunc("GetUnitLogicX(const id)",                          mHandleDummy);
    addModuleFunc("GetUnitLogicY(const id)",                          mHandleDummy);
    addModuleFunc("GetUnitZ(const id)",                               mHandleDummy);
    addModuleFunc("GetUnitHP(const id)",                              mHandleDummy);
    addModuleFunc("GetUnitLogicDimX(const id)",                       mHandleDummy);
    addModuleFunc("GetUnitLogicDimY(const id)",                       mHandleDummy);
    addModuleFunc("GetUnitType(const id)",                            mHandleDummy);
    addModuleFunc("ChangeUnitScript(const id, const scriptFile)",     mHandleDummy);
    addModuleFunc("GetUnitBattleGroup(const id)",                     mHandleDummy);

    // add transporter functions
    addModuleFunc("UnitsTransportedCount(const transportId)",           mHandleDummy);
    addModuleFunc("GetUnitTransported(const transportId, const index)", mHandleDummy);
    addModuleFunc("CanCarryInfantry(const id)",                         mHandleDummy);
    addModuleFunc("CanCarryVehicles(const id)",                         mHandleDummy);

    // add unit orders functions
    addModuleFunc("Move(const id, const x, const y)", mHandleDummy);
    addModuleFunc("Attack(const id, const victimId)", mHandleDummy);
    addModuleFunc("AttackGround(const id, const x, const y)", mHandleDummy);
    addModuleFunc("Stop(const id)", mHandleDummy);
    addModuleFunc("Load(const id, const transportId)", mHandleDummy);
    addModuleFunc("UnloadAll(const transportId, const x, const y)", mHandleDummy);
    addModuleFunc("Patrol(const id, const x, const y)", mHandleDummy);
    addModuleFunc("Land(const id, const x, const y)", mHandleDummy);

    // add misc functions
    addModuleFunc("MissionTime()", mHandleDummy);
    addModuleFunc("PostMessage(const msg)", mHandleDummy);
    addModuleFunc("EndMission()", mHandleDummy);
    addModuleFunc("Pause(const state)", mHandleDummy);
    addModuleFunc("MessageConsole(const state)", mHandleDummy);

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

ZString TFormMissionScript::mcDummy(ZCsl* csl)
{
  return "0";
}
//---------------------------------------------------------------------------

void TFormMissionScript::StringToMemo(const char* _str)
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
