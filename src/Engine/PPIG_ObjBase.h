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

//-----------------------------------------------------------------------------
//  Unit              : Class Base Object
//
//  Versiune          :
//
//  Descriere         :
//    * here are the base properties and methods for all the objects
//      envolved in the game
//    * AddAction and SetAction now work as dispaching methods: the subclasses
//      will implement these methods and will direct the action to the needed
//      subsystem(s)
//
//  Istorie           :
//    [09.07.2000] - [grabX] - unit created
//    [10.11.2002] - [Karg] - removed object actions, introduced subsystems support
//    [25.11.2002] - [Karg] - moved attackers and noises list to sensors subsystem
//    [25.07.2004] - [Karg] - added Can method
//-----------------------------------------------------------------------------

#ifndef PPIG_ObjBaseH
#define PPIG_ObjBaseH

#include "PPIG_EngineDef.h"
#include "PP_String.h"
#include "PP_Sprite.h"
#include "PPIG_ObjStimul.h"
#include "PP_Counter.h"
#include "PPIG_PathServer.h"
#include "Serializable.h"
#include "PPIG_SubSysAction.h"

class CIGBaseObject : public CSerializable
{
  private:
    // TMN: Make sure we prevent slicing
    CIGBaseObject(const CIGBaseObject& rhs);	// no impl.
    void operator=(const CIGBaseObject& rhs);	// no impl.

  public:
      //friend class CIGObjectAction;
      friend class CIGPathServer;

  public:
      CIGBaseObject();
      virtual ~CIGBaseObject();

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  public:
    //## Other Operations (specified)
      virtual CPString ClassName () {return IGCN_BASEOBJECT;}
      virtual bool IsDerivedFrom (CPString className);
      virtual void Update();
      //paint methods
      virtual void Paint(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel = 1);
      virtual void PaintMinimized(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel = 1);
      virtual void PaintSelection (CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel = 1);
      virtual void PaintTeam (CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel = 1);
      virtual void PaintHealthBar (CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevele = 1);
      //--------------

      //
      // Override this if you override the Paint
      //
      virtual CPRect GetPaintRect();
      //--------------

      //action methods
      virtual void AddAction(TIGSubSysActionData actionData) {};
      virtual void AddAction (CIGSubSysAction *Action) {};
      virtual void AddActionWithPriority (CIGSubSysAction *Action) {};
      virtual void SetAction (TIGSubSysActionData actionData) {};
      //--------------

      //map methods
      virtual void NoteInLayerMap ();
      virtual void RemoveFromLayerMap ();
      virtual void NoteInLogicMap();
      virtual void NoteInLogicMap(CPPoint Point);
      virtual void RemoveFromLogicMap();
      virtual void NoteStimul(CIGObjectStimul &stimul) {};
      //--------------

      virtual CPPoint GetClosestPointTo(CPPoint targetPoint);
      virtual void    CenterInPoint(CPPoint targetPoint);

      //
      // query an object of its capabilities
      // the CAN_* constants that should be sent as parameters are defined in
      // PPIG_EngineDef.h
      // the main use is for transport (check if a unit can carry units or
      // can unload)
      //
      virtual bool    Can(int _capability) { return false; }

      //way methods
      virtual void ComputeWay();
      virtual void NoteWay(int *way, int length);
      virtual bool IsNextPlaceEmpty();
      virtual bool IsInFixedPlace();
      virtual bool IsNextObjectMoving();
      //--------------

  public: //actions handlers to be overwritten
      //virtual void Stop();
      //virtual void Move();
      virtual void Die();

  public:
    //## Get and Set Operations for Class Attributes (generated)

      virtual CPSprite * GetSprite ();
      virtual void SetSprite (CPSprite * value);

      virtual bool GetVisible ();
      virtual void SetVisible (bool value);

      virtual bool GetUpdateEnabled ();
      virtual void SetUpdateEnabled (bool value);

      virtual CPCounter& GetUpdateCounter();
      virtual void SetUpdateCounter(CPCounter& value);
      virtual void SetUpdateCounter(int value);

      virtual __int32 GetActionMask ();
      virtual void SetActionMask (__int32 value);

