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
 application: Pure Power - in game

 description: Class Piron Component Interface - pure power battle screen
 last modify: 28 05 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#pragma hdrstop

#include <algorithm>
using namespace std;

#include "PPIG_CBattleControl.h"
#include "PPIG_Mission.h"
#include "PlayerProfile.h"
//#include "PPIG_EngineDef.h"
#include "PPIG_InterfaceDef.h"
#include "PPIG_Map.h"
#include "PPIG_BGManager.h"

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: contructor
---------------------------------------------------------------------------*/
CPIBattleControl::CPIBattleControl(CPIWinControl *Parent) : CPIControl(Parent)
{
   mMapWidth  = 0;
   mMapHeight = 0;
   mWindowLeft= 0;
   mWindowTop = 0;
   SetLeftMouseCaptureEnabled(true);
}

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CPIBattleControl::~CPIBattleControl()
{
}

/*---------------------------------------------------------------------------
 description: handles the tick event
---------------------------------------------------------------------------*/
void CPIBattleControl::Update()
{
  if (GetEnabled() == false)
  {
    return;
  }

   CPIControl::Update();
   GetMissionInstance()->Update();

}

/*---------------------------------------------------------------------------
 description: paints the battle scene
 others     : overwrites the control paint method
---------------------------------------------------------------------------*/
void CPIBattleControl::Paint()
{
   if (GetVisible() == false)
      return;

  SetClipRegion();

  // perform the adjustement for the window
  CIGBaseObject* focusObject = GetMissionInstance()->GetFocusObject();

  if (focusObject)
  {
    mWindowLeft = std::max(0, focusObject->GetCoord().x - GetWidth() / 2);
    mWindowLeft = std::min(mWindowLeft, GetMapWidth() * MAPCELLX - GetWidth());
    mWindowTop  = std::max(0, focusObject->GetCoord().y - GetHeight() / 2);
    mWindowTop  = std::min(mWindowTop, GetMapHeight() * MAPCELLY - GetHeight());
  }

  GetMissionInstance()->Paint(CPPoint(GetAbsoluteLeft(), GetAbsoluteTop()), CPPoint(GetWindowLeft(), GetWindowTop()), GetWidth(), GetHeight(), 1);
}

/*---------------------------------------------------------------------------
 description: on left mouse down event
 parameters : X, Y - real coord that is wanted in the center of the screen
---------------------------------------------------------------------------*/
void CPIBattleControl::OnLeftMouseDown(int X, int Y)
{
   CPIControl::OnLeftMouseDown(X, Y);
   GetMissionInstance()->SetBeginPoint(CPPoint(X - GetLeft() + GetWindowLeft(),
                                               Y - GetTop() + GetWindowTop()));
}

/*---------------------------------------------------------------------------
 description: on left mouse up event
 parameters : X, Y - real coord that is wanted in the center of the screen
---------------------------------------------------------------------------*/
void CPIBattleControl::OnLeftMouseUp(int X, int Y)
{
   CPIControl::OnLeftMouseUp(X, Y);
   GetMissionInstance()->SetEndPoint(CPPoint(X - GetLeft() + GetWindowLeft(),
                                             Y - GetTop() + GetWindowTop()));
}

/*---------------------------------------------------------------------------
 description: handles the right mouse click
 others     : it cancels thje current action
---------------------------------------------------------------------------*/
void CPIBattleControl::OnRightMouseClick()
{
   CPIControl::OnRightMouseClick();
   GetMissionInstance()->CancelAction();
}

