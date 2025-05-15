//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License as     //
// published by the Free Software Foundation; either version 2.1 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License  //
// for more details.                                                         //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------
//#if defined(__BORLANDC__) || defined(__SUNPRO_CC) || defined (__GNUG__)
//#include <strstream>
//#else
//#include "strstrea.h"	// TMN: Note use of obsolete header
//#endif
#pragma hdrstop

#include "PPIG_MissionControl.h"

#if PPT_COMPILER_MICROSOFT
#include <strstream>
#else
#include <sstream>
#endif
#include <algorithm>
using namespace std;

#include "basepath.h"
#include "config.h"
#include "PP_Stdlib.h"
//#pragma message("HERE HERE")
#include "PPIG_CBattleControl.h"
#include "PPIG_CRadar.h"
#include "PPIG_CScreen.h"

#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "PPIG_ObjInfantry.h"
#include "PPIG_ObjVehicle.h"
#include "PPIG_ObjTank.h"
#include "PPIG_ObjTran.h"
#include "PPIG_ObjRecon.h"
#include "PPIG_ObjArtillery.h"
#include "PPIG_ObjAircraft.h"
#include "PPIG_ObjFighter.h"
#include "PPIG_ObjChopper.h"
#include "PPIG_ObjDropship.h"
#include "PPIG_BattleGroup.h"
#include "PPIG_BGManager.h"
#include "PP_Timer.h"

//#include "PPIG_Exit.h"
#include "ZMathLib.h"
#include "WeaponRulesLib.h"
//---------------------------------------------------------------------------

CPPIG_MissionControl::CPPIG_MissionControl()
{
    // script init
    mValidCsl       = false;
    mScriptObserver = NULL;
    mModuleName     = "MissionControlCompiler";
    mCsl            = NULL;

    InitCsl();
    InitHandlers();
    mTicks       = GetTimerInstance()->GetTime();
}
//---------------------------------------------------------------------------

void CPPIG_MissionControl::addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback)
{
    mCsl->addFunc(mModuleName, szMemberName, callback);
}
//---------------------------------------------------------------------------

void CPPIG_MissionControl::InitHandlers()
{
    // handlers init
    // insertion
    mHandleInsertInfantryAtPoint  .set(this, &CPPIG_MissionControl::mcInsertInfantryAtPoint);
    mHandleInsertVehicleAtPoint   .set(this, &CPPIG_MissionControl::mcInsertVehicleAtPoint);
    mHandleInsertAircraftAtPoint  .set(this, &CPPIG_MissionControl::mcInsertAircraftAtPoint);

    // modifications

    // deletion
    mHandleKillUnit               .set(this, &CPPIG_MissionControl::mcKillUnit);
    mHandleRemoveUnit             .set(this, &CPPIG_MissionControl::mcRemoveUnit);

    // battlegroup
    mHandleGetBattleGroupCount    .set(this, &CPPIG_MissionControl::mcGetBattleGroupCount);
    mHandleGetBattleGroup         .set(this, &CPPIG_MissionControl::mcGetBattleGroup);
    mHandleGetBattleGroupUnitCount.set(this, &CPPIG_MissionControl::mcGetBattleGroupUnitCount);
    mHandleGetBattleGroupUnit     .set(this, &CPPIG_MissionControl::mcGetBattleGroupUnit);
    mHandleIsUnitInBattleGroup    .set(this, &CPPIG_MissionControl::mcIsUnitInBattleGroup);
    mHandleChangePolitics         .set(this, &CPPIG_MissionControl::mcChangePolitics);
    mHandleGetPolitics            .set(this, &CPPIG_MissionControl::mcGetPolitics);
    mHandleCreateBattleGroup      .set(this, &CPPIG_MissionControl::mcCreateBattleGroup);
    mHandleRemoveBattleGroup      .set(this, &CPPIG_MissionControl::mcRemoveBattleGroup);
    mHandleRelinquishCommand      .set(this, &CPPIG_MissionControl::mcRelinquishCommand);

    // map
    mHandleGetWidth               .set(this, &CPPIG_MissionControl::mcGetWidth);
    mHandleGetHeight              .set(this, &CPPIG_MissionControl::mcGetHeight);
    mHandleGetUnitAtPoint         .set(this, &CPPIG_MissionControl::mcGetUnitAtPoint);
    mHandleGetAltitude            .set(this, &CPPIG_MissionControl::mcGetAltitude);
    mHandleSetAltitude            .set(this, &CPPIG_MissionControl::mcSetAltitude);
    mHandleGetTileType            .set(this, &CPPIG_MissionControl::mcGetTileType);
    mHandleGetVisibility          .set(this, &CPPIG_MissionControl::mcGetVisibility);
    mHandleCanBePlaced            .set(this, &CPPIG_MissionControl::mcCanBePlaced);
    mHandleNotifyVisibilityMap    .set(this, &CPPIG_MissionControl::mcNotifyVisibilityMap);
    mHandleNotifyCircleVisibilityMap    .set(this, &CPPIG_MissionControl::mcNotifyCircleVisibilityMap);
    mHandleMoveCamera             .set(this, &CPPIG_MissionControl::mcMoveCamera);

    // Unit
    mHandleChangeUnitBattleGroup  .set(this, &CPPIG_MissionControl::mcChangeUnitBattleGroup);
    mHandleIsUnitDead             .set(this, &CPPIG_MissionControl::mcIsUnitDead);
    mHandleUnitExists             .set(this, &CPPIG_MissionControl::mcUnitExists);
    mHandleGetUnitLogicX          .set(this, &CPPIG_MissionControl::mcGetUnitLogicX);
    mHandleGetUnitLogicY          .set(this, &CPPIG_MissionControl::mcGetUnitLogicY);
    mHandleGetUnitZ               .set(this, &CPPIG_MissionControl::mcGetUnitZ);
    mHandleGetUnitHP              .set(this, &CPPIG_MissionControl::mcGetUnitHP);
    mHandleGetUnitLogicDimX       .set(this, &CPPIG_MissionControl::mcGetUnitLogicDimX);
    mHandleGetUnitLogicDimY       .set(this, &CPPIG_MissionControl::mcGetUnitLogicDimY);
    mHandleGetUnitType            .set(this, &CPPIG_MissionControl::mcGetUnitType);
    mHandleChangeUnitScript       .set(this, &CPPIG_MissionControl::mcChangeUnitScript);
    mHandleGetUnitBattleGroup     .set(this, &CPPIG_MissionControl::mcGetUnitBattleGroup);
    mHandleSetUnitScriptVar       .set(this, &CPPIG_MissionControl::mcSetUnitScriptVar);

    // Transporter
    mHandleUnitsTransportedCount  .set(this, &CPPIG_MissionControl::mcUnitsTransportedCount);
    mHandleGetUnitTransported     .set(this, &CPPIG_MissionControl::mcGetUnitTransported);
    mHandleCanCarryInfantry       .set(this, &CPPIG_MissionControl::mcCanCarryInfantry);
    mHandleCanCarryVehicles       .set(this, &CPPIG_MissionControl::mcCanCarryVehicles);

    // Unit orders
    mHandleMove                   .set(this, &CPPIG_MissionControl::mcMove);
    mHandleAttack                 .set(this, &CPPIG_MissionControl::mcAttack);
    mHandleAttackGround           .set(this, &CPPIG_MissionControl::mcAttackGround);
    mHandleStop                   .set(this, &CPPIG_MissionControl::mcStop);
    mHandleLoad                   .set(this, &CPPIG_MissionControl::mcLoad);
    mHandleUnloadAll              .set(this, &CPPIG_MissionControl::mcUnloadAll);
    mHandlePatrol                 .set(this, &CPPIG_MissionControl::mcPatrol);
    mHandleLand                   .set(this, &CPPIG_MissionControl::mcLand);

    // Unit orders (tactical)
    mHandleTacticMove             .set(this, &CPPIG_MissionControl::mcTacticMove);
    mHandleTacticAttack           .set(this, &CPPIG_MissionControl::mcTacticAttack);
    mHandleTacticAttackGround     .set(this, &CPPIG_MissionControl::mcTacticAttackGround);
    mHandleTacticStop             .set(this, &CPPIG_MissionControl::mcTacticStop);

    // Misc
    mHandleMissionTime            .set(this, &CPPIG_MissionControl::mcMissionTime);
    mHandlePostMessage            .set(this, &CPPIG_MissionControl::mcPostMessage);
    mHandleEndMission             .set(this, &CPPIG_MissionControl::mcEndMission);
    mHandlePause                  .set(this, &CPPIG_MissionControl::mcPause);
    mHandleMessageConsole         .set(this, &CPPIG_MissionControl::mcMessageConsole);
    mHandleFocusOnObject          .set(this, &CPPIG_MissionControl::mcFocusOnObject);
}
//---------------------------------------------------------------------------

