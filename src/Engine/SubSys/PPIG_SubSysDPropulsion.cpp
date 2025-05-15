//---------------------------------------------------------------------------

#include <math.h>
#include <stdlib.h>
#pragma hdrstop

#include "PPIG_SubSysDPropulsion.h"
#include "PPIG_ObjDamage.h"
#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "basic3d.h"
//---------------------------------------------------------------------------

CIGSubSysDPropulsion::CIGSubSysDPropulsion(CIGDamageObject* _parent) : CIGSubSysBase(_parent)
{
  GetSenseCounter().GetThis()->SetMaxValue(15);
  GetAnimCounter().GetThis()->SetMaxValue(15);
  mCurrentAngle = 0;
}
//---------------------------------------------------------------------------

CIGSubSysDPropulsion::~CIGSubSysDPropulsion()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: turns to the wanted angle
---------------------------------------------------------------------------*/
void CIGSubSysDPropulsion::TurnToAngle(float Angle)
{
  if (Angle == mCurrentAngle)
   return;

  if ((Angle - mCurrentAngle < PI && Angle - mCurrentAngle > 0) ||
     (Angle - mCurrentAngle < -PI))
    mCurrentAngle = mCurrentAngle + PI/12;
  else
    mCurrentAngle = mCurrentAngle - PI/12;

  if (mCurrentAngle >= 2*PI)
    mCurrentAngle = mCurrentAngle - 2*PI;

  if (mCurrentAngle < 0)
    mCurrentAngle = mCurrentAngle + 2*PI;

  if (fabs(mCurrentAngle - Angle) < PI/6)
    mCurrentAngle = Angle;
}

// Description : checks if the trajectory of the damage object is valid (does not hit
//               the terrain) 
// Param       : _startPoint, _endPoint - points in real coordinates
// Result      : (-1, -1) - if the path is valid
//               (x, y) - if the path is invalid (the point where the object hit the
//                        terrain
// Comments    : no bresenham, just a normal line scanner
CPPoint CIGSubSysDPropulsion::CheckPath(CPPoint _startPoint, CPPoint _endPoint)
{
  float x1 = _startPoint.x;
  float y1 = _startPoint.y;
  float x2 = _endPoint.x;
  float y2 = _endPoint.y;

  float lM;
  float lReferenceAltitude = GetMissionInstance()->GetMap()->GetAltitude(x1/MAPCELLX, y1/MAPCELLY);

  if (abs(y2 - y1) > abs(x2 - x1))
  {
    if (y2 == y1)
    {
      lM = (float)(x2 - x1) * 100.0;
    }
    else
    {
      lM = (float)(x2 - x1) / (y2 - y1);
    }

    int   y = y1;
    float x = x1;
    int   s = SIGN(y2 - y1);

    while (y != (int)y2)
    {
      if (GetMissionInstance()->GetMap()->GetAltitude(x/MAPCELLX, y/MAPCELLY) > lReferenceAltitude)
      {
        return CPPoint(x, y);
      }
      
      y += s;
      x += lM;
    }
    
    // last check
    if (GetMissionInstance()->GetMap()->GetAltitude(x/MAPCELLX, y/MAPCELLY) > lReferenceAltitude)
    {
      return CPPoint(x, y);
    }
  }
  else
  {
    if (x2 == x1)
    {
      lM = (float)(y2 - y1) * 100.0;
    }
    else
    {
      lM = (float)(y2 - y1) / (x2 - x1);
    }  

    float y = y1;
    int   x = x1;
    int   s = SIGN(x2 - x1);

    while (x != (int)x2)
    {
      if (GetMissionInstance()->GetMap()->GetAltitude(x/MAPCELLX, y/MAPCELLY) > lReferenceAltitude)
      {
        return CPPoint(x, y);
      }
      
      x += s;
      y += lM;
    }

    // last check
    if (GetMissionInstance()->GetMap()->GetAltitude(x/MAPCELLX, y/MAPCELLY) > lReferenceAltitude)
    {
      return CPPoint(x, y);
    }
  }
        
  return CPPoint(-1, -1);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: Sense
---------------------------------------------------------------------------*/
void CIGSubSysDPropulsion::SetSenseCounter(CPCounter& value)
{
   CIGSubSysBase::SetSenseCounter(value);
   SetAnimCounter(value());
}

void CIGSubSysDPropulsion::SetSenseCounter(int value)
{
   CIGSubSysBase::SetSenseCounter(value);
   SetAnimCounter(value);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets the instance of the wanted action
---------------------------------------------------------------------------*/
CIGSubSysAction* CIGSubSysDPropulsion::GetActionInstance(TIGSubSysActionData actionData)
{
   CIGSubSysAction *lTmpAction = NULL;
   CIGDamageObject* lParent = (CIGDamageObject*)GetParent();
   bool lIsDead = lParent->GetIsDead();

   switch(actionData.mType)
   {
//      case OA_STOP:
//         if (lIsDead == false)
//            lTmpAction = new CIGSubSysActionStop(this, actionData);
//         break;
      case OA_ATTACK:
         if (lIsDead == false)
            lTmpAction = new CIGSubSysActionAttack(this, actionData);
         break;
      case OA_ATTACKGROUND:
         if (lIsDead == false)
         {
           lTmpAction = new CIGSubSysActionAttackGround(this, actionData);
         }
         break;
   }

   return lTmpAction;      
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//
// Class serialize/deserialize section
//
//---------------------------------------------------------------------------

void CIGSubSysDPropulsion::Serialize(PP::Stream& _a)
{
  // base class serialize
  CIGSubSysBase::Serialize(_a);

  // members
  _a.writeFloat(&mCurrentAngle);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGSubSysDPropulsion::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void CIGSubSysDPropulsion::DeSerialize(PP::Stream& _a)
{
  // base class deserialize
  CIGSubSysBase::DeSerialize(_a);

  // members
  _a.readFloat(&mCurrentAngle);
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
