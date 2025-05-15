//---------------------------------------------------------------------------

#pragma hdrstop

#include "PPIG_SubSysTurretAction.h"
#include "PPIG_SubSysBase.h"
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION TURRET MOVE SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionTurretRotate::Clone()
{
   return new CIGSubSysActionTurretRotate(GetSubSys(), GetData());
}

void CIGSubSysActionTurretRotate::Handle()
{
   GetSubSys()->OnActionRotate();
}

void CIGSubSysActionTurretRotate::Initiate()
{
//   GetObject()->SetMoving(true);
//   GetObject()->ComputeWay();
}

void CIGSubSysActionTurretRotate::End()
{
//   CIGObjectAction::End();
   GetSubSys()->OnActionStop();
}

/////////////////////////////////////////////////////////////////////////////
//ACTION TURRET STOP SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionTurretStop::Clone()
{
   return new CIGSubSysActionTurretStop(GetSubSys(), GetData());
}
void CIGSubSysActionTurretStop::Handle()
{
   GetSubSys()->OnActionStop();
}
void CIGSubSysActionTurretStop::Initiate()
{
//   GetObject()->SetMoving(false);
//   GetObject()->SetWayLength(0);
}

/////////////////////////////////////////////////////////////////////////////
//ACTION ATTACK SECTION
/////////////////////////////////////////////////////////////////////////////
CIGSubSysAction *CIGSubSysActionTurretAttack::Clone()
{
   return new CIGSubSysActionTurretAttack(GetSubSys(), GetData());
}

void CIGSubSysActionTurretAttack::Handle()
{
   GetSubSys()->OnActionAttack();
}

void CIGSubSysActionTurretAttack::UpdateIdle()
{
   if (GetSubSys()->GetUpdateCounters(OA_ATTACK) >= 5)
      GetSubSys()->GetActionStepCounters(OA_ATTACK) = 1;
}

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

