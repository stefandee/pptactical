//---------------------------------------------------------------------------

#ifndef PPIG_SubSysTurretActionH
#define PPIG_SubSysTurretActionH
//---------------------------------------------------------------------------

#include "PPIG_SubSysAction.h"

/////////////////////////////////////////////////////////////////////////////
// ACTION TURRET MOVE SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionTurretRotate : public CIGSubSysAction 
{
   public:
      CIGSubSysActionTurretRotate(CIGSubSysBase *object, TIGSubSysActionData data) : CIGSubSysAction(object, data)
      {
         SetType(OA_ROTATE);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
      virtual void Initiate();
      virtual void End();
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//ACTION TURRET STOP SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionTurretStop : public CIGSubSysAction
{
   public:
      CIGSubSysActionTurretStop(CIGSubSysBase *object, TIGSubSysActionData data)
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
//ACTION TURRET FIRE/ATTACK SECTION
/////////////////////////////////////////////////////////////////////////////
class CIGSubSysActionTurretAttack: public CIGSubSysAction
{
   public:
      CIGSubSysActionTurretAttack(CIGSubSysBase *object, TIGSubSysActionData data)
                          :CIGSubSysAction(object, data)
      {
         SetType(OA_ATTACK);
      }
      virtual CIGSubSysAction* Clone();
      virtual void Handle();
      virtual void UpdateIdle();
};
//---------------------------------------------------------------------------

#endif
