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
#ifndef PPIG_ObjBoidsH
#define PPIG_ObjBoidsH

#include "PPIG_ObjBase.h"
#include "PPIG_ObjList.h"

class CIGBoidsObject : public CIGBaseObject
{
  public:
      CIGBoidsObject();
      virtual ~CIGBoidsObject();

      virtual CPString ClassName () { return "BOIDS"; }
      virtual bool IsDerivedFrom (CPString className);

      virtual void Update();
      virtual void Paint(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel);

  public:
      virtual int GetBattleGroupId() { return mBattleGroupId; }
      virtual void SetBattleGroupId(int _v) { mBattleGroupId = _v; }

      virtual int GetId() { return mId; }
      virtual void SetId(int _v) { mId = _v; }

      virtual void SetUpdateTime(int _v) { mUpdateTime = _v; }
      virtual int  GetUpdateTime() { return mUpdateTime; }

      virtual void SetUpdateTimePeriod(int _v) { mUpdateTimePeriod = _v; }
      virtual int  GetUpdateTimePeriod() { return mUpdateTimePeriod; }

      virtual bool CanBePlaced(CPPoint _point) { return true; }

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  protected:

      //virtual void TurnToSense(int wantedSense);

  private: //## implementation
      int                  mBattleGroupId;      //the battle group this object belongs to
      int                  mId;                 //the object id

      int                  mUpdateTime, mUpdateTimePeriod; // update parameters
};

/////////////////////////////////////////////////////////////////////////////
//GET-SET (INLINE) SECTION
/////////////////////////////////////////////////////////////////////////////

#endif

