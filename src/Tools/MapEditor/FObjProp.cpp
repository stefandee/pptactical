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

 description : form objects manager
 last modify: 26 12 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <string.h>
#include <fcntl.h>
#include <io.h>
#include <sstream>
#include "FObjProp.h"
#include "FMain.h"
#include "PPME_Defines.h"
#include "FMain.h"
#include "PPIG_Doctrine.h"
#include "ZMathLib.h"
#include "WeaponRulesLib.h"
#include "PPIG_Mission.h"
#include "PPIG_ObjStatic.h"
#include "PPIG_ObjInfantry.h"
#include "PPIG_ObjAircraft.h"
#include "PPIG_ObjVehicle.h"
#include "Tools.h"
#include "FBayManager.h"
#include "Stream.h"
#include "basepath.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma link "ScriptPopupMenu"
#pragma resource "*.dfm"
TFormObjManager *FormObjManager;
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PUBLIC AREA
/////////////////////////////////////////////////////////////////////////////

__fastcall TFormObjManager::TFormObjManager(TComponent* Owner)
   : TForm(Owner)
{
  objList = new CIGBaseObjectList;

  //
  // dynamic assignments of script popup
  //
  TScriptPopupMenu* lPopup = new TScriptPopupMenu(memoVSrc);

  CPString scriptPath = getGamePath() + CPString(PATH_SCRIPTS_VEHICLE);
  lPopup->SetInitialDir(PP::Stream::FixPath(scriptPath.c_str()).c_str());
  lPopup->SetFilter("C Script Language files|*.csl|All files|*.*");
  lPopup->SetDefaultExt("csl");

  memoVSrc->PopupMenu   = lPopup;

  lPopup = new TScriptPopupMenu(memoInfSrc);

  scriptPath = getGamePath() + CPString(PATH_SCRIPTS_INFANTRY);
  lPopup->SetInitialDir(PP::Stream::FixPath(scriptPath.c_str()).c_str());
  lPopup->SetFilter("C Script Language files|*.csl|All files|*.*");
  lPopup->SetDefaultExt("csl");

  memoInfSrc->PopupMenu = lPopup;

  lPopup = new TScriptPopupMenu(memoAirSrc);

  scriptPath = getGamePath() + CPString(PATH_SCRIPTS_AIRCRAFT);
  lPopup->SetInitialDir(PP::Stream::FixPath(scriptPath.c_str()).c_str());
  lPopup->SetFilter("C Script Language files|*.csl|All files|*.*");
  lPopup->SetDefaultExt("csl");

  memoAirSrc->PopupMenu = lPopup;

  lPopup = new TScriptPopupMenu(memoStaticSrc);

  scriptPath = getGamePath() + CPString(PATH_SCRIPTS_STATICS);
  lPopup->SetInitialDir(PP::Stream::FixPath(scriptPath.c_str()).c_str());
  lPopup->SetFilter("C Script Language files|*.csl|All files|*.*");
  lPopup->SetDefaultExt("csl");

  memoStaticSrc->PopupMenu = lPopup;
}
//---------------------------------------------------------------------------

void TFormObjManager::setList()
{
   objList->RemoveAll();

   for(int i = 0; i < GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS)->Count(); i++)
   {
     objList->Add(GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS)->GetObject(i));
   }

   for(int i = 0; i < GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDOTHERS)->Count(); i++)
   {
     objList->Add(GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDOTHERS)->GetObject(i));
   }

   for(int i = 0; i < GetMissionInstance()->GetObjects(MOL_SELECTEDOTHERS)->Count(); i++)
   {
     objList->Add(GetMissionInstance()->GetObjects(MOL_SELECTEDOTHERS)->GetObject(i));
   }

   updownObjects->Min      = 0;
   updownObjects->Max      = (short)(objList->Count() - 1);
   updownObjects->Position = (short)currentObj;
   if (objList->Count() > 0)
      setCurrentObject(0);
}
//---------------------------------------------------------------------------

