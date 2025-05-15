//---------------------------------------------------------------------------

#pragma hdrstop

#include "PPIG_SubSysTactics.h"
#include "PPIG_ObjUnit.h"
//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CIGSubSysTactics::CIGSubSysTactics(CIGUnit* _parent) : CIGSubSysBase(_parent)
{
  //Action step counter
  GetActionStepCounters(OA_TACTIC_MOVE)         .SetMaxValue(3);
  GetActionStepCounters(OA_TACTIC_ATTACK)       .SetMaxValue(1);
  GetActionStepCounters(OA_TACTIC_ATTACKGROUND) .SetMaxValue(1);
  GetActionStepCounters(OA_TACTIC_PATROL)       .SetMaxValue(3);
  GetActionStepCounters(OA_TACTIC_STOP)         .SetMaxValue(0);
  //------------------

  //Action update settings
  GetUpdateCounters(OA_TACTIC_MOVE)             .SetMaxValue(1);
  GetUpdateCounters(OA_TACTIC_ATTACKGROUND)     .SetMaxValue(1);
  GetUpdateCounters(OA_TACTIC_ATTACK)           .SetMaxValue(1);
  GetUpdateCounters(OA_TACTIC_PATROL)           .SetMaxValue(1);
}
//---------------------------------------------------------------------------

CIGSubSysTactics::~CIGSubSysTactics()
{
}
//---------------------------------------------------------------------------

void CIGSubSysTactics::OnActionTacticMove ()
{
  CIGUnit* parent = dynamic_cast<CIGUnit*>(GetParent());

  // execute the parent script
  if (parent->mValidCsl)
  {
    try
    {
      TIGSubSysActionData actionData = GetCurrentAction()->GetData();

      parent->mCompiler->call(parent->mModuleName, "OnActionTacticMove", 2, CPString(actionData.mTargetPoint.x).c_str(), CPString(actionData.mTargetPoint.y).c_str());
    }
    catch(const ZException& err)
    {
      LOG(ERROR) << "Fail to run OnActionTacticMove()";
      for (int i = 0; i < err.count(); i++)
      {
        LOG(ERROR) << (char*)err[i];
      }
    }
  }
  else
  {
    // fall back to default move behavior by stopping this action and posting a
    // low-level move action to the parent
    TIGSubSysActionData actionData = GetCurrentAction()->GetData();
    GetCurrentAction()->End();

    // setup the low-level move action
    actionData.mType = OA_MOVE;
    actionData.mTargetPoint = CPPoint(actionData.mTargetPoint.x * MAPCELLX, actionData.mTargetPoint.y * MAPCELLY);

    parent->SetAction(actionData);
  }
}
//---------------------------------------------------------------------------

void CIGSubSysTactics::OnActionTacticPatrol ()
{
  CIGUnit* parent = dynamic_cast<CIGUnit*>(GetParent());

  // execute the parent script
  if (parent->mValidCsl)
  {
    try
    {
      parent->mCompiler->call(parent->mModuleName, "OnActionTacticPatrol");
    }
    catch(const ZException& err)
    {
      LOG(ERROR) << "Fail to run OnActionTacticPatrol()";
      for (int i = 0; i < err.count(); i++)
      {
        LOG(ERROR) << (char*)err[i];
      }
    }
  }
  else
  {
    // fall back to default patrol behavior by stopping this action and posting a
    // low-level move action to the parent
    TIGSubSysActionData actionData = GetCurrentAction()->GetData();
    GetCurrentAction()->End();

    // setup the low-level move action
    actionData.mType = OA_PATROL;
    actionData.mTargetPoint = CPPoint(actionData.mTargetPoint.x * MAPCELLX, actionData.mTargetPoint.y * MAPCELLY);
    actionData.mStartPoint = CPPoint(actionData.mStartPoint.x * MAPCELLX, actionData.mStartPoint.y * MAPCELLY);

    parent->SetAction(actionData);
  }
}
//---------------------------------------------------------------------------

void CIGSubSysTactics::OnActionTacticAttack ()
{
  CIGUnit* parent = dynamic_cast<CIGUnit*>(GetParent());

  // execute the parent script
  if (parent->mValidCsl)
  {
    try
    {
      parent->mCompiler->call(parent->mModuleName, "OnActionTacticAttack");
    }
    catch(const ZException& err)
    {
      LOG(ERROR) << "Failed to run OnActionTacticAttack()";
      for (int i = 0; i < err.count(); i++)
      {
        LOG(ERROR) << (char*)err[i];
      }
    }
  }
  else
  {
    // fall back to default attack behavior by stopping this action and posting a
    // low-level move action to the parent
    TIGSubSysActionData actionData = GetCurrentAction()->GetData();
    GetCurrentAction()->End();

    // setup the low-level move action
    actionData.mType = OA_ATTACK;
    actionData.mTargetPoint = CPPoint(actionData.mTargetPoint.x * MAPCELLX, actionData.mTargetPoint.y * MAPCELLY);

    parent->SetAction(actionData);
  }
}
//---------------------------------------------------------------------------

void CIGSubSysTactics::OnActionTacticAttackGround ()
{
  CIGUnit* parent = dynamic_cast<CIGUnit*>(GetParent());

  // execute the parent script
  if (parent->mValidCsl)
  {
    try
    {
      parent->mCompiler->call(parent->mModuleName, "OnActionTacticAttackGround");
    }
    catch(const ZException& err)
    {
      LOG(ERROR) << "Fail to run OnActionTacticAttackGround()";
      for (int i = 0; i < err.count(); i++)
      {
        LOG(ERROR) << (char*)err[i];
      }
    }
  }
  else
  {
    // fall back to default attack ground behavior by stopping this action and posting a
    // low-level move action to the parent
    TIGSubSysActionData actionData = GetCurrentAction()->GetData();
    GetCurrentAction()->End();

    // setup the low-level move action
    actionData.mType = OA_ATTACKGROUND;
    actionData.mTargetPoint = CPPoint(actionData.mTargetPoint.x * MAPCELLX, actionData.mTargetPoint.y * MAPCELLY);

    parent->SetAction(actionData);
  }
}
//---------------------------------------------------------------------------

void CIGSubSysTactics::OnActionTacticStop()
{
  SetCurrentActionIndex(OA_TACTIC_STOP);
  GetCurrentAction()->End();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets the instance of the wanted action
---------------------------------------------------------------------------*/
CIGSubSysAction* CIGSubSysTactics::GetActionInstance(TIGSubSysActionData actionData)
{
   CIGSubSysAction *lTmpAction = NULL;
   CIGBaseObject* lParent = GetParent();
   bool lIsDead = lParent->GetIsDead();

   switch(actionData.mType)
   {
      case OA_TACTIC_MOVE:
         if (lIsDead == false)
            lTmpAction = new CIGSubSysActionTacticMove(this, actionData);
         break;

      case OA_TACTIC_ATTACK:
         if (lIsDead == false)
            lTmpAction = new CIGSubSysActionTacticAttack(this, actionData);
         break;

      case OA_TACTIC_PATROL:
         if (lIsDead == false)
           lTmpAction = new CIGSubSysActionTacticPatrol(this, actionData);
         break;

      case OA_TACTIC_ATTACKGROUND:
         if (lIsDead == false)
         {
           lTmpAction = new CIGSubSysActionTacticAttackGround(this, actionData);
         }
         break;

      case OA_TACTIC_STOP:
         if (lIsDead == false)
            lTmpAction = new CIGSubSysActionTacticStop(this, actionData);
         break;
   }
   return lTmpAction;
}
//---------------------------------------------------------------------------

