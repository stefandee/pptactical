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
 application: Pure Power

 description: Class Piron Component Interface - Radar Control
 last modify: 19 05 00
        by grabX
---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
#pragma hdrstop

#include "PPIG_CRadar.h"
#include "PPIG_Mission.h"
#include "PP_Graphic.h"
#include "PPIG_EngineDef.h"
#include "PPIG_InterfaceDef.h"
#include "PPIG_Map.h"
#include "PPIG_BGManager.h"

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CPIRadarControl::CPIRadarControl(CPIWinControl *Parent) : CPIControl(Parent)
{
   mMapWidth   = 0;
   mMapHeight  = 0;
   mWindowLeft = 0;
   mWindowTop  = 0;
   mScale      = 1;

   SetColor(0xFF404040);
   mRightMouseSensitive = true;
   SetLeftMouseCaptureEnabled(true);
   SetRightMouseCaptureEnabled(true);


}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CPIRadarControl::~CPIRadarControl()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the tick event
---------------------------------------------------------------------------*/
void CPIRadarControl::Update()
{
   CPIControl::Update();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the control
---------------------------------------------------------------------------*/
void CPIRadarControl::Paint()
{
   VLOG(9) << "CPIRadarControl::Paint";
   if (GetVisible() == false)
      return;
   SetClipRegion();
   CPGIGraphicSystem *lGSI;   //local Graphic System Instance
   try
   {
      lGSI = GetGraphicInstance();
   }
   catch(...)
   {
      LOG(ERROR) << "CPIPanel::Paint - UNABLE TO QUERY GRAPHICS !!!!!!!!!!";
      throw;
   }
/*
   if (GetSprite() != NULL)
      lGSI->PaintSpriteToSurface(NULL, GetAbsoluteLeft(), GetAbsoluteTop(), GetSprite(), GetAnimCounter(), GetAnimType());
   if (GetSurface() != NULL)
      lGSI->PaintSurfaceToSurface(NULL, CPRect(GetAbsoluteLeft(),
                                               GetAbsoluteTop(),
                                               GetAbsoluteLeft() + GetWidth(),
                                               GetAbsoluteTop()  + GetHeight()),
                                  GetSurface(),CPRect(0, 0, GetWidth(), GetHeight()));
 */

    lGSI->SetColor(GetColor());

   lGSI->Rectangle(CPRect(GetAbsoluteLeft(),
                              GetAbsoluteTop(),
                              GetAbsoluteLeft() + GetWidth()-1,
                              GetAbsoluteTop() + GetHeight()-1)
                  );

   lGSI->Rectangle(CPRect(GetAbsoluteLeft() + GetWindowLeft()/GetScale(),
                        GetAbsoluteTop() + GetWindowTop()/GetScale(),
                        GetAbsoluteLeft() + GetWindowLeft()/GetScale() + GetWindowWidth()/GetScale(),
                        GetAbsoluteTop() + GetWindowTop()/GetScale() + GetWindowHeight()/GetScale())
                   );


   GetMissionInstance()->PaintMinimized(CPPoint(GetAbsoluteLeft(), GetAbsoluteTop()),
                                        CPPoint(GetWindowLeft(), GetWindowTop()),
                                        GetWidth(), GetHeight(), GetScale());

//   CPIControl::Paint();
   VLOG(9) << "CPIRadarControl::Paint - over";
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on mouse move event handler
---------------------------------------------------------------------------*/
void CPIRadarControl::OnMouseMove (int X, int Y)
{
   if (GetWasRightMouseDown() == true)
   {
      //SetWindowLeft((X - GetAbsoluteLeft()) * GetScale());
      //SetWindowTop((Y - GetAbsoluteTop()) * GetScale());
      // TMN fixed
      SetWindowCenter((X - GetAbsoluteLeft()) * GetScale(),
                      (Y - GetAbsoluteTop()) * GetScale());
      OnChange();
   }
   CPIControl::OnMouseMove(X, Y);
   GetMissionInstance()->SetCurrentPoint(CPPoint((X - GetAbsoluteLeft())*GetScale(),
                                                 (Y - GetAbsoluteTop())*GetScale()));
   ComputeMouseType(X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on mouse move event handler
---------------------------------------------------------------------------*/
void CPIRadarControl::OnMouseOn (int X, int Y)
{
   CPIControl::OnMouseOn(X, Y);
   GetMissionInstance()->SetCurrentPoint(CPPoint((X - GetAbsoluteLeft())*GetScale(),
                                                 (Y - GetAbsoluteTop())*GetScale()));
   ComputeMouseType(X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on mouse down event handler
---------------------------------------------------------------------------*/
void CPIRadarControl::OnLeftMouseDown (int X, int Y)
{
   CPIControl::OnLeftMouseDown(X, Y);
   GetMissionInstance()->SetBeginPoint(CPPoint((X - GetAbsoluteLeft())*GetScale(),
                                               (Y - GetAbsoluteTop())*GetScale()));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on mouse up event handler
---------------------------------------------------------------------------*/
void CPIRadarControl::OnLeftMouseUp (int X, int Y)
{
   CPIControl::OnLeftMouseUp(X, Y);
   GetMissionInstance()->SetEndPoint(CPPoint((X - GetAbsoluteLeft())*GetScale(),
                                             (Y - GetAbsoluteTop())*GetScale()));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on mouse down event handler
---------------------------------------------------------------------------*/
void CPIRadarControl::OnRightMouseDown (int X, int Y)
{
   //SetWindowLeft((X - GetAbsoluteLeft()) * GetScale());
   //SetWindowTop((Y - GetAbsoluteTop()) * GetScale());

   // TMN fix
   SetWindowCenter((X - GetAbsoluteLeft()) * GetScale(),
                   (Y - GetAbsoluteTop()) * GetScale());

   CPIControl::OnRightMouseDown(X, Y);
   OnChange();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on mouse up event handler
---------------------------------------------------------------------------*/
void CPIRadarControl::OnRightMouseUp (int X, int Y)
{
   CPIControl::OnRightMouseUp(X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on possition change ?
---------------------------------------------------------------------------*/
void CPIRadarControl::OnChange()
{
   /*if (mHandleOnChange.valid())
      mHandleOnChange.callback(this);*/

    if (mHandleOnChange)
    {
        mHandleOnChange(this);
    }
}

/*
void CPIRadarControl::OnChange()
{
   if (mHandleOnChange != NULL)
      mHandleOnChange(this);
}
*/
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: computes the mouse type of the control
---------------------------------------------------------------------------*/
void CPIRadarControl::ComputeMouseType(int X, int Y)
{
   CIGBaseObjectList* lAirObjects;

   switch(GetMissionInstance()->GetCurrentAction())
   {
      case MA_SELECT:
      case MA_SELECTING:
      /*
      if (GetMissionInstance()->IsGroundObjectOfSide(OS_OWNED,
                                                    ((X - GetAbsoluteLeft())*GetScale())/MAPCELLX,
                                                    ((Y - GetAbsoluteTop())*GetScale())/MAPCELLY) == true)
         SetMouseType(MC_ONOWNED);
      else
      if (GetMissionInstance()->IsGroundObjectOfSide(OS_ENEMY,
                                                    ((X - GetAbsoluteLeft())*GetScale())/MAPCELLX,
                                                    ((Y - GetAbsoluteTop())*GetScale())/MAPCELLY) == true)
         SetMouseType(MC_ONENEMY);
      else
         SetMouseType(MC_DEFAULT);
      */
        SetMouseType(MC_DEFAULT);
        if (GetMissionInstance()->GetGroundObjectsRelation(OS_PLAYER,
                                                      ((X - GetAbsoluteLeft())*GetScale())/MAPCELLX,
                                                      ((Y - GetAbsoluteTop())*GetScale())/MAPCELLY) == REL_OWNED)
        {
           SetMouseType(MC_ONOWNED);
           break;
        }
        else
        {
          if (GetMissionInstance()->GetGroundObjectsRelation(OS_PLAYER,
                                                        ((X - GetAbsoluteLeft())*GetScale())/MAPCELLX,
                                                        ((Y - GetAbsoluteTop())*GetScale())/MAPCELLY) == REL_ENEMY &&
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

        // now make a sweep thru the air objects in that point
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

      default:
         SetMouseType(MC_DEFAULT);
   }
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
