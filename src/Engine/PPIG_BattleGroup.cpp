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
//#else
//#include <strstrea.h>	// TMN: Note use of obsolete header
//#endif
#pragma hdrstop

#if PPT_COMPILER_MICROSOFT
// keeping compatibilty with msvc 6...blindly; if this wont work on msvc 6 please inform me
#if (_MSC_VER >= 1400)
#include <sstream>
#else
#include <strstream>
#endif
#else
#include <sstream>
#endif

#include <algorithm>
using namespace std;

#include "config.h"
#include "PPIG_Mission.h"
//#include "PPIG_CScreen.h"
#include "PPIG_BattleGroup.h"
#include "PPIG_Map.h"
#include "PP_Stdlib.h"
#include "PP_Timer.h"
#include "PPIG_ObjReal.h"
#include "PPIG_ObjInfantry.h"

#include "ZMathLib.h"
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CPPIG_BattleGroup::CPPIG_BattleGroup()
{
    // default members init
    mName       = "BattleGroup";
    mId         = gfRandom(1000);
    mCompiler   = NULL;
    mValidCsl   = false;
    mScriptObserver = NULL;
    mModuleName = "BGCompiler";
    mUnits      = new CPList<CIGBaseObject>(2);
    mEnemy      = new CPList<CIGBaseObject>(2);
    mToAttack   = new CPList<CIGBaseObject>(2);

    mTicks       = GetTimerInstance()->GetTime();
    mTargets     = 0;

    InitHandlers();
    InitCsl();
}
//---------------------------------------------------------------------------

CPPIG_BattleGroup::CPPIG_BattleGroup(CPString _name, CPString _info, int _id)
{
    mName       = _name;
    mId         = _id;
    mCompiler   = NULL;
    mValidCsl   = false;
    mScriptObserver = NULL;
    mModuleName = "BGCompiler";
    mUnits      = new CPList<CIGBaseObject>(2);
    mEnemy      = new CPList<CIGBaseObject>(2);
    mToAttack   = new CPList<CIGBaseObject>(2);

    InitHandlers();
    InitCsl();
}
//---------------------------------------------------------------------------

CPPIG_BattleGroup::~CPPIG_BattleGroup()
{
    delete mUnits;
    delete mEnemy;
    delete mToAttack;

    if (mValidCsl)
    {
        delete mCompiler;
    }
}
//---------------------------------------------------------------------------

void CPPIG_BattleGroup::addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback)
{
    mCompiler->addFunc(mModuleName, szMemberName, callback);
}
//---------------------------------------------------------------------------

void CPPIG_BattleGroup::InitHandlers()
{
    // handlers init
    mHandleMissionTime        .set(this, &CPPIG_BattleGroup::mcMissionTime);
    mHandleIsUnitDead         .set(this, &CPPIG_BattleGroup::mcIsUnitDead);
    mHandleZoneEvent          .set(this, &CPPIG_BattleGroup::mcZoneEvent);
    mHandleZoneEvaluateThreat .set(this, &CPPIG_BattleGroup::mcZoneEvaluateThreat);
    mHandlePostMessage        .set(this, &CPPIG_BattleGroup::mcPostMessage);
    mHandleUnitsCount         .set(this, &CPPIG_BattleGroup::mcUnitsCount);
    mHandleUnitStatus         .set(this, &CPPIG_BattleGroup::mcUnitStatus);
    mHandleMove               .set(this, &CPPIG_BattleGroup::mcMove);
    mHandleUnitX              .set(this, &CPPIG_BattleGroup::mcUnitX);
    mHandleUnitY              .set(this, &CPPIG_BattleGroup::mcUnitY);
    mHandleUnitSense          .set(this, &CPPIG_BattleGroup::mcUnitSense);
    mHandleAttackRandomEnemy  .set(this, &CPPIG_BattleGroup::mcAttackRandomEnemy);

    mHandleSeekEnemies        .set(this, &CPPIG_BattleGroup::mcSeekEnemies);
    mHandleSetTargets         .set(this, &CPPIG_BattleGroup::mcSetTargets);
    mHandleToAttackCount      .set(this, &CPPIG_BattleGroup::mcToAttackCount);
    mHandleDirFromPoints      .set(this, &CPPIG_BattleGroup::mcDirFromPoints);
}
//---------------------------------------------------------------------------