      virtual int  GetDimX ();
      virtual void SetDimX (int value);

      virtual int GetDimY ();
      virtual void SetDimY (int value);

      virtual int  GetLogicDimX ();
      virtual void SetLogicDimX (int value);

      virtual int  GetLogicDimY ();
      virtual void SetLogicDimY (int value);

      virtual CPPoint GetCoord ();
      virtual void    SetCoord (CPPoint value);

      virtual CPPoint GetLogicCoord ();
      virtual void    SetLogicCoord (CPPoint value);

      virtual int  GetCoordZ ();
      virtual void SetCoordZ (int value);

      virtual int  GetLayer();
      virtual void SetLayer(int value);

      virtual void SetSelected(bool value) {}
      virtual bool GetSelected() {return false;}

      virtual int GetBattleGroupId() { return -1; }
      virtual void SetBattleGroupId(int _v) {}

      virtual int GetId() { return -1; }
      virtual void SetId(int _v) {}

      virtual int  GetWay(int index = 0);
      virtual int  GetNextWay();
      virtual void SetWay(int index, int value);

      virtual int  GetWayLength();
      virtual void SetWayLength(int value);

      virtual CPPoint& GetNextPoint();
      virtual void SetNextPoint(const CPPoint& value);

      virtual CPPoint& GetLastPoint();
      virtual void SetLastPoint(CPPoint &value);

      virtual CPCounter& GetSpeed();
      virtual void SetSpeed(const CPCounter& value);

      virtual float GetMaxSpeed (void) { return mMaxSpeed; }
      virtual void SetMaxSpeed (float _v) { if (_v >= 0) mMaxSpeed = _v; }

      virtual CIGBaseObject * GetWaitingFor ();
      virtual void SetWaitingFor (CIGBaseObject * value);

      virtual bool GetMoving();
      virtual void SetMoving(bool value);

      virtual CPString& GetName();
      virtual void SetName(CPString value);

      virtual int GetTeam()            {return -1;}
      virtual void SetTeam(int value)  {}

      virtual bool GetIsDead();
      virtual void SetIsDead(bool value);

      // check the object mask against the map and
      // return true if the object may be placed in the map
      virtual bool CanBePlaced(CPPoint _point);

      // check if the point falls inside the unit, in a pixel-perfect manner
      // individual units may override this method to provide specific implementation
      // (for example, tanks that are formed of two graphical parts, turret and body)
      virtual bool Pick(CPPoint _point) { return true; }

  private: //## implementation
      // Data Members for Class Attributes
      CPSprite            *mSprite;       //the sprite
      bool                 mVisible;      //if the object is to be painted
      bool                 mUpdateEnabled;//if the update os enabled for the
      CPCounter            mUpdateCounter;

      float                mMaxSpeed;

      int                  mActionMask;         //this is a bit mask that describes the action
                                                //ActionMask = 2 >> Action
      int                  mDimX;         //real X dim
      int                  mDimY;         //real Y dim
      int                  mLogicDimX;    //the logic dim
      int                  mLogicDimY;    //the logic dim
      CPPoint              mCoord;        //real coord of the left,top
      CPPoint              mLogicCoord;   //logic coord of the left,top
      int                  mCoordZ;       //coord on z axis
      int                  mLayer;        //the layer

      int                  mWay[WAY_MAX_LENGTH];   //the way
      int                  mWayLength;             //the way length
      CPPoint              mNextPoint;             //the next logic point to reach
      CPPoint              mLastPoint;             //the last logic point that was reached
      CPCounter            mSpeed;                 //the object speed (in real coord)

      CIGBaseObject       *mWaitingFor;
      bool                 mMoving;                //moving flag

      CPString             mName;                  //the name of the object
      bool                 mIsDead;
};

/////////////////////////////////////////////////////////////////////////////
//GET-SET (INLINE) SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: Sprite
---------------------------------------------------------------------------*/
inline CPSprite * CIGBaseObject::GetSprite ()
{
  return mSprite;
}

