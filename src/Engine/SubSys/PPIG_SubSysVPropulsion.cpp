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

#include "PPIG_SubSysVPropulsion.h"
#include "PPIG_SubSysAction.h"
#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "basepath.h"
#include "Paths.h"
#include "PPIG_SpaceSound.h"
#include "Equipment.h"
#include "PPIG_EngineDef.h"
#include "PPIG_SubSysVisual.h"
#include "PPIG_ObjTank.h"
#include "PPIG_ObjTran.h"
#include "PPIG_ObjDropship.h"
#include "PPIG_BGManager.h"
//---------------------------------------------------------------------------

CIGSubSysVPropulsion::CIGSubSysVPropulsion(CIGVehicle* _parent) : CIGSubSysBase(_parent)
{
  //Action step counter
  GetActionStepCounters(OA_STOP)         .SetMaxValue(0);
  GetActionStepCounters(OA_MOVE)         .SetMaxValue(0);
  GetActionStepCounters(OA_ATTACKMOVE)   .SetMaxValue(0);
  GetActionStepCounters(OA_PATROL)       .SetMaxValue(0);
  GetActionStepCounters(OA_HOLD)         .SetMaxValue(0);
  GetActionStepCounters(OA_DIE)          .SetMaxValue(0);
  GetActionStepCounters(OA_DEAD)         .SetMaxValue(0);
  GetActionStepCounters(OA_LOAD)         .SetMaxValue(3);
  //------------------

  //Action update settings
  GetUpdateCounters(OA_MOVE)             .SetMaxValue(1);
  GetUpdateCounters(OA_ATTACKMOVE)       .SetMaxValue(1);
  GetUpdateCounters(OA_PATROL)           .SetMaxValue(1);
  GetUpdateCounters(OA_DIE)              .SetMaxValue(20);
  GetUpdateCounters(OA_DEAD)             .SetMaxValue(8192);
  GetUpdateCounters(OA_LOAD)             .SetMaxValue(2);

  //------------------
  //Action anim types
  SetActionAnimTypes(OA_STOP, 0);
  SetActionAnimTypes(OA_MOVE, 0);
  SetActionAnimTypes(OA_ATTACKMOVE, 0);
  SetActionAnimTypes(OA_PATROL, 0);
  SetActionAnimTypes(OA_HOLD, 0);
  SetActionAnimTypes(OA_GUARD, 0);
  SetActionAnimTypes(OA_LOAD, 0);
  SetActionAnimTypes(OA_DIE, 3);
  SetActionAnimTypes(OA_DEAD, 3);
  //------------------

  // default values (setting sprite for parrent may override these values)
  // 8 columns in sprite/8 directions
  GetSenseCounter().SetMaxValue(7);
  GetAnimCounter().SetMaxValue(7);
}
//---------------------------------------------------------------------------

