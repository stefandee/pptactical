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
 last modify: 27 06 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PPIG_PathAlgH
#define PPIG_PathAlgH

#include "PPIG_EngineDef.h"

enum TIGPropulsion {TIGP_HOVER = 0, TIGP_WATER, TIGP_UNDERWATER, TIGP_TRACKED, TIGP_HALFTRACK, TIGP_WHEEL, TIGP_FOOT};

class CIGMap;

class CIGPathAlgorithm
{
   public:
      CIGPathAlgorithm();
      void Compute(int *Map, int mapWidth, int mapHeight,
                   CPPoint ptStart, CPPoint *ptEnd,
                   int Dim, int SlopeDown, int SlopeUp, TIGPropulsion Prop,
                   int *resultWay, int *resultLength);
      void ComputeZoneMap(int **Map);

   protected:
      bool GenerateNewBorder();
      bool IsPlaceEmpty(CPPoint _source, CPPoint _target);
      void GetInMapPosition(CPPoint *Point, int mapWidth, int mapHeight);
      void GetNearestPoint(CPPoint *Point, int minValue);
      bool TryPoint(CPPoint ptTry, CPPoint *Point, int minValue, bool *Found);

   private:
      int           mZoneMap[MAXMAPWIDTH][MAXMAPHEIGHT];
      int           mMap[MAXMAPWIDTH][MAXMAPHEIGHT];
      int           mWidth, mHeight;
      int           *mWay;
      int           mWayLength;
      CPPoint       mBorder[2][1000];  //border 1 and 2
      CPPoint       m_ptFinal;
      CPPoint       m_ptStart;
      int           mDim;
      int           mMaxDownSlope, mMaxUpSlope;
      TIGPropulsion mPropulsion;
      int           mCurrentStep;
      bool          mWalkMap[10][10]; // maps propulsions and terrain types
};

//---------------------------------------------------------------------------
#endif