inline void CIGBaseObject::SetSprite (CPSprite * value)
{
  VLOG(9) << "CIGBaseObject::SetSprite";

  mSprite = value;
  if (value != NULL)
  {
    VLOG(9) << mSprite->GetFileName() << " - width: " << mSprite->GetAnimWidth() << ", height: " << mSprite->GetAnimHeight() << ", count: " << mSprite->GetAnimCount();

    SetDimX(mSprite->GetAnimWidth());
    SetDimY(mSprite->GetAnimHeight());
    //GetAnimCounter().SetMaxValue(mSprite->GetAnimCount()-1);
  }

  VLOG(9) << "CIGBaseObject::SetSprite - over";
}

/*---------------------------------------------------------------------------
 description: Visible
---------------------------------------------------------------------------*/
inline bool CIGBaseObject::GetVisible ()
{
  return mVisible;
}

inline void CIGBaseObject::SetVisible (bool value)
{
  mVisible = value;
}

/*---------------------------------------------------------------------------
 description: UpdateEnabled
---------------------------------------------------------------------------*/
inline bool CIGBaseObject::GetUpdateEnabled ()
{
  return mUpdateEnabled;
}

inline void CIGBaseObject::SetUpdateEnabled (bool value)
{
  mUpdateEnabled = value;
}

/*---------------------------------------------------------------------------
 description: UpdateCounter
---------------------------------------------------------------------------*/
inline CPCounter& CIGBaseObject::GetUpdateCounter()
{
   return mUpdateCounter;
}

inline void CIGBaseObject::SetUpdateCounter(CPCounter& value)
{
   mUpdateCounter = value;
}

inline void CIGBaseObject::SetUpdateCounter(int value)
{
   mUpdateCounter = value;
}

/*---------------------------------------------------------------------------
 description: ActionMask
---------------------------------------------------------------------------*/
inline __int32 CIGBaseObject::GetActionMask ()
{
  return mActionMask;
}

inline void CIGBaseObject::SetActionMask (__int32 value)
{
  mActionMask = value;
}

/*---------------------------------------------------------------------------
 description: DimX
---------------------------------------------------------------------------*/
inline int CIGBaseObject::GetDimX ()
{
  return mDimX;
}

inline void CIGBaseObject::SetDimX (int value)
{
  VLOG(9) << "CIGBaseObject::SetDimX, value: " << value;

  mDimX = value;
  if(mDimX/MAPCELLX != 0)
    SetLogicDimX(mDimX/MAPCELLX);
  else
    SetLogicDimX(1);

  VLOG(9) << "CIGBaseObject::SetDimX - over";
}

/*---------------------------------------------------------------------------
 description: DimY
---------------------------------------------------------------------------*/
inline int CIGBaseObject::GetDimY ()
{
  return mDimY;
}

inline void CIGBaseObject::SetDimY (int value)
{
  VLOG(9) << "CIGBaseObject::SetDimY, value: " << value;

  mDimY = value;
  if(mDimX/MAPCELLX != 0)
  {
    SetLogicDimY(mDimY/MAPCELLY);
  }
  else
    SetLogicDimY(1);

  VLOG(9) << "CIGBaseObject::SetDimY - over";
}

/*---------------------------------------------------------------------------
 description: LogicDimX
---------------------------------------------------------------------------*/
inline int CIGBaseObject::GetLogicDimX ()
{
  return mLogicDimX;
}

inline void CIGBaseObject::SetLogicDimX (int value)
{
  VLOG(9) << "CIGBaseObject::SetLogicDimX, value: " << value;

  mLogicDimX = value;

  VLOG(9) << "CIGBaseObject::SetLogicDimX - over";
}

/*---------------------------------------------------------------------------
 description: LogcDimY
---------------------------------------------------------------------------*/
inline int CIGBaseObject::GetLogicDimY ()
{
  return mLogicDimY;
}

inline void CIGBaseObject::SetLogicDimY (int value)
{
  VLOG(9) << "CIGBaseObject::SetLogicDimY, value: " << value;

  mLogicDimY = value;

  VLOG(9) << "CIGBaseObject::SetLogicDimY - over";
}

/*---------------------------------------------------------------------------
 description: Coord
---------------------------------------------------------------------------*/
inline CPPoint CIGBaseObject::GetCoord ()
{
  return mCoord;
}