void CPPIG_MissionControl::InitCsl()
{
    CPString lScript;

    if (mValidCsl)
    {
        delete mCsl;
    }

    mValidCsl = false;

    if (!mScriptObserver)
    {
        return;
    }

    try
    {
        mCsl = new ZCsl();
    }
    catch(...)
    {
        LOG(FATAL) << "CPPIG_MissionControl::InitCsl - errors initializing script compiler";
        return;
    }

    try
    {
        // load math library
        ZMathLib* lMathLib = new ZMathLib(mCsl, "Math Library");
        lMathLib->InitLibrary();
        mCsl->loadLibrary(lMathLib);

        // load weapon rules library
        WeaponRulesLib* lWeaponRulesLib = new WeaponRulesLib(mCsl, "Weapon Rules Library");
        lWeaponRulesLib->InitLibrary();
        mCsl->loadLibrary(lWeaponRulesLib);

        // add insertion functions
        addModuleFunc("InsertInfantryAtPoint(const x, const y, const bgId, const index, const scriptFile)", mHandleInsertInfantryAtPoint);
        addModuleFunc("InsertVehicleAtPoint(const x, const y, const bgId, const index, const scriptFile)",  mHandleInsertVehicleAtPoint);
        addModuleFunc("InsertAircraftAtPoint(const x, const y, const bgId, const index, const scriptFile)", mHandleInsertAircraftAtPoint);

        // add modifications functions

        // add deletion functions
        addModuleFunc("KillUnit(const unitId)",   mHandleKillUnit);
        addModuleFunc("RemoveUnit(const unitId)", mHandleRemoveUnit);

        // add battlegroup functions
        addModuleFunc("GetBattleGroupCount()",                                    mHandleGetBattleGroupCount);
        addModuleFunc("GetBattleGroup(const index)",                              mHandleGetBattleGroup);
        addModuleFunc("GetBattleGroupUnitCount(const bgId)",                      mHandleGetBattleGroupUnitCount);
        addModuleFunc("GetBattleGroupUnit(const bgId, const index)",              mHandleGetBattleGroupUnit);
        addModuleFunc("IsUnitInBattleGroup(const bgId, const id)",                mHandleIsUnitInBattleGroup);
        addModuleFunc("ChangePolitics(const bgId1, const bgId2, const relation)", mHandleChangePolitics);
        addModuleFunc("GetPolitics(const bgId1, const bgId2)",                    mHandleGetPolitics);
        addModuleFunc("CreateBattleGroup(const name, const info)",                mHandleCreateBattleGroup);
        addModuleFunc("RemoveBattleGroup(const bgId)",                            mHandleRemoveBattleGroup);
        addModuleFunc("RelinquishCommand(const bgIdReceiver, const bgIdAborter)", mHandleRelinquishCommand);

        // add map functions
        addModuleFunc("GetWidth()",                                                     mHandleGetWidth);
        addModuleFunc("GetHeight()",                                                    mHandleGetHeight);
        addModuleFunc("GetUnitAtPoint(const x, const y)",                               mHandleGetUnitAtPoint);
        addModuleFunc("GetAltitude(const x, const y)",                                  mHandleGetAltitude);
        addModuleFunc("SetAltitude(const x, const y, const value)",                     mHandleSetAltitude);
        addModuleFunc("GetTileType(const x, const y)",                                  mHandleGetTileType);
        addModuleFunc("GetVisibility(const bgId, const x, const y)",                    mHandleGetVisibility);
        addModuleFunc("CanBePlaced(const x, const y)",                                  mHandleCanBePlaced);
        addModuleFunc("NotifyVisibilityMap(const x, const y, const bgId, const value)", mHandleNotifyVisibilityMap);
        addModuleFunc("NotifyCircleVisibilityMap(const x, const y, const radius, const bgId, const value)", mHandleNotifyCircleVisibilityMap);
        addModuleFunc("MoveCamera(const x, const y)",                                   mHandleMoveCamera);

        // add unit functions
        addModuleFunc("ChangeUnitBattleGroup(const unitId, const newBg)", mHandleChangeUnitBattleGroup);
        addModuleFunc("IsUnitDead(const id)",                             mHandleIsUnitDead);
        addModuleFunc("UnitExists(const id)",                             mHandleUnitExists);
        addModuleFunc("GetUnitLogicX(const id)",                          mHandleGetUnitLogicX);
        addModuleFunc("GetUnitLogicY(const id)",                          mHandleGetUnitLogicY);
        addModuleFunc("GetUnitZ(const id)",                               mHandleGetUnitZ);
        addModuleFunc("GetUnitHP(const id)",                              mHandleGetUnitHP);
        addModuleFunc("GetUnitLogicDimX(const id)",                       mHandleGetUnitLogicDimX);
        addModuleFunc("GetUnitLogicDimY(const id)",                       mHandleGetUnitLogicDimY);
        addModuleFunc("GetUnitType(const id)",                            mHandleGetUnitType);
        addModuleFunc("ChangeUnitScript(const id, const scriptFile)",     mHandleChangeUnitScript);
        addModuleFunc("GetUnitBattleGroup(const id)",                     mHandleGetUnitBattleGroup);
        addModuleFunc("SetUnitScriptVar(const id, const varname, const value)", mHandleSetUnitScriptVar);

        // add transporter functions
        addModuleFunc("UnitsTransportedCount(const transportId)",           mHandleUnitsTransportedCount);
        addModuleFunc("GetUnitTransported(const transportId, const index)", mHandleGetUnitTransported);
        addModuleFunc("CanCarryInfantry(const id)",                         mHandleCanCarryInfantry);
        addModuleFunc("CanCarryVehicles(const id)",                         mHandleCanCarryVehicles);

        // add unit orders functions
        addModuleFunc("Move(const id, const x, const y)", mHandleMove);
        addModuleFunc("Attack(const id, const victimId)", mHandleAttack);
        addModuleFunc("AttackGround(const id, const x, const y)", mHandleAttackGround);
        addModuleFunc("Stop(const id)", mHandleStop);
        addModuleFunc("Load(const id, const transportId)", mHandleLoad);
        addModuleFunc("UnloadAll(const transportId, const x, const y)", mHandleUnloadAll);
        addModuleFunc("Patrol(const id, const x, const y)", mHandlePatrol);
        addModuleFunc("Land(const id, const x, const y)", mHandleLand);

        // add unit orders (tactic) functions
        addModuleFunc("TacticMove(const id, const x, const y)", mHandleTacticMove);
        addModuleFunc("TacticAttack(const id, const victimId)", mHandleTacticAttack);
        addModuleFunc("TacticAttackGround(const id, const x, const y)", mHandleTacticAttackGround);
        addModuleFunc("TacticStop(const id)", mHandleTacticStop);

        // add misc functions
        addModuleFunc("MissionTime()", mHandleMissionTime);
        addModuleFunc("PostMessage(const msg)", mHandlePostMessage);
        addModuleFunc("EndMission()", mHandleEndMission);
        addModuleFunc("Pause(const state)", mHandlePause);
        addModuleFunc("MessageConsole(const state)", mHandleMessageConsole);
        addModuleFunc("FocusOnObject(const id)", mHandleFocusOnObject);
    }
    catch(...)
    {
        LOG(FATAL) << "CPPIG_MissionControl::InitCsl - errors initializing script libraries.";
        delete mCsl;
        return;
    }

    lScript = mScriptObserver->GetScript();

    try
    {
        // The const_cast is a workaround for a Microsoft interface bug.
#if PPT_COMPILER_MICROSOFT
        std::istrstream str(const_cast<char*>(lScript.c_str()));
#else
        std::istringstream str(const_cast<char*>(lScript.c_str()));
#endif

        // compile
        mCsl->loadScript(mModuleName, &str);

        mValidCsl = true;
    }
    catch(const ZException& err)
    {
        LOG(FATAL) << "CPPIG_MissionControl::InitCsl - errors compiling script";
        for (int i = 0; i < err.count(); i++)
        {
            LOG(FATAL) << (char*)err[i];
        }

        delete mCsl;
    }
}
//---------------------------------------------------------------------------

