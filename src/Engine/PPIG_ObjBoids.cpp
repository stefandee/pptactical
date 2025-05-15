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
#include "PPIG_ObjBoids.h"
#include "PP_Graphic.h"
#include "PPIG_Map.h"
#include "PP_Timer.h"

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CIGBoidsObject::CIGBoidsObject()
{
   SetLayer(MOL_GROUNDEFFECTS);

   mBattleGroupId = 0;
   mId            = -1;

   SetLogicDimX(1);
   SetLogicDimY(1);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CIGBoidsObject::~CIGBoidsObject()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if this class is derived from className
---------------------------------------------------------------------------*/
bool CIGBoidsObject::IsDerivedFrom (CPString className)
{
   return ((className == ClassName()) || (CIGBaseObject::IsDerivedFrom(className)));
}
//---------------------------------------------------------------------------

void CIGBoidsObject::Serialize(PP::Stream& _a)
{
}
//---------------------------------------------------------------------------

unsigned __int32 CIGBoidsObject::DataSize()
{
  return 0;
  //return sizeof(bool) + sizeof(__int32) * 10;
}
//---------------------------------------------------------------------------

void CIGBoidsObject::DeSerialize(PP::Stream& _a)
{
}
//---------------------------------------------------------------------------

void CIGBoidsObject::Update()
{
  VLOG(9) << "CIGBoidsObject::Update";

  if (GetTimerInstance()->GetTime() - GetUpdateTime() < (unsigned)GetUpdateTimePeriod())
  {
    return;
  }

  SetUpdateTime(GetTimerInstance()->GetTime());

  // write the update code here
}
//---------------------------------------------------------------------------

void CIGBoidsObject::Paint(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel)
{
  VLOG(9) << "CIGBoidsObject::Paint";

  //if (!GetVisible())
  //{
  //  return;
  //}

  // demo code - you need to add viewport and window to the coordinates (GetCoord) to paint
  // at the real location inside the window
  // it's just a transformation (window coordinates to screen coordinates)
  GetGraphicInstance()->SetColor(0xFFFFFFFF);
  GetGraphicInstance()->Line(CPPoint(GetCoord().x + MAPCELLX/2 + (ptViewPort.x - ptWindow.x)/zoomLevel, GetCoord().y + MAPCELLY/2 + (ptViewPort.y - ptWindow.y)/zoomLevel),
                             CPPoint(GetCoord().x + MAPCELLX/2 +  + (ptViewPort.x - ptWindow.x)/zoomLevel + 2 * MAPCELLX,
                                     GetCoord().y + MAPCELLY/2 - 2 * MAPCELLY + (ptViewPort.y - ptWindow.y)/zoomLevel)
                             );

  /*
  GetGraphicInstance()->TextOut(
    NULL,
    GetCoord().x + (ptViewPort.x - ptWindow.x)/zoomLevel,
    GetCoord().y + MAPCELLY/2 + (ptViewPort.y - ptWindow.y)/zoomLevel,
    CPString(GetLogicCoord().x) + "," + CPString(GetLogicCoord().y),
    TA_LEFT
    );
  */

  VLOG(9) << "CIGBoidsObject::Paint - over";
}

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif


