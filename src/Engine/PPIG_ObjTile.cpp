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

 description: Class In Game Base Tile
 last modify: 07 05 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#pragma hdrstop

#include "PPIG_ObjTile.h"
#include "PP_Graphic.h"
#include "PPIG_Mission.h"

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CIGBaseTile::CIGBaseTile() : CIGBaseObject()
{
   SetLayer(MOL_BASETILE);
   SetLogicDimX(1);
   SetLogicDimY(1);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if this class is derived from className
 parameters : className - the parent class
---------------------------------------------------------------------------*/
bool CIGBaseTile::IsDerivedFrom (CPString className)
{
  return (className == ClassName()) ? true : false;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the object sprite (animtype & animCount)
 parameters : ptViewport - the zone of the screen where the paint is wanted
              rcWindow - the visible portion of the map
              Width, Height - is probably redundant and represents the viewable area
              zoomLevel - is 1 by default
---------------------------------------------------------------------------*/

void CIGBaseTile::Paint (CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel)
{
  /*
  if (GetVisible() == true)
  {
    if (GetSprite() != NULL)
    {
       GetGraphicInstance()->PaintSpriteToSurface(NULL,
                                  (GetCoord().x + ptViewPort.x - ptWindow.x)/zoomLevel,
                                  (GetCoord().y + ptViewPort.y - ptWindow.y)/zoomLevel,
                                  GetSprite(), 0, 0);
    }
  }
  */

  CIGBaseObject::Paint(ptViewPort, ptWindow, Width, Height, zoomLevel);
}
//---------------------------------------------------------------------------


void CIGBaseTile::NoteInLayerMap ()
{
  GetMissionInstance()->NoteInLayerMap(GetLayer(), this, GetLogicCoord().x, GetLogicCoord().y);
}
//---------------------------------------------------------------------------

void CIGBaseTile::RemoveFromLayerMap ()
{
  GetMissionInstance()->RemoveFromLayerMap(GetLayer(), this, GetLogicCoord().x, GetLogicCoord().y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: notes the object in logical map
---------------------------------------------------------------------------*/
void CIGBaseTile::NoteInLogicMap()
{
  //NoteInLogicMap(GetLogicCoord());
}
//---------------------------------------------------------------------------

void CIGBaseTile::NoteInLogicMap(CPPoint Point)
{
   /*
   const int lMaxX = Point.x + 3;//GetLogicDimX();
   const int lMaxY = Point.y + 3;//GetLogicDimY();

   if (mType == MTT_WATER)
   {
     for (int i=Point.x; i<lMaxX; ++i)
     {
        for (int j=Point.y; j<lMaxY; j++)
        {
           GetMissionInstance()->NoteInGroundLogicMap(this, i, j);
        }
     }

     return;
   }

   // Karg: weird workaround?!?
   if (mType == MTT_LAND)
   {
     for (int i=Point.x; i<lMaxX; ++i)
     {
        for (int j=Point.y; j<lMaxY; j++)
        {
           GetMissionInstance()->RemoveFromGroundLogicMap(this, i, j);
        }
     }

     return;
   }
   */
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
