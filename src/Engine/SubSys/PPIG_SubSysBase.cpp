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

#pragma hdrstop

#include "PPIG_SubSysBase.h"
#include "PPIG_Mission.h"
//---------------------------------------------------------------------------

CIGSubSysBase::CIGSubSysBase(CIGBaseObject* _parent)
{
  mParent = _parent;

  mAnimType      = 0;
  mAnimCounter   = 0;
  GetSenseCounter().SetMaxValue(0);
  mActions       = new CIGSubSysActionList();
  mCurrentAction = NULL;
  mUpdateCounter.SetMaxValue(1);
  //GetSenseCounter().SetMaxValue(0);
  GetActionStepCounter().SetMaxValue(0);
  GetActionStepCounter().Reset();
  mCurrentActionIndex  = 0;
  for (int i=0; i<OBJECT_ACTION_COUNT; i++)
  {
    GetUpdateCounters(i).SetMaxValue(3);
    GetActionStepCounters(i).SetMaxValue(0);
    SetActionAnimTypes(i, 0);
  }

  mUpdateEnabled = false;
}
//---------------------------------------------------------------------------

CIGSubSysBase::~CIGSubSysBase()
{
  mActions->FreeAll();

  try
  {
    delete mActions;
  }
  catch(...)
  {
    LOG(ERROR) << "CIGSubSysBase::~CIGBaseObject - failed to dealocate ActionList";
  }
}
//---------------------------------------------------------------------------