inline void CIGBaseObject::SetCoord (CPPoint value)
{
  mCoord = value;
  SetLogicCoord(CPPoint(value.x/MAPCELLX, value.y/MAPCELLY));
}

/*---------------------------------------------------------------------------
 description: LogicCoord
---------------------------------------------------------------------------*/
inline CPPoint CIGBaseObject::GetLogicCoord ()
{
  return mLogicCoord;
}

inline void CIGBaseObject::SetLogicCoord (CPPoint value)
{
  if (mLogicCoord.x == -1) //first set
    SetNextPoint(value);
  mLogicCoord = value;
}

/*---------------------------------------------------------------------------
 description: CoordZ
---------------------------------------------------------------------------*/
inline int CIGBaseObject::GetCoordZ ()
{
  return mCoordZ;
}

inline void CIGBaseObject::SetCoordZ (int value)
{
  mCoordZ = value;
}

/*---------------------------------------------------------------------------
 description: Layer
---------------------------------------------------------------------------*/
inline int CIGBaseObject::GetLayer()
{
   return mLayer;
}
inline void CIGBaseObject::SetLayer(int value)
{
   mLayer = value;
}

/*---------------------------------------------------------------------------
 description: Way
---------------------------------------------------------------------------*/
inline int CIGBaseObject::GetWay(int index)
{
   return mWay[index];
}

inline int CIGBaseObject::GetNextWay()
{
   return GetWay(GetWayLength() - 1);
}

inline void CIGBaseObject::SetWay(int index, int value)
{
   mWay[index] = value;
}

/*---------------------------------------------------------------------------
 description: Way Length
---------------------------------------------------------------------------*/
inline int  CIGBaseObject::GetWayLength()
{
   return mWayLength;
}

inline void CIGBaseObject::SetWayLength(int value)
{
   mWayLength = value;
}

/*---------------------------------------------------------------------------
 description: Next Point
---------------------------------------------------------------------------*/
inline CPPoint& CIGBaseObject::GetNextPoint()
{
   return mNextPoint;
}

inline void CIGBaseObject::SetNextPoint(const CPPoint& value)
{
   mNextPoint = value;
}

/*---------------------------------------------------------------------------
 description: Last Point
---------------------------------------------------------------------------*/
inline CPPoint& CIGBaseObject::GetLastPoint()
{
   return mLastPoint;
}

inline void CIGBaseObject::SetLastPoint(CPPoint &value)
{
   mLastPoint = value;
}

/*---------------------------------------------------------------------------
 description: Speed
---------------------------------------------------------------------------*/
inline CPCounter& CIGBaseObject::GetSpeed()
{
   return mSpeed;
}

inline void CIGBaseObject::SetSpeed(const CPCounter& value)
{
   mSpeed = value;
}

/*---------------------------------------------------------------------------
 description: waiting for
---------------------------------------------------------------------------*/
inline CIGBaseObject * CIGBaseObject::GetWaitingFor ()
{
  return mWaitingFor;
}

inline void CIGBaseObject::SetWaitingFor (CIGBaseObject * value)
{
  mWaitingFor = value;
}

/*---------------------------------------------------------------------------
 description: Moving
---------------------------------------------------------------------------*/
inline bool CIGBaseObject::GetMoving()
{
   return mMoving;
}
inline void CIGBaseObject::SetMoving(bool value)
{
   mMoving = value;
   if (IsInFixedPlace() == true)
      NoteInLogicMap();
   else
      NoteInLogicMap(GetNextPoint() );
}

/*---------------------------------------------------------------------------
 description: Name
---------------------------------------------------------------------------*/
inline CPString& CIGBaseObject::GetName()
{
   return mName;
}

inline void CIGBaseObject::SetName(CPString value)
{
   mName = value;
}

/*---------------------------------------------------------------------------
 description: Dead
---------------------------------------------------------------------------*/
inline bool CIGBaseObject::GetIsDead()
{
   return mIsDead;
}

inline void CIGBaseObject::SetIsDead(bool value)
{
   mIsDead = value;
}
//---------------------------------------------------------------------------
#endif

