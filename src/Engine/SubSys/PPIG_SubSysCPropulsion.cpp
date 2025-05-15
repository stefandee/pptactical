//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998 - 2003 Stefan Dicu & Tudor Girba                       //
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
#include <math.h>
#pragma hdrstop

#include "PPIG_SubSysCPropulsion.h"
#include "PPIG_ObjChopper.h"
#include "PPIG_SubSysAction.h"
#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "basepath.h"
#include "Paths.h"
#include "PPIG_SpaceSound.h"
#include "Equipment.h"
#include "PPIG_EngineDef.h"
#include "PPIG_SubSysVisual.h"
#include "basic3d.h"
#include "PPIG_BGManager.h"
//---------------------------------------------------------------------------

CIGSubSysCPropulsion::CIGSubSysCPropulsion(CIGChopperUnit* _parent) : CIGSubSysBase(_parent)
{
    //Action step counter
    GetActionStepCounters(OA_STOP)         .SetMaxValue(0);
    GetActionStepCounters(OA_MOVE)         .SetMaxValue(0);
    GetActionStepCounters(OA_ATTACKMOVE)   .SetMaxValue(0);
    GetActionStepCounters(OA_PATROL)       .SetMaxValue(3);
    GetActionStepCounters(OA_HOLD)         .SetMaxValue(0);
    GetActionStepCounters(OA_DIE)          .SetMaxValue(0);
    GetActionStepCounters(OA_DEAD)         .SetMaxValue(0);
    //------------------

    //Action update settings
    GetUpdateCounters(OA_MOVE)             .SetMaxValue(1);
    GetUpdateCounters(OA_ATTACKMOVE)       .SetMaxValue(1);
    GetUpdateCounters(OA_PATROL)           .SetMaxValue(1);
    GetUpdateCounters(OA_DIE)              .SetMaxValue(1);
    GetUpdateCounters(OA_DEAD)             .SetMaxValue(200);

    //------------------
    //Action anim types
    SetActionAnimTypes(OA_STOP, 0);
    SetActionAnimTypes(OA_MOVE, 0);
    SetActionAnimTypes(OA_ATTACKMOVE, 0);
    SetActionAnimTypes(OA_PATROL, 0);
    SetActionAnimTypes(OA_HOLD, 0);
    SetActionAnimTypes(OA_GUARD, 0);
    SetActionAnimTypes(OA_DIE, 0);
    SetActionAnimTypes(OA_DEAD, 0);
    //------------------

    // default values (setting sprite for parent may override these values)
    // 8 columns in sprite/8 directions
    GetSenseCounter().SetMaxValue(7);
    GetAnimCounter().SetMaxValue(7);

    mTimer.SetMaxValue(2);

    mCurrentAngle = 0;
    mRealCoord = CVector3D(0, 0, 0);
}
//---------------------------------------------------------------------------

CIGSubSysCPropulsion::~CIGSubSysCPropulsion()
{
}
//---------------------------------------------------------------------------

