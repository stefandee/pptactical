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
 application: PP IN GAME

 description: In Game Engine Defines
 last modify: 10 06 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#include <algorithm>
#pragma hdrstop

#include "PPIG_EngineDef.h"
#include "basic3d.h"
#include "basic2d.h"
#include <math.h>

#include "PP_String.h"

//simple directions
CPPoint gDir[8]={CPPoint(0,-1),CPPoint(1,-1), CPPoint(1,0), CPPoint(1,1),
                CPPoint(0,1), CPPoint(-1,1), CPPoint(-1,0),CPPoint(-1,-1)};

CPPoint gReverseDir[8] =
               {CPPoint(0,1),CPPoint(-1,1),CPPoint(-1,0),CPPoint(-1,-1),
                CPPoint(0,-1),CPPoint(1,-1),CPPoint(1,0),CPPoint(1,1)};

/*const float  gDirAngle[8] =
               {(float)(PI/2), (float)(PI/4), 0.0f, (float)(7*PI/4),
			   (float)(3*PI/2), (float)(5*PI/4), (float)PI, (float)(3*PI/4) };*/

float  gDirAngle[8] =
               {
                   (float)(3*PI/2),
                   (float)(7*PI/4),
                   0.0f,
                   (float)(PI/4),
                   (float)(PI/2),
                   (float)(3*PI/4),
                   (float)PI,
                   (float)(5*PI/4)
               };

/*float  gComplexDirAngle[16] = {(float)(PI/2), (float)(3*PI/8), (float)(PI/4), (float)(PI/8),
                               0.0f, (float)(15*PI/8), (float)(7*PI/4), (float)(13*PI/8),
                               (float)(3*PI/2), (float)(11*PI/8), (float)(5*PI/4), (float)(9*PI/8),
                               (float)PI, (float)(7*PI/8), (float)(3*PI/4), (float)(5*PI/8)};*/

float gComplexDirAngle[16] =
               {
                   (float)(3*PI/2),
                   (float)(13*PI/8),
                   (float)(7*PI/4),
                   (float)(15*PI/8),
                   0.0f,
                   (float)(PI/8),
                   (float)(PI/4),
                   (float)(3*PI/8),
                   (float)(PI/2),
                   (float)(5*PI/8),
                   (float)(3*PI/4),
                   (float)(7*PI/8),
                   (float)PI,
                   (float)(9*PI/8),
                   (float)(5*PI/4),
                   (float)(11*PI/8)
               };

/**< don't forget to use 0xFF for alpha */
TColor32 gSideColor[4] = {0xFF808080, 0xFF0000A1, 0xFF00A1A1, 0xFFA10000};

int gfGetDirFromPoints(CPPoint ptStart, CPPoint ptEnd)
{
   float lAngle = CenterRadianAngle(ptEnd.x - ptStart.x, ptEnd.y - ptStart.y);
   float lMin = (float)(2*PI);
   int lFound = 0;
   for (int i=0; i<8; i++)
      if (fabs(lAngle - gDirAngle[i]) < lMin)
      {
         lMin = fabs(lAngle - gDirAngle[i]);
         lFound = i;
      }
   return lFound;
}

int gfGetDirFromAngle(float Angle)
{
   float lMin = (float)(2*PI);

   int lFound = 0;

   for (int i = 0; i < 8; i++)
   {
      if (fabs(Angle - gDirAngle[i]) < lMin)
      {
         lMin = fabs(Angle - gDirAngle[i]);
         lFound = i;
      }
   }

   return lFound;
}

int gfGetComplexDirFromAngle(float Angle)
{
   float lMin = (float)(10*PI);

   int lFound = 0;

   for (int i=0; i<16; i++)
   {
      if (fabs(Angle - gComplexDirAngle[i]) < lMin)
      {
         lMin = fabs(Angle - gComplexDirAngle[i]);
         lFound = i;
      }
   }

   return lFound;
}

int gfGetComplexDirFromPoints(CPPoint ptStart, CPPoint ptEnd)
{
   float lAngle = CenterRadianAngle(ptEnd.x - ptStart.x, ptEnd.y - ptStart.y);
   return gfGetComplexDirFromAngle(lAngle);
}

template <class T> static inline T our_max(T a, T b) { return a>b?a:b; }
int gfGetDist2D(CPPoint pt1, CPPoint pt2)
{
   return our_max(abs(pt1.x - pt2.x), abs(pt1.y - pt2.y));
}

//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