void CPPIG_BattleGroup::InitCsl()
{
    CPString lScript;

    if (mValidCsl)
    {
        delete mCompiler;
    }

    mValidCsl = false;

    if (!mScriptObserver)
    {
        return;
    }

    try
    {
        mCompiler = new ZCsl();
    }
    catch(...)
    {
        LOG(ERROR) << "CPPIG_BattleGroup::InitCsl - errors initializing script compiler";
        return;
    }

    try
    {
        // load math library
        ZMathLib* lMathLib = new ZMathLib(mCompiler, "Math Library");
        lMathLib->InitLibrary();
        mCompiler->loadLibrary(lMathLib);

        // add functions
        addModuleFunc("MissionTime()", mHandleMissionTime);
        //addModuleFunc("IsUnitDead(const id)", mHandleIsUnitDead);
        //addModuleFunc("ZoneEvent(const xs, const ys, const xe, const ye)", mHandleZoneEvent);
        //addModuleFunc("ZoneEvaluateThreat(const xs, const ys, const xe, const ye)", mHandleZoneEvaluateThreat);
        addModuleFunc("PostMessage(const msg)", mHandlePostMessage);

        addModuleFunc("UnitsCount()",                           mHandleUnitsCount );
        addModuleFunc("UnitStatus(const index)",                mHandleUnitStatus );
        addModuleFunc("Move(const index, const x, const y)",    mHandleMove       );
        addModuleFunc("UnitX(const index)",                     mHandleUnitX      );
        addModuleFunc("UnitY(const index)",                     mHandleUnitY      );
        addModuleFunc("UnitSense(const index)",                 mHandleUnitSense  );

        addModuleFunc("AttackRandomEnemy()", mHandleAttackRandomEnemy);
        addModuleFunc("SeekEnemies()", mHandleSeekEnemies);
        addModuleFunc("SetTargets(const no)", mHandleSetTargets);
        addModuleFunc("ToAttackCount()", mHandleToAttackCount);
        addModuleFunc("DirFromPoints(const x1, const y1, const x2, const y2)", mHandleDirFromPoints);
    }
    catch(...)
    {
        delete mCompiler;
    }

    lScript = mScriptObserver->GetScript();

    try
    {
        // The const_cast is a workaround for a Microsoft interface bug.
#if PPT_COMPILER_MICROSOFT
#if (_MSC_VER >= 1400)
        istringstream str(const_cast<char*>(lScript.c_str()));
#else
        istrstream str(const_cast<char*>(lScript.c_str()));
#endif
#else
        istringstream str(const_cast<char*>(lScript.c_str()));
#endif

        // compile
        mCompiler->loadScript(mModuleName, &str);

        mValidCsl = true;
    }
    catch(const ZException& err)
    {
        LOG(ERROR) << "CPPIG_BattleGroup::InitCsl - errors compiling script";
        for (int i = 0; i < err.count(); i++)
        {
            LOG(ERROR) << (char*)err[i];
        }
    }
}
//---------------------------------------------------------------------------

void CPPIG_BattleGroup::Update()
{
    // update script three time per second
    if (GetTimerInstance()->GetTime() - mTicks > 333)
    {
        mTicks = GetTimerInstance()->GetTime();

        if (mValidCsl)
        {
            try
            {
                mCompiler->call(mModuleName, "main");
            }
            catch(const ZException& err)
            {
                LOG(ERROR) << "CPPIG_BattleGroup::Update - error(s) calling main function";
                for (int i = 0; i < err.count(); i++)
                {
                    LOG(ERROR) << (char*)err[i];
                }
            }
        }
    }

    // TODO: correct this code
    for(int i = 0; i < mToAttack->Count(); i++)
    {
        if (mToAttack->Get(i)->GetIsDead())
        {
            mToAttack->Remove(i);
            continue;
        }

        bool lInRange = false;

        for(int j = 0; j < mUnits->Count(); j++)
        {
            if (!mUnits->Get(j)->GetIsDead())
            {
                if (gfGetDist2D(mToAttack->Get(i)->GetLogicCoord(), mUnits->Get(j)->GetLogicCoord()) < 8)
                {
                    lInRange = true;
                    break;
                }
            }
        }

        if (!lInRange)
        {
            mToAttack->Remove(i);
        }
    }
}
//---------------------------------------------------------------------------

