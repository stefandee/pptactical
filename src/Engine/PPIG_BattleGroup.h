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
//  Unit              : Battle Group Commander
//
//  Version           : 1.0
//
//  Description       :
//    * implements a battle group commander (bg-c)
//    * a bg-c has units under his command and a script with his objectives; he
//      tries to meet the objectives in the same fashion a human player would do
//    * a bg-c works with map sectors of fixed dimensions (8x8); this helps the
//      movement
//
//  History           :
//    [16.12.2001] - [Karg] - unit created
//    [10.01.2002] - [Karg] - added unit list and unit management interface
//    [10.01.2002] - [Karg] - added all features
//-----------------------------------------------------------------------------

#ifndef PPIG_BattleGroupH
#define PPIG_BattleGroupH
//---------------------------------------------------------------------------

#include "PP_String.h"
//#include "PPIG_ObjList.h"
#include "ZCsl.hpp"
#include "PPIG_Doctrine.h"
#include "PPIG_ObjBase.h"

//class CIGRealObject;
//class CIGBaseObject;

class CPPIG_BattleGroup : public CSerializable, public Serializable2
{
  public: // c-d
    CPPIG_BattleGroup();
    CPPIG_BattleGroup(CPString _name, CPString _info, int _id);

    virtual void Update();

    virtual ~CPPIG_BattleGroup();

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

    void Serialize(BaseSerializer& out);
    void DeSerialize(BaseDeSerializer& in);

  public: // get-set
    CPString GetName() { return mName; };
    void     SetName(CPString _v) { mName = _v; }

    int      GetId() { return mId; }
    void     SetId(int _v);

    CPString GetDescription() { return mDescription; };
    void     SetDescription(CPString _v) { mDescription = _v; }

    int      GetUnitCount() { return mUnits->Count(); }
    CIGBaseObject* GetUnitByIndex(int _index);

    void                  SetScriptObserver(CPPIG_ScriptObserver* _obs);
    CPPIG_ScriptObserver* GetScriptObserver() { return mScriptObserver; };

  public: // unit management interface
    virtual bool AddUnit    (CIGBaseObject* _unit);
    virtual bool RemoveUnit (CIGBaseObject* _unit);

  private: // internal stuff
    CPString               mName;
    CPString               mDescription;
    int                    mId;
    bool                   mValidCsl;
    ZCsl*                  mCompiler;
    ZString                mModuleName;
    CPPIG_ScriptObserver*  mScriptObserver;
    CPList<CIGBaseObject>* mUnits;
    int                    mTicks;

    bool                   ValidUnitIndex(int _index);

  private: // battle tactics data
    CPList<CIGBaseObject>* mEnemy, *mToAttack;
    int                    mTargets;

  private: // script
    void InitCsl();
    void InitHandlers();
    void addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback);

    ZCsl_callback_t mHandleMissionTime;
    ZCsl_callback_t mHandleIsUnitDead;
    ZCsl_callback_t mHandleZoneEvent;
    ZCsl_callback_t mHandleZoneEvaluateThreat;
    ZCsl_callback_t mHandlePostMessage;
    ZCsl_callback_t mHandleUnitsCount;
    ZCsl_callback_t mHandleUnitStatus;
    ZCsl_callback_t mHandleMove;
    ZCsl_callback_t mHandleUnitX;
    ZCsl_callback_t mHandleUnitY;
    ZCsl_callback_t mHandleUnitSense;
    ZCsl_callback_t mHandleAttackRandomEnemy;
    ZCsl_callback_t mHandleSeekEnemies;
    ZCsl_callback_t mHandleSetTargets;
    ZCsl_callback_t mHandleToAttackCount;
    ZCsl_callback_t mHandleDirFromPoints;

    // implementations
    ZString mcMissionTime(ZCsl* csl);
    ZString mcIsUnitDead(ZCsl* csl);
    ZString mcZoneEvent(ZCsl* csl);
    ZString mcZoneEvaluateThreat(ZCsl* csl);
    ZString mcPostMessage(ZCsl* csl);

    ZString mcUnitsCount(ZCsl* csl);
    ZString mcUnitStatus(ZCsl* csl);
    ZString mcMove(ZCsl* csl);
    ZString mcUnitX(ZCsl* csl);
    ZString mcUnitY(ZCsl* csl);
    ZString mcUnitSense(ZCsl* csl);

    ZString mcAttackRandomEnemy(ZCsl* csl);

    ZString mcSeekEnemies(ZCsl* csl);
    ZString mcSetTargets(ZCsl* csl);
    ZString mcToAttackCount(ZCsl* csl);

    ZString mcDirFromPoints(ZCsl* csl);
};

#endif