void CIGSubSysBase::Update()
{
// if (GetUpdateEnabled() == true)
// {
    UpdateActionCounters();
    VLOG(9) << "CIGSubSysBase::Update ";
    if (GetCurrentAction() != NULL)
    {
       //int lTemp = GetCurrentActionIndex();

       if (GetUpdateCounters(GetCurrentActionIndex()).MaxReached() == true)
       {
          GetActionStepCounters(GetCurrentActionIndex())++;

          SetAnimType(GetActionAnimTypes(GetCurrentAction()->GetType()));
          GetCurrentAction()->Update();
       }
       else
       {
          GetCurrentAction()->UpdateIdle();
       }
    }
    else if (GetActions()->IsEmpty() == false)
    {//there is an enqueued action
       SetCurrentAction(GetActions()->GetAction(0));
       GetActions()->Remove(0);
    }
    VLOG(9) << "CIGSubSysBase::Update - over";
//   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: updates all action counters
---------------------------------------------------------------------------*/
void CIGSubSysBase::UpdateActionCounters()
{
  for (int i=0; i<OBJECT_ACTION_COUNT; i++)
  {
   if (GetUpdateCounters(i).MaxReached() == false)
   {
     GetUpdateCounters(i)++;
   }
  }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: adds (enqueue) an action to action list
 parameters : Action - action to be added
---------------------------------------------------------------------------*/
void CIGSubSysBase::AddAction (TIGSubSysActionData actionData)
{
   CIGSubSysAction *lTmpAction = GetActionInstance(actionData);
   GetActions()->Add(lTmpAction);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: adds (enqueue) an action to action list
 parameters : Action - action to be added
---------------------------------------------------------------------------*/
void CIGSubSysBase::AddAction (CIGSubSysAction *Action)
{
   GetActions()->Add(Action);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: adds an action into the action list in order of the priority
 parameters : Action - action to be added
---------------------------------------------------------------------------*/
void CIGSubSysBase::AddActionWithPriority (CIGSubSysAction *Action)
{
   GetActions()->Add(Action); //not really implemented yet
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sets the current action and erase the action list
 parameters : ActionData - data of the action to be set
---------------------------------------------------------------------------*/
void CIGSubSysBase::SetAction (TIGSubSysActionData actionData)
{
   GetActions()->FreeAll();
   actionData.mTargetPoint = CPPoint(actionData.mTargetPoint.x/MAPCELLX, actionData.mTargetPoint.y/MAPCELLY);
   CIGSubSysAction *lTmpAction = 0;

   lTmpAction = GetActionInstance(actionData);
   CancelCurrentAction();
   SetCurrentAction(lTmpAction);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: cancels the current action
              this is called from outside the action
---------------------------------------------------------------------------*/
void CIGSubSysBase::CancelCurrentAction()
{
   if (GetCurrentAction() != NULL)
   {
      GetCurrentAction()->Cancel();
      delete GetCurrentAction();
      SetCurrentAction(NULL);
      Stop();
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on end action event handler
              this is called from outside the action
 others     : overwrite this method to obtain acces to this event
---------------------------------------------------------------------------*/
void CIGSubSysBase::OnBeginAction()
{
   VLOG(9) << "CIGSubSysBase::OnBeginAction";
   if (GetCurrentAction() != NULL)
      GetCurrentAction()->Initiate();
   VLOG(9) << "CIGSubSysBase::OnBeginAction - over";
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on end action event handler
              this is called from the action
 others     : overwrite this method to obtain acces to this event
---------------------------------------------------------------------------*/
void CIGSubSysBase::OnEndAction()
{
   VLOG(9) << "CIGSubSysBase::OnEndAction";
   if (GetCurrentAction() != NULL)
   {
//      delete GetCurrentAction();
      if (GetCurrentAction()->GetType() != OA_STOP)
         Stop();
      SetCurrentAction(NULL);
   }
   VLOG(9) << "CIGSubSysBase::OnEndAction - over";
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sets the counters associated with the action
 parameters : actionIndex - the index of the action
---------------------------------------------------------------------------*/
void CIGSubSysBase::SetActionCounters(int actionIndex)
{
   GetUpdateCounter().SetMaxValue(GetUpdateCounters(actionIndex));
   GetUpdateCounter().Reset();

   GetActionStepCounter().SetMaxValue(GetActionStepCounters(actionIndex));
   GetActionStepCounter().Reset();
   SetCurrentActionIndex(OA_STOP);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description:
---------------------------------------------------------------------------*/
void CIGSubSysBase::OnActionDie()
{
  VLOG(9) << "CIGSubSysBase::OnActionDie";
  if (GetActionStepCounters(OA_DIE).MaxReached() == true)
  {
    VLOG(9) << "CIGSubSysBase::OnActionDie - DEAD";
    TIGSubSysActionData lTmpActionData;
    lTmpActionData.mType = OA_DEAD;
    SetAction(lTmpActionData);
  }
  VLOG(9) << "CIGSubSysBase::OnActionDie - over";
}
//---------------------------------------------------------------------------

void CIGSubSysBase::SetMountPoint(int _index, CPPoint _value)
{
  if (_index < 0 || _index > 7)
  {
    return;
  }

  mMountPoints[_index] = _value;
}
//---------------------------------------------------------------------------

CPPoint CIGSubSysBase::GetMountPoint(int _index)
{
  if (_index < 0 || _index > 7)
  {
    return CPPoint(-1, -1);
  }

  return mMountPoints[_index];
}
//---------------------------------------------------------------------------

void CIGSubSysBase::SetCurrentAction (CIGSubSysAction *value)
{
//   if (mCurrentAction != NULL)
//      delete mCurrentAction;
   mCurrentAction = value;
   if (mCurrentAction != NULL)
   {
      SetActionCounters(mCurrentAction->GetType());
      CPPoint lTmpPoint = mParent->GetLogicCoord();
      mCurrentAction->SetStartPoint(lTmpPoint);
      OnBeginAction();
   }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
// Base Object serialize/deserialize section
//
//---------------------------------------------------------------------------

void CIGSubSysBase::Serialize(PP::Stream& _a)
{
  CSerCounter          lUpdateCounter(mUpdateCounter);
  CSerCounter          lAnimCounter(mAnimCounter);
  //CSerPoint            lCoord(mCoord);
  CSerCounter          lSenseCounter(mSenseCounter);
  CSerCounter          lActionStepCounter(mActionStepCounter);
  bool                 lHasCurrentAction;

  _a.writeBool(&mUpdateEnabled);
  lUpdateCounter.Serialize(_a);
  _a.writeInt(&mAnimType);
  lAnimCounter.Serialize(_a);
  lSenseCounter.Serialize(_a);
  lActionStepCounter.Serialize(_a);
  for(int i = 0; i < OBJECT_ACTION_COUNT; i++)
  {
    CSerCounter lTempCounter;

    lTempCounter = mUpdateCounters[i];
    lTempCounter.Serialize(_a);

    lTempCounter = mActionStepCounters[i];
    lTempCounter.Serialize(_a);

    _a.writeInt(&mActionAnimTypes[i]);
  }

  lHasCurrentAction = (mCurrentAction != 0);

  _a.writeBool(&lHasCurrentAction);

  if (lHasCurrentAction)
  {
    mCurrentAction->Serialize(_a);
  }

  _a.writeInt(&mCurrentActionIndex);

  for(int i = 0; i < 8; i++)
  {
    CSerPoint(mMountPoints[i]).Serialize(_a);
  }
}
//---------------------------------------------------------------------------

unsigned __int32 CIGSubSysBase::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void CIGSubSysBase::DeSerialize(PP::Stream& _a)
{
  CSerCounter          lUpdateCounter;
  CSerCounter          lAnimCounter;
  CSerCounter          lSenseCounter;
  CSerCounter          lActionStepCounter;
  CSerCounter          lUpdateCounters[OBJECT_ACTION_COUNT];
  CSerCounter          lActionStepCounters[OBJECT_ACTION_COUNT];
  bool                 lHasCurrentAction;

  _a.readBool(&mUpdateEnabled);
  lUpdateCounter.DeSerialize(_a);
  _a.readInt(&mAnimType);
  lAnimCounter.DeSerialize(_a);
  lSenseCounter.DeSerialize(_a);
  lActionStepCounter.DeSerialize(_a);
  for(int i = 0; i < OBJECT_ACTION_COUNT; i++)
  {
    lUpdateCounters[i].DeSerialize(_a);

    lActionStepCounters[i].DeSerialize(_a);

    _a.readInt(&mActionAnimTypes[i]);
  }

  // read the current action, if any
  CancelCurrentAction();

  _a.readBool(&lHasCurrentAction);

  if (lHasCurrentAction)
  {
    TIGSubSysActionData lData;

    CIGSubSysAction lAction = CIGSubSysAction(this, lData);

    lAction.DeSerialize(_a);

    SetAction(lAction.GetData());
  }

  _a.readInt(&mCurrentActionIndex);

  for(int i = 0; i < 8; i++)
  {
    CSerPoint lPoint;

    lPoint.DeSerialize(_a);

    mMountPoints[i] = lPoint.Get();
  }

  // assignments
  mUpdateCounter     = lUpdateCounter.Get();
  mAnimCounter       = lAnimCounter.Get();
  mSenseCounter      = lSenseCounter.Get();

  //SetSenseCounter(lSenseCounter.Get());

  mActionStepCounter = lActionStepCounter.Get();
  for(int i = 0; i < OBJECT_ACTION_COUNT; i++)
  {
    mUpdateCounters[i]     = lUpdateCounters[i].Get();
    mActionStepCounters[i] = lActionStepCounters[i].Get();
  }

  /*
  if (lHasCurrentAction)
  {
    //SetCurrentAction(lAction);
    mCurrentAction = lAction;
    SetActionCounters(mCurrentAction->GetType());
    SetCurrentActionIndex(mCurrentAction->GetType());
  }

  //SetCurrentActionIndex(mCurrentActionIndex);

  int temp = GetCurrentActionIndex();
  */
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