CIGSubSysAction* CIGSubSysCPropulsion::GetActionInstance(TIGSubSysActionData actionData)
{
    CIGSubSysAction *lTmpAction = NULL;
    CIGAircraft* lParent = (CIGAircraft*)GetParent();
    bool lIsDead = lParent->GetIsDead();

    switch(actionData.mType)
    {
    case OA_STOP:
        if (lIsDead == false)
        {
            lTmpAction = new CIGSubSysActionStop(this, actionData);
        }
        break;

    case OA_LAND:
        if (lIsDead == false)
        {
            lTmpAction = new CIGSubSysActionLand(this, actionData);
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
        break;

    case OA_HOLD:
        break;

    case OA_DIE:
        if (!lIsDead)
        {
            return new CIGSubSysActionDie(this, actionData);
        }
        break;

    case OA_DEAD:
        return new CIGSubSysActionDead(this, actionData);
    }

    return lTmpAction;
}
//---------------------------------------------------------------------------

void CIGSubSysCPropulsion::Update()
{
    CIGSubSysBase::Update();
}
//---------------------------------------------------------------------------

void CIGSubSysCPropulsion::StartActionMove()
{
    mRealCoord = CVector3D(GetParent()->GetCoord().x, GetParent()->GetCoord().y, GetParent()->GetCoordZ());

    int sense = GetSenseCounter()();
    mCurrentAngle = gDirAngle[sense];
}
//---------------------------------------------------------------------------

void CIGSubSysCPropulsion::OnActionStop ()
{
    // an air unit may stop everywhere (even if not in fixed place)
    SetCurrentActionIndex(OA_STOP);
    GetCurrentAction()->End();
}
//---------------------------------------------------------------------------

void CIGSubSysCPropulsion::OnActionMove ()
{
    CIGChopperUnit* lParent = (CIGChopperUnit*)GetParent();

    // before move, make sure the chopper is at the right altitude
    if (lParent->GetCoordZ() < AIRCRAFT_Z_LEVEL)
    {
        if (lParent->GetCoordZ() <= 0)
        {
            lParent->RemoveFromLayerMap();
            lParent->RemoveFromLogicMap();
            lParent->SetLayer(MOL_AIROBJECTS);
            lParent->NoteInLayerMap();
            lParent->SetOscilatorUpdateEnabled(true);
        }

        lParent->SetCoordZ(lParent->GetCoordZ() + 1);
        return;
    }

    bool result = SteeringArrive();

    if (result)
    {
        GetCurrentAction()->End();
    }
}
//---------------------------------------------------------------------------

void CIGSubSysCPropulsion::OnActionPatrol ()
{
    OnActionMove();
}
//---------------------------------------------------------------------------

void CIGSubSysCPropulsion::OnActionHold()
{
}
//---------------------------------------------------------------------------

void CIGSubSysCPropulsion::OnActionGuard()
{
}
//---------------------------------------------------------------------------

void CIGSubSysCPropulsion::OnActionDie()
{
    GetParent()->RemoveFromLayerMap();
}
//---------------------------------------------------------------------------

void CIGSubSysCPropulsion::OnActionDead()
{
    CIGAircraft* lParent = (CIGAircraft*)GetParent();

    VLOG(9) << "CIGSubSysCPropulsion::OnActionDead";
    if (GetActionStepCounters(OA_DEAD).MaxReached() == true)
    {
        //GetParent()->RemoveFromLogicMap();

        GetMissionInstance()->AddToDeleteObjects(lParent);
        GetMissionInstance()->GetBGManager()->RemoveUnit(lParent);
    }
    VLOG(9) << "CIGSubSysCPropulsion::OnActionDead - over";
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: land the chopper to a point
 parameters :
 others     : first move the chopper to destination, then land
---------------------------------------------------------------------------*/
void CIGSubSysCPropulsion::OnActionLand()
{
    CIGChopperUnit* lParent = (CIGChopperUnit*)GetParent();

    double distanceToTarget = DistanceToTarget();

    if (distanceToTarget <= 1.0)
    {
        if (lParent->CanLand(lParent->GetLogicCoord()))
        {
            int lZ = lParent->GetCoordZ();

            if (lZ > 0)
            {
                lParent->SetCoordZ(lZ - 1);
            }
            else
            {
                // the unit landed; notify in ground layer map
                lParent->RemoveFromLayerMap();
                //lParent->RemoveFromLogicMap();
                lParent->SetLayer(MOL_GROUNDUNIT);
                lParent->NoteInLogicMap();
                lParent->NoteInLayerMap();
                lParent->SetOscilatorUpdateEnabled(false);
                GetCurrentAction()->End();
            }
        }
        else
        {
            // seems that there is a unit that is under the chopper: abandon landing
            lParent->NoteInLayerMap();
            GetCurrentAction()->End();
        }
    }
    else
    {
        // before move, make sure the chopper is at the right altitude
        int lZ = lParent->GetCoordZ();

        if (lZ < AIRCRAFT_Z_LEVEL)
        {
            if (lZ <= 0)
            {
                // remove from layer map
                lParent->RemoveFromLayerMap();
                lParent->RemoveFromLogicMap();
                lParent->SetLayer(MOL_AIROBJECTS);
                //lParent->NoteInLogicMap();
                lParent->NoteInLayerMap();
            }

            lParent->SetCoordZ(lZ + 1);
            return;
        }

        SteeringArrive();
    }

    // TODO: refactor and compact code with OnActionMove
    /*
    CIGChopperUnit* lParent = (CIGChopperUnit*)GetParent();

    CPPoint srcPoint = lParent->GetCoord();
    CPPoint destPoint = GetCurrentAction()->GetData().mTargetPoint;

    TPos3D pt1, pt2;
    pt1.x = srcPoint.x;
    pt1.y = srcPoint.y;
    pt1.z = 0;
    pt2.x = destPoint.x * MAPCELLX;
    pt2.y = destPoint.y * MAPCELLY;
    pt2.z = 0;

    // the chopper reached destination point, initiate landing "procedures"
    // maybe it could be implemented using states?
    if (Dist3D(pt1, pt2) < lParent->GetSpeed())
    {
        if (lParent->CanLand(lParent->GetLogicCoord()))
        {
            int lZ = lParent->GetCoordZ();

            if (lZ > 0)
            {
                lParent->SetCoordZ(lZ - 1);
            }
            else
            {
                // the unit landed; notify in ground layer map
                lParent->RemoveFromLayerMap();
                //lParent->RemoveFromLogicMap();
                lParent->SetLayer(MOL_GROUNDUNIT);
                lParent->NoteInLogicMap();
                lParent->NoteInLayerMap();
                lParent->SetOscilatorUpdateEnabled(false);
                GetCurrentAction()->End();
            }
        }
        else
        {
            // seems that there is a unit that is under the chopper: abandon landing
            lParent->NoteInLayerMap();
            GetCurrentAction()->End();
        }

        return;
    }

    // before move, make sure the chopper is at the right altitude
    int lZ = lParent->GetCoordZ();

    if (lZ < AIRCRAFT_Z_LEVEL)
    {
        if (lZ <= 0)
        {
            // remove from layer map
            lParent->RemoveFromLayerMap();
            lParent->RemoveFromLogicMap();
            lParent->SetLayer(MOL_AIROBJECTS);
            //lParent->NoteInLogicMap();
            lParent->NoteInLayerMap();
        }

        lParent->SetCoordZ(lZ + 1);
        return;
    }

    // we are on the right altitude, we can fly
    // same algorithm as movement
    float lAngle = CenterRadianAngle(
                       -srcPoint.x + destPoint.x * MAPCELLX,
                       -srcPoint.y + destPoint.y * MAPCELLY);

    TurnToAngle(lAngle);

    int lDir = gfGetDirFromAngle(mCurrentAngle);

    CIGSubSysBase::SetSenseCounter(lDir);
    ResetVisual();

    mTimer.Inc();

    if (mTimer.MaxReached())
    {
        mTimer.Reset();
        SetAnimCounter(lDir);
    }

    Move();
    */
}
//---------------------------------------------------------------------------

void CIGSubSysCPropulsion::OnActionTakeoff()
{
}
//---------------------------------------------------------------------------

void CIGSubSysCPropulsion::Stop()
{
    CIGAircraft* lParent = (CIGAircraft*)GetParent();

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
void CIGSubSysCPropulsion::TurnToSense(int wantedSense)
{
    SetSenseCounter(GetSenseCounter().ToValue(wantedSense)());
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: SenseCounter
---------------------------------------------------------------------------*/
void CIGSubSysCPropulsion::SetSenseCounter(int value)
{
    CIGSubSysBase::SetSenseCounter(value);
    SetAnimCounter(value);
    ResetVisual();
}
//---------------------------------------------------------------------------

void CIGSubSysCPropulsion::SetSenseCounter(CPCounter &value)
{
    CIGSubSysBase::SetSenseCounter(value);
    SetAnimCounter(value());
    ResetVisual();
}
//---------------------------------------------------------------------------

void CIGSubSysCPropulsion::TurnToAngle(float Angle)
{
    if (Angle == mCurrentAngle)
        return;

    if ((Angle - mCurrentAngle < PI && Angle - mCurrentAngle > 0) ||
            (Angle - mCurrentAngle < -PI))
        mCurrentAngle = mCurrentAngle + PI/32;
    else
        mCurrentAngle = mCurrentAngle - PI/32;

    if (mCurrentAngle >= 2*PI)
        mCurrentAngle = mCurrentAngle - 2*PI;

    if (mCurrentAngle < 0)
        mCurrentAngle = mCurrentAngle + 2*PI;

    if (fabs(mCurrentAngle - Angle) < PI/32)
        mCurrentAngle = Angle;
}
//---------------------------------------------------------------------------

void CIGSubSysCPropulsion::ResetVisual()
{
    CIGChopperUnit* lParent = (CIGChopperUnit*)GetParent();
    int lTmpDir = gDirAngle[GetSenseCounter()] * 180.0 / PI;

    if (lParent->GetLookAt() == UL_NOTHING && !(lTmpDir > lParent->GetScanAngle() - 4 && lTmpDir < lParent->GetScanAngle() + 4))
    {
        lParent->SetScanRange(6);
        lParent->SetScanAngle(lTmpDir);
    }
}
//---------------------------------------------------------------------------

void CIGSubSysCPropulsion::Move(CVector3D& dir)
{
    CIGChopperUnit* lParent = (CIGChopperUnit*)GetParent();

    lParent->RemoveFromLayerMap();

    int speed = lParent->GetSpeed()();
    lParent->SetCoord(CPPoint(lParent->GetCoord().x + speed * dir.GetX(),
                              lParent->GetCoord().y + speed * dir.GetY()));

    // TODO prevent from going off map (not a big problem with small units - couple of cells width/height)

    lParent->NoteInLayerMap();
}
//---------------------------------------------------------------------------

/** \brief Implements "steering arrive" behaviour (e.g. https://www.youtube.com/watch?v=2CL1maXeQCI)
 *
 * May be used in some types of games where the design doesn't require predictive trajectory of the air unit (e.g.
 * the unit may wander over enemy or not yet explored territory). If such behaviour is not wanted, switch to other algorithms.
 *
 * \param none
 * \return true if the unit has reached destination (as defined by the current action data), false otherwise
 *
 */
bool CIGSubSysCPropulsion::SteeringArrive ()
{
    /*
    // steering arrive uses the current position of the unit
    CPPoint srcPoint = lParent->GetCoord();
    CPPoint destPoint = GetCurrentAction()->GetData().mTargetPoint;

    CVector3D wantedDir = CVector3D(srcPoint, destPoint);
    wantedDir.Normalize();

    double distanceToTarget = srcPoint.Distance(destPoint);

    if (distanceToTarget < 100)
    {
        // TODO adjust speed depending on the distance to target, current speed and max speed
    }
    else
    {
        wantedDir.ScalarMultiply(lParent->GetMaxSpeed());
    }

    CVector3D steer = wantedDir.Sub(mVelocity);

    float currentAngle = CenterRadianAngle(mVelocity.x, mVelocity.y);
    int lDir = gfGetDirFromAngle(currentAngle);

    SetAnimCounter(lDir);
    CIGSubSysBase::SetSenseCounter(lDir);

    Move(steer);
    */

    CIGBaseObject* lParent = GetParent();
    CPPoint srcPoint = lParent->GetCoord();
    CPPoint destPoint = CPPoint(GetCurrentAction()->GetData().mTargetPoint.x * MAPCELLX, GetCurrentAction()->GetData().mTargetPoint.y * MAPCELLY);

    double distToTarget = srcPoint.Distance(destPoint);

    if (distToTarget <= 1.0)
    {
        lParent->SetCoord(CPPoint(destPoint.x, destPoint.y));
        //GetCurrentAction()->End();

        return true;
    }

    float lAngle = CenterRadianAngle(
      -srcPoint.x + destPoint.x,
      -srcPoint.y + destPoint.y);

    //LOG(INFO) << mCurrentAngle << " - " << lAngle;

    int lDir = gfGetDirFromAngle(mCurrentAngle);

    CIGSubSysBase::SetSenseCounter(lDir);
    ResetVisual();

    mTimer.Inc();

    if (mTimer.MaxReached())
    {
      mTimer.Reset();
      SetAnimCounter(lDir);
    }

    if (lAngle != mCurrentAngle)
    {
        TurnToAngle(lAngle);

        return false;
    }

    // decelerate toward target
    if (distToTarget < 100)
    {
        int speed = GetParent()->GetSpeed()();
        int targetSpeed = GetParent()->GetMaxSpeed() * distToTarget / 100.0;

        // don't drop the speed at 0, otherwise the unit may never reach the destination
        // TODO re-factor this after speed and coordinates are changed to float
        speed = std::max(targetSpeed, 1);

        lParent->SetSpeed(speed);
    }
    else
    {
        lParent->SetSpeed( lParent->GetMaxSpeed() );
    }

    CVector3D wantedDir = CVector3D(destPoint.x - srcPoint.x, destPoint.y - srcPoint.y, 0.0);
    wantedDir.Normalize();

    Move(wantedDir);

    return false;
}
//---------------------------------------------------------------------------

double CIGSubSysCPropulsion::DistanceToTarget()
{
    CPPoint srcPoint = GetParent()->GetCoord();
    CPPoint destPoint = CPPoint(GetCurrentAction()->GetData().mTargetPoint.x * MAPCELLX, GetCurrentAction()->GetData().mTargetPoint.y * MAPCELLY);

    double distToTarget = srcPoint.Distance(destPoint);

    return distToTarget;
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
