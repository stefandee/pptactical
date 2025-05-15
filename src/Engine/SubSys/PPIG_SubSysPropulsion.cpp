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

#include "PPIG_SubSysPropulsion.h"
#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "basepath.h"
#include "Paths.h"
#include "PPIG_SpaceSound.h"
#include "Equipment.h"
#include "PPIG_EngineDef.h"
#include "PPIG_ObjInfantry.h"
#include "PPIG_ObjDamage.h"
#include "PPIG_ObjRocket.h"
#include "PPIG_ObjBullet.h"
#include "PPIG_ObjLaser.h"
#include "PPIG_ObjPlasma.h"
#include "PPIG_ObjTran.h"
#include "PPIG_ObjDropship.h"
#include "PPIG_WeaponRules.h"
#include "PPIG_BGManager.h"
#include "PPIG_CScreen.h"
//#include "PPIG_ObjBase.h"

//---------------------------------------------------------------------------

CIGSubSysPropulsion::CIGSubSysPropulsion(CIGInfantryUnit* _parent) : CIGSubSysBase(_parent)
{
  //Action step counter
  GetActionStepCounters(OA_STOP)         .SetMaxValue(0);
  GetActionStepCounters(OA_MOVE)         .SetMaxValue(3);
  GetActionStepCounters(OA_LOAD)         .SetMaxValue(3);
  GetActionStepCounters(OA_ATTACKMOVE)   .SetMaxValue(3);
  GetActionStepCounters(OA_ATTACK)       .SetMaxValue(1);
  GetActionStepCounters(OA_ATTACKGROUND) .SetMaxValue(1);
  GetActionStepCounters(OA_PATROL)       .SetMaxValue(3);
  GetActionStepCounters(OA_HOLD)         .SetMaxValue(0);
  GetActionStepCounters(OA_DIE)          .SetMaxValue(2);
  GetActionStepCounters(OA_DEAD)         .SetMaxValue(1);
  //------------------

  //Action update settings
  GetUpdateCounters(OA_MOVE)             .SetMaxValue(2);
  GetUpdateCounters(OA_LOAD)             .SetMaxValue(2);
  GetUpdateCounters(OA_ATTACK)           .SetMaxValue(120); // this will be updated by the weapon fire rate
  GetUpdateCounters(OA_ATTACKGROUND)     .SetMaxValue(120); // this will be updated by the weapon fire rate
  GetUpdateCounters(OA_PATROL)           .SetMaxValue(1);
  GetUpdateCounters(OA_DIE)              .SetMaxValue(20);
  GetUpdateCounters(OA_DEAD)             .SetMaxValue(8192);
  GetUpdateCounters(OA_ATTACKMOVE)       .SetMaxValue(1);

  //------------------
  //Action anim types
  SetActionAnimTypes(OA_STOP, 0);
  SetActionAnimTypes(OA_MOVE, 1);
  SetActionAnimTypes(OA_LOAD, 1);
  SetActionAnimTypes(OA_ATTACKMOVE, 5);
  SetActionAnimTypes(OA_ATTACK, 9);
  SetActionAnimTypes(OA_ATTACKGROUND, 9); // really?
  SetActionAnimTypes(OA_PATROL, 1);
  SetActionAnimTypes(OA_HOLD, 13);
  SetActionAnimTypes(OA_GUARD, 0);
  SetActionAnimTypes(OA_CAPTURE, 0);
  SetActionAnimTypes(OA_DIE, 11);
  SetActionAnimTypes(OA_DEAD, 13);
  //------------------

  // default values (setting sprite for parent may override these values)
  // 8 columns in sprite/8 directions
  GetSenseCounter().SetMaxValue(7);
  GetAnimCounter().SetMaxValue(7);
}
//---------------------------------------------------------------------------

