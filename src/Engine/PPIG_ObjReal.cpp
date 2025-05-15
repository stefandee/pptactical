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

 description: Class In Game Real Object
 last modify: 12 06 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#pragma hdrstop

#include "config.h"
#include "PPIG_ObjReal.h"
#include "PP_Graphic.h"
#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "PP_Timer.h"
#include "PPIG_BGManager.h"

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CIGRealObject::CIGRealObject()
{
   mSelected   = false;
   //mSide       = 0;
   mTeam       = -1;
   mMaxHP      = 1;
   mCurrentHP  = 0;
   mContainedObjects = NULL;

   mBulletRes  = 0;
   mEnergyRes  = 0;
   mFireRes    = 0;
   mPlasmaRes  = 0;
   mExplodeRes = 0;

   mBattleGroupId = -1;
   mId            = -1;
   mMass       = 1;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CIGRealObject::~CIGRealObject()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if this class is derived from className
---------------------------------------------------------------------------*/
bool CIGRealObject::IsDerivedFrom (CPString className)
{
   return ((className == ClassName()) || (CIGBaseObject::IsDerivedFrom(className)));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the selection of the object
 parameters : ptViewport - the zone of the screen where the paint is wanted
              rcWindow - the visible portion of the map
              zoomLevel - is 1 by default
---------------------------------------------------------------------------*/
void CIGRealObject::PaintSelection (CPPoint ptViewPort, CPPoint ptWindow,
                                    int Width, int Height, int zoomLevel)
{
   // do not paint dead objects selection...
   if (GetIsDead())
   {
     return;
   }

   if (GetSelected() == true)
   {
      CPGIGraphicSystem *lGSI;   //graphic system interface
      try
      {
         lGSI = GetGraphicInstance();
      }
      catch(...)
      {
         LOG(FATAL) << "CIGRealObject::Paint - UNABLE TO QUERY GRAPHICS !!!!!!!!!!";
         throw;
      }

      lGSI->SetColor(GetMissionInstance()->GetBGManager()->GetRelationAsColor(OS_PLAYER, GetBattleGroupId()));
      lGSI->SelectionRectangle(CPRect(GetCoord().x + ptViewPort.x - ptWindow.x + GetDimX()/6,
                                   GetCoord().y + ptViewPort.y - ptWindow.y + GetDimY()/6,
                                   GetCoord().x + ptViewPort.x - ptWindow.x + (GetDimX()*5)/6,
                                   GetCoord().y + ptViewPort.y - ptWindow.y + (GetDimY()*5)/6),
                              3
                              );
                              //gSideColor[GetSide()]);
   }

   PaintHealthBar(ptViewPort, ptWindow, Width, Height, zoomLevel);
   PaintTeam(ptViewPort, ptWindow, Width, Height, zoomLevel);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the team of the object
 parameters : ptViewport - the zone of the screen where the paint is wanted
              rcWindow - the visible portion of the map
              zoomLevel - is 1 by default
---------------------------------------------------------------------------*/
void CIGRealObject::PaintTeam (CPPoint ptViewPort, CPPoint ptWindow,
                               int Width, int Height, int zoomLevel)
{
//   if (GetTeam() != -1 && GetSide() == OS_OWNED)
   if (GetTeam() != -1 && GetBattleGroupId() == OS_PLAYER)
   {
      GetGraphicInstance()->TextOut2(GetCoord().x + ptViewPort.x - ptWindow.x + GetDimX()/6 ,
                                    GetCoord().y + ptViewPort.y - ptWindow.y + GetDimY()/6 - 5,
                                    CPString(GetTeam()+1));
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the health bar of the object
 parameters : ptViewport - the zone of the screen where the paint is wanted
              rcWindow - the visible portion of the map
              zoomLevel - is 1 by default
---------------------------------------------------------------------------*/
void CIGRealObject::PaintHealthBar (CPPoint ptViewPort, CPPoint ptWindow,
                                    int Width, int Height, int zoomLevel)
{
   CPGIGraphicSystem *lGSI;   //graphic system interface
   try
   {
      lGSI = GetGraphicInstance();
   }
   catch(...)
   {
      LOG(FATAL) << "CIGRealObject::Paint - UNABLE TO QUERY GRAPHICS !!!!!!!!!!";
      throw;
   }

   lGSI->SetColor(GetMissionInstance()->GetBGManager()->GetRelationAsColor(OS_PLAYER, GetBattleGroupId()));
   lGSI->Rectangle(CPRect(GetCoord().x + ptViewPort.x - ptWindow.x + GetDimX()/6+3,
                        GetCoord().y + ptViewPort.y - ptWindow.y + GetDimY()*5/6-2,
                        GetCoord().x + ptViewPort.x - ptWindow.x + GetDimX()*5/6-3,
                        GetCoord().y + ptViewPort.y - ptWindow.y + GetDimY()*5/6+2)
                   );
                    //gSideColor[GetSide()]);

   lGSI->FillRectangle(CPRect(GetCoord().x + ptViewPort.x - ptWindow.x + GetDimX()/6+4,
                            GetCoord().y + ptViewPort.y - ptWindow.y + GetDimY()*5/6-1,
                            GetCoord().x + ptViewPort.x - ptWindow.x + GetDimX()/6+4+((GetDimX()*4/6 - 8)*GetCurrentHP())/GetMaxHP() + 1,
                            GetCoord().y + ptViewPort.y - ptWindow.y + GetDimY()*5/6+1)
                       );
                     //gSideColor[GetSide()]);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the object sprite (animtype & animCount) (paint in radar?)
 parameters : ptViewport - the zone of the screen where the paint is wanted
              rcWindow - the visible portion of the map
              Width, Height - is probably redundant and represents the viewable area
              zoomLevel - is 1 by default
---------------------------------------------------------------------------*/
void CIGRealObject::PaintMinimized(CPPoint ptViewPort, CPPoint ptWindow,
                                   int Width, int Height, int zoomLevel)
{
   bool lVisible = false;
   int  lColor;

   // do not paint dead objects
   if (GetIsDead())
   {
     return;
   }

   // another little hack, this should be done somewhere (map)
   for(int x = GetLogicCoord().x; x < GetLogicCoord().x + GetDimX() / MAPCELLX; x++)
   {
     for(int y = GetLogicCoord().y; y < GetLogicCoord().y + GetDimY() / MAPCELLY; y++)
     {
       if (GetMissionInstance()->GetMap()->GetVisibility(OS_PLAYER, x, y) == 2)
       {
         lVisible = true;
         break;
       }
     }

     if (lVisible)
     {
       break;
     }
   }

   // don't paint minimized if no part of the object is visible
   if (!lVisible)
   {
     return;
   }


   if (GetDimX()/MAPCELLX <= 1 && GetDimY()/MAPCELLY <= 1)
   {
      if (GetSelected())
      {
        lColor = 0xFFFFFFFF;
      }
      else
      {
        lColor = GetMissionInstance()->GetBGManager()->GetRelationAsColor(OS_PLAYER, GetBattleGroupId());
      }

      GetGraphicInstance()->SetColor(lColor);
      GetGraphicInstance()->PutPixel(CPPoint((GetCoord().x + GetDimX()/2)/zoomLevel + ptViewPort.x,
                                             (GetCoord().y + GetDimY()/2)/zoomLevel + ptViewPort.y)
                                     );
   }
   else
   {
     CPRect lRect = CPRect(
                    GetCoord().x/zoomLevel + ptViewPort.x,
                    GetCoord().y/zoomLevel + ptViewPort.y,
                    (GetCoord().x + GetDimX())/zoomLevel + ptViewPort.x,
                    (GetCoord().y + GetDimY())/zoomLevel + ptViewPort.y
                    );

     GetGraphicInstance()->SetColor(GetMissionInstance()->GetBGManager()->GetRelationAsColor(OS_PLAYER, GetBattleGroupId()));
     GetGraphicInstance()->FillRectangle(lRect);

     if (GetSelected())
     {
       lRect.top--;
       lRect.left--;
       lRect.bottom++;
       lRect.right++;

       GetGraphicInstance()->SetColor(0xFFFFFFFF);
       GetGraphicInstance()->Rectangle(lRect);
     }
   }

   /*
   GetGraphicInstance()->Circle(NULL,
                                CPPoint((GetCoord().x + GetDimX()/2)/zoomLevel + ptViewPort.x,
                                        (GetCoord().y + GetDimY()/2)/zoomLevel + ptViewPort.y),
                               1,
                               GetMissionInstance()->GetBGManager()->GetRelationAsColor(OS_PLAYER, GetBattleGroupId())
                               );
                               //gSideColor[GetSide()]);

   //in continuare se tiparesc patru puncte in colturi
   if (GetSelected() == true)
   {
      GetGraphicInstance()->PutPixel(NULL,
                                     CPPoint((GetCoord().x + GetDimX()/2)/zoomLevel + ptViewPort.x,
                                             (GetCoord().y + GetDimY()/2)/zoomLevel + ptViewPort.y),
                                     0xffffffff);
   }
   */
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the the object is of enemy type
 parameters : X, Y - logic coord.
---------------------------------------------------------------------------*/
bool CIGRealObject::IsEnemy(int X, int Y)
{
   /*
   if (GetSide() == OS_ENEMY)
      return GetMissionInstance()->IsGroundObjectOfSide(OS_OWNED, X, Y);
   else
      return GetMissionInstance()->IsGroundObjectOfSide(OS_ENEMY, X, Y);
   */
   int lTargetBGroupId = GetMissionInstance()->GetGroundObjectBGroup(X, Y);

   return (GetMissionInstance()->GetBGManager()->GetRelation(GetBattleGroupId(), lTargetBGroupId) == REL_ENEMY);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the the object is in team
 parameters : Team - the queried team
---------------------------------------------------------------------------*/
bool CIGRealObject::IsInTeam (int Team)
{
   return (GetTeam() == Team);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if it contains objects
---------------------------------------------------------------------------*/
bool CIGRealObject::ContainsObjects()
{
   if (GetContainedObjects() == NULL)
      return false;
   return (!GetContainedObjects()->IsEmpty());
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the pbject can contain other objects
---------------------------------------------------------------------------*/
bool CIGRealObject::CanContainObjects()
{
   return (GetContainedObjects() != NULL);
}
//---------------------------------------------------------------------------

void CIGRealObject::Serialize(PP::Stream& _a)
{
  int lUpdateTime = GetTimerInstance()->GetTime() - mUpdateTime;

  CIGBaseObject::Serialize(_a);

  _a.writeBool(&mSelected);
  _a.writeInt(&mTeam          );
  _a.writeInt(&mMaxHP         );
  _a.writeInt(&mCurrentHP     );
  _a.writeInt(&mBattleGroupId );
  _a.writeInt(&mId            );
  _a.writeInt(&mBulletRes     );
  _a.writeInt(&mEnergyRes     );
  _a.writeInt(&mFireRes       );
  _a.writeInt(&mPlasmaRes     );
  _a.writeInt(&mExplodeRes    );
  _a.writeInt(&mMass          );

  _a.writeInt(&lUpdateTime );
  _a.writeInt(&mUpdateTimePeriod );

  // TODO: contained objects serialize
  if (mContainedObjects)
  {
    VLOG(5) << "CIGRealObject::Serialize - skipped contained.";
  }
}
//---------------------------------------------------------------------------

unsigned __int32 CIGRealObject::DataSize()
{
  return sizeof(bool) + sizeof(__int32) * 10;
}
//---------------------------------------------------------------------------

void CIGRealObject::DeSerialize(PP::Stream& _a)
{
  int lUpdateTime;

  CIGBaseObject::DeSerialize(_a);

  _a.readBool(&mSelected);

  // discard the selection flag, there's no real need to save it unless we also save the mission selection lists
  mSelected = false;

  _a.readInt(&mTeam          );
  _a.readInt(&mMaxHP         );
  _a.readInt(&mCurrentHP     );
  _a.readInt(&mBattleGroupId );
  _a.readInt(&mId            );
  _a.readInt(&mBulletRes     );
  _a.readInt(&mEnergyRes     );
  _a.readInt(&mFireRes       );
  _a.readInt(&mPlasmaRes     );
  _a.readInt(&mExplodeRes    );
  _a.readInt(&mMass          );

  _a.readInt(&lUpdateTime );
  _a.readInt(&mUpdateTimePeriod );

  // setup
  mUpdateTime = GetTimerInstance()->GetTime() - lUpdateTime;

  // TODO: contained objects deserialize
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif


