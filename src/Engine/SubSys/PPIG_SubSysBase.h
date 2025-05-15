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
//  Unit              : Class In Game Sub Systems Base Class
//
//  Version           : 1.0
//
//  Description       :
//    * implementation for a generic object subsystem
//    * it is very conveninent to build an object (infantry, vehicle, whatever)
//      out of components (or subsystems); in this way, an object won't get
//      inflated because of the many responsabilities it has to carry - these
//      responsabilities are passed to the object subsystems
//    * possible subsystems: propulsion, body (for managing damage stimuls and
//      hit-points), turret, cargo bay for vehicles, cargo bay for infantry,
//      radar jamming equipment
//    * info on counters!
//
//  History           :
//    [05.11.2002] - [Karg] - unit created
//    [10.11.2002] - [Karg] - added Anim counters
//-----------------------------------------------------------------------------

#ifndef PPIG_SubSysBaseH
#define PPIG_SubSysBaseH
//---------------------------------------------------------------------------

#include "PPIG_EngineDef.h"
#include "PP_String.h"
#include "PP_Sprite.h"
#include "PPIG_SubSysActionList.h"
#include "PPIG_SubSysAction.h"
#include "PPIG_ObjStimul.h"
#include "PP_Counter.h"
#include "Serializable.h"
//#include "PPIG_ObjVehicle.h"

class CIGBaseObject;

class CIGSubSysBase : public CSerializable
{
  public: // c-d
    CIGSubSysBase(CIGBaseObject* _parent);
    virtual ~CIGSubSysBase();

  public: // action methods
    virtual void UpdateActionCounters();
    virtual CIGSubSysAction* GetActionInstance(TIGSubSysActionData actionData) {return NULL;}
    virtual void AddAction(TIGSubSysActionData actionData);
    virtual void AddAction (CIGSubSysAction *Action);
    virtual void AddActionWithPriority (CIGSubSysAction *Action);
    virtual void SetAction (TIGSubSysActionData actionData);
    virtual void CancelCurrentAction();
    virtual void OnBeginAction();
    virtual void OnEndAction();
    virtual void SetActionCounters(int actionIndex);

  public: // get-set
    virtual CIGSubSysActionList *GetActions ();
    virtual void SetActions (CIGSubSysActionList *value);

    virtual CIGSubSysAction *GetCurrentAction ();
    virtual void SetCurrentAction (CIGSubSysAction *value);

    virtual CPCounter& GetActionStepCounter ();
    virtual void SetActionStepCounter (int value);
    virtual void SetActionStepCounter (CPCounter &value );

    virtual CPCounter&  GetUpdateCounters(int index);
    virtual void SetUpdateCounters(int index, int value);

    virtual CPCounter&  GetActionStepCounters(int index);
    virtual void SetActionStepCounters(int index, int value);

    virtual int  GetActionAnimTypes(int index);
    virtual void SetActionAnimTypes(int index, int value);

    virtual int  GetCurrentActionIndex();
    virtual void SetCurrentActionIndex(int value);

    virtual CPCounter& GetUpdateCounter();
    virtual void SetUpdateCounter(CPCounter& value);
    virtual void SetUpdateCounter(int value);

    void SetUpdateEnabled(bool _v) { mUpdateEnabled = _v; }
    bool GetUpdateEnabled() { return mUpdateEnabled; }

    virtual CIGBaseObject* GetParent() { return mParent; }
    virtual void SetParent(CIGBaseObject* _v) { mParent = _v; }

    virtual int  GetAnimType ();
    virtual void SetAnimType (int value);

    virtual CPCounter& GetAnimCounter ();
    virtual void SetAnimCounter (CPCounter& value);
    virtual void SetAnimCounter (int value);

    virtual CPCounter& GetSenseCounter();
    virtual void SetSenseCounter(CPCounter& value);
    virtual void SetSenseCounter(int value);

    virtual void    SetMountPoint(int _index, CPPoint _value);
    virtual CPPoint GetMountPoint(int _index);
    virtual int     GetMountPointCount() { return 8; }

  public: // ops
    virtual void Update();

  public: // actions
    // Base actions
    virtual void OnActionStop   ()                  {}
    virtual void OnActionMove   ()                  {}
    virtual void OnActionRotate ()                  {}
    virtual void OnActionAttack ()                  {}
    virtual void OnActionAttackGround ()            {}
    virtual void OnActionPatrol ()                  {}
    virtual void OnActionHold   ()                  {}
    virtual void OnActionGuard  ()                  {}
    virtual void OnActionCapture()                  {}
    virtual void OnActionLoad   ()                  {}
    virtual void OnActionUnload ()                  {}
    virtual void OnActionUnloadAll ()               {}
    virtual void OnActionDie    ();
    virtual void OnActionDead   ()                  {}
    virtual void OnActionExplode()                  {}
    virtual void OnActionSelfDestruct()             {}
    virtual void OnActionTakeCover()                {}
    virtual void OnActionTakeoff()                  {}
    virtual void OnActionLand()                     {}

    // TEST CODE FOR SDJ
    virtual void OnActionBuild()                    {}

    // Tactical actions
    virtual void OnActionTacticMove   ()          {}
    virtual void OnActionTacticAttack ()          {}
    virtual void OnActionTacticAttackGround ()    {}
    virtual void OnActionTacticPatrol ()          {}
    virtual void OnActionTacticStop ()            {}

    virtual void OnStimul (CIGObjectStimul& Stimul) {}

    virtual void StartActionMove() {}

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  public:
    virtual void Stop() {};

  private:
    bool                 mUpdateEnabled;//if the update os enabled for the
    CPCounter            mUpdateCounter;
    CPCounter            mActionStepCounter;

    CIGSubSysActionList *mActions;      //the enqueued actions
    CIGSubSysAction     *mCurrentAction;//the current action

    int                  mAnimType;     //current anim type  - y pos in sprite
    CPCounter            mAnimCounter;  //current pos of animation - x pos in sprite
    CPCounter            mSenseCounter; //means orientation

    CPCounter            mUpdateCounters[OBJECT_ACTION_COUNT];     //the update max value
    CPCounter            mActionStepCounters[OBJECT_ACTION_COUNT]; //the actions max values
    int                  mActionAnimTypes[OBJECT_ACTION_COUNT];    //the animations related to the actions
    int                  mCurrentActionIndex;                      //
    CIGBaseObject        *mParent;

    CPPoint              mMountPoints[8]; // the points where the subsystem mounts
};

/*---------------------------------------------------------------------------
 description: Actions
---------------------------------------------------------------------------*/
inline CIGSubSysActionList *CIGSubSysBase::GetActions ()
{
  return mActions;
}

inline void CIGSubSysBase::SetActions (CIGSubSysActionList *value)
{
  mActions = value;
}

/*---------------------------------------------------------------------------
 description: CurrentAction
---------------------------------------------------------------------------*/
inline CIGSubSysAction *CIGSubSysBase::GetCurrentAction ()
{
   return mCurrentAction;
}

/*---------------------------------------------------------------------------
 description: UpdateCounter
---------------------------------------------------------------------------*/
inline CPCounter& CIGSubSysBase::GetUpdateCounter()
{
   return mUpdateCounter;
}

inline void CIGSubSysBase::SetUpdateCounter(CPCounter& value)
{
   mUpdateCounter = value;
}

inline void CIGSubSysBase::SetUpdateCounter(int value)
{
   mUpdateCounter = value;
}

/*---------------------------------------------------------------------------
 description: Action Step Counter
---------------------------------------------------------------------------*/
inline CPCounter& CIGSubSysBase::GetActionStepCounter ()
{
  return mActionStepCounter;
}
inline void CIGSubSysBase::SetActionStepCounter (int value)
{
  mActionStepCounter = value;
}
inline void CIGSubSysBase::SetActionStepCounter (CPCounter &value)
{
   mActionStepCounter = value;
}

/*---------------------------------------------------------------------------
 description: Update Counters
---------------------------------------------------------------------------*/
inline CPCounter& CIGSubSysBase::GetUpdateCounters(int index)
{
   return mUpdateCounters[index];
}

inline void CIGSubSysBase::SetUpdateCounters(int index, int value)
{
   mUpdateCounters[index] = value;
}

/*---------------------------------------------------------------------------
 description: StepUpdateCounters
---------------------------------------------------------------------------*/
inline CPCounter& CIGSubSysBase::GetActionStepCounters(int index)
{
   return mActionStepCounters[index];
}

inline void CIGSubSysBase::SetActionStepCounters(int index, int value)
{
   mActionStepCounters[index] = value;
}

/*---------------------------------------------------------------------------
 description: Action Anim Types
---------------------------------------------------------------------------*/
inline int CIGSubSysBase::GetActionAnimTypes(int index)
{
   return mActionAnimTypes[index];
}

inline void CIGSubSysBase::SetActionAnimTypes(int index, int value)
{
   mActionAnimTypes[index] = value;
}

/*---------------------------------------------------------------------------
 description: Current Action Index
---------------------------------------------------------------------------*/
inline int CIGSubSysBase::GetCurrentActionIndex()
{
   return mCurrentActionIndex;
}

inline void CIGSubSysBase::SetCurrentActionIndex(int value)
{
   mCurrentActionIndex = value;

   if (value >= 0)
   {
      // Karg: attack counter is the weapon reload time
      if ((value != OA_ATTACK) && (value != OA_ATTACKGROUND))
        GetUpdateCounters(value).Reset();
   }
}

/*---------------------------------------------------------------------------
 description: Sense
---------------------------------------------------------------------------*/
inline CPCounter& CIGSubSysBase::GetSenseCounter()
{
   return mSenseCounter;
}
inline void CIGSubSysBase::SetSenseCounter(CPCounter& value)
{
   mSenseCounter = value;
}
inline void CIGSubSysBase::SetSenseCounter(int value)
{
   mSenseCounter = value;
}

/*---------------------------------------------------------------------------
 description: AnimType
---------------------------------------------------------------------------*/
inline int CIGSubSysBase::GetAnimType ()
{
  return mAnimType;
}

inline void CIGSubSysBase::SetAnimType (int value)
{
  mAnimType = value;
}

/*---------------------------------------------------------------------------
 description: AnimCounter
---------------------------------------------------------------------------*/
inline CPCounter& CIGSubSysBase::GetAnimCounter ()
{
  return mAnimCounter;
}
inline void CIGSubSysBase::SetAnimCounter (CPCounter& value)
{
   mAnimCounter = value;
}
inline void CIGSubSysBase::SetAnimCounter (int value)
{
   mAnimCounter = value;
}

#endif
