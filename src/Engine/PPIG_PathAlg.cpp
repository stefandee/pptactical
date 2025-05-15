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

 description: Class In Game Path Algorithm
              implements the FORD algorithm
 last modify: 27 06 00
        by grabX
---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
#include <math.h>
#include <cstring>
#pragma hdrstop

#include <algorithm>
using namespace std;
#include "PPIG_PathAlg.h"

#include "PPIG_Map.h"
#include "PPIG_Mission.h"

int sDirOrder[8] = {0, 2, 4, 6, 1, 3, 5, 7};

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CIGPathAlgorithm::CIGPathAlgorithm()
{
//   memset(mZoneMap, 1, MAXMAPWIDTH*MAXMAPHEIGHT*sizeof(int));
  // explicit fill of what can walk over who :)
  mWalkMap[TIGP_HOVER][MTT_WATER]   = true;
  mWalkMap[TIGP_HOVER][MTT_SHORE]   = true;
  mWalkMap[TIGP_HOVER][MTT_LAND]    = true;
  mWalkMap[TIGP_HOVER][MTT_UNKNOWN] = false;

  mWalkMap[TIGP_WATER][MTT_WATER]   = true;
  mWalkMap[TIGP_WATER][MTT_SHORE]   = false;
  mWalkMap[TIGP_WATER][MTT_LAND]    = false;
  mWalkMap[TIGP_WATER][MTT_UNKNOWN] = false;

  mWalkMap[TIGP_UNDERWATER][MTT_WATER]   = true;
  mWalkMap[TIGP_UNDERWATER][MTT_SHORE]   = false;
  mWalkMap[TIGP_UNDERWATER][MTT_LAND]    = false;
  mWalkMap[TIGP_UNDERWATER][MTT_UNKNOWN] = false;

  mWalkMap[TIGP_TRACKED][MTT_WATER]   = false;
  mWalkMap[TIGP_TRACKED][MTT_SHORE]   = true;
  mWalkMap[TIGP_TRACKED][MTT_LAND]    = true;
  mWalkMap[TIGP_TRACKED][MTT_UNKNOWN] = false;

  mWalkMap[TIGP_HALFTRACK][MTT_WATER]   = false;
  mWalkMap[TIGP_HALFTRACK][MTT_SHORE]   = true;
  mWalkMap[TIGP_HALFTRACK][MTT_LAND]    = true;
  mWalkMap[TIGP_HALFTRACK][MTT_UNKNOWN] = false;

  mWalkMap[TIGP_WHEEL][MTT_WATER]   = false;
  mWalkMap[TIGP_WHEEL][MTT_SHORE]   = false;
  mWalkMap[TIGP_WHEEL][MTT_LAND]    = true;
  mWalkMap[TIGP_WHEEL][MTT_UNKNOWN] = false;

  mWalkMap[TIGP_FOOT][MTT_WATER]   = false;
  mWalkMap[TIGP_FOOT][MTT_SHORE]   = true;
  mWalkMap[TIGP_FOOT][MTT_LAND]    = true;
  mWalkMap[TIGP_FOOT][MTT_UNKNOWN] = false;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: computes the zone map according to the Map
 parameters : the map of obstacles
---------------------------------------------------------------------------*/
void CIGPathAlgorithm::ComputeZoneMap(int **Map)
{

}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: computes the algorithm
 parameters : Map                 - the initial map of obstacles
              mapWidth, mapHeight - map dims in logic coord
              Step                - the algorithm step (the unit dimension)
              resultWay           - a array with the way
              resultLength        - the way length
 others     :
---------------------------------------------------------------------------*/
void CIGPathAlgorithm::Compute(int *Map, int mapWidth, int mapHeight,
                               CPPoint ptStart, CPPoint *ptEnd,
                               int Dim, int SlopeDown, int SlopeUp, TIGPropulsion Prop,
                               int *resultWay, int *resultLength)
{
   GetInMapPosition(ptEnd, mapWidth, mapHeight);
   if (ptStart.x == ptEnd->x && ptStart.y == ptEnd->y)
   {
      *resultLength = 0;
      return;
   }

   int lMap[MAXMAPWIDTH][MAXMAPHEIGHT];
   memcpy(lMap, Map, MAXMAPWIDTH*MAXMAPHEIGHT*sizeof(int));

   for (int i=0; i<mapWidth; i++)
      for( int j =0; j<mapHeight; j++)
         mMap[i][j] = lMap[i][j];

   mWidth        = mapWidth;
   mHeight       = mapHeight;
   mDim          = Dim;
   mMaxUpSlope   = SlopeUp;
   mMaxDownSlope = SlopeDown;
   mPropulsion   = Prop;
   m_ptFinal     = *ptEnd;
   m_ptStart     = ptStart;
   mWayLength    = 0;
   mWay          = resultWay;

   mCurrentStep    = 1;
   mBorder[0][0]   = ptStart;
   mBorder[0][1].x = -1;
   mBorder[1][0].x = -1;

   for(int i = 0; i < mDim; i++)
   {
     for(int j = 0; j < mDim; j++)
     {
       mMap[ptStart.x + i][ptStart.y + j] = -1;
     }
   }

   mMap[ptStart.x][ptStart.y] = 1;

   //TEST CODE
   /*
   logWriteLn("Before");
   for (int i=0; i<mapWidth; i++)
   {
      for (int j=0; j<mapHeight; j++)
      {
         logWrite(CPString(mMap[j][i]).c_str());
         for (int k= 0; k<3 - CPString(mMap[j][i]).Length(); k++)
            logWrite(" ");
      }
      logWrite("\n");
   }
   logWrite("\n");
   logWriteLn("Map dim : ");
   logWriteILn(mapWidth);
   logWriteILn(mapHeight);
   logWriteLn("Init Point");
   logWriteILn(ptStart.x);
   logWriteILn(ptStart.y);
   logWriteLn("Final Point");
   logWriteILn(ptEnd->x);
   logWriteILn(ptEnd->y);
   */
   //--------------


   //logWriteLn("Generate new border...");
   //logWrite("Dim is "); logWriteILn(mDim);
   while (GenerateNewBorder() == true);

   // Dump the map to the log file
   /*
   logWriteLn("After");
   for (int i=0; i<mapWidth; i++)
   {
      for (int j=0; j<mapHeight; j++)
      {
         if (j == m_ptFinal.x && i == m_ptFinal.y)
         {
           logWrite("X");

           for (int k= 0; k<3 - 1; k++)
              logWrite(" ");
         }
         else
         {
           logWrite(CPString(mMap[j][i]).c_str());

           for (int k= 0; k<3 - CPString(mMap[j][i]).Length(); k++)
              logWrite(" ");
         }
      }
      logWrite("\n");
   }
   logWrite("\n");
   */
   //-------------

   //logWriteLn("Get nearest point...");
   GetNearestPoint(ptEnd, 1);

   //logWriteLn("if-ing...");
   if (mMap[ptEnd->x][ptEnd->y] > 0)
   {  //way found
       int i = ptEnd->x;
       int j = ptEnd->y;

       //logWriteLn("CIGPathAlgorithm::Compute - way found");

       // TODO: check this
       *resultLength = (mMap[ptEnd->x][ptEnd->y]-1)/* * Dim*/;
       //*resultLength = mMap[ptEnd->x][ptEnd->y]-1;

       int lLength = 0;
       int lOldLength = 0;

       //logWriteLn("whiling...");
       //logWrite("Dim is "); logWriteILn(Dim);
       //logWrite("resultLength "); logWriteILn(*resultLength);

       // resultLength and Dim are somehow related
       while (lLength < *resultLength)
       {
          for (int x=0; x<8; x++)
          {
             if (mMap[i][j] == mMap[i + gReverseDir[sDirOrder[x]].x]
                                   [j + gReverseDir[sDirOrder[x]].y] + 1 &&
                 mMap[i][j] > 0)
             {
                i += /*Dim* */gReverseDir[sDirOrder[x]].x;
                j += /*Dim* */gReverseDir[sDirOrder[x]].y;

                mWay[lLength++] = sDirOrder[x];

                // TODO: since mWay is filled with directions to go from tile to
                // tile, for units larger than 1x1 add more directions, taking into
                // account the points (i,j) and (i + Dim*gReverseDir[sDirOrder[x]].x, j + Dim*gReverseDir[sDirOrder[x]].y)
                //for(int k = 0; k < Dim; k++)
                //{
                //  mWay[lLength++] = sDirOrder[x];
                //}

                //logWrite("Length: "); logWriteILn(lLength);
                break;
             }
          }

          // safety!
          if (lOldLength == lLength)
          {
            break;
          }

          lOldLength = lLength;
       }
   }
   else
   {// no way found !!!!!!!!!
      VLOG(9) << "CIGPathAlgorithm::Compute - no way found";
      mWayLength = 0;
   }

   //logWrite("Way Length = ");
   //logWriteILn(*resultLength);
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: generates a new border
 others     : returns false if final is reached
---------------------------------------------------------------------------*/
bool CIGPathAlgorithm::GenerateNewBorder()
{
   mCurrentStep++;
   int lOldIndex = mCurrentStep%2;
   int lNewIndex = (mCurrentStep+1)%2;
   CPPoint l_ptCrt;
   int lCountNew = 0;
   bool lNotFound = false;

   for (int i = 0; mBorder[lOldIndex][i].x != -1; i++)
   {
      for (int dir = 0; dir < 8; dir++)
      {
         //l_ptCrt = CPPoint(mBorder[lOldIndex][i].x + mDim*gDir[dir].x, mBorder[lOldIndex][i].y + mDim*gDir[dir].y);
         l_ptCrt = CPPoint(mBorder[lOldIndex][i].x + /*mDim* */gDir[dir].x, mBorder[lOldIndex][i].y + /*mDim* */gDir[dir].y);

         // Karg: lol grabX, too lazy to negate a condition? :)
         if ((l_ptCrt.x < 0)||(l_ptCrt.x >= mWidth)||
             (l_ptCrt.y < 0)||(l_ptCrt.y >= mHeight));
         else  // this means is inside the map
         {
           if ( IsPlaceEmpty(mBorder[lOldIndex][i], l_ptCrt))
           {
              if ((l_ptCrt.x == m_ptFinal.x)&&(l_ptCrt.y == m_ptFinal.y))
              /*
              int minX = min(mBorder[lOldIndex][i].x, mBorder[lOldIndex][i].x + mDim*gDir[dir].x);
              int maxX = max(mBorder[lOldIndex][i].x, mBorder[lOldIndex][i].x + mDim*gDir[dir].x);
              int minY = min(mBorder[lOldIndex][i].y, mBorder[lOldIndex][i].y + mDim*gDir[dir].y);
              int maxY = max(mBorder[lOldIndex][i].y, mBorder[lOldIndex][i].y + mDim*gDir[dir].y);

              if ((maxX >= m_ptFinal.x)&&(maxY >= m_ptFinal.y) &&
                  (minX <= m_ptFinal.x)&&(minY <= m_ptFinal.y))
              */
              { //destination found
                 mMap[l_ptCrt.x][l_ptCrt.y] = mCurrentStep;
                 return false;
              }
              mMap[l_ptCrt.x][l_ptCrt.y] = mCurrentStep;
              mBorder[lNewIndex][lCountNew] = l_ptCrt;
              lCountNew++;
              mBorder[lNewIndex][lCountNew].x = -1; //end of border marker
              lNotFound = true;
           }
         }
      }
   }
   return lNotFound;
}
//---------------------------------------------------------------------------

// Description : checks if the unit searching the path can be placed in the cell
// Param       : Point - the cell
// Result      : true - if it can be placed
// Comments    : a unit may be placed on a cell if the propulsion type is able
//               to pass over the tile type and the slope is less or equal than
//               the slope generated by the source and target point
bool CIGPathAlgorithm::IsPlaceEmpty(CPPoint _source, CPPoint _target)
{
   // first, check if the object can be placed; treat the object as having 1x1
   // dimensions
   bool lMapOk = (mMap[_target.x][_target.y] <= 0 && mMap[_target.x][_target.y] >= -2);

   if (!lMapOk)
   {
     return false;
   }

   if (mDim > 1)
   {
     // now, check if the rest of the object can be placed
     for(int i = 0; i < mDim; i++)
     {
       for(int j = 0; j < mDim; j++)
       {
         // keep the object inside the map :)
         if (_target.y + j >= mHeight || _target.x + i >= mWidth)
         {
           continue;
         }

         if (i != 0 || j != 0)
         {
           // the rest of the object can step on its own trail or can be placed
           // if it is in the starting rectangle
           bool lMapOk =
           (
             mMap[_target.x + i][_target.y + j] >= -2 ||
             (m_ptStart.x <= _target.x + i && m_ptStart.x + mDim > _target.x + i && m_ptStart.y <= _target.y + j && m_ptStart.y + mDim > _target.y + j)
           );

           if (!lMapOk)
           {
             return false;
           }
         }
       }
     }
   }

   /*
   for(int i = 0; i < mDim; i++)
   {
     for(int j = 0; j < mDim; j++)
     {
       bool lMapOk = (mMap[_target.x + i][_target.y + j] <= 0 && mMap[_target.x + i][_target.y + j] >= -2);

       if (!lMapOk)
       {
         return false;
       }
     }
   }
   */

   TTileType lType   = GetMissionInstance()->GetMap()->GetTileType(_target.x, _target.y);

   if (!mWalkMap[mPropulsion][lType])
   {
     return false;
   }

   int lSourceHeight = GetMissionInstance()->GetMap()->GetAltitude(_source.x, _source.y);
   int lTargetHeight = GetMissionInstance()->GetMap()->GetAltitude(_target.x, _target.y);

   float lAbs = gfGetDist2D(CPPoint(_source.x * MAPCELLX, _source.y * MAPCELLY), CPPoint(_target.x * MAPCELLX, _target.y * MAPCELLY));
   float lOrd = lSourceHeight - lTargetHeight;
   float lAngle = atan(fabs(lOrd)/lAbs) * 180 / PI;

   // the object is going up!
   if ((lOrd < 0) && (lAngle > mMaxUpSlope))
   {
     return false;
   }

   // the object is going down!
   if ((lOrd > 0) && (lAngle > mMaxDownSlope))
   {
     return false;
   }

   return true;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: if the Point is outside the map => inside
---------------------------------------------------------------------------*/
void CIGPathAlgorithm::GetInMapPosition(CPPoint *Point, int mapWidth, int mapHeight)
{
   //in map condition
   if (Point->x > mapWidth-1)
      Point->x = mapWidth-1;
   if (Point->x < 0)
      Point->x = 0;
   if (Point->y > mapHeight-1)
      Point->y = mapHeight-1;
   if (Point->y < 0)
      Point->y = 0;
}
//---------------------------------------------------------------------------


/*---------------------------------------------------------------------------
 description: gets the nearest place to the Point
 parameters : Point    - the starting point
              minPos   - the min value in the mMap that is suitable
---------------------------------------------------------------------------*/
void CIGPathAlgorithm::GetNearestPoint(CPPoint *Point, int minValue)
{
   if (mMap[Point->x][Point->y] <= 0)
   {
      bool   lFinalFound = false;
      for (int i=0; !lFinalFound; i++)
      {
         for (int j=-i; j<=i; j++)
         {
            if (TryPoint(CPPoint(Point->x - i, Point->y + j), Point, minValue, &lFinalFound))
               break;
            if (TryPoint(CPPoint(Point->x + i, Point->y + j), Point, minValue, &lFinalFound))
               break;
            if (TryPoint(CPPoint(Point->x + j, Point->y + i), Point, minValue, &lFinalFound))
               break;
            if (TryPoint(CPPoint(Point->x + j, Point->y - i), Point, minValue, &lFinalFound))
               break;
         }
      }
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: checks the ptTry point to be greater than minValue and sets Point
 parameters : ptTry   - the tried point
              Point   - the result point
              minValue- the min value required for the place to be
              Found   - shows if the point was found
 others     : is used by the GetNearestPoint
---------------------------------------------------------------------------*/
bool CIGPathAlgorithm::TryPoint(CPPoint ptTry, CPPoint *Point, int minValue, bool *Found)
{
   if (mMap[ptTry.x][ptTry.y] >= minValue)
   {
      *Found = true;
      *Point = ptTry;
      return true;
   }
   return false;
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
