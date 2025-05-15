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
#pragma hdrstop

#include "PPIG_SubSysBase.h"
#include "PPIG_SubSysAction.h"
#include "PPIG_Mission.h"

void TIGSubSysActionData::Serialize(PP::Stream& _a)
{
  CSerPoint lTargetPoint(mTargetPoint);
  CSerPoint lStartPoint(mStartPoint);

  int lTargetObjectId = -1;

  if (mTargetObject != NULL)
  {
      lTargetObjectId = mTargetObject->GetId();
  }

  _a.writeInt(&mType);
  _a.writeInt(&mPriority);
  lTargetPoint.Serialize(_a);
  lStartPoint.Serialize(_a);
  _a.writeInt(&lTargetObjectId);

  for(int i = 0; i < 4; i++)
  {
    _a.writeInt(&mData[i]);
  }
}
//---------------------------------------------------------------------------

unsigned __int32 TIGSubSysActionData::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void TIGSubSysActionData::DeSerialize(PP::Stream& _a)
{
  CSerPoint lTargetPoint(mTargetPoint);
  CSerPoint lStartPoint(mStartPoint);
  int       lTargetObjectId;

  _a.readInt(&mType);
  _a.readInt(&mPriority);
  lTargetPoint.DeSerialize(_a);
  lStartPoint.DeSerialize(_a);
  _a.readInt(&lTargetObjectId);

  for(int i = 0; i < 4; i++)
  {
    _a.readInt(&mData[i]);
  }

  mTargetPoint  = lTargetPoint.Get();
  mStartPoint   = lStartPoint.Get();
  mTargetObject = GetMissionInstance()->GetObjectById(lTargetObjectId);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CIGSubSysAction::CIGSubSysAction(CIGSubSysBase *object, TIGSubSysActionData data)
{
   mChildAction   = NULL;
   mParentAction  = NULL;
   mSubSys        = object;
   mData          = data;
   mEnabled       = true;
   mUpdateCounter.SetMaxValue(0);
}
//---------------------------------------------------------------------------

// Description : copy constructor
// Params      : object to copy
// Result      : n/a
// Comments    : because of some warnings I got compiling this
/*
CIGSubSysAction::CIGSubSysAction(CIGSubSysAction& _toCopy)
{
  mObject = _toCopy.mObject;
  // tre' sa mai vina si altele
}
//---------------------------------------------------------------------------
*/

/*---------------------------------------------------------------------------
 description: gets a pointer to a copy of the object
---------------------------------------------------------------------------*/
CIGSubSysAction* CIGSubSysAction::Clone()
{
   return new CIGSubSysAction(mSubSys, mData);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: updates the counter
---------------------------------------------------------------------------*/
void CIGSubSysAction::Update()
{
//   GetUpdateCounter()++;
//   if (GetUpdateCounter().MinReached() == true)
   if (GetEnabled() == true)
      Handle();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: the handler method when action is charging
 others     : it should be overwritten in a descendant class
---------------------------------------------------------------------------*/
void CIGSubSysAction::UpdateIdle()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: updates the child action if it exists
---------------------------------------------------------------------------*/
bool CIGSubSysAction::UpdateChildAction()
{
   if (mChildAction != NULL)
   {
      mChildAction->Update();
      return true;
   }
   return false;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the action
 others     : it should be overwritten in a descendant class
---------------------------------------------------------------------------*/
void CIGSubSysAction::Handle()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: initiates the action
 others     : it shoould be overwritten in a descendant class
---------------------------------------------------------------------------*/
void CIGSubSysAction::Initiate()
{
   GetSubSys()->SetActionCounters(GetType());
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: cancels the action and it's child action (if any)
              from outside
 others     : it shoould be overwritten in a descendant class
---------------------------------------------------------------------------*/
void CIGSubSysAction::Cancel()
{
   if (mChildAction != NULL)
   {
      mChildAction->Cancel();
      delete mChildAction;
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: ends the action
              it's called from the handler method of the action
 others     : it should be overwritten in a descendant class
---------------------------------------------------------------------------*/
void CIGSubSysAction::End()
{
  if (mParentAction != NULL)
  {
    mParentAction->OnChildActionEnd();
  }
  else
  {
    GetSubSys()->OnEndAction();
  }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: child end event notify
              it should erase the child action
---------------------------------------------------------------------------*/
void CIGSubSysAction::OnChildActionEnd()
{
//   delete mChildAction;
   mChildAction = NULL;
}
//---------------------------------------------------------------------------

void CIGSubSysAction::Serialize(PP::Stream& _a)
{
  mData.Serialize(_a);

  CSerCounter(mUpdateCounter).Serialize(_a);
  _a.writeBool(&mEnabled);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGSubSysAction::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void CIGSubSysAction::DeSerialize(PP::Stream& _a)
{
  mData.DeSerialize(_a);

  CSerCounter lUpdateCounter;

  lUpdateCounter.DeSerialize(_a);

  _a.readBool(&mEnabled);

  mUpdateCounter = lUpdateCounter.Get();
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION STOP SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionStop::Clone()
{
   return new CIGSubSysActionStop(GetSubSys(), GetData());
}
void CIGSubSysActionStop::Handle()
{
   GetSubSys()->OnActionStop();
}
void CIGSubSysActionStop::Initiate()
{
   GetSubSys()->GetParent()->SetMoving(false);
   GetSubSys()->GetParent()->SetWayLength(0);
}

/////////////////////////////////////////////////////////////////////////////
//ACTION MOVE SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionMove::Clone()
{
   return new CIGSubSysActionMove(GetSubSys(), GetData());
}
void CIGSubSysActionMove::Handle()
{
   GetSubSys()->OnActionMove();
}
void CIGSubSysActionMove::Initiate()
{
   GetSubSys()->GetParent()->SetMoving(true);
   GetSubSys()->GetParent()->ComputeWay();
   GetSubSys()->StartActionMove();
}
void CIGSubSysActionMove::End()
{
  CIGSubSysAction::End();
  GetSubSys()->Stop();
}

/////////////////////////////////////////////////////////////////////////////
//ACTION ATTACK SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionAttack::Clone()
{
   return new CIGSubSysActionAttack(GetSubSys(), GetData());
}
void CIGSubSysActionAttack::Handle()
{
   GetSubSys()->OnActionAttack();
}
void CIGSubSysActionAttack::UpdateIdle()
{
   if (GetSubSys()->GetUpdateCounters(OA_ATTACK) >= 5)
      GetSubSys()->GetActionStepCounters(OA_ATTACK) = 1;
}

/////////////////////////////////////////////////////////////////////////////
//ACTION ATTACK GROUND SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionAttackGround::Clone()
{
   return new CIGSubSysActionAttackGround(GetSubSys(), GetData());
}
void CIGSubSysActionAttackGround::Handle()
{
   GetSubSys()->OnActionAttackGround();
}
void CIGSubSysActionAttackGround::UpdateIdle()
{
   if (GetSubSys()->GetUpdateCounters(OA_ATTACKGROUND) >= 5)
      GetSubSys()->GetActionStepCounters(OA_ATTACKGROUND) = 1;
}

/////////////////////////////////////////////////////////////////////////////
//ACTION PATROL SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionPatrol::Clone()
{
   return new CIGSubSysActionPatrol(GetSubSys(), GetData());
}
void CIGSubSysActionPatrol::Handle()
{
   GetSubSys()->OnActionPatrol();
}
void CIGSubSysActionPatrol::Initiate()
{
   GetSubSys()->GetParent()->SetMoving(true);
   GetSubSys()->GetParent()->ComputeWay();
}
void CIGSubSysActionPatrol::End()
{
  TIGSubSysActionData lTmp;
  lTmp.mType           = OA_PATROL;
  lTmp.mPriority       = mData.mPriority;
  lTmp.mTargetPoint    = mData.mStartPoint;
  lTmp.mTargetObject   = NULL;
  //CIGBaseObject *lObj  = GetSubSys()->GetParent();
  CIGSubSysBase *lObj  = GetSubSys();
  CIGSubSysAction::End();

  if (lObj != NULL)
  {
    lObj->AddAction(lTmp);
  }
}

/////////////////////////////////////////////////////////////////////////////
//ACTION GUARD SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionGuard::Clone()
{
   return new CIGSubSysActionGuard(GetSubSys(), GetData());
}
void CIGSubSysActionGuard::Handle()
{
   GetSubSys()->OnActionGuard();
}
void CIGSubSysActionGuard::Initiate()
{
   GetSubSys()->GetParent()->SetMoving(false);
   GetSubSys()->GetParent()->SetWayLength(0);
}

/////////////////////////////////////////////////////////////////////////////
//ACTION HOLD SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionHold::Clone()
{
   return new CIGSubSysActionHold(GetSubSys(), GetData());
}
void CIGSubSysActionHold::Handle()
{
   GetSubSys()->OnActionHold();
}
void CIGSubSysActionHold::Initiate()
{
   GetSubSys()->GetParent()->SetMoving(false);
   GetSubSys()->GetParent()->SetWayLength(0);
}

/////////////////////////////////////////////////////////////////////////////
//ACTION LOAD SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionLoad::Clone()
{
   return new CIGSubSysActionLoad(GetSubSys(), GetData());
}
void CIGSubSysActionLoad::Handle()
{
   GetSubSys()->OnActionLoad();
}
void CIGSubSysActionLoad::Initiate()
{
}

/////////////////////////////////////////////////////////////////////////////
//ACTION UNLOAD SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionUnload::Clone()
{
   return new CIGSubSysActionUnload(GetSubSys(), GetData());
}

void CIGSubSysActionUnload::Handle()
{
   GetSubSys()->OnActionUnload();
}

void CIGSubSysActionUnload::Initiate()
{
}

/////////////////////////////////////////////////////////////////////////////
//ACTION UNLOAD ALL SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionUnloadAll::Clone()
{
   return new CIGSubSysActionUnloadAll(GetSubSys(), GetData());
}
void CIGSubSysActionUnloadAll::Handle()
{
   GetSubSys()->OnActionUnloadAll();
}
void CIGSubSysActionUnloadAll::Initiate()
{
   //GetSubSys()->GetParent()->SetMoving(true);
   //GetSubSys()->GetParent()->ComputeWay();
}

/////////////////////////////////////////////////////////////////////////////
//ACTION CAPTURE SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionCapture::Clone()
{
   return new CIGSubSysActionCapture(GetSubSys(), GetData());
}
void CIGSubSysActionCapture::Handle()
{
   GetSubSys()->OnActionCapture();
}
void CIGSubSysActionCapture::Initiate()
{
  GetSubSys()->GetParent()->SetMoving(false);
  GetSubSys()->GetParent()->SetWayLength(0);
}


/////////////////////////////////////////////////////////////////////////////
//ACTION DIE SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionDie::Clone()
{
   return new CIGSubSysActionDie(GetSubSys(), GetData());
}
void CIGSubSysActionDie::Handle()
{
   GetSubSys()->OnActionDie();
}
void CIGSubSysActionDie::Initiate()
{
   GetSubSys()->SetCurrentActionIndex(OA_DIE);
}

/////////////////////////////////////////////////////////////////////////////
//ACTION DEAD SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionDead::Clone()
{
   return new CIGSubSysActionDead(GetSubSys(), GetData());
}
void CIGSubSysActionDead::Handle()
{
   GetSubSys()->OnActionDead();
}
void CIGSubSysActionDead::Initiate()
{
   GetSubSys()->SetCurrentActionIndex(OA_DEAD);
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION LAND SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionLand::Clone()
{
   return new CIGSubSysActionLand(GetSubSys(), GetData());
}
void CIGSubSysActionLand::Handle()
{
   GetSubSys()->OnActionLand();
}
void CIGSubSysActionLand::Initiate()
{
   GetSubSys()->SetCurrentActionIndex(OA_LAND);
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// ACTION BUILD SECTION
// TEST CODE FOR SDJ
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionBuild::Clone()
{
   return new CIGSubSysActionBuild(GetSubSys(), GetData());
}
void CIGSubSysActionBuild::Handle()
{
   GetSubSys()->OnActionBuild();
}
void CIGSubSysActionBuild::Initiate()
{
   GetSubSys()->SetCurrentActionIndex(OA_BUILD);
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION TACTIC MOVE SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionTacticMove::Clone()
{
   return new CIGSubSysActionTacticMove(GetSubSys(), GetData());
}
void CIGSubSysActionTacticMove::Handle()
{
   GetSubSys()->OnActionTacticMove();
}
void CIGSubSysActionTacticMove::Initiate()
{
}
void CIGSubSysActionTacticMove::End()
{
  CIGSubSysAction::End();
  GetSubSys()->Stop();
}

/////////////////////////////////////////////////////////////////////////////
//ACTION TACTIC ATTACK SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionTacticAttack::Clone()
{
   return new CIGSubSysActionTacticAttack(GetSubSys(), GetData());
}
void CIGSubSysActionTacticAttack::Handle()
{
   GetSubSys()->OnActionTacticAttack();
}

/////////////////////////////////////////////////////////////////////////////
//ACTION TACTIC ATTACK GROUND SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionTacticAttackGround::Clone()
{
   return new CIGSubSysActionTacticAttackGround(GetSubSys(), GetData());
}
void CIGSubSysActionTacticAttackGround::Handle()
{
   GetSubSys()->OnActionTacticAttackGround();
}

/////////////////////////////////////////////////////////////////////////////
//ACTION TACTIC PATROL SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionTacticPatrol::Clone()
{
   return new CIGSubSysActionTacticPatrol(GetSubSys(), GetData());
}
void CIGSubSysActionTacticPatrol::Handle()
{
   GetSubSys()->OnActionTacticPatrol();
}

/////////////////////////////////////////////////////////////////////////////
//ACTION TACTIC STOP SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionTacticStop::Clone()
{
   return new CIGSubSysActionTacticStop(GetSubSys(), GetData());
}
void CIGSubSysActionTacticStop::Handle()
{
   GetSubSys()->OnActionTacticStop();
}

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