void CPPIG_BattleGroup::SetScriptObserver(CPPIG_ScriptObserver* _obs)
{
    mScriptObserver = _obs;

    InitCsl();
}
//---------------------------------------------------------------------------

bool CPPIG_BattleGroup::AddUnit    (CIGBaseObject* _unit)
{
    if (mUnits->Contain(_unit))
    {
        return false;
    }

    mUnits->Add(_unit);
    _unit->SetBattleGroupId(mId);
    return true;
}
//---------------------------------------------------------------------------

bool CPPIG_BattleGroup::RemoveUnit (CIGBaseObject* _unit)
{
    if (!mUnits->Contain(_unit))
    {
        return false;
    }

    for(int i = 0; i < mUnits->Count(); i++)
    {
        if (mUnits->Get(i) == _unit)
        {
            mUnits->Remove(i);
            return true;
        }
    }

    // will never happen
    return false;
}
//---------------------------------------------------------------------------

void CPPIG_BattleGroup::SetId(int _v)
{
    mId = _v;

    for(int i = 0; i < mUnits->Count(); i++)
    {
        mUnits->Get(i)->SetBattleGroupId(mId);
    }
}

bool CPPIG_BattleGroup::ValidUnitIndex(int _index)
{
    return (_index >= 0 && _index < mUnits->Count());
}
//---------------------------------------------------------------------------

