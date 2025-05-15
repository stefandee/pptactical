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
/*---------------------------------------------------------------------------
 application: Pure Power In Game

 description: Class Base Object Action
 last modify: 23 06 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PPIG_SubSysActionH
#define PPIG_SubSysActionH

//#include "PP_GuiDef.h"
#include "PPIG_EngineDef.h"
#include "PP_Counter.h"
#include "basic2d.h"
#include "Serializable.h"

/////////////////////////////////////////////////////////////////////////////
//OBJECT ACTION SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGBaseObject;
class CIGSubSysBase;

typedef struct SIGSubSysActionData
{
  public:
    int            mType;            //type of the action
    int            mPriority;        //the priority of the action
    CPPoint        mTargetPoint;     //the target point of the action in real coord
    CPPoint        mStartPoint;      //the starting point of the action in real coord
    CIGBaseObject* mTargetObject;    //the target object of the action
    int            mData[4];

  public:
    SIGSubSysActionData() : mType(-1), mPriority(0), mTargetObject(NULL) {}

  public: // serialization - deserialization
    void Serialize(PP::Stream& _a);
    unsigned __int32 DataSize();
    void DeSerialize(PP::Stream& _a);

} TIGSubSysActionData;

class CIGSubSysAction : public CSerializable
{
  friend class CIGSubSysBase;
  public:
      CIGSubSysAction(CIGSubSysBase *object, TIGSubSysActionData data);
      //CIGObjectAction(CIGObjectAction& _toCopy);
      ~CIGSubSysAction() {}
      virtual CIGSubSysAction* Clone();

      virtual void Update();
      virtual void UpdateIdle();
      virtual bool UpdateChildAction();
      virtual void Handle();
      virtual void Initiate();
      virtual void Cancel();
      virtual void End();
      virtual void OnChildActionEnd();

  public: //Get-Set methods
      virtual TIGSubSysActionData GetData()  {return mData;}

      virtual int GetType()                  {return mData.mType;}
      virtual void SetType(int value)        {mData.mType = value;}

      virtual CPCounter& GetUpdateCounter()  {return mUpdateCounter;}
      virtual void SetUpdateCounter(CPCounter& value)
                                             {mUpdateCounter = value;}
      virtual void SetUpdateCounter(int value)
                                             {mUpdateCounter = value;}
      virtual CIGSubSysBase* GetSubSys()     {return mSubSys;}
      virtual void SetEnabled(bool value)    {mEnabled = value;}
      virtual bool GetEnabled()              {return mEnabled;}
      virtual void SetStartPoint(CPPoint& value)
                                             { mData.mStartPoint = value; }
      virtual void SetTargetPoint(const CPPoint& value)
                                             { mData.mTargetPoint = value; }
  public: // serialization - deserialization
    void Serialize(PP::Stream& _a);
    unsigned __int32 DataSize();
    void DeSerialize(PP::Stream& _a);

  protected:
      TIGSubSysActionData   mData;            //the data of the action
      CIGSubSysAction      *mChildAction;
      CIGSubSysAction      *mParentAction;

  private: // Data Members for Class Attributes
      CIGSubSysBase         *mSubSys;          //the target object of the action
      CPCounter             mUpdateCounter;
      bool                  mEnabled;
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION STOP SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionStop: public CIGSubSysAction
{
   public:
      CIGSubSysActionStop(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_STOP);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
      virtual void Initiate();
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION MOVE SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionMove: public CIGSubSysAction
{
   public:
      CIGSubSysActionMove(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_MOVE);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
      virtual void Initiate();
      virtual void End();
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION ATTACK SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionAttack: public CIGSubSysAction
{
   public:
      CIGSubSysActionAttack(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_ATTACK);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
      virtual void UpdateIdle();
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION ATTACK GROUND SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionAttackGround: public CIGSubSysAction
{
   public:
      CIGSubSysActionAttackGround(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_ATTACKGROUND);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
      virtual void UpdateIdle();
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION PATROL SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionPatrol: public CIGSubSysAction
{
   public:
      CIGSubSysActionPatrol(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_PATROL);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
      virtual void Initiate();
      virtual void End();
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION GUARD SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionGuard: public CIGSubSysAction
{
   public:
      CIGSubSysActionGuard(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_GUARD);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
      virtual void Initiate();
};
//---------------------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////
//ACTION HOLD SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionHold: public CIGSubSysAction
{
   public:
      CIGSubSysActionHold(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_HOLD);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
      virtual void Initiate();
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION LOAD SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionLoad: public CIGSubSysAction
{
   public:
      CIGSubSysActionLoad(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_LOAD);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
      virtual void Initiate();
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION UNLOAD SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionUnload: public CIGSubSysAction
{
   public:
      CIGSubSysActionUnload(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_UNLOAD);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
      virtual void Initiate();
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION UNLOAD ALL SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionUnloadAll: public CIGSubSysAction
{
   public:
      CIGSubSysActionUnloadAll(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_UNLOAD_ALL);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
      virtual void Initiate();
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION CAPTURE SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionCapture: public CIGSubSysAction
{
   public:
      CIGSubSysActionCapture(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_CAPTURE);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
      virtual void Initiate();
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION DIE SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionDie: public CIGSubSysAction
{
   public:
      CIGSubSysActionDie(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_DIE);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
      virtual void Initiate();
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION DEAD SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionDead: public CIGSubSysAction
{
   public:
      CIGSubSysActionDead(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_DEAD);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
      virtual void Initiate();
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION LAND SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionLand: public CIGSubSysAction
{
   public:
      CIGSubSysActionLand(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_LAND);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
      virtual void Initiate();
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// ACTION BUILD SECTION
// TEST CODE FOR SDJ
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionBuild: public CIGSubSysAction
{
   public:
      CIGSubSysActionBuild(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_BUILD);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
      virtual void Initiate();
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION TACTIC MOVE SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionTacticMove: public CIGSubSysAction
{
   public:
      CIGSubSysActionTacticMove(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_TACTIC_MOVE);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
      virtual void Initiate();
      virtual void End();
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION TACTIC ATTACK SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionTacticAttack: public CIGSubSysAction
{
   public:
      CIGSubSysActionTacticAttack(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_TACTIC_ATTACK);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION TACTIC ATTACK GROUND SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionTacticAttackGround: public CIGSubSysAction
{
   public:
      CIGSubSysActionTacticAttackGround(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_TACTIC_ATTACKGROUND);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION TACTIC PATROL SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionTacticPatrol: public CIGSubSysAction
{
   public:
      CIGSubSysActionTacticPatrol(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_TACTIC_PATROL);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION TACTIC STOP SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionTacticStop: public CIGSubSysAction
{
   public:
      CIGSubSysActionTacticStop(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_TACTIC_STOP);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
};
//---------------------------------------------------------------------------

#endif
