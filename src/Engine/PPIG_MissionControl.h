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

//-----------------------------------------------------------------------------
//  Unit              : Mission Control
//
//  Version           : 1.0
//
//  Description       :
//    * handles the mission scripting
//    * separated from CIGMission so that it wont bloat the already huge CIGMission class
//
//  History           :
//    [20.12.2005] - [Karg] - added tactical orders
//-----------------------------------------------------------------------------

#ifndef PPIG_MissionControlH
#define PPIG_MissionControlH
//---------------------------------------------------------------------------

#include "ZCsl.hpp"
#include "PP_String.h"
#include "PPIG_Doctrine.h"

class CPPIG_MissionControl : public CSerializable, public Serializable2
{
  public:
    CPPIG_MissionControl();

    void Update();

    ~CPPIG_MissionControl();

    // get-set
    void                  SetScriptObserver(CPPIG_ScriptObserver* _obs);
    CPPIG_ScriptObserver* GetScriptObserver() { return mScriptObserver; };

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

    virtual void Serialize(BaseSerializer& out);
    virtual void DeSerialize(BaseDeSerializer& in);

  protected:

  private:
    CPPIG_ScriptObserver* mScriptObserver;
    ZString  mModuleName;
    bool     mValidCsl;
    ZCsl*    mCsl;
    int      mTicks;

    void InitCsl();
    void InitHandlers();

