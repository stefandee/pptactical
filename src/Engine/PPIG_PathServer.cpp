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
#pragma hdrstop

#include "config.h"
#include "PPIG_PathServer.h"
#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "PP_Timer.h"

CIGPathServer* CIGPathServer::mInstance = NULL;

/*---------------------------------------------------------------------------
 description: gets the path server instance
---------------------------------------------------------------------------*/
CIGPathServer* GetPathServerInstance()
{
  return CIGPathServer::Instance();
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: creates the mInstance attribute and returns it
---------------------------------------------------------------------------*/
CIGPathServer* CIGPathServer::Instance()
{
   if (mInstance == NULL)
      mInstance = ::new CIGPathServer();
   return mInstance;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: deletes the mInstance attribute
---------------------------------------------------------------------------*/
void CIGPathServer::Release()
{
   if (mInstance != NULL)
   {
     delete mInstance;
     mInstance = NULL;
   }
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CIGPathServer::CIGPathServer()
{
   //mRequests = new tList;
   mRequests = new CPList<TIGPath>(1024);
   mAlg      = new CIGPathAlgorithm;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CIGPathServer::~CIGPathServer()
{
   delete mRequests;
   delete mAlg;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: computes the requests
 parameters : maxtTime - the max tick count it can update
---------------------------------------------------------------------------*/
void CIGPathServer::Update(unsigned __int32 maxTime)
{
   maxTime = GetTimerInstance()->GetTime() + 10;
   if (maxTime > GetTimerInstance()->GetTime() && GetRequests()->Count() > 0)
   {
      //here it must call the compute method
      CIGMission *lMI = GetMissionInstance();
      TIGPath *lTmp = (TIGPath *)(GetRequests()->Get(0));
      int lLength, lWay[100];

      // print the ground logic map
      /*
      for(int x = 0; x < lMI->GetMap()->GetLogicWidth(); x++)
      {
        for(int y = 0; y < lMI->GetMap()->GetLogicHeight(); y++)
        {
          logWrite(CPString(lMI->GetMap()->mGroundLogicMap[x][y]) + " ");
        }

        logWriteLn("");
      }
      */

      VLOG(9) << "mAlg->Compute...";
      mAlg->Compute((int *)lMI->GetMap()->mGroundLogicMap,
                    lMI->GetMap()->GetLogicWidth(), lMI->GetMap()->GetLogicHeight(),
                    lTmp->m_ptStart, &lTmp->m_ptEnd,
                    lTmp->mDim, lTmp->mMaxDownSlope, lTmp->mMaxUpSlope, lTmp->mPropulsion,
                    lWay, &lLength);

      //logWriteLn("Note Path...");
      NotePath(lWay, lLength);
      //logWriteLn("Now deleting request...");
      GetRequests()->Remove(0);
      //logWriteLn("Request deleted...");
      //GetRequests()->Capacity = GetRequests()->Count;
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: posts the request message
 parameters : object  - the sender object
              ptStart - the start point of the search (logic coord)
              ptEnd   - the end point of the search (logic coord)
---------------------------------------------------------------------------*/
void CIGPathServer::PostRequest(CIGBaseObject *object, CPPoint ptStart, CPPoint ptEnd, int objDim, int _slopeDown, int _slopeUp, TIGPropulsion _propulsion)
{
   //logWriteLn("new CIGPathServer::PostRequest");

   TIGPath *lTmp = new TIGPath(object, ptStart, ptEnd, objDim, _slopeDown, _slopeUp, _propulsion);
   GetRequests()->Add(lTmp);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: removes the
---------------------------------------------------------------------------*/
void CIGPathServer::RemoveRequest(CIGBaseObject *object)
{
   VLOG(9) << "new CIGPathServer::RemoveRequest";

   for (int i=0; i<GetRequests()->Count(); i++)
      if (((TIGPath *)(GetRequests()->Get(i)))->mObject == object)
      {
         TIGPath *lTmp = (TIGPath *)(GetRequests()->Get(i));
         GetRequests()->Remove(i);
         delete lTmp;
         break;
      }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: notes the path for the mRequests[0]
 parameters : Path   - the path
              Length - the path length
---------------------------------------------------------------------------*/
void CIGPathServer::NotePath(int *Path, int Length)
{
   //logWriteLn("new CIGPathServer::NotePath");
   if (GetRequests()->Count() > 0)
   {
      TIGPath *lTmp = (TIGPath *)(GetRequests()->Get(0));
      lTmp->mObject->NoteWay(Path, Length);
      delete lTmp;
   }
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