CIGBaseObject* CPPIG_BattleGroup::GetUnitByIndex(int _index)
{
    if (!ValidUnitIndex(_index))
    {
        return 0;
    }

    return mUnits->Get(_index);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
// Implementation of script methods follows
//
//---------------------------------------------------------------------------

ZString CPPIG_BattleGroup::mcMissionTime(ZCsl* csl)
{
    return ZString(GetMissionInstance()->GetMissionTime());
}
//---------------------------------------------------------------------------

ZString CPPIG_BattleGroup::mcIsUnitDead(ZCsl* csl)
{
    return "";
}
//---------------------------------------------------------------------------

ZString CPPIG_BattleGroup::mcZoneEvent(ZCsl* csl)
{
    return "";
}
//---------------------------------------------------------------------------

ZString CPPIG_BattleGroup::mcZoneEvaluateThreat(ZCsl* csl)
{
    return "";
}
//---------------------------------------------------------------------------

ZString CPPIG_BattleGroup::mcPostMessage(ZCsl* csl)
{
    CPString lMsg = CPString(csl->get("msg").buffer());

    //GetMainScreenInstance()->AddMessage(lMsg + "\n");

    return "";
}
//---------------------------------------------------------------------------

ZString CPPIG_BattleGroup::mcUnitsCount(ZCsl* csl)
{
    return mUnits->Count();
}
//---------------------------------------------------------------------------

ZString CPPIG_BattleGroup::mcUnitStatus(ZCsl* csl)
{
    int lIndex = csl->get("index").asInt();

    if (!ValidUnitIndex(lIndex))
    {
        return -1;
    }

    // get object
    CIGBaseObject* lObj = mUnits->Get(lIndex);

    // return -1 if it's a null
    if (!lObj)
    {
        return -1;
    }

    if (lObj->GetIsDead())
    {
        return 1;
    }

    if (lObj->GetMoving())
    {
        return 2;
    }

    // standing or firing
    return 3;
}
//---------------------------------------------------------------------------

ZString CPPIG_BattleGroup::mcMove(ZCsl* csl)
{
    int lIndex = csl->get("index").asInt(); // unit id
    int lX  = csl->get("x").asInt();  // where to x
    int lY  = csl->get("y").asInt();  // where to y


    if (!ValidUnitIndex(lIndex))
    {
        return 0;
    }

    CIGBaseObject* lUnit = mUnits->Get(lIndex);

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

ZString CPPIG_BattleGroup::mcUnitX(ZCsl* csl)
{
    int lIndex = csl->get("index").asInt(); // unit id

    if (!ValidUnitIndex(lIndex))
    {
        return -1;
    }

    CIGBaseObject* lUnit = mUnits->Get(lIndex);

    if (!lUnit)
    {
        return -1;
    }

    return lUnit->GetLogicCoord().x;
}
//---------------------------------------------------------------------------

ZString CPPIG_BattleGroup::mcUnitY(ZCsl* csl)
{
    int lIndex = csl->get("index").asInt(); // unit id

    if (!ValidUnitIndex(lIndex))
    {
        return -1;
    }

    CIGBaseObject* lUnit = mUnits->Get(lIndex);

    if (!lUnit)
    {
        return -1;
    }

    return lUnit->GetLogicCoord().y;
}
//---------------------------------------------------------------------------

ZString CPPIG_BattleGroup::mcUnitSense(ZCsl* csl)
{
    int lIndex = csl->get("index").asInt(); // unit id

    if (!ValidUnitIndex(lIndex))
    {
        return -1;
    }

    CIGBaseObject* lUnit = mUnits->Get(lIndex);

    if (!lUnit)
    {
        return -1;
    }

    if (lUnit->ClassName() == CPString(IGCN_INFANTRYUNIT))
    {
        return ((CIGInfantryUnit*)lUnit)->GetSenseCounter();
    }

    return -1;
}
//---------------------------------------------------------------------------

ZString CPPIG_BattleGroup::mcSeekEnemies(ZCsl* csl)
{
    CPPoint lCoord;
    CIGBaseObject* lObj;

    mEnemy->Clear();
    //mToAttack->Clear();

    for(int i = 0; i < mUnits->Count(); i++)
    {
        lCoord = mUnits->Get(i)->GetLogicCoord();
        for(int x = lCoord.x - 8; x < lCoord.x + 8; x++)
        {
            for(int y = lCoord.y - 8; y < lCoord.y + 8; y++)
            {
                if (((CIGRealObject*)(mUnits->Get(i)))->IsEnemy(x, y))
                {
                    lObj = GetMissionInstance()->GetMap()->GetGroundObject(x, y);

                    if (!mEnemy->Contain(lObj) && !lObj->GetIsDead())
                    {
                        mEnemy->Add(lObj);
                    }
                }
            }
        }
    }

    VLOG(9) << "Seen enemies: " << mEnemy->Count();
    return mEnemy->Count();
}
//---------------------------------------------------------------------------

ZString CPPIG_BattleGroup::mcAttackRandomEnemy(ZCsl* csl)
{
    CIGBaseObject* lObj;

    if ((mUnits->Count() <= 0) || (mEnemy->Count() <= 0))
    {
        return "";
    }

    if (mTargets <= 0)
    {
        mTargets = 1;
    }

    mTargets = 2;

    int lMaxTargets = min(mTargets, mEnemy->Count()), lTargets = 0;

    for(int i = 0; i < mUnits->Count(); i++)
    {
        lObj = mEnemy->Get(lTargets);

        if (!lObj)
        {
            continue;
        }

        if (!mToAttack->Contain(lObj))
        {
            mToAttack->Add(lObj);
        }

        TIGSubSysActionData lTmpActionData;

        lTmpActionData.mTargetObject = lObj;
        lTmpActionData.mType         = OA_ATTACK;
        lTmpActionData.mTargetPoint  = lObj->GetLogicCoord();

        mUnits->Get(i)->AddAction(lTmpActionData);

        lTargets += 1;
        lTargets %= lMaxTargets;
    }

    return "";
}
//---------------------------------------------------------------------------

ZString CPPIG_BattleGroup::mcSetTargets(ZCsl* csl)
{
    ZString lVal = csl->get("no");

    mTargets = lVal.asInt();

    return "";
}
//---------------------------------------------------------------------------

ZString CPPIG_BattleGroup::mcToAttackCount(ZCsl* csl)
{
    return mToAttack->Count();
}
//---------------------------------------------------------------------------

ZString CPPIG_BattleGroup::mcDirFromPoints(ZCsl* csl)
{
    int x1 = csl->get("x1").asInt();
    int y1 = csl->get("y1").asInt();
    int x2 = csl->get("x2").asInt();
    int y2 = csl->get("y2").asInt();

    int dir = gfGetDirFromPoints(CPPoint(x1, y1), CPPoint(x2, y2));

    return dir;
}
//---------------------------------------------------------------------------


void CPPIG_BattleGroup::Serialize(PP::Stream& _a)
{
    CSerInt    lTicks(GetTimerInstance()->GetTime() - mTicks), lId(mId);
    int        lScriptId = -1;
    CSerString lName(mName), lModuleName((char*)mModuleName), lBgId("CPPIG_BattleGroup");

    // setup
    if (mScriptObserver)
    {
        lScriptId = mScriptObserver->GetScriptId();
    }

    // serialization of bg id (in case of a mess aka stream desync)
    lBgId      .Serialize(_a);

    // serialize rest of data
    lName      .Serialize(_a);
    lId        .Serialize(_a);
    _a.writeInt(&lScriptId);
    lModuleName.Serialize(_a);
    lTicks     .Serialize(_a);

    // TODO: battlegroup serialize the script compiler (ZCsl)
    // zcsl.Serialize();
}
//---------------------------------------------------------------------------

unsigned __int32 CPPIG_BattleGroup::DataSize()
{
    return 0;
}
//---------------------------------------------------------------------------

void CPPIG_BattleGroup::DeSerialize(PP::Stream& _a)
{
    CSerInt    lTicks, lId;
    int        lScriptId;
    CSerString lName, lModuleName, lBgId;

    // deserialization of data
    lBgId      .DeSerialize(_a);

    if (lBgId.Get() != CPString("CPPIG_BattleGroup"))
    {
        //throw CArchiveError("CPPIG_BattleGroup - stream skew!");
    }

    lName      .DeSerialize(_a);
    lId        .DeSerialize(_a);
    _a.readInt(&lScriptId);
    lModuleName.DeSerialize(_a);
    lTicks     .DeSerialize(_a);

    // TODO: battlegroup deserialize the script compiler (ZCsl)
    // bla.DeSerialize();

    // members set
    mName       = lName.Get();
    mId         = lId.Get();
    //lScriptId   = lScriptId.Get();
    mModuleName = ZString(lModuleName.Get().c_str());
    mTicks      = GetTimerInstance()->GetTime() - lTicks.Get();

    // set script
    SetScriptObserver(GetMissionInstance()->GetDoctrine()->GetScriptObserver(lScriptId));

    // register a view
    if (mId != OS_LANDSCAPE)
    {
        VLOG(9) << "Registering view for " << mId;
        //GetMissionInstance()->GetMap()->RegisterView(mId);
    }
}
//---------------------------------------------------------------------------

void CPPIG_BattleGroup::Serialize(BaseSerializer& out)
{
    int        lScriptId = -1;

    // setup
    if (mScriptObserver)
    {
        lScriptId = mScriptObserver->GetScriptId();
    }

    out.beginElement("bg");

    out.createAttribute("sync", "CPPIG_BattleGroup");
    out.createAttribute("name", mName.c_str());
    out.createAttribute("id", mId);
    out.createAttribute("scriptId", lScriptId);
    out.createAttribute("module", (char*)mModuleName);
    out.createAttribute("ticks", (int)(GetTimerInstance()->GetTime() - mTicks));

    // TODO: battlegroup serialize the script compiler (ZCsl)
    // zcsl.Serialize();

    out.endElement();
}
//---------------------------------------------------------------------------

void CPPIG_BattleGroup::DeSerialize(BaseDeSerializer& in)
{

}
//---------------------------------------------------------------------------