void TFormObjManager::setCurrentObject(int currentobj)
{
   lbObject->Caption = AnsiString(currentobj + 1) + "/" +
                       AnsiString(objList->Count());
                       
   currentObj = currentobj;

   sheetStatic->TabVisible  = false;
   sheetUnit->TabVisible    = false;
   sheetVehicle->TabVisible = false;
   sheetAir->TabVisible     = false;

   if (objList->GetObject(currentObj)->ClassName() == CPString(IGCN_STATICOBJECT))
   {
     setStaticData();
     sheetStatic->TabVisible = true;
   }

   if (objList->GetObject(currentObj)->ClassName() == CPString(IGCN_INFANTRYUNIT))
   {
     setInfData();
     sheetUnit->TabVisible = true;
   }

   if (objList->GetObject(currentObj)->IsDerivedFrom(IGCN_VEHICLEUNIT))
   {
     setVData();
     sheetVehicle->TabVisible = true;
   }

   if (objList->GetObject(currentObj)->IsDerivedFrom(IGCN_AIRUNIT))
   {
     setAirData();
     sheetAir->TabVisible = true;
   }

   if (btnCenterView->Down)
   {
      FormMain->centerViewPort(
        objList->GetObject(currentObj)->GetCoord().x,
        objList->GetObject(currentObj)->GetCoord().y
      );
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: pune din theStruct pe ecran
---------------------------------------------------------------------------*/
void TFormObjManager::setStaticData()
{
   struct _StaticObject lStatic = ((CIGStaticObject *)objList->GetObject(currentObj))->GetStaticData();

   lbStaticName->Caption      = lStatic.objName;
   memoStaticStory->Text      = lStatic.objDescription;
   lbStaticHP->Caption        = lStatic.hp;
   editStaticCurrentHP->Text  = ((CIGStaticObject *)objList->GetObject(currentObj))->GetCurrentHP();
   edStaticId->Text           = objList->GetObject(currentObj)->GetId();

   // script to memo
   memoStaticSrc->Clear();
   memoStaticErrors->Clear();

   if (((CIGStaticObject *)objList->GetObject(currentObj))->GetScriptObserver())
   {
     ScriptToMemo(memoStaticSrc, ((CIGStaticObject *)objList->GetObject(currentObj))->GetScriptObserver()->GetScript().c_str());
   }
   else
   {
     memoStaticSrc->Text = "";
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: pune din recInf pe ecran
---------------------------------------------------------------------------*/
void TFormObjManager::setInfData()
{
   lbInfName->Caption = "";
   if (((CIGInfantryUnit *)objList->GetObject(currentObj))->GetInfUnifiedData().type == 0)
   {
      lbInfName->Caption = AnsiString(((CIGInfantryUnit *)objList->GetObject(currentObj))->GetInfUnifiedData().name) + " " +
                           ((CIGInfantryUnit *)objList->GetObject(currentObj))->GetInfUnifiedData().surname;
   }
   else
   {
      lbInfName->Caption = ((CIGInfantryUnit *)objList->GetObject(currentObj))->GetInfUnifiedData().kind;
   }

   // infantry data
   //memoInfStory->Text = ((CInfantryUnit *)objList->List[currentObj])->recInf->description;
   edInfHp->Text      = ((CIGInfantryUnit *)(objList->GetObject(currentObj)))->GetCurrentHP();
   lbInfHP->Caption   = ((CIGInfantryUnit *)objList->GetObject(currentObj))->GetInfUnifiedData().hp;
   edInfId->Text      = objList->GetObject(currentObj)->GetId();
   edInfBodyAngle->Value = ((CIGInfantryUnit *)objList->GetObject(currentObj))->GetSenseCounter();
   edInfScanAngle->Value = ((CIGInfantryUnit *)objList->GetObject(currentObj))->GetScanAngle();

   // script to memo now
   memoInfSrc->Clear();
   memoInfErrors->Clear();

   if (((CIGInfantryUnit *)objList->GetObject(currentObj))->GetScriptObserver())
   {
     ScriptToMemo(memoInfSrc, ((CIGInfantryUnit *)objList->GetObject(currentObj))->GetScriptObserver()->GetScript().c_str());
   }
   else
   {
     memoInfSrc->Text = "";
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: pune din recV pe ecran
---------------------------------------------------------------------------*/
void TFormObjManager::setVData()
{
   lbVName->Caption = "";
   lbVName->Caption = AnsiString(((CIGVehicle *)objList->GetObject(currentObj))->GetVehicleUnifiedData().name) + "/" +
                        ((CIGVehicle *)objList->GetObject(currentObj))->GetVehicleUnifiedData().callsign + "(" +
                        GetVehicleType(((CIGVehicle *)objList->GetObject(currentObj))->GetVehicleUnifiedData().vType) + ")";

   // vehicle data
   //memoInfStory->Text = ((CInfantryUnit *)objList->List[currentObj])->recInf->description;
   edVHp->Text           = ((CIGVehicle *)(objList->GetObject(currentObj)))->GetCurrentHP();
   lbVHp->Caption        = ((CIGVehicle *)objList->GetObject(currentObj))->GetVehicleUnifiedData().hp;
   edVId->Text           = objList->GetObject(currentObj)->GetId();
   
   edVBodyAngle->Value   = ((CIGVehicle *)objList->GetObject(currentObj))->GetBodySenseCounter();
   edVTurretAngle->Value = ((CIGVehicle *)objList->GetObject(currentObj))->GetTurretSenseCounter();
   edVScanAngle->Value   = ((CIGVehicle *)objList->GetObject(currentObj))->GetScanAngle();

   // script to memo now
   memoVSrc->Clear();
   memoVErrors->Clear();

   if (((CIGVehicle *)objList->GetObject(currentObj))->GetScriptObserver())
   {
     ScriptToMemo(memoVSrc, ((CIGVehicle*)objList->GetObject(currentObj))->GetScriptObserver()->GetScript().c_str());
   }
   else
   {
     memoVSrc->Text = "";
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: puts the aircraft informations in the controls
---------------------------------------------------------------------------*/
void TFormObjManager::setAirData()
{
   lbAirName->Caption = "";
   lbAirName->Caption = AnsiString(((CIGAircraft *)objList->GetObject(currentObj))->GetAircraftUnifiedData().name) + "/" +
                        ((CIGAircraft *)objList->GetObject(currentObj))->GetAircraftUnifiedData().callsign + "(" +
                        GetVehicleType(((CIGAircraft *)objList->GetObject(currentObj))->GetAircraftUnifiedData().vType) + ")";

   // vehicle data
   edAirHp->Text           = ((CIGAircraft *)(objList->GetObject(currentObj)))->GetCurrentHP();
   lbAirHp->Caption        = ((CIGAircraft *)objList->GetObject(currentObj))->GetAircraftUnifiedData().hp;
   edAirId->Text           = objList->GetObject(currentObj)->GetId();
   
   // TODO: sense counter for aircraft?
   //edAirBodyAngle->Value   = ((CIGAircraft *)objList->GetObject(currentObj))->GetSenseCounter();
   edAirAltitude->Value    = ((CIGAircraft *)objList->GetObject(currentObj))->GetCoordZ();

   // script to memo now
   memoAirSrc->Clear();
   memoAirErrors->Clear();

   if (((CIGAircraft *)objList->GetObject(currentObj))->GetScriptObserver())
   {
     ScriptToMemo(memoAirSrc, ((CIGAircraft*)objList->GetObject(currentObj))->GetScriptObserver()->GetScript().c_str());
   }
   else
   {
     memoAirSrc->Text = "";
   }
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PUBLISHED AREA
/////////////////////////////////////////////////////////////////////////////
void TFormObjManager::componentUpdate()
{
   objList->RemoveAll();

   for(int i = 0; i < GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS)->Count(); i++)
   {
     objList->Add(GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS)->GetObject(i));
   }

   for(int i = 0; i < GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDOTHERS)->Count(); i++)
   {
     objList->Add(GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDOTHERS)->GetObject(i));
   }

   for(int i = 0; i < GetMissionInstance()->GetObjects(MOL_SELECTEDOTHERS)->Count(); i++)
   {
     objList->Add(GetMissionInstance()->GetObjects(MOL_SELECTEDOTHERS)->GetObject(i));
   }

   if (objList->Count() > 0)
   {
     updownObjects->Enabled = true;
     updownObjects->Min      = 0;
     updownObjects->Max      = objList->Count() - 1;
     updownObjects->Position = 0;

     if (objList->Count() > 0)
     {
        setCurrentObject(0);
     }
   }
   else
   {
     updownObjects->Enabled    = false;
     sheetStatic->TabVisible   = false;
     sheetUnit->TabVisible     = false;
     sheetVehicle->TabVisible  = false;
     sheetAir->TabVisible      = false;
     lbObject->Caption         = "n/a";
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::updownObjectsChanging(TObject *Sender,
      bool &AllowChange)
{
   setCurrentObject(updownObjects->Position);
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::FormShow(TObject *Sender)
{
  componentUpdate();
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::FormActivate(TObject *Sender)
{
  componentUpdate();
}
//---------------------------------------------------------------------------

void TFormObjManager::ScriptToMemo(TMemo* _memo, const char* _str)
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
      _memo->Lines->Add(lStrLine);
      lStrLine = "";
    }
  }

  if (lStrLine != "")
  {
    _memo->Lines->Add(lStrLine);
  }  
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// VALUE EDIT
/////////////////////////////////////////////////////////////////////////////
void __fastcall TFormObjManager::edInfHpExit(TObject *Sender)
{
  // verify if the hp is a number
  int lHp = 0;
  try
  {
    lHp = edInfHp->Text.ToInt();
  }
  catch(const EConvertError &e)
  {
    lHp = 0;
    edInfHp->Text = "0";
  }

  if ((lHp > ((CIGInfantryUnit *)objList->GetObject(currentObj))->GetInfUnifiedData().hp) ||
     (lHp < 0))
  {
    lHp = ((CIGInfantryUnit *)objList->GetObject(currentObj))->GetInfUnifiedData().hp;
    edInfHp->Text = lHp;
  }

  ((CIGInfantryUnit *)objList->GetObject(currentObj))->SetCurrentHP(lHp);
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::edInfIdExit(TObject *Sender)
{
  CIGInfantryUnit * lUnit = (CIGInfantryUnit *)objList->GetObject(currentObj);

  // verify if the id is a number
  int lId = 0;
  try
  {
    lId = edInfId->Text.ToInt();
  }
  catch(const EConvertError &e)
  {
    lId = 0;
    edInfId->Text = "0";
  }

  // verify if the id is unique
  if (!GetMissionInstance()->UnitIdUnique(lId))
  {
    lId = GetMissionInstance()->GetUniqueUnitId();
    edInfId->Text = lId;
  }

  lUnit->SetId(lId);

  // now change the script name
  /*
  if (lUnit->GetScript())
  {
    lUnit->GetScript()->SetName(CPString("Infantry") + CPString(lUnit->mId));
  }
  */
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::memoInfSrcExit(TObject *Sender)
{
  // modify unit script
  CIGInfantryUnit* lUnit = ((CIGInfantryUnit *)objList->GetObject(currentObj));
  AnsiString lStr = "";

  for(int i = 0; i < memoInfSrc->Lines->Count; i++)
  {
    lStr += memoInfSrc->Lines->Strings[i] + "\n";
  }

  // long lines, the pleasure of my life
  lUnit->SetScriptObserver( GetMissionInstance()->GetDoctrine()->AddScript(lUnit->GetScriptObserver(), CPString(lStr.c_str()) ) );

  //FormMain->theMission->mDoctrine->SetScript(CPString("Infantry") + CPString(lUnit->mId), CPString(lStr.c_str()));
  //lUnit->SetScript(FormMain->theMission->mDoctrine->GetScriptByName(CPString("Infantry") + CPString(lUnit->mId)));
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::edVHpExit(TObject *Sender)
{
  // verify if the hp is a number
  int lHp = 0;
  try
  {
    lHp = edVHp->Text.ToInt();
  }
  catch(const EConvertError &e)
  {
    lHp = 0;
    edVHp->Text = "0";
  }

  if ((lHp > ((CIGVehicle *)objList->GetObject(currentObj))->GetVehicleUnifiedData().hp) ||
     (lHp < 0))
  {
    lHp = ((CIGVehicle* )objList->GetObject(currentObj))->GetVehicleUnifiedData().hp;
    edVHp->Text = lHp;
  }

  ((CIGVehicle* )objList->GetObject(currentObj))->SetCurrentHP(lHp);
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::edVIdExit(TObject *Sender)
{
  CIGVehicle * lUnit = (CIGVehicle *)objList->GetObject(currentObj);

  // verify if the id is a number
  int lId = 0;
  try
  {
    lId = edVId->Text.ToInt();
  }
  catch(const EConvertError &e)
  {
    lId = 0;
    edVId->Text = "0";
  }

  // verify if the id is unique
  if (!GetMissionInstance()->UnitIdUnique(lId))
  {
    lId = GetMissionInstance()->GetUniqueUnitId();
    edVId->Text = lId;
  }

  lUnit->SetId(lId);
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::memoVSrcExit(TObject *Sender)
{
  // modify unit script
  CIGVehicle* lUnit = ((CIGVehicle *)objList->GetObject(currentObj));
  AnsiString lStr = "";

  for(int i = 0; i < memoVSrc->Lines->Count; i++)
  {
    lStr += memoVSrc->Lines->Strings[i] + "\n";
  }

  // long lines, the pleasure of my life
  lUnit->SetScriptObserver( GetMissionInstance()->GetDoctrine()->AddScript(lUnit->GetScriptObserver(), CPString(lStr.c_str()) ) );

  //FormMain->theMission->mDoctrine->SetScript(CPString("Infantry") + CPString(lUnit->mId), CPString(lStr.c_str()));
  //lUnit->SetScript(FormMain->theMission->mDoctrine->GetScriptByName(CPString("Infantry") + CPString(lUnit->mId)));
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// SCRIPT
/////////////////////////////////////////////////////////////////////////////

ZString TFormObjManager::mcDummy(ZCsl* csl)
{
  return "0";
}
//---------------------------------------------------------------------------

void TFormObjManager::addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback)
{
  mCompiler->addFunc(mModuleName, szMemberName, callback);
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::btnInfCompileClick(TObject *Sender)
{
  CIGInfantryUnit* lUnit = ((CIGInfantryUnit*)objList->GetObject(currentObj));

  if (!lUnit->GetScriptObserver())
  {
    memoInfErrors->Clear();
    memoInfErrors->Lines->Add("Nothing to compile.");
    return;
  }

  // do some compile
  // compile the script, put the errors on a memo
  //int ret = 0;
  bool lCslOk;
  AnsiString lStr = "";

  memoInfErrors->Clear();

  try
  {
    // new'g a compile object
    mCompiler = new ZCsl(ZCsl::disAll);
    lCslOk    = true;

    // load math library
    ZMathLib* lMathLib = new ZMathLib(mCompiler, "Math Library");
    lMathLib->InitLibrary();
    mCompiler->loadLibrary(lMathLib);

    // load weapon rules library
    WeaponRulesLib* lWeaponRulesLib = new WeaponRulesLib(mCompiler, "Weapon Rules Library");
    lWeaponRulesLib->InitLibrary();
    mCompiler->loadLibrary(lWeaponRulesLib);
    
    // add functions
    addModuleFunc("PostMessage(const msg)", mHandleDummy);
    addModuleFunc("PlaySound(const no)", mHandleDummy);

    addModuleFunc("Move(const x, const y)", mHandleDummy);
    addModuleFunc("Stop()", mHandleDummy);
    addModuleFunc("MyX()", mHandleDummy);
    addModuleFunc("MyY()", mHandleDummy);
    addModuleFunc("Myz()", mHandleDummy);

    addModuleFunc("SetScanAngle(const index, const deg)", mHandleDummy);
    addModuleFunc("GetScanAngle(const index, )", mHandleDummy);
    addModuleFunc("LookTo(const index, const id)", mHandleDummy);
    addModuleFunc("LookAt(const index, const x, const y)", mHandleDummy);
    addModuleFunc("Sweeping(const index)", mHandleDummy);
    addModuleFunc("Moving()", mHandleDummy);

    addModuleFunc("Scan(const index)", mHandleDummy);
    addModuleFunc("UnitInViewCount(const index)", mHandleDummy);
    addModuleFunc("UnitInView(const index, const unitindex)", mHandleDummy);

    addModuleFunc("WeaponRange(const no)", mHandleDummy);
    addModuleFunc("WeaponDamage(const no, const dtype)", mHandleDummy);

    addModuleFunc("InVisual(const x, const y)", mHandleDummy    );
    addModuleFunc("UnitId(const x, const y)",   mHandleDummy      );
    addModuleFunc("UnitDist(const id)",         mHandleDummy    );
    addModuleFunc("UnitStatus(const id)",       mHandleDummy  );
    addModuleFunc("UnitThreat(const id)",       mHandleDummy  );
    addModuleFunc("UnitRelation(const id)",     mHandleDummy);
    addModuleFunc("UnitType(const id)",         mHandleDummy);
    addModuleFunc("UnitX(const id)",            mHandleDummy);
    addModuleFunc("UnitY(const id)",            mHandleDummy);
    addModuleFunc("UnitZ(const id)",            mHandleDummy);

    addModuleFunc("FireAt(const x, const y)", mHandleDummy);
    addModuleFunc("FireTo(const id)", mHandleDummy);

    addModuleFunc("NoiseType(const index)",  mHandleDummy);
    addModuleFunc("NoiseX(const index)",     mHandleDummy);
    addModuleFunc("NoiseY(const index)",     mHandleDummy);
    addModuleFunc("NoiseCount()",            mHandleDummy);
    addModuleFunc("NoisePurge()",            mHandleDummy);

    addModuleFunc("AttackerOf(const id, const index)",   mHandleDummy);
    addModuleFunc("AttackerOfCount(const id)",           mHandleDummy);

    addModuleFunc("MyHp()", mHandleDummy);
    addModuleFunc("MyId()", mHandleDummy);

    addModuleFunc("GetGroundType(const x, const y)",    mHandleDummy);
    addModuleFunc("GetMapVisibility(const x, const y)", mHandleDummy);
    // maneuvers
    /*
    mCompiler->addFunc(mModuleName, "TakeCover()", mHandleDummy);
    mCompiler->addFunc(mModuleName, "UnderFire()", mHandleDummy);
    mCompiler->addFunc(mModuleName, "EvasiveManeuvers()", mHandleDummy);
    mCompiler->addFunc(mModuleName, "Evade()", mHandleDummy);
    mCompiler->addFunc(mModuleName, "SelectTarget(const unitId)", mHandleDummy);
    mCompiler->addFunc(mModuleName, "EvaluateAttackChance()", mHandleDummy);
    mCompiler->addFunc(mModuleName, "Attack(const unitId)", mHandleDummy);
    mCompiler->addFunc(mModuleName, "SelectWeapon(const targetId)", mHandleDummy);
    mCompiler->addFunc(mModuleName, "SelectDevice(const targetId)", mHandleDummy);
    mCompiler->addFunc(mModuleName, "Hold()", mHandleDummy);
    mCompiler->addFunc(mModuleName, "Prone()", mHandleDummy);
    mCompiler->addFunc(mModuleName, "Crouch()", mHandleDummy);
    mCompiler->addFunc(mModuleName, "Stand()", mHandleDummy);
    */

    // arrange the script source
    for(int i = 0; i < memoInfSrc->Lines->Count; i++)
    {
      lStr += memoInfSrc->Lines->Strings[i] + "\n";
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
      memoInfErrors->Lines->Add((char*)err[i]);
    }

    if (lCslOk)
    {
      delete mCompiler;
    }

    return;
  }

  memoInfErrors->Lines->Add("Compile Ok.");
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::btnVCompileClick(TObject *Sender)
{
  CIGVehicle* lUnit = ((CIGVehicle*)objList->GetObject(currentObj));

  if (!lUnit->GetScriptObserver())
  {
    memoVErrors->Clear();
    memoVErrors->Lines->Add("Nothing to compile.");
    return;
  }

  // do some compile
  // compile the script, put the errors on a memo
  //int ret = 0;
  bool lCslOk;
  AnsiString lStr = "";

  memoVErrors->Clear();

  try
  {
    // new'g a compile object
    mCompiler = new ZCsl(ZCsl::disAll);
    lCslOk    = true;

    // load math library
    ZMathLib* lMathLib = new ZMathLib(mCompiler, "Math Library");
    lMathLib->InitLibrary();
    mCompiler->loadLibrary(lMathLib);

    // load weapon rules library
    WeaponRulesLib* lWeaponRulesLib = new WeaponRulesLib(mCompiler, "Weapon Rules Library");
    lWeaponRulesLib->InitLibrary();
    mCompiler->loadLibrary(lWeaponRulesLib);
    
    // add functions
    addModuleFunc("PostMessage(const msg)", mHandleDummy);
    addModuleFunc("PlaySound(const no)", mHandleDummy);

    addModuleFunc("Move(const x, const y)", mHandleDummy);
    addModuleFunc("Stop()", mHandleDummy);
    addModuleFunc("Unload(const x, const y, const unitIndex)", mHandleDummy);
    addModuleFunc("UnloadAll(const x, const y)", mHandleDummy);
    
    addModuleFunc("MyX()", mHandleDummy);
    addModuleFunc("MyY()", mHandleDummy);
    addModuleFunc("MyZ()", mHandleDummy);

    addModuleFunc("SetScanAngle(const deg)", mHandleDummy);
    addModuleFunc("GetScanAngle()", mHandleDummy);
    addModuleFunc("LookTo(const id)", mHandleDummy);
    addModuleFunc("LookAt(const x, const y)", mHandleDummy);
    addModuleFunc("Sweeping()", mHandleDummy);
    addModuleFunc("Moving()", mHandleDummy);

    addModuleFunc("GetTurretSense()", mHandleDummy);
    addModuleFunc("SetTurretSense(const angle)", mHandleDummy);

    addModuleFunc("Scan()", mHandleDummy);
    addModuleFunc("UnitInViewCount()", mHandleDummy);
    addModuleFunc("UnitInView(const index)", mHandleDummy);

    addModuleFunc("WeaponRange(const no)", mHandleDummy);
    addModuleFunc("WeaponDamage(const no, const dtype)", mHandleDummy);

    addModuleFunc("InVisual(const x, const y)", mHandleDummy);
    addModuleFunc("UnitId(const x, const y)",   mHandleDummy);
    addModuleFunc("UnitDist(const id)",         mHandleDummy);
    addModuleFunc("UnitStatus(const id)",       mHandleDummy);
    addModuleFunc("UnitThreat(const id)",       mHandleDummy);
    addModuleFunc("UnitRelation(const id)",     mHandleDummy);
    addModuleFunc("UnitType(const id)",         mHandleDummy);
    addModuleFunc("UnitX(const id)",            mHandleDummy);
    addModuleFunc("UnitY(const id)",            mHandleDummy);
    addModuleFunc("UnitZ(const id)",            mHandleDummy);

    addModuleFunc("FireAt(const x, const y)", mHandleDummy);
    addModuleFunc("FireTo(const id)", mHandleDummy);

    addModuleFunc("NoiseType(const index)",  mHandleDummy);
    addModuleFunc("NoiseX(const index)",     mHandleDummy);
    addModuleFunc("NoiseY(const index)",     mHandleDummy);
    addModuleFunc("NoiseCount()",            mHandleDummy);
    addModuleFunc("NoisePurge()",            mHandleDummy);

    addModuleFunc("AttackerOf(const id, const index)",   mHandleDummy);
    addModuleFunc("AttackerOfCount(const id)",           mHandleDummy);

    addModuleFunc("MyHp()", mHandleDummy);
    addModuleFunc("MyId()", mHandleDummy);

    addModuleFunc("GetGroundType(const x, const y)",    mHandleDummy);
    addModuleFunc("GetMapVisibility(const x, const y)", mHandleDummy);

    // arrange the script source
    for(int i = 0; i < memoVSrc->Lines->Count; i++)
    {
      lStr += memoVSrc->Lines->Strings[i] + "\n";
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
      memoVErrors->Lines->Add((char*)err[i]);
    }

    if (lCslOk)
    {
      delete mCompiler;
    }

    return;
  }

  memoVErrors->Lines->Add("Compile Ok.");
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::edInfScanAngleChange(TObject *Sender)
{
  CIGInfantryUnit* lUnit = ((CIGInfantryUnit*)objList->GetObject(currentObj));

  lUnit->SetScanAngle(edInfScanAngle->Value);
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::edInfBodyAngleChange(TObject *Sender)
{
  CIGInfantryUnit* lUnit = ((CIGInfantryUnit*)objList->GetObject(currentObj));

  lUnit->SetSenseCounter(edInfBodyAngle->Value);
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::edVBodyAngleChange(TObject *Sender)
{
  CIGVehicle* lUnit = ((CIGVehicle*)objList->GetObject(currentObj));

  lUnit->SetBodySenseCounter(edVBodyAngle->Value);
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::edVTurretAngleChange(TObject *Sender)
{
  CIGVehicle* lUnit = ((CIGVehicle*)objList->GetObject(currentObj));

  lUnit->SetTurretSenseCounter(edVTurretAngle->Value);
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::edVScanAngleChange(TObject *Sender)
{
  CIGVehicle* lUnit = ((CIGVehicle*)objList->GetObject(currentObj));

  lUnit->SetScanAngle(edVScanAngle->Value);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
//                                AIRCRAFTS
//
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::edAirAltitudeChange(TObject *Sender)
{
  CIGAircraft* lUnit = ((CIGAircraft*)objList->GetObject(currentObj));

  if (edAirAltitude->Value < 0)
  {
    return;
  }

  // the unit is on the ground and the altitude is above the ground
  // remove the unit from the ground and place it in the air layer
  if (lUnit->GetCoordZ() <= 0 && edAirAltitude->Value != 0)
  {
    lUnit->RemoveFromLayerMap();
    lUnit->RemoveFromLogicMap();
    lUnit->SetLayer(MOL_AIROBJECTS);
    lUnit->NoteInLayerMap();
  }

  lUnit->SetCoordZ(edAirAltitude->Value);

  // notify the aircraft, as it is on the ground and set the layer to
  // air
  if (edAirAltitude->Value == 0)
  {
    lUnit->RemoveFromLayerMap();
    lUnit->SetLayer(MOL_GROUNDUNIT);
    lUnit->NoteInLogicMap();
    lUnit->NoteInLayerMap();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::edAirBodyAngleChange(TObject *Sender)
{
  CIGAircraft* lUnit = ((CIGAircraft*)objList->GetObject(currentObj));

  lUnit->SetSenseCounter(edAirBodyAngle->Value);
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::edAirIdExit(TObject *Sender)
{
  CIGAircraft* lUnit = ((CIGAircraft*)objList->GetObject(currentObj));

  int lId = edAirId->Text.ToIntDef(lUnit->GetId());

  // verify if the id is unique
  if (!GetMissionInstance()->UnitIdUnique(lId))
  {
    lId = GetMissionInstance()->GetUniqueUnitId();
    edVId->Text = lId;
  }

  lUnit->SetId(lId);
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::edAirHpExit(TObject *Sender)
{
  CIGAircraft* lUnit = ((CIGAircraft*)objList->GetObject(currentObj));

  int lHp = edAirHp->Text.ToIntDef(lUnit->GetCurrentHP());

  if (lHp > lUnit->GetAircraftUnifiedData().hp)
  {
    lHp = lUnit->GetAircraftUnifiedData().hp;
  }

  if (lHp < 0)
  {
    lHp = 0;
  }

  edAirHp->Text = lHp;

  lUnit->SetCurrentHP(lHp);
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::memoAirSrcExit(TObject *Sender)
{
  // modify unit script
  CIGAircraft* lUnit = ((CIGAircraft *)objList->GetObject(currentObj));
  AnsiString lStr = "";

  for(int i = 0; i < memoAirSrc->Lines->Count; i++)
  {
    lStr += memoAirSrc->Lines->Strings[i] + "\n";
  }

  // long lines, the pleasure of my life
  lUnit->SetScriptObserver( GetMissionInstance()->GetDoctrine()->AddScript(lUnit->GetScriptObserver(), CPString(lStr.c_str()) ) );
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::btnAirCompileClick(TObject *Sender)
{
  // TODO: add aircraft scripting interface
  CIGAircraft* lUnit = ((CIGAircraft*)objList->GetObject(currentObj));

  if (!lUnit->GetScriptObserver())
  {
    memoAirErrors->Clear();
    memoAirErrors->Lines->Add("Nothing to compile.");
    return;
  }

  // do some compile
  // compile the script, put the errors on a memo
  //int ret = 0;
  bool lCslOk;
  AnsiString lStr = "";

  memoAirErrors->Clear();

  try
  {
    // new'g a compile object
    mCompiler = new ZCsl(ZCsl::disAll);
    lCslOk    = true;

    // load math library
    ZMathLib* lMathLib = new ZMathLib(mCompiler, "Math Library");
    lMathLib->InitLibrary();
    mCompiler->loadLibrary(lMathLib);

    // load weapon rules library
    WeaponRulesLib* lWeaponRulesLib = new WeaponRulesLib(mCompiler, "Weapon Rules Library");
    lWeaponRulesLib->InitLibrary();
    mCompiler->loadLibrary(lWeaponRulesLib);

    // add functions
    addModuleFunc("PostMessage(const msg)", mHandleDummy);
    addModuleFunc("PlaySound(const no)", mHandleDummy);

    addModuleFunc("Move(const x, const y)", mHandleDummy);
    addModuleFunc("Stop()", mHandleDummy);
    addModuleFunc("Unload(const x, const y, const unitIndex)", mHandleDummy);
    addModuleFunc("UnloadAll(const x, const y)", mHandleDummy);
    addModuleFunc("Land(const x, const y)", mHandleDummy);

    addModuleFunc("MyX()", mHandleDummy);
    addModuleFunc("MyY()", mHandleDummy);
    addModuleFunc("MyZ()", mHandleDummy);

    addModuleFunc("SetScanAngle(const deg)", mHandleDummy);
    addModuleFunc("GetScanAngle()", mHandleDummy);
    addModuleFunc("LookTo(const id)", mHandleDummy);
    addModuleFunc("LookAt(const x, const y)", mHandleDummy);
    addModuleFunc("Sweeping()", mHandleDummy);
    addModuleFunc("Moving()", mHandleDummy);

    addModuleFunc("Scan()", mHandleDummy);
    addModuleFunc("UnitInViewCount()", mHandleDummy);
    addModuleFunc("UnitInView(const index)", mHandleDummy);

    addModuleFunc("WeaponRange(const no)", mHandleDummy);
    addModuleFunc("WeaponDamage(const no, const dtype)", mHandleDummy);

    addModuleFunc("InVisual(const x, const y)", mHandleDummy    );
    addModuleFunc("UnitId(const x, const y)",   mHandleDummy      );
    addModuleFunc("UnitDist(const id)",         mHandleDummy    );
    addModuleFunc("UnitStatus(const id)",       mHandleDummy  );
    addModuleFunc("UnitThreat(const id)",       mHandleDummy  );
    addModuleFunc("UnitRelation(const id)",     mHandleDummy);
    addModuleFunc("UnitType(const id)",         mHandleDummy);
    addModuleFunc("UnitX(const id)",            mHandleDummy);
    addModuleFunc("UnitY(const id)",            mHandleDummy);
    addModuleFunc("UnitZ(const id)",            mHandleDummy);

    addModuleFunc("FireAt(const x, const y)", mHandleDummy);
    addModuleFunc("FireTo(const id)", mHandleDummy);

    addModuleFunc("NoiseType(const index)",  mHandleDummy);
    addModuleFunc("NoiseX(const index)",     mHandleDummy);
    addModuleFunc("NoiseY(const index)",     mHandleDummy);
    addModuleFunc("NoiseCount()",            mHandleDummy);
    addModuleFunc("NoisePurge()",            mHandleDummy);

    addModuleFunc("AttackerOf(const id, const index)",   mHandleDummy);
    addModuleFunc("AttackerOfCount(const id)",           mHandleDummy);

    addModuleFunc("MyHp()", mHandleDummy);
    addModuleFunc("MyId()", mHandleDummy);

    addModuleFunc("GetGroundType(const x, const y)",    mHandleDummy);
    addModuleFunc("GetMapVisibility(const x, const y)", mHandleDummy);

    // we should check wether the unit is a dropship and add specific scripting?
    addModuleFunc("BayCount()", mHandleDummy);

    // arrange the script source
    for(int i = 0; i < memoAirSrc->Lines->Count; i++)
    {
      lStr += memoAirSrc->Lines->Strings[i] + "\n";
    }

    std::stringstream str(lStr.c_str());

    // compile
    mCompiler->loadScript(mModuleName, &str);

    // delete compiler object
    delete mCompiler;
  }
  catch(const ZException& err)
  {
    for (int i = 0; i < err.count(); i++)
    {
      memoAirErrors->Lines->Add((char*)err[i]);
    }

    if (lCslOk)
    {
      delete mCompiler;
    }

    return;
  }

  memoAirErrors->Lines->Add("Compile Ok.");
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::btnVBayClick(TObject *Sender)
{
  CIGUnit* lUnit = dynamic_cast<CIGUnit*>(objList->GetObject(currentObj));

  FormBayManager->SetTransport(lUnit);
  FormBayManager->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::btnAirBayClick(TObject *Sender)
{
  CIGUnit* lUnit = dynamic_cast<CIGUnit*>(objList->GetObject(currentObj));

  FormBayManager->SetTransport(lUnit);
  FormBayManager->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::memoStaticSrcExit(TObject *Sender)
{
  // modify unit script
  CIGStaticObject* lUnit = ((CIGStaticObject *)objList->GetObject(currentObj));
  AnsiString lStr = "";

  for(int i = 0; i < memoStaticSrc->Lines->Count; i++)
  {
    lStr += memoStaticSrc->Lines->Strings[i] + "\n";
  }

  // long lines, the pleasure of my life
  lUnit->SetScriptObserver( GetMissionInstance()->GetDoctrine()->AddScript(lUnit->GetScriptObserver(), CPString(lStr.c_str()) ) );
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::btnStaticCompileClick(TObject *Sender)
{
  CIGStaticObject* lUnit = ((CIGStaticObject*)objList->GetObject(currentObj));

  if (!lUnit->GetScriptObserver())
  {
    memoStaticErrors->Clear();
    memoStaticErrors->Lines->Add("Nothing to compile.");
    return;
  }

  // do some compile
  // compile the script, put the errors on a memo
  //int ret = 0;
  bool lCslOk;
  AnsiString lStr = "";

  memoStaticErrors->Clear();

  try
  {
    // new'g a compile object
    mCompiler = new ZCsl(ZCsl::disAll);
    lCslOk    = true;

    // load math library
    ZMathLib* lMathLib = new ZMathLib(mCompiler, "Math Library");
    lMathLib->InitLibrary();
    mCompiler->loadLibrary(lMathLib);

    // load weapon rules library
    WeaponRulesLib* lWeaponRulesLib = new WeaponRulesLib(mCompiler, "Weapon Rules Library");
    lWeaponRulesLib->InitLibrary();
    mCompiler->loadLibrary(lWeaponRulesLib);

    // add functions
    addModuleFunc("PostMessage(const msg)", mHandleDummy);
    addModuleFunc("LayerSetEnabled(const layer, const value)", mHandleDummy);
    addModuleFunc("LayerGetEnabled(const layer)", mHandleDummy);
    addModuleFunc("LayerSetVisible(const layer, const value)", mHandleDummy);
    addModuleFunc("LayerGetVisible(const layer)", mHandleDummy);
    addModuleFunc("LayerGetX(const layer)", mHandleDummy);
    addModuleFunc("LayerGetY(const layer)", mHandleDummy);
    addModuleFunc("LayerSetX(const layer, const value)", mHandleDummy);
    addModuleFunc("LayerSetY(const layer, const value)", mHandleDummy);
    addModuleFunc("LayerSetCycleDelay(const layer, const value)", mHandleDummy);
    addModuleFunc("LayerGetCycleDelay(const layer)", mHandleDummy);
    addModuleFunc("LayerSetFrameDelay(const layer, const value)", mHandleDummy);
    addModuleFunc("LayerGetFrameDelay(const layer)", mHandleDummy);
    addModuleFunc("MyX()", mHandleDummy);
    addModuleFunc("MyY()", mHandleDummy);
    addModuleFunc("GetMask(const x, const y)", mHandleDummy);
    addModuleFunc("SetMask(const x, const y, const value)", mHandleDummy);
    addModuleFunc("GetAltitude(const x, const y)", mHandleDummy);
    addModuleFunc("SetAltitude(const x, const y, const value)", mHandleDummy);
    addModuleFunc("MyHp()", mHandleDummy);
    addModuleFunc("MyId()", mHandleDummy);
    addModuleFunc("CanFire()", mHandleDummy);

    // arrange the script source
    for(int i = 0; i < memoStaticSrc->Lines->Count; i++)
    {
      lStr += memoStaticSrc->Lines->Strings[i] + "\n";
    }

    std::stringstream str(lStr.c_str());

    // compile
    mCompiler->loadScript(mModuleName, &str);

    // delete compiler object
    delete mCompiler;
  }
  catch(const ZException& err)
  {
    for (int i = 0; i < err.count(); i++)
    {
      memoStaticErrors->Lines->Add((char*)err[i]);
    }

    if (lCslOk)
    {
      delete mCompiler;
    }

    return;
  }

  memoStaticErrors->Lines->Add("Compile Ok.");
}
//---------------------------------------------------------------------------

void __fastcall TFormObjManager::edStaticIdExit(TObject *Sender)
{
  CIGStaticObject * lStatic = (CIGStaticObject *)objList->GetObject(currentObj);

  // verify if the id is unique
  int id = edStaticId->Text.ToIntDef(0);

  if (!GetMissionInstance()->UnitIdUnique(id))
  {
    id = GetMissionInstance()->GetUniqueUnitId();
    edStaticId->Text = id;
  }

  lStatic->SetId(id);
}
//---------------------------------------------------------------------------

