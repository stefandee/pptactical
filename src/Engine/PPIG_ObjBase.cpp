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

 description: Class Base Object
              here are the base properties and methods for all the objects
              envolved in the game
 last modify: 09 07 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#include <cstring>
#pragma hdrstop

#include "PPIG_ObjBase.h"
#include "PPIG_Mission.h"
#include "PP_Graphic.h"

/*---------------------------------------------------------------------------
description: constructor - protected
---------------------------------------------------------------------------*/
CIGBaseObject::CIGBaseObject()
{
   VLOG(5) << "CIGBaseObject::CIGBaseObject";
   mSprite        = NULL;
   mVisible       = true;
   mUpdateEnabled = true;
   mUpdateCounter = 0;
   mActionMask    = 0;
   mDimX          = 0;
   mDimY          = 0;
   mLogicDimX     = 0;
   mLogicDimY     = 0;
   mCoord         = CPPoint(0, 0);
   mLogicCoord    = CPPoint(-1, -1);
   mCoordZ        = 0;
   mLayer         = 0;
   mUpdateCounter.SetMaxValue(1);

   mWayLength     = 0;
   mWay[0]        = WAY_OVER;
   GetSpeed()     = 0;
   GetSpeed().SetMaxValue(1);
   mWaitingFor   = NULL;
   mMoving       = false;

   mName         = "Unknown object";
   mIsDead       = false;

   mMaxSpeed     = 1.;

   VLOG(5) << "CIGBaseObject::CIGBaseObject - over";
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor - protected
---------------------------------------------------------------------------*/
CIGBaseObject::~CIGBaseObject()
{
  //logWrite("Actions in list to delete "); logWriteILn(mActions->Count());
  /*
  mActions->FreeAll();
  try
  {
    delete mActions;
  }
  catch(...)
  {
    logWriteLn("CIGBaseObject::~CIGBaseObject - failed to dealocate ActionList");
  }
  */

  //
  // an error that lurked in this code for 6 years ;) - the sprite should have
  // been released, otherwise it simply took space in the graphic system (at least
  // when using the map editor a lot for loading and saving and with different
  // sets of sprites)
  //
  //GetGraphicInstance()->ReleaseSprite(mSprite, SPRL_MISSION);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if this class is derived from className
 parameters : className - the parent class
---------------------------------------------------------------------------*/
bool CIGBaseObject::IsDerivedFrom (CPString className)
{
  return (className == CIGBaseObject::ClassName()) ? true : false;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the tick event
---------------------------------------------------------------------------*/
void CIGBaseObject::Update ()
{
   /*
   if (GetUpdateEnabled() == true)
   {
      UpdateActionCounters();
      logWriteLn("CIGBaseObject::Update ", LOGDET_LOWEST);
      if (GetCurrentAction() != NULL)
      {
         if (GetUpdateCounters(GetCurrentActionIndex()).MaxReached() == true)
         {
            GetActionStepCounters(GetCurrentActionIndex())++;

            SetAnimType(GetActionAnimTypes(GetCurrentAction()->GetType()));
            GetCurrentAction()->Update();
         }
         else
         {
            GetCurrentAction()->UpdateIdle();
         }
      }
      else if (GetActions()->IsEmpty() == false)
      {//there is an enqueued action
         SetCurrentAction(GetActions()->GetAction(0));
         GetActions()->Remove(0);
      }
      logWriteLn("CIGBaseObject::Update - over", LOGDET_LOWEST);
   }
   */
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the object sprite (animtype & animCount)
 parameters : ptViewport - the zone of the screen where the paint is wanted
              rcWindow - the visible portion of the map
              Width, Height - is probably redundant and represents the viewable area
              zoomLevel - is 1 by default
---------------------------------------------------------------------------*/
void CIGBaseObject::Paint (CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel)
{
   if (GetVisible() == true)
   {
      if (GetSprite() != NULL)
      {
         GetGraphicInstance()->PaintSprite(
                                    (GetCoord().x + ptViewPort.x - ptWindow.x)/zoomLevel,
                                    (GetCoord().y + ptViewPort.y - ptWindow.y)/zoomLevel,
                                    GetSprite(), 0, 0);
      }
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the object sprite (animtype & animCount)
 parameters : ptViewport - the zone of the screen where the paint is wanted
              rcWindow - the visible portion of the map
              Width, Height - is probably redundant and represents the viewable area
              zoomLevel - is 1 by default
---------------------------------------------------------------------------*/
void CIGBaseObject::PaintMinimized(CPPoint ptViewPort, CPPoint ptWindow,
                                   int Width, int Height, int zoomLevel)
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the selection of the object
 parameters : ptViewport - the zone of the screen where the paint is wanted
              rcWindow - the visible portion of the map
              zoomLevel - is 1 by default
---------------------------------------------------------------------------*/
void CIGBaseObject::PaintSelection (CPPoint ptViewPort, CPPoint ptWindow,
                                    int Width, int Height, int zoomLevel)
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the team of the object
 parameters : ptViewport - the zone of the screen where the paint is wanted
              rcWindow - the visible portion of the map
              zoomLevel - is 1 by default
---------------------------------------------------------------------------*/
void CIGBaseObject::PaintTeam (CPPoint ptViewPort, CPPoint ptWindow,
                               int Width, int Height, int zoomLevel)
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the health bar of the object
 parameters : ptViewport - the zone of the screen where the paint is wanted
              rcWindow - the visible portion of the map
              zoomLevel - is 1 by default
---------------------------------------------------------------------------*/
void CIGBaseObject::PaintHealthBar (CPPoint ptViewPort, CPPoint ptWindow,
                                    int Width, int Height, int zoomLevel)
{
}
//---------------------------------------------------------------------------

// Description : returns the rectangle where the object will currently render
// Param       :
// Result      :
// Comments    : it is used by unit selection; this is only a default implementation
//               override whenever you modify the paint algorithm (by adding an
//               oscilator, the z coord and so on
CPRect CIGBaseObject::GetPaintRect()
{
  if (mSprite == NULL)
  {
      return CPRect(-1, -1, 0, 0);
  }

  return mSprite->GetPaintRect(GetCoord(), 0, 0);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: moves the object in the current sense with the current speed
---------------------------------------------------------------------------*/
/*
void CIGBaseObject::Move()
{
   RemoveFromLayerMap();
   SetCoord(CPPoint(GetCoord().x + GetSpeed()()*gDir[GetSenseCounter()()].x,
                    GetCoord().y + GetSpeed()()*gDir[GetSenseCounter()()].y ));
   NoteInLayerMap();
}
//---------------------------------------------------------------------------
*/

/*---------------------------------------------------------------------------
 description: kills the object
 others     : to be overwritten
---------------------------------------------------------------------------*/
void CIGBaseObject::Die()
{
   TIGSubSysActionData lData;
   lData.mType = OA_DIE;
   lData.mStartPoint = GetLogicCoord();
   lData.mTargetPoint = GetLogicCoord();
   SetAction(lData);

   // a dead unit cannot be in a team or in any selection (the later probably depends
   // on each specific game design)
   SetTeam(-1);
   GetMissionInstance()->RemoveFromSelection(this);
   SetSelected(false);

   SetIsDead(true);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: virtual method
---------------------------------------------------------------------------*/
void CIGBaseObject::NoteInLayerMap ()
{
   CPPoint lCoord   = GetLogicCoord();
   const int Xbegin = lCoord.x;
   const int Xend   = Xbegin + GetLogicDimX();
   const int Ybegin = lCoord.y;
   const int Yend   = Ybegin + GetLogicDimY();

   if (!CanBePlaced(GetLogicCoord()))
   {
     return;
   }

   for (int x=Xbegin; x<Xend; ++x)
   {
      for (int y=Ybegin; y<Yend; ++y)
      {
        GetMissionInstance()->NoteInLayerMap(GetLayer(), this, x, y);
      }
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: virtual method, removes the object from layer map
---------------------------------------------------------------------------*/
void CIGBaseObject::RemoveFromLayerMap ()
{
   CPPoint lCoord  = GetLogicCoord();
   const int lMaxX = lCoord.x + GetLogicDimX();
   const int lMaxY = lCoord.y + GetLogicDimY();

   for (int i=lCoord.x; i<lMaxX; ++i)
   {
      for (int j=lCoord.y; j<lMaxY; j++)
      {
         GetMissionInstance()->RemoveFromLayerMap(GetLayer(), this, i, j);
      }
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: notes the object in logical map
---------------------------------------------------------------------------*/
void CIGBaseObject::NoteInLogicMap()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: notes this object in the specified point
 parameters : Point - the beginning point (in logic coord)
---------------------------------------------------------------------------*/
void CIGBaseObject::NoteInLogicMap(CPPoint Point)
{
}
//---------------------------------------------------------------------------

bool CIGBaseObject::CanBePlaced(CPPoint _point)
{
  return false;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: removes the object from logic map
---------------------------------------------------------------------------*/
void CIGBaseObject::RemoveFromLogicMap()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns the object logic point closest to the target
 parameters : targetPoint - the wanted point in logic coord
---------------------------------------------------------------------------*/
CPPoint CIGBaseObject::GetClosestPointTo(CPPoint targetPoint)
{
   return GetLogicCoord();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: centers the object in point
 parameters : targetPoint - is the center point
                          - real coord
---------------------------------------------------------------------------*/
void CIGBaseObject::CenterInPoint(CPPoint targetPoint)
{
   SetCoord(CPPoint(targetPoint.x-GetDimX()/2, targetPoint.y-GetDimY()/2));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: notes the way
 parameters : way    - vecor of senses
              length - length of way
---------------------------------------------------------------------------*/
void CIGBaseObject::NoteWay(int *way, int length)
{
   memcpy(mWay, way, length*sizeof(int));
   SetWayLength(length);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: computes the way (probably posts a requests to path server)
 others     : to be overwritten
---------------------------------------------------------------------------*/
void CIGBaseObject::ComputeWay()
{
   SetWay(0, WAY_SEARCHING);
   SetWayLength(1);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the next place in th way list is empty
 others     : should be overwritten
---------------------------------------------------------------------------*/
bool CIGBaseObject::IsNextPlaceEmpty()
{
   return true;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the real coord can be devided by the map cell dim
---------------------------------------------------------------------------*/
bool CIGBaseObject::IsInFixedPlace()
{
   return (GetCoord().x % MAPCELLX == 0 &&
           GetCoord().y % MAPCELLY == 0);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the real coord can be devided by the map cell dim
 others     : should be overwritten
---------------------------------------------------------------------------*/
bool CIGBaseObject::IsNextObjectMoving()
{
   return false;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//
// Base Object serialize/deserialize section
//
//---------------------------------------------------------------------------

void CIGBaseObject::Serialize(PP::Stream& _a)
{
  CSerCounter          lUpdateCounter(mUpdateCounter);
  CSerPoint            lCoord(mCoord);
  CSerPoint            lLogicCoord(mLogicCoord);
  CSerPoint            lNextPoint;
  CSerPoint            lLastPoint;
  CSerCounter          lSpeed(mSpeed);
  CSerString           lName;
  int                  lSpriteIndex = -1;
  int                  lWaitForId = -1;

  // serialization
  if (mSprite)
  {
    lSpriteIndex = GetGraphicInstance()->GetSpriteIndex(mSprite, SPRL_MISSION);
  }
  _a.writeInt(&lSpriteIndex);

  _a.writeBool(&mVisible);
  _a.writeBool(&mUpdateEnabled);
  lUpdateCounter.Serialize(_a);
  _a.writeInt(&mActionMask);
  _a.writeInt(&mDimX);
  _a.writeInt(&mDimY);
  _a.writeInt(&mLogicDimX);
  _a.writeInt(&mLogicDimY);
  lCoord.Serialize(_a);
  lLogicCoord.Serialize(_a);
  _a.writeInt(&mCoordZ);
  _a.writeInt(&mLayer);
  for(int i = 0; i < WAY_MAX_LENGTH; i++)
  {
    _a.writeInt(&mWay[i]);
  }
  _a.writeInt(&mWayLength);
  lNextPoint.Serialize(_a);
  lLastPoint.Serialize(_a);
  lSpeed.Serialize(_a);
  _a.writeFloat(&mMaxSpeed);
  _a.writeBool(&mMoving);
  lName.Serialize(_a);
  _a.writeBool(&mIsDead);

  lWaitForId = -1;
  if (mWaitingFor)
  {
    lWaitForId = mWaitingFor->GetId();
  }
  _a.writeInt(&lWaitForId);

  /*
  mActions->DeSerialize(_a);
  lHasCurrentAction.DeSerialize(_a);

  if (lHasCurrentAction.Get())
  {
    // fill an object action data structure

    mCurrentAction = new CIGObjectAction(this, );
    mCurrentAction->DeSerialize(_a);
  }
  */
}
//---------------------------------------------------------------------------

unsigned __int32 CIGBaseObject::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void CIGBaseObject::DeSerialize(PP::Stream& _a)
{
  CSerCounter          lUpdateCounter;
  CSerPoint            lCoord;
  CSerPoint            lLogicCoord;
  CSerPoint            lNextPoint;
  CSerPoint            lLastPoint;
  CSerCounter          lSpeed;
  CSerString           lName;
  int                  lSpriteIndex;
  int                  lWaitForId;

  _a.readInt(&lSpriteIndex);
  _a.readBool(&mVisible);
  _a.readBool(&mUpdateEnabled);
  lUpdateCounter.DeSerialize(_a);

  // is it necessary to save the action mask?
  // might not be necessary if action mask doesn't change dynamically
  //_a.readInt(&mActionMask);
  _a.skip(sizeof(mActionMask));

  _a.readInt(&mDimX);
  _a.readInt(&mDimY);
  _a.readInt(&mLogicDimX);
  _a.readInt(&mLogicDimY);
  lCoord.DeSerialize(_a);
  lLogicCoord.DeSerialize(_a);
  _a.readInt(&mCoordZ);
  _a.readInt(&mLayer);
  for(int i = 0; i < WAY_MAX_LENGTH; i++)
  {
    _a.readInt(&mWay[i]);
  }
  _a.readInt(&mWayLength);
  lNextPoint.DeSerialize(_a);
  lLastPoint.DeSerialize(_a);
  lSpeed.DeSerialize(_a);
  _a.readFloat(&mMaxSpeed);
  _a.readBool(&mMoving);
  lName.DeSerialize(_a);
  _a.readBool(&mIsDead);
  _a.readInt(&lWaitForId);
  /*
  mActions->DeSerialize(_a);
  lHasCurrentAction.DeSerialize(_a);

  if (lHasCurrentAction.Get())
  {
    // fill an object action data structure

    mCurrentAction = new CIGObjectAction(this, );
    mCurrentAction->DeSerialize(_a);
  }
  */

  // assignments
  mSprite            = GetGraphicInstance()->GetSpriteByIndex(lSpriteIndex, SPRL_MISSION);
  mUpdateCounter     = lUpdateCounter.Get();
  mWaitingFor        = GetMissionInstance()->GetObjectById(lWaitForId);
  mCoord             = lCoord.Get();
  mLogicCoord        = lLogicCoord.Get();
  mNextPoint         = lNextPoint.Get();
  mLastPoint         = lLastPoint.Get();

  mSpeed             = lSpeed.Get();

  mName              = lName.Get();

  // notifications
  //NoteInLayerMap();
  //NoteInLogicMap();
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif



