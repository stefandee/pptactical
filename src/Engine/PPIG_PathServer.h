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

 description: Class In Game Path Server
 last modify: 23 06 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PPIG_PathServerH
#define PPIG_PathServerH

#include "PPIG_PathAlg.h"                                                                   
#include "basic2d.h"
#include "KList.h"

class CIGBaseObject;

typedef struct SIGPath
{
   SIGPath(CIGBaseObject *object, CPPoint ptStart, CPPoint ptEnd, int objDim, int _slopeDown, int _slopeUp, TIGPropulsion _propulsion)
   {
      mObject       = object;
      m_ptStart     = ptStart;
      m_ptEnd       = ptEnd;
      mDim          = objDim;
      mMaxUpSlope   = _slopeUp;
      mMaxDownSlope = _slopeDown;
      mPropulsion   = _propulsion;
   }

   CIGBaseObject *mObject;
   CPPoint        m_ptStart;
   CPPoint        m_ptEnd;
   int            mDim;          // the dimension of the object, it is interpreted as step
   int            mMaxUpSlope;   // the maximum slope the object is able to climb
   int            mMaxDownSlope;  // the maximum slope the object is able to descend
   TIGPropulsion  mPropulsion;   // the propulsion of the object (defines which type of ground the object may walk over)
} TIGPath;

class CIGPathServer
{
   public:
      static CIGPathServer* Instance();
      static void Release();
      void Update(unsigned __int32 maxTime);
      void PostRequest(CIGBaseObject *object, CPPoint ptStart, CPPoint ptEnd, int objDim, int _slopeDown, int _slopeUp, TIGPropulsion _propulsion);
      void RemoveRequest(CIGBaseObject *object);

   protected:
      CIGPathServer();
      ~CIGPathServer();

      //tList* GetRequests();
      CPList<TIGPath>* GetRequests();
      void   NotePath(int *Path, int Length);

   private:
      static CIGPathServer*       mInstance;
      //tList                      *mRequests;
      CPList<TIGPath>             *mRequests;
      CIGPathAlgorithm            *mAlg;
};

CIGPathServer* GetPathServerInstance();

/////////////////////////////////////////////////////////////////////////////
//(INLINE) GET-SET SECTION
/////////////////////////////////////////////////////////////////////////////

inline CPList<TIGPath>* CIGPathServer::GetRequests()
{
   return mRequests;
}

//---------------------------------------------------------------------------
#endif