void CPPIG_MissionControl::Update()
{
    // alte update'uri
    if (GetTimerInstance()->GetTime() - mTicks > 1000)
    {
        mTicks = GetTimerInstance()->GetTime();

        // o data pe secunda se apeleaza si scriptul
        if (mValidCsl)
        {
            try
            {
                mCsl->call(mModuleName, "main");
            }
            catch(const ZException& err)
            {
                LOG(ERROR) << "CPPIG_MissionControl::Update - error(s) calling main function";
                for (int i = 0; i < err.count(); i++)
                {
                    LOG(ERROR) << (char*)err[i];
                }
            }
        }
    }
}
//---------------------------------------------------------------------------

void CPPIG_MissionControl::SetScriptObserver(CPPIG_ScriptObserver* _obs)
{
    if (mValidCsl)
    {
        delete mCsl;
    }

    mScriptObserver = _obs;
    mValidCsl       = false;

    InitCsl();
}
//---------------------------------------------------------------------------

CPPIG_MissionControl::~CPPIG_MissionControl()
{
    delete mCsl;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//
// Implementations of callbacks
//
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcInsertInfantryAtPoint(ZCsl* csl)
{
    int      lX          = csl->get("x").asInt();     // x of the unit (logical coordinate)
    int      lY          = csl->get("y").asInt();     // y of the unit (logical coordinate)
    int      lBgId       = csl->get("bgId").asInt();  // the battlegroup in which the unit will be created
    int      lIndex      = csl->get("index").asInt(); // index in the AIInfantry.dat file
    CPString lScriptFile = CPString(csl->get("scriptFile").buffer()); // script file name for behavior

    int      lUnitId     = GetMissionInstance()->GetUniqueUnitId();

    // check if the battlegroup exists
    CPPIG_BattleGroup* lBg = GetMissionInstance()->GetBGManager()->GetBGroupById(lBgId);

    if (!lBg)
    {
        return 0;
    }

    // check if the index in the file exists and load the record
    TInfantryUnified lInfantryData;

    if (!getAIInfRecord((getGamePath() + CPString(PATH_AIINFANTRY) + CPString(FILE_AIINFANTRY)).c_str(), lIndex, &lInfantryData))
    {
        return 0;
    }

    // now create the infantry unit
    CIGInfantryUnit* lUnit = 0;

    try
    {
        lUnit = new CIGInfantryUnit();
    }
    catch(...)
    {
        LOG(ERROR) << "CPPIG_MissionControl::mcInsertInfantryAtPoint - cannot create unit.";
        return 0;
    }

    // settings
    if (!lUnit->CanBePlaced(CPPoint(lX, lY)))
    {
        delete lUnit;
        return 0;
    }

    lUnit->SetCoord(CPPoint(lX * MAPCELLX, lY * MAPCELLY));
    lUnit->SetInfUnifiedData(lInfantryData);
    lUnit->SetId(lUnitId);
    lUnit->SetBattleGroupId(lBg->GetId());
    lUnit->SetSelected(false);
    lUnit->SetScriptObserver( GetMissionInstance()->GetDoctrine()->AddScriptFromFile(lUnit->GetScriptObserver(), PATH_SCRIPTS_INFANTRY + lScriptFile ) );

    CPSprite* lSprite = GetGraphicInstance()->AddSprite(CPString(PATH_INF_GFX) + lInfantryData.sprite, 8, 14, SPRL_MISSION);
    lUnit->SetSprite(lSprite);

    // now add the unit in the mission structures
    GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Add(lUnit);
    GetMissionInstance()->GetBGManager()->AddUnit(lUnit);

    switch (GetMissionInstance()->GetBGManager()->GetRelation(OS_PLAYER, lUnit->GetBattleGroupId()))
    {
    case REL_NEUTRAL:
        GetMissionInstance()->GetObjects(MOL_NEUTRALUNITS)->Add(lUnit);
        break;
    case REL_OWNED:
        GetMissionInstance()->GetObjects(MOL_OWNEDUNITS)->Add(lUnit);
        break;
    case REL_FRIENDLY:
        GetMissionInstance()->GetObjects(MOL_FRIENDLYUNITS)->Add(lUnit);
        break;
    case REL_ENEMY:
        GetMissionInstance()->GetObjects(MOL_ENEMYUNITS)->Add(lUnit);
        break;
    default:
        // do nothing
        LOG(ERROR) << "CPPIG_MissionControl::mcInsertInfantryAtPoint - Infantry relation to player unknown.";
        break;
    }

    // finally, notify the unit in the maps
    lUnit->NoteInLayerMap();
    lUnit->NoteInLogicMap();

    return lUnitId;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcInsertVehicleAtPoint(ZCsl* csl)
{
    int      lX          = csl->get("x").asInt();     // x of the unit (logical coordinate)
    int      lY          = csl->get("y").asInt();     // y of the unit (logical coordinate)
    int      lBgId       = csl->get("bgId").asInt();  // the battlegroup in which the unit will be created
    int      lIndex      = csl->get("index").asInt(); // index in the AIVehicles.dat file
    CPString lScriptFile = CPString(csl->get("scriptFile").buffer()); // script file name for behavior

    int      lUnitId     = GetMissionInstance()->GetUniqueUnitId();

    // check if the battlegroup exists
    CPPIG_BattleGroup* lBg = GetMissionInstance()->GetBGManager()->GetBGroupById(lBgId);

    if (!lBg)
    {
        return 0;
    }

    TVehicleUnified lData;

    if (!getAIVehRecord((getGamePath() + CPString(PATH_AIVEHICLE) + CPString(FILE_AIVEHICLE)).c_str(), lIndex, &lData))
    {
        return 0;
    }

    CIGVehicle* lUnit;

    try
    {
        switch(lData.vType)
        {
        case 0:
            lUnit = new CIGTankUnit();
            break;

        case 1:
            lUnit = new CIGArtilleryUnit();
            break;

        case 2:
            lUnit = new CIGTranUnit();
            break;

        case 3:
            lUnit = new CIGReconUnit();
            break;

        default:
            return 0;
        }
    }
    catch(...)
    {
        LOG(ERROR) << "CPPIG_MissionControl::mcInsertVehicleAtPoint - cannot create unit.";
        return 0;
    }

    // settings
    if (!lUnit->CanBePlaced(CPPoint(lX, lY)))
    {
        delete lUnit;
        return 0;
    }

    lUnit->SetCoord(CPPoint(lX * MAPCELLX, lY * MAPCELLY));
    lUnit->SetVehicleUnifiedData(lData);
    lUnit->SetId(lUnitId);
    lUnit->SetBattleGroupId(lBg->GetId());
    lUnit->SetSelected(false);
    lUnit->SetScriptObserver( GetMissionInstance()->GetDoctrine()->AddScriptFromFile(lUnit->GetScriptObserver(), PATH_SCRIPTS_VEHICLE + lScriptFile ) );

    CPSprite* lSprite = GetGraphicInstance()->AddSprite(CPString(PATH_VEHICLES_GFX) + lData.sprite, 8, 14, SPRL_MISSION);
    lUnit->SetSprite(lSprite);

    // now add the unit in the mission structures
    GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Add(lUnit);
    GetMissionInstance()->GetBGManager()->AddUnit(lUnit);

    switch (GetMissionInstance()->GetBGManager()->GetRelation(OS_PLAYER, lUnit->GetBattleGroupId()))
    {
    case REL_NEUTRAL:
        GetMissionInstance()->GetObjects(MOL_NEUTRALUNITS)->Add(lUnit);
        break;
    case REL_OWNED:
        GetMissionInstance()->GetObjects(MOL_OWNEDUNITS)->Add(lUnit);
        break;
    case REL_FRIENDLY:
        GetMissionInstance()->GetObjects(MOL_FRIENDLYUNITS)->Add(lUnit);
        break;
    case REL_ENEMY:
        GetMissionInstance()->GetObjects(MOL_ENEMYUNITS)->Add(lUnit);
        break;
    default:
        // do nothing
        LOG(ERROR) << "CPPIG_MissionControl::mcInsertVehicleAtPoint - vehicle relation to player unknown.";
        break;
    }

    // finally, notify the unit in the maps
    lUnit->NoteInLayerMap();
    lUnit->NoteInLogicMap();

    return lUnitId;
}
//---------------------------------------------------------------------------

// will place the aircraft always in the air; use a land command to put it on the
// ground after adding it
ZString CPPIG_MissionControl::mcInsertAircraftAtPoint(ZCsl* csl)
{
    int      lX          = csl->get("x").asInt();     // x of the unit (logical coordinate)
    int      lY          = csl->get("y").asInt();     // y of the unit (logical coordinate)
    int      lBgId       = csl->get("bgId").asInt();  // the battlegroup in which the unit will be created
    int      lIndex      = csl->get("index").asInt(); // index in the AIVehicles.dat file
    CPString lScriptFile = CPString(csl->get("scriptFile").buffer()); // script file name for behavior

    int      lUnitId     = GetMissionInstance()->GetUniqueUnitId();

    // check if the battlegroup exists
    CPPIG_BattleGroup* lBg = GetMissionInstance()->GetBGManager()->GetBGroupById(lBgId);

    if (!lBg)
    {
        LOG(ERROR) << "CPPIG_MissionControl::mcInsertAircraftAtPoint - failed to obtain a suitable battlegroup.";
        return 0;
    }

    TVehicleUnified lData;

    if (!getAIVehRecord((getGamePath() + CPString(PATH_AIVEHICLE) + CPString(FILE_AIVEHICLE)).c_str(), lIndex, &lData))
    {
        LOG(ERROR) << "CPPIG_MissionControl::mcInsertAircraftAtPoint - failed to read the record.";
        return 0;
    }

    CIGAircraft* lUnit;

    try
    {
        switch(lData.vType)
        {
        case 4:
            lUnit = new CIGFighterUnit();
            break;

        case 5:
            //lAircraftUnit = new CIGBomberUnit();
            break;

        case 6:
            lUnit = new CIGChopperUnit();
            break;

        case 7:
            lUnit = new CIGDropship();
            break;

        default:
            return 0;
        }
    }
    catch(...)
    {
        LOG(ERROR) << "CPPIG_MissionControl::mcInsertAircraftAtPoint - cannot create unit.";
        return 0;
    }

    lUnit->SetCoord(CPPoint(lX * MAPCELLX, lY * MAPCELLY));
    lUnit->SetAircraftUnifiedData(lData);
    lUnit->SetId(lUnitId);
    lUnit->SetBattleGroupId(lBg->GetId());
    lUnit->SetSelected(false);
    lUnit->SetScriptObserver( GetMissionInstance()->GetDoctrine()->AddScriptFromFile(lUnit->GetScriptObserver(), PATH_SCRIPTS_AIRCRAFT + lScriptFile ) );

    CPSprite* lSprite = GetGraphicInstance()->AddSprite(CPString(PATH_VEHICLES_GFX) + lData.sprite, 8, 14, SPRL_MISSION);
    lUnit->SetSprite(lSprite);

    // now add the unit in the mission structures
    GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Add(lUnit);
    GetMissionInstance()->GetBGManager()->AddUnit(lUnit);

    switch (GetMissionInstance()->GetBGManager()->GetRelation(OS_PLAYER, lUnit->GetBattleGroupId()))
    {
    case REL_NEUTRAL:
        GetMissionInstance()->GetObjects(MOL_NEUTRALUNITS)->Add(lUnit);
        break;
    case REL_OWNED:
        GetMissionInstance()->GetObjects(MOL_OWNEDUNITS)->Add(lUnit);
        break;
    case REL_FRIENDLY:
        GetMissionInstance()->GetObjects(MOL_FRIENDLYUNITS)->Add(lUnit);
        break;
    case REL_ENEMY:
        GetMissionInstance()->GetObjects(MOL_ENEMYUNITS)->Add(lUnit);
        break;
    default:
        // do nothing
        LOG(ERROR) << "CPPIG_MissionControl::mcInsertAircraftAtPoint - aircraft relation to player unknown.";
        break;
    }

    // finally, notify the unit in the maps
    lUnit->NoteInLayerMap();
    //lUnit->NoteInLogicMap();

    return lUnitId;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcKillUnit(ZCsl* csl)
{
    int lUnitId       = csl->get("unitId").asInt();  // the id of the unit to be killed

    // set action die for this unit
    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lUnitId);

    if (lUnit)
    {
        if (lUnit->GetIsDead())
        {
            return 0;
        }

        lUnit->Die();
        return 1;
    }

    return 0;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcRemoveUnit(ZCsl* csl)
{
    int lUnitId       = csl->get("unitId").asInt();  // the id of the unit to be removed

    // get the unit and if it exists, remove it
    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lUnitId);

    if (lUnit)
    {
        lUnit->RemoveFromLayerMap();
        lUnit->RemoveFromLogicMap();

        GetMissionInstance()->AddToDeleteObjects(lUnit);
        //GetMissionInstance()->GetBGManager()->RemoveUnit(lUnit);

        return 1;
    }

    return 0;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcGetBattleGroupCount(ZCsl* csl)
{
    return GetMissionInstance()->GetBGManager()->BGroupCount();
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcGetBattleGroup(ZCsl* csl)
{
    int lBgIndex       = csl->get("index").asInt();  // the index of the battlegroup

    CPPIG_BattleGroup* lBg = GetMissionInstance()->GetBGManager()->GetBGroupByIndex(lBgIndex);

    if (lBg)
    {
        return lBg->GetId();
    }

    return -1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcGetBattleGroupUnitCount(ZCsl* csl)
{
    int lBgId = csl->get("bgId").asInt();  // the id of the battlegroup

    CPPIG_BattleGroup* lBg = GetMissionInstance()->GetBGManager()->GetBGroupById(lBgId);

    if (lBg)
    {
        return lBg->GetUnitCount();
    }

    return -1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcGetBattleGroupUnit(ZCsl* csl)
{
    int lBgId  = csl->get("bgId").asInt();  // the id of the battlegroup
    int lIndex = csl->get("index").asInt();  // the index of the unit in battlegroup

    CPPIG_BattleGroup* lBg = GetMissionInstance()->GetBGManager()->GetBGroupById(lBgId);

    if (lBg)
    {
        CIGBaseObject* lUnit = lBg->GetUnitByIndex(lIndex);

        if (lUnit)
        {
            return lUnit->GetId();
        }
    }

    return -1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcIsUnitInBattleGroup(ZCsl* csl)
{
    //int lBgId   = csl->get("bgId").asInt();    // the id of the battlegroup
    //int lUnitId = csl->get("id").asInt();  // the id of the unit to check

    return 0;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcChangePolitics(ZCsl* csl)
{
    int lBgId1 = csl->get("bgId1").asInt();  // the id of the first battlegroup
    int lBgId2 = csl->get("bgId2").asInt();  // the id of the second battlegroup
    int lRel   = csl->get("relation").asInt();  // the relation to be set

    GetMissionInstance()->GetBGManager()->SetUniRelation(lBgId1, lBgId2, (TPoliticsRelations)lRel);

    return "";
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcGetPolitics(ZCsl* csl)
{
    int lBgId1 = csl->get("bgId1").asInt();  // the id of the first battlegroup
    int lBgId2 = csl->get("bgId2").asInt();  // the id of the second battlegroup

    return GetMissionInstance()->GetBGManager()->GetRelation(lBgId1, lBgId2);
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcCreateBattleGroup(ZCsl* csl)
{
    CPString lName = CPString(csl->get("name").buffer()); // name of the bg to be created
    CPString lInfo = CPString(csl->get("info").buffer()); // info for the bg to be created
    int      lId   = gfRandom(1210); // :)

    CPPIG_BattleGroup* lBg;

    try
    {
        lBg = new CPPIG_BattleGroup(lName, lInfo, lId);
    }
    catch(...)
    {
        return -1;
    }

    GetMissionInstance()->GetBGManager()->AddBGroup(lBg);
    GetMissionInstance()->GetMap()->RegisterView(lId);

    return lId;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcRemoveBattleGroup(ZCsl* csl)
{
    int lBgId = csl->get("bgId").asInt();  // the id of the battlegroup

    CPPIG_BattleGroup* lBg = GetMissionInstance()->GetBGManager()->GetBGroupById(lBgId);

    if (lBg)
    {
        GetMissionInstance()->GetBGManager()->RemoveBGroup(lBg);
        return 1;
    }

    return 0;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcRelinquishCommand(ZCsl* csl)
{
    int lBgIdReceiver = csl->get("bgIdReceiver").asInt();  // the id of the receiver battlegroup
    int lBgIdAborter  = csl->get("bgIdAborter").asInt();  // the id of the aborter battlegroup


    CPPIG_BattleGroup* lBgReceiver = GetMissionInstance()->GetBGManager()->GetBGroupById(lBgIdReceiver);
    CPPIG_BattleGroup* lBgAborter  = GetMissionInstance()->GetBGManager()->GetBGroupById(lBgIdAborter);

    if (!lBgReceiver || !lBgAborter)
    {
        return 0;
    }

    // move the units from aborter to receiver - not implemented
    return 0;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcGetUnitAtPoint(ZCsl* csl)
{
    int lX = csl->get("x").asInt();     // x of the unit (logical coordinate)
    int lY = csl->get("y").asInt();     // y of the unit (logical coordinate)

    CIGBaseObject* lUnit = GetMissionInstance()->GetMap()->GetGroundObject(lX, lY);

    if (lUnit)
    {
        return lUnit->GetId();
    }

    return -1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcGetWidth(ZCsl* csl)
{
    return GetMissionInstance()->GetMap()->GetLogicWidth();
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcGetHeight(ZCsl* csl)
{
    return GetMissionInstance()->GetMap()->GetLogicHeight();
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcGetAltitude(ZCsl* csl)
{
    int lX = csl->get("x").asInt();     // x of the unit (logical coordinate)
    int lY = csl->get("y").asInt();     // y of the unit (logical coordinate)

    return GetMissionInstance()->GetMap()->GetAltitude(lX, lY);
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcSetAltitude(ZCsl* csl)
{
    int lX     = csl->get("x").asInt();     // x of the unit (logical coordinate)
    int lY     = csl->get("y").asInt();     // y of the unit (logical coordinate)
    int lValue = csl->get("value").asInt(); // new value

    if (lValue >= 0)
    {
        GetMissionInstance()->GetMap()->SetAltitude(lX, lY, lValue);
    }

    return "";
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcGetTileType(ZCsl* csl)
{
    int lX = csl->get("x").asInt();     // x of the unit (logical coordinate)
    int lY = csl->get("y").asInt();     // y of the unit (logical coordinate)

    return GetMissionInstance()->GetMap()->GetTileType(lX, lY);
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcGetVisibility(ZCsl* csl)
{
    int lBgId = csl->get("bgId").asInt(); // id of the battlegroup
    int lX    = csl->get("x").asInt();     // x of the unit (logical coordinate)
    int lY    = csl->get("y").asInt();     // y of the unit (logical coordinate)

    return GetMissionInstance()->GetMap()->GetVisibility(lBgId, lX, lY);
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcCanBePlaced(ZCsl* csl)
{
    return "";
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcNotifyVisibilityMap(ZCsl* csl)
{
    int lX     = csl->get("x").asInt();     // x of the unit (logical coordinate)
    int lY     = csl->get("y").asInt();     // y of the unit (logical coordinate)
    int lBgId  = csl->get("bgId").asInt(); // id of the battlegroup
    int lValue = csl->get("value").asInt(); // value for the visibility

    if (lValue != 0 && lValue != 1 && lValue != 2)
    {
        return 0;
    }

    CPPoint lPoints[1] = { CPPoint(lX, lY) };

    GetMissionInstance()->GetMap()->NotifyVisibilityMap(lBgId, lPoints, 1, lValue);

    return 1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcNotifyCircleVisibilityMap(ZCsl* csl)
{
    int lX      = csl->get("x").asInt();      // x of the unit (logical coordinate)
    int lY      = csl->get("y").asInt();      // y of the unit (logical coordinate)
    int lRadius = csl->get("radius").asInt(); // radius of the circle
    int lBgId   = csl->get("bgId").asInt();   // id of the battlegroup
    int lValue  = csl->get("value").asInt();  // value for the visibility

    if (lValue != 0 && lValue != 1 && lValue != 2)
    {
        return 0;
    }

    int lSqrRadius = lRadius * lRadius;

    // maybe an incremental solution? neah, too lazy and too tired
    for(int x = lX - lRadius; x < lX + lRadius; x++)
    {
        for(int y = lY - lRadius; y < lY + lRadius; y++)
        {
            if (x * x + y * y <= lSqrRadius)
            {
                CPPoint lPoints[1] = { CPPoint(x, y) };
                GetMissionInstance()->GetMap()->NotifyVisibilityMap(lBgId, lPoints, 1, lValue);
            }
        }
    }

    //CPPoint lPoints[1] = { CPPoint(lX, lY) };
    //GetMissionInstance()->GetMap()->NotifyVisibilityMap(lBgId, lPoints, 1, lValue);

    return 1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcMoveCamera(ZCsl* csl)
{
    int lX     = csl->get("x").asInt();     // x of the unit (logical coordinate)
    int lY     = csl->get("y").asInt();     // y of the unit (logical coordinate)

    int lWidth  = GetMainScreenInstance()->mBattleControl->GetWidth();
    int lHeight = GetMainScreenInstance()->mBattleControl->GetHeight();

    int lLeft = max(0, lX * MAPCELLX - lWidth / 2);
    int lTop = max(0, lY * MAPCELLY - lHeight / 2);

    lLeft = min(lLeft, GetMissionInstance()->GetMap()->GetWidth() - lWidth);
    lTop  = min(lTop, GetMissionInstance()->GetMap()->GetHeight() - lHeight);

    // happy spaghetti code
    GetMainScreenInstance()->mBattleControl->SetWindowLeft(lLeft);
    GetMainScreenInstance()->mBattleControl->SetWindowTop(lTop);

    GetMainScreenInstance()->mRadarControl->SetWindowLeft(lLeft);
    GetMainScreenInstance()->mRadarControl->SetWindowTop(lTop);

    GetMainScreenInstance()->OnRadarChange(0);

    return "";
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcChangeUnitBattleGroup(ZCsl* csl)
{
    int lUnitId     = csl->get("unitId").asInt(); // unit id
    int lTargetBgId = csl->get("newBg").asInt();  // id of the new battlegroup

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lUnitId);

    if (!lUnit)
    {
        return 0;
    }

    // check if the target battle group exists
    CPPIG_BattleGroup* lTargetBg = GetMissionInstance()->GetBGManager()->GetBGroupById(lTargetBgId);

    if (!lTargetBg)
    {
        return 0;
    }

    int lCurrentBg = lUnit->GetBattleGroupId();

    // check if the current battlegroup exists (is the unit orphan? :)
    CPPIG_BattleGroup* lBg = GetMissionInstance()->GetBGManager()->GetBGroupById(lCurrentBg);

    if (lBg)
    {
        lBg->RemoveUnit(lUnit);
    }

    lUnit->SetBattleGroupId(lTargetBgId);
    lTargetBg->AddUnit(lUnit);

    return 1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcIsUnitDead(ZCsl* csl)
{
    int lId     = csl->get("id").asInt(); // unit id

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return -1;
    }

    return (int)(lUnit->GetIsDead());
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcUnitExists(ZCsl* csl)
{
    int lId     = csl->get("id").asInt(); // unit id

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return 0;
    }

    return 1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcGetUnitLogicX(ZCsl* csl)
{
    int lId     = csl->get("id").asInt(); // unit id

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return -1;
    }

    return lUnit->GetLogicCoord().x;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcGetUnitLogicY(ZCsl* csl)
{
    int lId     = csl->get("id").asInt(); // unit id

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return -1;
    }

    return lUnit->GetLogicCoord().y;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcGetUnitZ(ZCsl* csl)
{
    int lId     = csl->get("id").asInt(); // unit id

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return -1;
    }

    return lUnit->GetCoordZ();
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcGetUnitHP(ZCsl* csl)
{
    int lId     = csl->get("id").asInt(); // unit id

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return -1;
    }

    if (lUnit->IsDerivedFrom(IGCN_REALOBJECT))
    {
        return ((CIGRealObject*)lUnit)->GetCurrentHP();
    }

    return -1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcGetUnitLogicDimX(ZCsl* csl)
{
    int lId     = csl->get("id").asInt(); // unit id

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return -1;
    }

    return lUnit->GetLogicDimX();
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcGetUnitLogicDimY(ZCsl* csl)
{
    int lId     = csl->get("id").asInt(); // unit id

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return -1;
    }

    return lUnit->GetLogicDimY();
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcGetUnitType(ZCsl* csl)
{
    int lId     = csl->get("id").asInt(); // unit id

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return -1;
    }

    if (lUnit->IsDerivedFrom(IGCN_STATICOBJECT))
    {
        return 1;
    }

    if (lUnit->IsDerivedFrom(IGCN_INFANTRYUNIT))
    {
        return 2;
    }

    if (lUnit->IsDerivedFrom(IGCN_TANKUNIT))
    {
        return 3;
    }

    if (lUnit->IsDerivedFrom(IGCN_RECONUNIT))
    {
        return 4;
    }

    if (lUnit->IsDerivedFrom(IGCN_TRANUNIT))
    {
        return 5;
    }

    if (lUnit->IsDerivedFrom(IGCN_ARTILLERYUNIT))
    {
        return 6;
    }

    if (lUnit->IsDerivedFrom(IGCN_FIGHTERUNIT))
    {
        return 7;
    }

    if (lUnit->IsDerivedFrom(IGCN_BOMBERUNIT))
    {
        return 8;
    }

    if (lUnit->IsDerivedFrom(IGCN_CHOPPERUNIT))
    {
        return 9;
    }

    if (lUnit->IsDerivedFrom(IGCN_DROPSHIPUNIT))
    {
        return 10;
    }

    // unknown unit type
    return -1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcChangeUnitScript(ZCsl* csl)
{
    int lId              = csl->get("id").asInt(); // unit id
    CPString lScriptFile = CPString(csl->get("scriptFile").buffer()); // script file name for behavior

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return 0;
    }

    if (lUnit->IsDerivedFrom(IGCN_INFANTRYUNIT))
    {
        ((CIGInfantryUnit*)lUnit)->SetScriptObserver(GetMissionInstance()->GetDoctrine()->AddScriptFromFile(((CIGInfantryUnit*)lUnit)->GetScriptObserver(), PATH_SCRIPTS_INFANTRY + lScriptFile ) );

        return 1;
    }

    if (lUnit->IsDerivedFrom(IGCN_VEHICLEUNIT))
    {
        ((CIGVehicle*)lUnit)->SetScriptObserver(GetMissionInstance()->GetDoctrine()->AddScriptFromFile(((CIGVehicle*)lUnit)->GetScriptObserver(), PATH_SCRIPTS_VEHICLE + lScriptFile ) );

        return 1;
    }

    if (lUnit->IsDerivedFrom(IGCN_AIRUNIT))
    {
        ((CIGAircraft*)lUnit)->SetScriptObserver(GetMissionInstance()->GetDoctrine()->AddScriptFromFile(((CIGAircraft*)lUnit)->GetScriptObserver(), PATH_SCRIPTS_AIRCRAFT + lScriptFile ) );

        return 1;
    }

    // unit type is not able to support scripts
    return 0;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcGetUnitBattleGroup(ZCsl* csl)
{
    int lId              = csl->get("id").asInt(); // unit id

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return -1;
    }

    return lUnit->GetBattleGroupId();
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcSetUnitScriptVar(ZCsl* csl)
{
    int lId     = csl->get("id").asInt(); // unit id

    CIGBaseObject* lObj = GetMissionInstance()->GetObjectById(lId);

    if (!lObj)
    {
        return 0;
    }

    if (!lObj->IsDerivedFrom(IGCN_UNIT))
    {
        return 0;
    }

    ZString varName  = csl->get("varname");
    ZString varValue = csl->get("value");

    CIGUnit* lUnit = (CIGUnit*)lObj;

    lUnit->SetScriptVar(CPString(varName.buffer()), CPString(varValue.buffer()));

    return 1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcUnitsTransportedCount(ZCsl* csl)
{
    int lId = csl->get("transportId").asInt(); // unit id

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return 0;
    }

    if (lUnit->ClassName() == CPString(IGCN_DROPSHIPUNIT))
    {
        return ((CIGDropship*)lUnit)->GetCapacity();
    }

    return -1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcGetUnitTransported(ZCsl* csl)
{
    return -1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcCanCarryInfantry(ZCsl* csl)
{
    int lId              = csl->get("id").asInt(); // unit id

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return -1;
    }

    return (int)lUnit->Can(CAN_CARRY_INFANTRY);
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcCanCarryVehicles(ZCsl* csl)
{
    int lId              = csl->get("id").asInt(); // unit id

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return -1;
    }

    return (int)lUnit->Can(CAN_CARRY_VEHICLES);
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcMove(ZCsl* csl)
{
    int lId = csl->get("id").asInt(); // unit id
    int lX  = csl->get("x").asInt();  // where to x
    int lY  = csl->get("y").asInt();  // where to y

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return 0;
    }

    // issue a move order
    TIGSubSysActionData lTmpActionData;

    lTmpActionData.mType          = OA_MOVE;
    lTmpActionData.mTargetPoint.x = lX * MAPCELLX;
    lTmpActionData.mTargetPoint.y = lY * MAPCELLY;

    lUnit->SetAction(lTmpActionData);

    return 1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcAttack(ZCsl* csl)
{
    int lId       = csl->get("id").asInt(); // unit id
    int lVictimId = csl->get("victimId").asInt(); // unit id

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return 0;
    }

    CIGBaseObject* lVictimUnit = GetMissionInstance()->GetObjectById(lVictimId);

    if (!lVictimUnit)
    {
        return 0;
    }

    // there are no politics/bg checks - it's allowed to attack any unit
    // fill up the action data structure
    TIGSubSysActionData lTmpActionData;

    lTmpActionData.mType          = OA_ATTACK;
    lTmpActionData.mTargetObject  = lVictimUnit;
    lTmpActionData.mTargetPoint   = lVictimUnit->GetCoord();

    lUnit->SetAction(lTmpActionData);

    return 1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcAttackGround(ZCsl* csl)
{
    int lId = csl->get("id").asInt(); // unit id
    int lX  = csl->get("x").asInt();  // where to x
    int lY  = csl->get("y").asInt();  // where to y

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return 0;
    }

    // issue a attack ground order
    TIGSubSysActionData lTmpActionData;

    lTmpActionData.mType         = OA_ATTACKGROUND;
    lTmpActionData.mTargetPoint  = CPPoint(lX * MAPCELLX, lY * MAPCELLY);
    lTmpActionData.mTargetObject = 0;

    lUnit->SetAction(lTmpActionData);

    return 1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcStop(ZCsl* csl)
{
    int lId = csl->get("id").asInt(); // unit id

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return 0;
    }

    // issue a attack ground order
    TIGSubSysActionData lTmpActionData;

    lTmpActionData.mType = OA_STOP;

    lUnit->SetAction(lTmpActionData);

    return 1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcLoad(ZCsl* csl)
{
    int lId          = csl->get("id").asInt(); // unit id
    int lTransportId = csl->get("transportId").asInt(); // transporter unit id

    // check the existence of units and the validity of load operation
    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return 0;
    }

    CIGBaseObject* lTransportUnit = GetMissionInstance()->GetObjectById(lTransportId);

    if (!lTransportUnit)
    {
        return 0;
    }

    // it doesn't matter, but we need to return a success value
    if (lTransportUnit->ClassName() != CPString(IGCN_TRANUNIT))
    {
        return 0;
    }

    // check cases of loading
    if (lTransportUnit->Can(CAN_CARRY_INFANTRY) && lUnit->ClassName() == CPString(IGCN_INFANTRYUNIT))
    {
        // issue a load order
        TIGSubSysActionData lTmpActionData;

        lTmpActionData.mType         = OA_LOAD;
        lTmpActionData.mTargetObject = lTransportUnit;
        lTmpActionData.mTargetPoint  = lTransportUnit->GetCoord();

        lUnit->SetAction(lTmpActionData);

        return 1;
    }

    return 0;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcUnloadAll(ZCsl* csl)
{
    int lId = csl->get("transportId").asInt(); // unit id
    int lX  = csl->get("x").asInt();  // where to x
    int lY  = csl->get("y").asInt();  // where to y

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return 0;
    }

    // it doesn't matter, but we need to return a success value
    if (lUnit->ClassName() != CPString(IGCN_TRANUNIT))
    {
        return 0;
    }

    // issue a patrol order
    TIGSubSysActionData lTmpActionData;

    lTmpActionData.mType         = OA_UNLOAD_ALL;
    lTmpActionData.mTargetObject = 0;
    lTmpActionData.mTargetPoint  = CPPoint(lX * MAPCELLX, lY * MAPCELLY);

    lUnit->SetAction(lTmpActionData);

    return 1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcPatrol(ZCsl* csl)
{
    int lId = csl->get("id").asInt(); // unit id
    int lX  = csl->get("x").asInt();  // where to x
    int lY  = csl->get("y").asInt();  // where to y

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return 0;
    }

    // issue a patrol order
    TIGSubSysActionData lTmpActionData;

    lTmpActionData.mType        = OA_PATROL;
    lTmpActionData.mTargetPoint = CPPoint(lX * MAPCELLX, lY * MAPCELLY);

    lUnit->SetAction(lTmpActionData);

    return 1;
}
//---------------------------------------------------------------------------

// TODO: we could also restrict the land action only to chopper/dropships
ZString CPPIG_MissionControl::mcLand(ZCsl* csl)
{
    int lId = csl->get("id").asInt(); // unit id
    int lX  = csl->get("x").asInt();  // where to x
    int lY  = csl->get("y").asInt();  // where to y

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return 0;
    }

    // issue a land order
    TIGSubSysActionData lTmpActionData;

    lTmpActionData.mType        = OA_LAND;
    lTmpActionData.mTargetPoint = CPPoint(lX * MAPCELLX, lY * MAPCELLY);

    lUnit->SetAction(lTmpActionData);

    return 1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcTacticMove(ZCsl* csl)
{
    int lId = csl->get("id").asInt(); // unit id
    int lX  = csl->get("x").asInt();  // where to x
    int lY  = csl->get("y").asInt();  // where to y

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return 0;
    }

    // issue a move order
    TIGSubSysActionData lTmpActionData;

    lTmpActionData.mType          = OA_TACTIC_MOVE;
    lTmpActionData.mTargetPoint.x = lX * MAPCELLX;
    lTmpActionData.mTargetPoint.y = lY * MAPCELLY;

    lUnit->SetAction(lTmpActionData);

    return 1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcTacticAttack(ZCsl* csl)
{
    int lId       = csl->get("id").asInt(); // unit id
    int lVictimId = csl->get("victimId").asInt(); // unit id

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return 0;
    }

    CIGBaseObject* lVictimUnit = GetMissionInstance()->GetObjectById(lVictimId);

    if (!lVictimUnit)
    {
        return 0;
    }

    // there are no politics/bg checks - it's allowed to attack any unit
    // fill up the action data structure
    TIGSubSysActionData lTmpActionData;

    lTmpActionData.mType          = OA_TACTIC_ATTACK;
    lTmpActionData.mTargetObject  = lVictimUnit;
    lTmpActionData.mTargetPoint   = lVictimUnit->GetCoord();

    lUnit->SetAction(lTmpActionData);

    return 1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcTacticAttackGround(ZCsl* csl)
{
    int lId = csl->get("id").asInt(); // unit id
    int lX  = csl->get("x").asInt();  // where to x
    int lY  = csl->get("y").asInt();  // where to y

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return 0;
    }

    // issue a attack ground order
    TIGSubSysActionData lTmpActionData;

    lTmpActionData.mType         = OA_TACTIC_ATTACKGROUND;
    lTmpActionData.mTargetPoint  = CPPoint(lX * MAPCELLX, lY * MAPCELLY);
    lTmpActionData.mTargetObject = 0;

    lUnit->SetAction(lTmpActionData);

    return 1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcTacticStop(ZCsl* csl)
{
    int lId = csl->get("id").asInt(); // unit id

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return 0;
    }

    // issue a attack ground order
    TIGSubSysActionData lTmpActionData;

    lTmpActionData.mType = OA_TACTIC_STOP;

    lUnit->SetAction(lTmpActionData);

    return 1;
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcMissionTime(ZCsl* csl)
{
    CPString lStr = ZString(GetMissionInstance()->GetMissionTime()).buffer();

    //return ZString(GetMissionInstance()->GetMissionTime());
    return GetMissionInstance()->GetMissionTime();
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcPostMessage(ZCsl* csl)
{
    CPString lMsg = CPString(csl->get("msg").buffer());

    GetMainScreenInstance()->AddMessage(lMsg + "\n");
    return "";
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcEndMission(ZCsl* csl)
{
    //GetExitInstance()->SetTerminated(true);

    return "";
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcPause(ZCsl* csl)
{
    int lState = csl->get("state").asInt(); // 0 - unpause, 1 - pause

    GetMissionInstance()->SetEnabled(bool(lState));

    return "";
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcMessageConsole(ZCsl* csl)
{
    return "";
}
//---------------------------------------------------------------------------

ZString CPPIG_MissionControl::mcFocusOnObject(ZCsl* csl)
{
    int lId = csl->get("id").asInt(); // unit id

    CIGBaseObject* lUnit = GetMissionInstance()->GetObjectById(lId);

    if (!lUnit)
    {
        return "";
    }

    // -1 means resseting the focus to nothing
    if (lId == -1)
    {
        GetMissionInstance()->SetFocusObject(0);
    }
    else
    {
        GetMissionInstance()->SetFocusObject(lUnit);
    }

    return "";
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//
// Serialization/Deserialization
//
//---------------------------------------------------------------------------

void CPPIG_MissionControl::Serialize(PP::Stream& _a)
{
    CSerString lModuleName((char*)mModuleName);
    int        lScriptId = -1, lTicks = GetTimerInstance()->GetTime() - mTicks; // quite a hack, heh?

    // setup
    if (mScriptObserver)
    {
        lScriptId    = mScriptObserver->GetScriptId();
    }

    // serialize
    lModuleName.Serialize(_a);
    _a.writeInt(&lScriptId);
    _a.writeInt(&lTicks);
}
//---------------------------------------------------------------------------

unsigned __int32 CPPIG_MissionControl::DataSize()
{
    return 2 * sizeof(__int32);
}
//---------------------------------------------------------------------------

void CPPIG_MissionControl::DeSerialize(PP::Stream& _a)
{
    CSerString lModuleName;
    int        lScriptId, lTicks;

    GetMissionInstance()->GetDoctrine()->RemoveScriptObserver(mScriptObserver);
    mScriptObserver = NULL;
    mValidCsl       = false;

    // deserialize
    lModuleName.DeSerialize(_a);
    _a.readInt(&lScriptId);
    _a.readInt(&lTicks);

    // setup
    mModuleName = ZString(lModuleName.Get().c_str());
    if (lScriptId != -1)
    {
        SetScriptObserver(GetMissionInstance()->GetDoctrine()->GetScriptObserver(lScriptId));
    }
    mTicks = GetTimerInstance()->GetTime() - lTicks;
}
//---------------------------------------------------------------------------

void CPPIG_MissionControl::Serialize(BaseSerializer& out)
{
    // setup
    int lScriptId = -1, lTicks = GetTimerInstance()->GetTime() - mTicks; // quite a hack, heh?

    if (mScriptObserver)
    {
        lScriptId = mScriptObserver->GetScriptId();
    }

    // serialize
    out.beginElement("missioncontrol");

    out.createAttribute("module", (char*)mModuleName);
    out.createAttribute("scriptId", lScriptId);
    out.createAttribute("ticks", lTicks);

    out.endElement();
}
//---------------------------------------------------------------------------

void CPPIG_MissionControl::DeSerialize(BaseDeSerializer& in)
{
    // TODO
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