CIGSubSysPropulsion::~CIGSubSysPropulsion()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: stop action handler
---------------------------------------------------------------------------*/
void CIGSubSysPropulsion::OnActionStop ()
{
   CIGInfantryUnit* lParent = (CIGInfantryUnit*)GetParent();

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
void CIGSubSysPropulsion::OnActionMove ()
{
   CIGInfantryUnit* lParent = (CIGInfantryUnit*)GetParent();

   if (lParent->GetWayLength() > 0)
   {//there is still way
      if (lParent->GetNextWay() != WAY_SEARCHING)
      {//that means the path is found
         if (lParent->IsInFixedPlace() == true)
         {//next step
            SetCurrentActionIndex(OA_STOP);
            TurnToSense(lParent->GetNextWay());

            //int l1 = GetSenseCounter();
            //int l2 = GetSenseCounter().GetMaxValue();
            //int l3 = lParent->GetNextWay();
            //int l4 = GetAnimCounter();

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

               //if (lParent->IsNextPlaceEmpty() == true)
               // fixing unit overlapping
               if (IsNextPlaceEmpty() == true)
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
         GetCurrentAction()->End();
         //Stop();
      }
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: patrol action handler
---------------------------------------------------------------------------*/
void CIGSubSysPropulsion::OnActionPatrol ()
{
   OnActionMove();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: attack action handler
---------------------------------------------------------------------------*/
void CIGSubSysPropulsion::OnActionAttack ()
{
  CIGInfantryUnit* lParent = (CIGInfantryUnit*)GetParent();

  VLOG(9) << "CIGSubSysPropulsion::OnActionAttack ";

  if (GetCurrentAction()->GetData().mTargetObject->GetIsDead() == true)
  {
    GetCurrentAction()->End();
    return;
  }

  // check if the infantry unit may fire the object by asking the weapon rules class
  if (!CIGWeaponRules().CanFireObject(lParent, lParent->GetWepType(), GetCurrentAction()->GetData().mTargetObject))
  {
    GetCurrentAction()->End();
    return;
  }

  if (lParent->IsInFixedPlace() == true)
  {
      CPPoint lCoord = lParent->GetLogicCoord();
      if (gfGetDist2D(lCoord, GetCurrentAction()->GetData().mTargetObject->GetClosestPointTo(lCoord)) <= lParent->GetRange())
      {
         SetCurrentActionIndex(OA_ATTACK);
         GetActionStepCounters(OA_ATTACK) = 0;

         int lTmpSense = gfGetDirFromPoints(lCoord, GetCurrentAction()->GetData().mTargetObject->GetLogicCoord());
         if (GetSenseCounter()() == lTmpSense)
         {
            Fire(); // creates the damage object
         }
         else
         {
            SetCurrentActionIndex(OA_STOP);
            TurnToSense(lTmpSense); // turn to orientation
         }
      }
      /*
      else
      {
        GetCurrentAction()->End();
      }
      */
      else
      {
         // provide a default implementation for when the target is not in weapon range
         // it's also possible to provide a tactic attack in the script and implement a custom behaviour
         if (lParent->GetWayLength() > 0)
         {
            OnActionMove();
         }
         else
         {
            SetCurrentActionIndex(OA_STOP);

            CPPoint lTarget = GetCurrentAction()->GetData().mTargetObject->GetClosestPointTo(lParent->GetLogicCoord());

            lTarget.x = lTarget.x * MAPCELLX;
            lTarget.y = lTarget.y * MAPCELLY;

            GetCurrentAction()->GetData().mTargetPoint = lTarget;

            lParent->ComputeWay();

            OnActionMove();
         }
      }
  }
  else
  {//is not in fixed place
    SetCurrentActionIndex(OA_ATTACKMOVE);
    Move();
  }

  VLOG(9) << "CIGSubSysPropulsion::OnActionAttack - over";
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: attack action handler
---------------------------------------------------------------------------*/
void CIGSubSysPropulsion::OnActionAttackGround ()
{
   CIGInfantryUnit* lParent = (CIGInfantryUnit*)GetParent();

   VLOG(9) << "CIGSubSysPropulsion::OnActionAttackGround ";

   if (lParent->IsInFixedPlace() == true)
   {
      if (gfGetDist2D(lParent->GetLogicCoord(), GetCurrentAction()->GetData().mTargetPoint) <= lParent->GetRange())
      {
         SetCurrentActionIndex(OA_ATTACKGROUND);
         GetActionStepCounters(OA_ATTACKGROUND) = 0;

         int lTmpSense = gfGetDirFromPoints(lParent->GetLogicCoord(), GetCurrentAction()->GetData().mTargetPoint);
         if (GetSenseCounter()() == lTmpSense)
         {//creates the damage object
            Fire();
         }
         else
         {//turn to sense
            SetCurrentActionIndex(OA_STOP);
            TurnToSense(lTmpSense);
         }
      }
      else
      {//the dist is greater than range
        GetCurrentAction()->End();
      /*
         if (GetWayLength() > 0)
            OnActionMove();
         else
         {
            SetCurrentActionIndex(OA_STOP);

            //GetCurrentAction()->GetData().mTargetPoint =
            //         GetCurrentAction()->GetData().mTargetObject->GetClosestPointTo(GetLogicCoord());
            ComputeWay();
            OnActionMove();
         }
      */
      }
   }
   else
   {//is not in fixed place
      SetCurrentActionIndex(OA_ATTACKMOVE);
      Move();
   }

   VLOG(9) << "CIGSubSysPropulsion::OnActionAttackGround - over";
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: hold action handler
---------------------------------------------------------------------------*/
void CIGSubSysPropulsion::OnActionHold ()
{
/*   logWriteLn("CIGSubSysPropulsion::OnActionHold ", LOGDET_LOWEST);
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
   logWriteLn("CIGSubSysPropulsion::OnActionHold - over", LOGDET_LOWEST);
*/
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: load action handler
---------------------------------------------------------------------------*/
void CIGSubSysPropulsion::OnActionLoad ()
{
   CIGInfantryUnit* lParent = (CIGInfantryUnit*)GetParent();

   VLOG(9) << "CIGSubSysPropulsion::OnActionLoad ";

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
     // check if the unit is close to the transport vehicle and the transport vehicle is stopped
     if (gfGetDist2D(lCoord, GetCurrentAction()->GetData().mTargetObject->GetClosestPointTo(lCoord)) <= 2 &&
         !(GetCurrentAction()->GetData().mTargetObject->GetMoving()))
     {
       if (GetCurrentAction()->GetData().mTargetObject->ClassName() == CPString(IGCN_TRANUNIT))
       {
         CIGTranUnit* lTranObj = (CIGTranUnit*)(GetCurrentAction()->GetData().mTargetObject);

         // it's not the safest method to check that a transport is dead before
         // loading - i assume that a dead object stays long enough that
         // any actions that have it as target finish
         // another method would be to observe the target object...
         if (!lTranObj->GetIsDead())
         {
           lTranObj->Load(lParent);
         }
       }

       if (GetCurrentAction()->GetData().mTargetObject->ClassName() == CPString(IGCN_DROPSHIPUNIT))
       {
         CIGDropship* lDropshipObj = (CIGDropship*)(GetCurrentAction()->GetData().mTargetObject);

         // TODO: a method to check that a transport have been removed
         // it's not the safest method to check that a transport is dead before
         // loading - i assume that a dead object stays long enough that
         // any actions that have it as target finish
         // another method would be to observe the target object...
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

       // and remove the unit from selections lists
       switch (GetMissionInstance()->GetBGManager()->GetRelation(OS_PLAYER, lParent->GetBattleGroupId()))
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

   VLOG(9) << "CIGSubSysPropulsion::OnActionLoad - over";
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: guard action handler
 TODO implement this action
---------------------------------------------------------------------------*/
void CIGSubSysPropulsion::OnActionGuard ()
{
    GetMainScreenInstance()->AddMessage("Guard action is not implemented!\n");

    TIGSubSysActionData actionData = GetCurrentAction()->GetData();

    GetCurrentAction()->End();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: capture action handler
---------------------------------------------------------------------------*/
void CIGSubSysPropulsion::OnActionCapture ()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description:
---------------------------------------------------------------------------*/
void CIGSubSysPropulsion::OnActionDead()
{
  CIGInfantryUnit* lParent = (CIGInfantryUnit*)GetParent();

  VLOG(9) << "CIGSubSysPropulsion::OnActionDead";
  if (GetActionStepCounters(OA_DEAD).MaxReached() == true)
  {
    GetParent()->RemoveFromLayerMap();
    GetParent()->RemoveFromLogicMap();

    GetMissionInstance()->AddToDeleteObjects(lParent);
    GetMissionInstance()->GetBGManager()->RemoveUnit(lParent);
  }
  VLOG(9) << "CIGSubSysPropulsion::OnActionDead - over";
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: creates the damage object and the sound
---------------------------------------------------------------------------*/
void CIGSubSysPropulsion::Fire()
{
   CIGInfantryUnit* lParent = (CIGInfantryUnit*)GetParent();
//   CPGIGraphicSystem *lGSI;   //local Graphic System Instance
//   lGSI = GetGraphicInstance();

  // weapon reloads, cannot fire

  //logWrite("Fire method: ");
  //logWriteI(GetCurrentAction()->GetData().mTargetPoint.x);
  //logWrite(",");
  //logWriteI(GetCurrentAction()->GetData().mTargetPoint.y);
  //logWrite(", weapon range: "); logWriteILn(mRange);

  if (GetCurrentActionIndex() == OA_ATTACK)
  {
    if (!GetUpdateCounters(OA_ATTACK).MaxReached())
    {
      return;
    }
  }

  if (GetCurrentActionIndex() == OA_ATTACKGROUND)
  {
    if (!GetUpdateCounters(OA_ATTACKGROUND).MaxReached())
    {
      return;
    }
  }

  CIGDamageObject *lObj = NULL;
  switch(lParent->GetWepType())
  {
    case IWT_RIFLE:
    case IWT_MACHINE_GUN:
    case IWT_DOUBLE_GUN:
       lObj = new CIGBulletObject();
       break;

    case IWT_LASER_GUN:
       lObj = new CIGLaserObject();
       break;

    case IWT_PLASMA_THROWER:
       lObj = new CIGPlasmaObject();
       break;

    case IWT_FLAME_THROWER:
       lObj = new CIGPlasmaObject();
       break;

    case IWT_GRENADE_LAUNCHER:
       break;

    case IWT_ROCKET_LAUNCHER:
       lObj = new CIGRocketObject();
  }
//   lObj->SetSprite(lGSI->AddSprite(CPString((getGamePath() + PATH_EXPLODES_GFX + "missiles_sprites_16pos.png").c_str()), 8, 2));

  if (!lObj)
  {
    return;
  }

  int lSpriteWidth      = 0, lSpriteHeight = 0;
  CPSprite* lProjSprite = lParent->GetProjectileSprite();

  if (lProjSprite)
  {
    lSpriteWidth  = lProjSprite->GetAnimWidth();
    lSpriteHeight = lProjSprite->GetAnimHeight();
  }

  lObj->SetSprite(lProjSprite);

  //
  // It's not the correct formula, but provides a fairly decent visual approximation
  //
  lObj->SetCoord(CPPoint(lParent->GetCoord().x + lParent->GetInfUnifiedData().mFireHoles[GetSenseCounter()()].x - lSpriteWidth/2,
                         lParent->GetCoord().y + lParent->GetInfUnifiedData().mFireHoles[GetSenseCounter()()].y - lSpriteHeight/2));

  lObj->SetCoordZ(lParent->GetCoordZ());
  lObj->SetSenseCounter(GetSenseCounter());
  lObj->NoteInLayerMap();
  lObj->SetEffectSprite(lParent->GetEffectSprite());
  //   lObj->SetSplashRadius(2);
  //   lObj->SetEffectSprite(lGSI->AddSprite(CPString((getGamePath() + PATH_EXPLODES_GFX + "Explode01.png").c_str()), 8, 2));
  lObj->SetSourceObject(lParent);
  lObj->SetSoundOnHit(lParent->GetSoundOnHit());
  lObj->SetPower(lParent->GetDamage());
  lObj->SetLife(lParent->GetInfUnifiedData().mTimeToLive);
  lObj->SetTrackPersistency(lParent->GetInfUnifiedData().mTrackPersistency);

  TIGSubSysActionData lTmpActionData;

  switch(GetCurrentActionIndex())
  {
    case OA_ATTACK:
    lObj->SetTargetObject(GetCurrentAction()->GetData().mTargetObject);
    lObj->SetTargetPoint(GetCurrentAction()->GetData().mTargetObject->GetClosestPointTo(lParent->GetLogicCoord()));
    lObj->SetTargetCoordZ(GetCurrentAction()->GetData().mTargetObject->GetCoordZ());

    // reset counter so that we can fire again sometime
    GetUpdateCounters(OA_ATTACK) = 0;

    // set the attack action for the damage object...
    // ...this will further go to the propulsion of the damage object
    lTmpActionData.mType = OA_ATTACK;
    lObj->SetAction(lTmpActionData);
    break;

    case OA_ATTACKGROUND:
    lObj->SetTargetObject(NULL);
    lObj->SetTargetPoint(GetCurrentAction()->GetData().mTargetPoint);
    lObj->SetTargetCoordZ(GetMissionInstance()->GetMap()->GetAltitude(GetCurrentAction()->GetData().mTargetPoint.x, GetCurrentAction()->GetData().mTargetPoint.y));

    // reset counter so that we can fire again sometime
    GetUpdateCounters(OA_ATTACKGROUND) = 0;

    // set the attack action for the damage object...
    // ...this will further go to the propulsion of the damage object
    lTmpActionData.mType = OA_ATTACKGROUND;
    lObj->SetAction(lTmpActionData);
    break;
  }

  lObj->Begin();
  GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Add(lObj); //!!!!!!!!!!!!!!!

  try
  {
    CIGSpaceSoundSystem* lSSSI = CIGSpaceSoundSystem::Instance();
    lSSSI->Play(lParent->GetSoundOnFire(), lParent->GetLogicCoord().x, lParent->GetLogicCoord().y);
  }
  catch(...)
  {
  }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets the instance of the wanted action
---------------------------------------------------------------------------*/
CIGSubSysAction* CIGSubSysPropulsion::GetActionInstance(TIGSubSysActionData actionData)
{
   CIGSubSysAction *lTmpAction = NULL;
   CIGInfantryUnit* lParent = (CIGInfantryUnit*)GetParent();
   bool lIsDead = lParent->GetIsDead();

   switch(actionData.mType)
   {
      case OA_STOP:
         if (lIsDead == false)
            lTmpAction = new CIGSubSysActionStop(this, actionData);
         break;
      case OA_MOVE:
         if (lIsDead == false)
            lTmpAction = new CIGSubSysActionMove(this, actionData);
         break;
      case OA_ATTACK:
         if (lIsDead == false)
            lTmpAction = new CIGSubSysActionAttack(this, actionData);
         break;
      case OA_PATROL:
         if (lIsDead == false)
           lTmpAction = new CIGSubSysActionPatrol(this, actionData);
         break;
      case OA_GUARD:
         if (lIsDead == false)
            lTmpAction = new CIGSubSysActionGuard(this, actionData);
         break;
      case OA_HOLD:
//         if (GetIsDead() == false)
//            lTmpAction = new CIGObjectActionHold(this, actionData);
         break;
      case OA_LOAD:
         if (lIsDead == false)
            lTmpAction = new CIGSubSysActionLoad(this, actionData);
         break;
      case OA_CAPTURE:
//         if (GetIsDead() == false)
//            lTmpAction = new CIGObjectActionCapture(this, actionData);
         break;
      case OA_DIE:
         lTmpAction = new CIGSubSysActionDie(this, actionData);
         break;
      case OA_DEAD:
         lTmpAction = new CIGSubSysActionDead(this, actionData);
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

void CIGSubSysPropulsion::Move()
{
   CIGInfantryUnit* lParent = (CIGInfantryUnit*)GetParent();

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
void CIGSubSysPropulsion::Stop()
{
   CIGInfantryUnit* lParent = (CIGInfantryUnit*)GetParent();

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
void CIGSubSysPropulsion::TurnToSense(int wantedSense)
{
  SetSenseCounter(GetSenseCounter().ToValue(wantedSense)());
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: SenseCounter
---------------------------------------------------------------------------*/
void CIGSubSysPropulsion::SetSenseCounter(int value)
{
  CIGInfantryUnit* lParent = (CIGInfantryUnit*)GetParent();

  CIGSubSysBase::SetSenseCounter(value);
  SetAnimCounter(value);

  int lTmpDir = gDirAngle[GetSenseCounter()] * 180.0 / PI;

  if (lParent->GetLookAt() == UL_NOTHING && !(lTmpDir > lParent->GetScanAngle() - 4 && lTmpDir < lParent->GetScanAngle() + 4))
  {
    lParent->SetScanRange(6);
    lParent->SetScanAngle(lTmpDir);
  }
}
//---------------------------------------------------------------------------

void CIGSubSysPropulsion::SetSenseCounter(CPCounter &value)
{
  CIGInfantryUnit* lParent = (CIGInfantryUnit*)GetParent();

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
bool CIGSubSysPropulsion::IsNextPlaceEmpty()
{
  CIGInfantryUnit* lParent = (CIGInfantryUnit*)GetParent();
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
bool CIGSubSysPropulsion::IsNextObjectMoving()
{
  CIGInfantryUnit* lParent = (CIGInfantryUnit*)GetParent();
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