CIGSubSysVPropulsion::~CIGSubSysVPropulsion()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: stop action handler
---------------------------------------------------------------------------*/
void CIGSubSysVPropulsion::OnActionStop ()
{
   CIGVehicle* lParent = (CIGVehicle*)GetParent();

   if (lParent->IsInFixedPlace() == false)
   {
      VLOG(9) << "NOT IN FIXED PLACE";
      SetCurrentActionIndex(OA_MOVE);
      lParent->RemoveFromLayerMap();
      lParent->SetCoord(CPPoint(lParent->GetCoord().x + lParent->GetSpeed()()*gDir[GetSenseCounter()()].x,
                       lParent->GetCoord().y + lParent->GetSpeed()()*gDir[GetSenseCounter()()].y ));
      lParent->NoteInLayerMap();
   }
   else
   {
      VLOG(9) << "IN FIXED PLACE";
      SetCurrentActionIndex(OA_STOP);
      GetCurrentAction()->End();
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: move action handler
---------------------------------------------------------------------------*/
void CIGSubSysVPropulsion::OnActionMove ()
{
   CIGVehicle* lParent = (CIGVehicle*)GetParent();

   if (lParent->GetWayLength() > 0)
   {//there is still way
      if (lParent->GetNextWay() != WAY_SEARCHING)
      {//that means the path is founded
         if (lParent->IsInFixedPlace() == true)
         {//next step
            SetCurrentActionIndex(OA_STOP);
            TurnToSense(lParent->GetNextWay());
            if (GetSenseCounter() == lParent->GetNextWay())
            {
               if (GetCurrentAction()->GetType() == OA_MOVE ||
                   GetCurrentAction()->GetType() == OA_PATROL)
                  SetCurrentActionIndex(OA_MOVE);
               else
                  SetCurrentActionIndex(OA_ATTACKMOVE);

               // because of variable references, this is done in two steps
               CPPoint lTmpPoint = lParent->GetLogicCoord();
               lParent->SetLastPoint(lTmpPoint);

               lParent->SetNextPoint(CPPoint(lParent->GetLogicCoord().x + gDir[GetSenseCounter()()].x,
                                    lParent->GetLogicCoord().y + gDir[GetSenseCounter()()].y));

               // fixing unit overlapping
               if (IsNextPlaceEmpty() == true)
               //if (lParent->IsNextPlaceEmpty() == true)
               {//next place is empty => step
                  CPPoint lNextPoint = lParent->GetNextPoint();

                  lParent->RemoveFromLogicMap();
                  lParent->NoteInLogicMap(lNextPoint);
                  Move();
                  lParent->SetWayLength(lParent->GetWayLength() - 1);

                  // set Z coord of the object to the altitude of the tile
                  // it's not so necessary, but just to be niceeee :)
                  lParent->SetCoordZ(GetMissionInstance()->GetMap()->GetAltitude(lNextPoint.x, lNextPoint.y));

                  // TODO: adjust the speed of the parent according to the slope
               }
               else
               {  //the next place is not empty => wait if the object is moving
                  //wait state
                  // TODO: Movelock - is this correct? or should the original grabX code remains?
                  SetCurrentActionIndex(OA_STOP);
                  lParent->ComputeWay();


                  // original code
                  //if (IsNextObjectMoving() == false)
                  //{
                  //   ComputeWay();
                  //}
                  //else
                  //{
                  // wait ?
                  //}/
               }
            }
         }
         else
         {//is not in fixed place => step forward
            if (GetCurrentAction()->GetType() == OA_MOVE ||
                GetCurrentAction()->GetType() == OA_PATROL)
               SetCurrentActionIndex(OA_MOVE);
            else
               SetCurrentActionIndex(OA_ATTACKMOVE);
            Move();
         }
      }
      else //is waiting for the path server
      {
         GetActionStepCounter()--;
      }
   }
   else //no other steps in the way
   {
      if (lParent->IsInFixedPlace() == false)
      {
         if (GetCurrentAction()->GetType() == OA_MOVE ||
             GetCurrentAction()->GetType() == OA_PATROL)
            SetCurrentActionIndex(OA_MOVE);
         else
            SetCurrentActionIndex(OA_ATTACKMOVE);
//         SetCurrentActionIndex(OA_MOVE);
         Move();
      }
      else
      {
        //Stop();
        GetCurrentAction()->End();
      }
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: patrol action handler
---------------------------------------------------------------------------*/
void CIGSubSysVPropulsion::OnActionPatrol ()
{
   OnActionMove();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: hold action handler
---------------------------------------------------------------------------*/
void CIGSubSysVPropulsion::OnActionHold ()
{
/*   logWriteLn("CIGSubSysVPropulsion::OnActionHold ", LOGDET_LOWEST);
   SetCurrentActionIndex(OA_HOLD);
   bool lFound = false;
   TIGObjectActionData lTmpActionData;
   for (int i=GetRange(); i<GetRange() && !lFound; i++)
      for (int j=-GetRange(); j<GetRange()&& !lFound; j++)
         if (IsEnemy(GetLogicCoord().x + i, GetLogicCoord().y + j) == true)
         {
            lTmpActionData.mTargetObject = GetMissionInstance()->GetMap()->GetGroundObject(GetLogicCoord().x + i, GetLogicCoord().y + j);
            lTmpActionData.mTargetPoint = CPPoint((GetLogicCoord().x + i)*MAPCELLX,
                                                  (GetLogicCoord().y + j)*MAPCELLY);
            lTmpActionData.mType = OA_ATTACK;
            lFound = true;
         }
   if (lFound == true)
      SetAction(lTmpActionData);
   logWriteLn("CIGSubSysVPropulsion::OnActionHold - over", LOGDET_LOWEST);
*/
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: guard action handler
---------------------------------------------------------------------------*/
void CIGSubSysVPropulsion::OnActionGuard ()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description:
---------------------------------------------------------------------------*/
void CIGSubSysVPropulsion::OnActionDead()
{
  CIGVehicle* lParent = (CIGVehicle*)GetParent();

  VLOG(9) << "CIGSubSysVPropulsion::OnActionDead";
  if (GetActionStepCounters(OA_DEAD).MaxReached() == true)
  {
    GetParent()->RemoveFromLayerMap();
    GetParent()->RemoveFromLogicMap();

    GetMissionInstance()->AddToDeleteObjects(lParent);
    GetMissionInstance()->GetBGManager()->RemoveUnit(lParent);
  }
  VLOG(9) << "CIGSubSysVPropulsion::OnActionDead - over";
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: load action handler
---------------------------------------------------------------------------*/
void CIGSubSysVPropulsion::OnActionLoad ()
{
   CIGVehicle* lParent = (CIGVehicle*)GetParent();

   VLOG(9) << "CIGSubSysVPropulsion::OnActionLoad ";

   // check if the target transporter hasn't passed away
   if (GetCurrentAction()->GetData().mTargetObject->GetIsDead() == true)
   {
      GetCurrentAction()->End();
      return;
   }

   // if the unit isn't close to the transporter, then move it there
   CPPoint lCoord = lParent->GetLogicCoord();

   if (lParent->IsInFixedPlace() == true)
   {
     // check if the unit is close to the transport vehicle/aircraft and the transport vehicle is stopped
     if (gfGetDist2D(lCoord, GetCurrentAction()->GetData().mTargetObject->GetClosestPointTo(lCoord)) <= 2 &&
         !(GetCurrentAction()->GetData().mTargetObject->GetMoving()))
     {
       if (GetCurrentAction()->GetData().mTargetObject->ClassName() == CPString(IGCN_DROPSHIPUNIT))
       {
         CIGDropship* lDropshipObj = (CIGDropship*)(GetCurrentAction()->GetData().mTargetObject);

         // TODO: check PPIG_SubSysPropulsion for detailed information on this
         // check - sorry ;)
         if (!lDropshipObj->GetIsDead())
         {
           lDropshipObj->Load(lParent);
         }
       }

       GetCurrentAction()->End();

       lParent->RemoveFromLogicMap();
       lParent->RemoveFromLayerMap();

       // remove this unit from all objects list, the transporter will be the responsible
       // for this unit
       GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Remove(lParent);

       TPoliticsRelations relation = GetMissionInstance()->GetBGManager()->GetRelation(OS_PLAYER, lParent->GetBattleGroupId());

       // and remove the unit from selections lists
       switch (relation)
        {
           case REL_NEUTRAL:
              GetMissionInstance()->GetObjects(MOL_NEUTRALUNITS)->Remove(lParent);
              GetMissionInstance()->GetObjects(MOL_SELECTEDOTHERS)->Remove(lParent);
              break;
           case REL_OWNED:
              GetMissionInstance()->GetObjects(MOL_OWNEDUNITS)->Remove(lParent);
              GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS)->Remove(lParent);
              break;
           case REL_FRIENDLY:
              GetMissionInstance()->GetObjects(MOL_FRIENDLYUNITS)->Add(lParent);
              break;
           case REL_ENEMY:
              GetMissionInstance()->GetObjects(MOL_ENEMYUNITS)->Remove(lParent);
              GetMissionInstance()->GetObjects(MOL_SELECTEDOTHERS)->Remove(lParent);
              break;
           default:
              VLOG(2) << "Unkown relation " << relation << " between player and battle group with id " << lParent->GetBattleGroupId();
              break;
       }

       // deselect the unit
       lParent->SetSelected(false);
     }
     else
     {
       if (lParent->GetWayLength() > 0)
          OnActionMove();
       else
       {
          SetCurrentActionIndex(OA_STOP);

          GetCurrentAction()->SetTargetPoint(GetCurrentAction()->GetData().mTargetObject->GetClosestPointTo(lParent->GetLogicCoord()));

          lParent->ComputeWay();

          OnActionMove();
       }
     }
   }
   else
   {
     SetCurrentActionIndex(OA_LOAD);
     Move();
   }

   VLOG(9) << "CIGSubSysVPropulsion::OnActionLoad - over";
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets the instance of the wanted action
---------------------------------------------------------------------------*/
CIGSubSysAction* CIGSubSysVPropulsion::GetActionInstance(TIGSubSysActionData actionData)
{
   CIGSubSysAction *lTmpAction = NULL;
   CIGVehicle* lParent = (CIGVehicle*)GetParent();
   bool lIsDead = lParent->GetIsDead();

   switch(actionData.mType)
   {
      case OA_STOP:
         if (lIsDead == false)
         {
           lTmpAction = new CIGSubSysActionStop(this, actionData);
         }
         break;
      case OA_MOVE:
         if (lIsDead == false)
         {
           lTmpAction = new CIGSubSysActionMove(this, actionData);
         }
         break;
      case OA_PATROL:
         if (lIsDead == false)
         {
           lTmpAction = new CIGSubSysActionPatrol(this, actionData);
         }
         break;
      case OA_GUARD:
//         if (GetIsDead() == false)
//            lTmpAction = new CIGObjectActionGuard(this, actionData);
         break;
      case OA_HOLD:
//         if (GetIsDead() == false)
//            lTmpAction = new CIGObjectActionHold(this, actionData);
         break;
      case OA_UNLOAD_ALL:
         if (lIsDead == false)
         {
           // for a transporter, unload all means move to the drop point
           lTmpAction = new CIGSubSysActionMove(this, actionData);
         }
         break;
      case OA_LOAD:
         if (lIsDead == false)
            lTmpAction = new CIGSubSysActionLoad(this, actionData);
         break;
      case OA_DIE:
         lTmpAction = new CIGSubSysActionDie(this, actionData);
         break;

      case OA_DEAD:
         lTmpAction = new CIGSubSysActionDead(this, actionData);
         break;
   }
   return lTmpAction;
}
//---------------------------------------------------------------------------

void CIGSubSysVPropulsion::Move()
{
   CIGTankUnit* lParent = (CIGTankUnit*)GetParent();

   lParent->RemoveFromLayerMap();

   //UpdateView(0); // gray the view
   lParent->SetCoord(CPPoint(lParent->GetCoord().x + lParent->GetSpeed()()*gDir[GetSenseCounter()()].x,
                    lParent->GetCoord().y + lParent->GetSpeed()()*gDir[GetSenseCounter()()].y));

   lParent->UpdateView(2);

   lParent->NoteInLayerMap();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: stops the object, in fact it puts the current action to stop
---------------------------------------------------------------------------*/
void CIGSubSysVPropulsion::Stop()
{
   CIGVehicle* lParent = (CIGVehicle*)GetParent();

   lParent->SetMoving(false);
   TIGSubSysActionData lTmpActionData;
   lTmpActionData.mType = OA_STOP;
   SetCurrentAction(GetActionInstance(lTmpActionData));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: modify the sense to the wanted sense
 parameters : wantedSense - the wanted sense
 others     : this should be overwritten
---------------------------------------------------------------------------*/
void CIGSubSysVPropulsion::TurnToSense(int wantedSense)
{
   SetSenseCounter(GetSenseCounter().ToValue(wantedSense)());
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: SenseCounter
---------------------------------------------------------------------------*/
void CIGSubSysVPropulsion::SetSenseCounter(int value)
{
  CIGTankUnit* lParent = (CIGTankUnit*)GetParent();

  CIGSubSysBase::SetSenseCounter(value);
  SetAnimCounter(value);

  int lTmpDir = gDirAngle[GetSenseCounter()] * 180.0 / PI;

  if (lParent->GetLookAt() == UL_NOTHING && !(lTmpDir > lParent->GetScanAngle() - 4 && lTmpDir < lParent->GetScanAngle() + 4))
  {
    lParent->SetScanRange(6);
    lParent->SetScanAngle(lTmpDir);
  }
  /*
  int lTmpDir = gDirAngle[GetSenseCounter()] * 180.0 / PI;

  if (mLookAt == IUL_NOTHING && !(lTmpDir > mScanAngle - 4 && lTmpDir < mScanAngle + 4))
  {
    mScanRange = 6;
    mScanAngle = lTmpDir;
  }
  */
}
//---------------------------------------------------------------------------

void CIGSubSysVPropulsion::SetSenseCounter(CPCounter &value)
{
  CIGTankUnit* lParent = (CIGTankUnit*)GetParent();

  CIGSubSysBase::SetSenseCounter(value);
  SetAnimCounter(value());

  int lTmpDir = gDirAngle[GetSenseCounter()] * 180.0 / PI;

  if (lParent->GetLookAt() == UL_NOTHING && !(lTmpDir > lParent->GetScanAngle() - 4 && lTmpDir < lParent->GetScanAngle() + 4))
  {
    lParent->SetScanRange(6);
    lParent->SetScanAngle(lTmpDir);
  }
  /*
  int lTmpDir = gDirAngle[GetSenseCounter()] * 180.0 / PI;

  if (mLookAt == IUL_NOTHING && !(lTmpDir + 4 > mScanAngle && lTmpDir - 4 < mScanAngle))
  {
    mScanRange = 6;
    mScanAngle = lTmpDir;
  }
  */
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////

/*---------------------------------------------------------------------------
 description: returns true if the place is empty
 parameters : Point - the queried point
---------------------------------------------------------------------------*/
bool CIGSubSysVPropulsion::IsNextPlaceEmpty()
{
  CIGVehicle* lParent = (CIGVehicle*)GetParent();
  CPPoint lCoord = lParent->GetLogicCoord();

  for (int i=lCoord.x + gDir[GetSenseCounter()()].x; i<lCoord.x + gDir[GetSenseCounter()()].x + lParent->GetLogicDimX(); i++)
  {
    for (int j=lCoord.y + gDir[GetSenseCounter()()].y; j<lCoord.y + gDir[GetSenseCounter()()].y + lParent->GetLogicDimY(); j++)
    {
       if (GetMissionInstance()->IsGroundPlaceEmpty(lParent, i, j) == false)
       {
          return false;
       }
    }
  }

  return true;
}
//--------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description:returns true if the object in next place is moving
---------------------------------------------------------------------------*/
bool CIGSubSysVPropulsion::IsNextObjectMoving()
{
  CIGVehicle* lParent = (CIGVehicle*)GetParent();
  CPPoint lCoord = lParent->GetLogicCoord();

  for (int i=lCoord.x + gDir[GetSenseCounter()()].x; i<lCoord.x + gDir[GetSenseCounter()()].x + lParent->GetLogicDimX(); i++)
  {
    for (int j=lCoord.y + gDir[GetSenseCounter()()].y; j<lCoord.y + gDir[GetSenseCounter()()].y + lParent->GetLogicDimY(); j++)
    {
       if (GetMissionInstance()->IsGroundObjectMoving(lParent, i, j) == false)
       {
          return false;
       }
    }
  }

  return true;
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