/*---------------------------------------------------------------------------
 description: on left mouse move event
 parameters : X, Y - real coord that is wanted in the center of the screen
 others     :
---------------------------------------------------------------------------*/
void CPIBattleControl::OnMouseMove(int X, int Y)
{
   CPIControl::OnMouseMove(X, Y);
   if (X < GetLeft())
      X = GetLeft();
   if (X >= GetLeft() + GetWidth())
      X = GetLeft() + GetWidth() - 1;
   if (Y < GetTop())
      Y = GetTop();
   if (Y >= GetTop() + GetHeight())
      Y = GetTop() + GetHeight() - 1;

   mObjectHintPos.x = X - GetLeft();
   mObjectHintPos.y = Y - GetTop() + 20;

   GetMissionInstance()->SetCurrentPoint(CPPoint(X - GetLeft() + GetWindowLeft(),
                                                 Y - GetTop() + GetWindowTop()));

   SetHint(GetMissionInstance()->GetObjectHint());

   SetShowHint(!GetHint().isEmpty() && GetProfileInstance()->GetShowUnitTooltip());

   ComputeMouseType(X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on left mouse move event
 parameters : X, Y - real coord that is wanted in the center of the screen
 others     :
---------------------------------------------------------------------------*/
void CPIBattleControl::OnMouseOn(int X, int Y)
{
   CPIControl::OnMouseOn(X, Y);
   if (X < GetLeft())
      X = GetLeft();
   if (X >= GetLeft() + GetWidth())
      X = GetLeft() + GetWidth() - 1;
   if (Y < GetTop())
      Y = GetTop();
   if (Y >= GetTop() + GetHeight())
      Y = GetTop() + GetHeight() - 1;

   GetMissionInstance()->SetCurrentPoint(CPPoint(X - GetLeft() + GetWindowLeft(),
                                                 Y - GetTop() + GetWindowTop()));
   ComputeMouseType(X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: computes the mouse type of the control
---------------------------------------------------------------------------*/
void CPIBattleControl::ComputeMouseType(int X, int Y)
{
   //CIGBaseObject*     lObject;
   CIGBaseObjectList* lAirObjects;
   //int                lBg;

   switch(GetMissionInstance()->GetCurrentAction())
   {
      case MA_SELECT:
      case MA_SELECTING:
      /*
      if (GetMissionInstance()->IsGroundObjectOfSide(OS_OWNED,
                                                    (X - GetLeft() + GetWindowLeft())/MAPCELLX,
                                                    (Y - GetTop() + GetWindowTop())/MAPCELLY) == true)
         SetMouseType(MC_ONOWNED);
      else
      if (GetMissionInstance()->IsGroundObjectOfSide(OS_ENEMY,
                                                    (X - GetLeft() + GetWindowLeft())/MAPCELLX,
                                                    (Y - GetTop() + GetWindowTop())/MAPCELLY) == true)
         SetMouseType(MC_ONENEMY);
      else
         SetMouseType(MC_DEFAULT);
      */
        SetMouseType(MC_DEFAULT);

        if (GetMissionInstance()->GetMap()->GetGroundObject((X - GetLeft() + GetWindowLeft())/MAPCELLX, (Y - GetTop() + GetWindowTop())/MAPCELLY))
        {
          if (GetMissionInstance()->GetGroundObjectsRelation(OS_PLAYER,
                                                        (X - GetLeft() + GetWindowLeft())/MAPCELLX,
                                                        (Y - GetTop() + GetWindowTop())/MAPCELLY) == REL_OWNED)
          {
            SetMouseType(MC_ONOWNED);
            break;
          }
          else
          {
            if (GetMissionInstance()->GetGroundObjectsRelation(OS_PLAYER,
                                                        (X - GetLeft() + GetWindowLeft())/MAPCELLX,
                                                        (Y - GetTop() + GetWindowTop())/MAPCELLY) == REL_ENEMY &&
                GetMissionInstance()->GetMap()->GetVisibility(OS_PLAYER, (X - GetLeft() + GetWindowLeft())/MAPCELLX, (Y - GetTop() + GetWindowTop())/MAPCELLY) == 2)
            {
              SetMouseType(MC_ONENEMY);
              break;
            }
            /*
            else
            {
              SetMouseType(MC_DEFAULT);
              break;
            }
            */
          }
        }

         lAirObjects  = GetMissionInstance()->GetMap()->GetAirObjects((X - GetLeft() + GetWindowLeft())/MAPCELLX, (Y - GetTop() + GetWindowTop())/MAPCELLY);

         if (lAirObjects)
         {
           for(int i = 0; i < lAirObjects->Count(); i++)
           {
             int lBg = lAirObjects->GetObject(i)->GetBattleGroupId();

             TPoliticsRelations lRelation = GetMissionInstance()->GetBGManager()->GetRelation(OS_PLAYER, lBg);

             if (lRelation == REL_OWNED)
             {
               SetMouseType(MC_ONOWNED);
               break;
             }

             if (GetMissionInstance()->GetMap()->GetVisibility(OS_PLAYER, (X - GetLeft() + GetWindowLeft())/MAPCELLX, (Y - GetTop() + GetWindowTop())/MAPCELLY) == 2)
             {
               SetMouseType(MC_ONENEMY);
               break;
             }

             /*
             else
             {
               SetMouseType(MC_DEFAULT);
               break;
             }
             */
           }
         }

        break;

      case MA_MOVE:
         SetMouseType(MC_MOVE);
         break;

      case MA_ATTACK:
      case MA_SUPPRESS_FIRE:
         SetMouseType(MC_ATTACK);
         break;

      case MA_PATROL:
         SetMouseType(MC_PATROL);
         break;

      case MA_GUARD:
         SetMouseType(MC_GUARD);
         break;

      case MA_HOLD:
         SetMouseType(MC_DEFAULT);
         break;

      case MA_CAPTURE:
         SetMouseType(MC_CAPTURE);
         break;

      case MA_LOAD:
          //lBg = GetMissionInstance()->GetMap()->GetGroundObjectBGroup((X - GetLeft() + GetWindowLeft())/MAPCELLX, (Y - GetTop() + GetWindowTop())/MAPCELLY);
          //lObject = GetMissionInstance()->GetMap()->GetGroundObject((X - GetLeft() + GetWindowLeft())/MAPCELLX, (Y - GetTop() + GetWindowTop())/MAPCELLY);

          //if (!lObject)
          //{
          //  break;
          //}

          //if (lBg == OS_PLAYER && lObject->ClassName() == CPString(IGCN_TRANUNIT))
          //{
            SetMouseType(MC_LOAD);
            //SetCurrentAction(MA_LOAD);
          //}
         break;

      case MA_UNLOAD_ALL:
         SetMouseType(MC_UNLOAD);
         break;

      case MA_LAND:
          // TODO check if it can land at the current spot; if not, use a "denied" cursor
         SetMouseType(MC_LAND);
         break;

      default:
         SetMouseType(MC_DEFAULT);
   }
}
//---------------------------------------------------------------------------

void CPIBattleControl::SetWindowLeft (int value)
{
  if (!GetMissionInstance()->GetFocusObject())
  {
    mWindowLeft = value;
  }
}
//---------------------------------------------------------------------------

void CPIBattleControl::SetWindowTop (int value)
{
  if (!GetMissionInstance()->GetFocusObject())
  {
    mWindowTop = value;
  }
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