  private:
    // handlers
    void addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback);

    // insertion
    ZCsl_callback_t mHandleInsertInfantryAtPoint;
    ZCsl_callback_t mHandleInsertVehicleAtPoint;
    ZCsl_callback_t mHandleInsertAircraftAtPoint;

    // modifications

    // deletion
    ZCsl_callback_t mHandleKillUnit;
    ZCsl_callback_t mHandleRemoveUnit;

    // battlegroup
    ZCsl_callback_t mHandleGetBattleGroupCount;
    ZCsl_callback_t mHandleGetBattleGroup;
    ZCsl_callback_t mHandleGetBattleGroupUnitCount;
    ZCsl_callback_t mHandleGetBattleGroupUnit;
    ZCsl_callback_t mHandleIsUnitInBattleGroup;
    ZCsl_callback_t mHandleChangePolitics;
    ZCsl_callback_t mHandleGetPolitics;
    ZCsl_callback_t mHandleCreateBattleGroup;
    ZCsl_callback_t mHandleRemoveBattleGroup;
    ZCsl_callback_t mHandleRelinquishCommand;

    // map
    ZCsl_callback_t mHandleGetWidth;
    ZCsl_callback_t mHandleGetHeight;
    ZCsl_callback_t mHandleGetUnitAtPoint;
    ZCsl_callback_t mHandleGetAltitude;
    ZCsl_callback_t mHandleSetAltitude;
    ZCsl_callback_t mHandleGetTileType;
    ZCsl_callback_t mHandleGetVisibility;
    ZCsl_callback_t mHandleCanBePlaced;
    ZCsl_callback_t mHandleNotifyVisibilityMap;
    ZCsl_callback_t mHandleNotifyCircleVisibilityMap;
    ZCsl_callback_t mHandleMoveCamera;

    // Unit
    ZCsl_callback_t mHandleChangeUnitBattleGroup;
    ZCsl_callback_t mHandleIsUnitDead;
    ZCsl_callback_t mHandleUnitExists;
    ZCsl_callback_t mHandleGetUnitLogicX;
    ZCsl_callback_t mHandleGetUnitLogicY;
    ZCsl_callback_t mHandleGetUnitZ;
    ZCsl_callback_t mHandleGetUnitHP;
    ZCsl_callback_t mHandleGetUnitLogicDimX;
    ZCsl_callback_t mHandleGetUnitLogicDimY;
    ZCsl_callback_t mHandleGetUnitType;
    ZCsl_callback_t mHandleChangeUnitScript;
    ZCsl_callback_t mHandleGetUnitBattleGroup;
    ZCsl_callback_t mHandleSetUnitScriptVar;

    // Transporter
    ZCsl_callback_t mHandleUnitsTransportedCount;
    ZCsl_callback_t mHandleGetUnitTransported;
    ZCsl_callback_t mHandleCanCarryInfantry;
    ZCsl_callback_t mHandleCanCarryVehicles;

    // Unit orders
    ZCsl_callback_t mHandleMove;
    ZCsl_callback_t mHandleAttack;
    ZCsl_callback_t mHandleAttackGround;
    ZCsl_callback_t mHandleStop;
    ZCsl_callback_t mHandleLoad;
    ZCsl_callback_t mHandleUnloadAll;
    ZCsl_callback_t mHandlePatrol;
    ZCsl_callback_t mHandleLand;

    // Unit orders (tactical)
    ZCsl_callback_t  mHandleTacticMove;
    ZCsl_callback_t  mHandleTacticAttackGround;
    ZCsl_callback_t  mHandleTacticAttack;
    ZCsl_callback_t  mHandleTacticStop;

    // Misc
    ZCsl_callback_t mHandleMissionTime;
    ZCsl_callback_t mHandlePostMessage;
    ZCsl_callback_t mHandleEndMission;
    ZCsl_callback_t mHandlePause;
    ZCsl_callback_t mHandleMessageConsole;
    ZCsl_callback_t mHandleFocusOnObject;

    // implementations

    // insertion
    ZString mcInsertInfantryAtPoint(ZCsl* csl);
    ZString mcInsertVehicleAtPoint(ZCsl* csl);
    ZString mcInsertAircraftAtPoint(ZCsl* csl);

    // modifications

    // deletion
    ZString mcKillUnit(ZCsl* csl);
    ZString mcRemoveUnit(ZCsl* csl);

    // battlegroup
    ZString mcGetBattleGroupCount(ZCsl* csl);
    ZString mcGetBattleGroup(ZCsl* csl);
    ZString mcGetBattleGroupUnitCount(ZCsl* csl);
    ZString mcGetBattleGroupUnit(ZCsl* csl);
    ZString mcIsUnitInBattleGroup(ZCsl* csl);
    ZString mcChangePolitics(ZCsl* csl);
    ZString mcGetPolitics(ZCsl* csl);
    ZString mcCreateBattleGroup(ZCsl* csl);
    ZString mcRemoveBattleGroup(ZCsl* csl);
    ZString mcRelinquishCommand(ZCsl* csl);

    // map
    ZString mcGetWidth(ZCsl* csl);
    ZString mcGetHeight(ZCsl* csl);
    ZString mcGetUnitAtPoint(ZCsl* csl);
    ZString mcGetAltitude(ZCsl* csl);
    ZString mcSetAltitude(ZCsl* csl);
    ZString mcGetTileType(ZCsl* csl);
    ZString mcGetVisibility(ZCsl* csl);
    ZString mcCanBePlaced(ZCsl* csl);
    ZString mcNotifyVisibilityMap(ZCsl* csl);
    ZString mcNotifyCircleVisibilityMap(ZCsl* csl);
    ZString mcMoveCamera(ZCsl* csl);

    // Unit
    ZString mcChangeUnitBattleGroup(ZCsl* csl);
    ZString mcIsUnitDead(ZCsl* csl);
    ZString mcUnitExists(ZCsl* csl);
    ZString mcGetUnitLogicX(ZCsl* csl);
    ZString mcGetUnitLogicY(ZCsl* csl);
    ZString mcGetUnitZ(ZCsl* csl);
    ZString mcGetUnitHP(ZCsl* csl);
    ZString mcGetUnitLogicDimX(ZCsl* csl);
    ZString mcGetUnitLogicDimY(ZCsl* csl);
    ZString mcGetUnitType(ZCsl* csl);
    ZString mcChangeUnitScript(ZCsl* csl);
    ZString mcGetUnitBattleGroup(ZCsl* csl);
    ZString mcSetUnitScriptVar(ZCsl* csl);

    // Transporter
    ZString mcUnitsTransportedCount(ZCsl* csl);
    ZString mcGetUnitTransported(ZCsl* csl);
    ZString mcCanCarryInfantry(ZCsl* csl);
    ZString mcCanCarryVehicles(ZCsl* csl);

    // Unit orders
    ZString mcMove(ZCsl* csl);
    ZString mcAttack(ZCsl* csl);
    ZString mcAttackGround(ZCsl* csl);
    ZString mcStop(ZCsl* csl);
    ZString mcLoad(ZCsl* csl);
    ZString mcUnloadAll(ZCsl* csl);
    ZString mcPatrol(ZCsl* csl);
    ZString mcLand(ZCsl* csl);

    // Unit orders (tactical)
    ZString mcTacticMove(ZCsl* csl);
    ZString mcTacticAttack(ZCsl* csl);
    ZString mcTacticAttackGround(ZCsl* csl);
    ZString mcTacticStop(ZCsl* csl);

    // misc
    ZString mcMissionTime(ZCsl* csl);
    ZString mcPostMessage(ZCsl* csl);
    ZString mcEndMission(ZCsl* csl);
    ZString mcPause(ZCsl* csl);
    ZString mcMessageConsole(ZCsl* csl);
    ZString mcFocusOnObject(ZCsl* csl);
};

#endif
