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
 application: Pure Power - in game

 description: Class Mission
              it manages all the objects and provides interface for user
              commands; a god class that begs for refactoring
 last modify: 10 06 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PPIG_MissionH
#define PPIG_MissionH

#include "PP_GuiDef.h"
#include "PPIG_EngineDef.h"
#include "PP_Counter.h"
#include "Serializable.h"
#include "XmlSerializable.h"
#include "PPIG_ObjList.h"

// fwd. decl.
class CIGMap;
class CIGBaseObject;
class CPPIG_MissionControl;
class CPPIG_Doctrine;
class CPString;
class CPPIG_BGManager;
class CIGObjectStimul;
class CIGTransportRules;

class CIGMission : public CSerializable, public Serializable2
{
   public:
      class CErrorGeneric {};

      static CIGMission *Instance();
      static void Release();

   protected:
      static CIGMission *mInstance;
      CIGMission();
      ~CIGMission();

   public:
      void Update();
      void Paint(CPPoint ptViewport, CPPoint ptWindow, int Width, int Height, int zoomLevel = 1);
      void PaintMinimized(CPPoint ptViewport, CPPoint ptWindow, int Width, int Height, int zoomLevel = 1);
      //void LoadFromFile(CPString fileName);
      //void SaveToFile(CPString fileName);
      //void LoadDroppedTeam();

      void MakeSelection();
      void CancelSelection();
      void AddSelection();
      void AddRemoveSelection();
      void SetSelectionToTeam(int Team);
      void GetSelectionFromTeam(int Team);
      void AddSelectionFromTeam(int Team);
      void AddRemoveSelectionFromTeam(int Team);
      void RemoveFromSelection(CIGBaseObject *Object);

      void CreateTargetEffect();

      void NoteInLayerMap(int Layer, CIGBaseObject *Object, int X, int Y);
      void RemoveFromLayerMap(int Layer, CIGBaseObject *Object, int X, int Y);
      void NoteInGroundLogicMap(CIGBaseObject *Object, int X, int Y);
      void RemoveFromGroundLogicMap(CIGBaseObject *Object, int X, int Y);

      //bool IsGroundObjectOfSide(char Side, int X, int Y);
      bool IsGroundObjectInBGroup(int _bgId, int X, int Y);
      int  GetGroundObjectBGroup(int _x, int _y);
      TPoliticsRelations GetGroundObjectsRelation(int _bgId, int X, int Y);
      bool IsGroundPlaceEmpty(CIGBaseObject* Object, int X, int Y);
      bool IsGroundObjectMoving(CIGBaseObject* Object, int X, int Y);
      CIGBaseObject* GetObjectAtReal(int X, int Y);
      CIGBaseObjectList* GetObjectInRectReal(int X, int Y, int Width, int Height);

      void NoteGroundStimul(CIGBaseObject *Sender, CIGObjectStimul& Stimul, int X, int Y);
      void NoteAirStimul(CIGBaseObject *Sender, CIGObjectStimul& Stimul, int X, int Y, int Z);

      int GetObjectsActionMask();
      bool CheckOwnedActionMask(int action);

      CPString GetObjectHint();

      int GetMissionTime() { return mMissionTime; };


   public: // focusing
      void SetFocusObject(CIGBaseObject* _obj);
      CIGBaseObject* GetFocusObject() { return mFocusObj; }
      void ActionFocusOnObject();

   public: // serialization - deserialization
      virtual void Serialize(PP::Stream& _a);
      virtual unsigned __int32 DataSize();
      virtual void DeSerialize(PP::Stream& _a);

      virtual void Serialize(BaseSerializer& out);
      virtual void DeSerialize(BaseDeSerializer& in);

   public: //get - set and related
      bool GetEnabled();
      void SetEnabled(bool value);

      bool GetFlag(int flag);
      void SetFlag(int flag, bool value);
      void InvertFlag(int flag);

      CPCounter&  GetUpdateCounter();
      void SetUpdateCounter(CPCounter& value);
      void SetUpdateCounter(int value);
      void SetUpdateCounterMaxValue(int value);

      int  GetSelectionMask();
      void SetSelectionMask(int value);
      void AddSelectionMask(int value);

      CIGBaseObjectList *GetObjects(int Index);
      void SetObjects(int Index, CIGBaseObjectList *value);
      CIGBaseObject* GetObjectById(int _id);

      int  GetUniqueUnitId();
      bool UnitIdUnique(int _id);

      CIGBaseObjectList *GetToDeleteObjects();
      void SetToDeleteObjects(CIGBaseObjectList *value);
      void AddToDeleteObjects(CIGBaseObject *Object);

      int  GetCurrentAction();
      void SetCurrentAction(int value);

      int  GetAction();
      void SetAction(int value);
      void CancelAction(); //equals to SetAction(MA_CANCEL)
      void ResetAction();

      CPPoint GetBeginPoint();
      void SetBeginPoint(CPPoint value);

      CPPoint GetCurrentPoint();
      void SetCurrentPoint(CPPoint value);

      CPPoint GetEndPoint();
      void SetEndPoint(CPPoint value);

      CPString GetMissionName() { return mMissionName; }
      void     SetMissionName(CPString _v) { mMissionName = _v; }

      CPString GetMissionInfo() { return mMissionInfo; }
      void     SetMissionInfo(CPString _v) { mMissionInfo = _v; }

      bool GetSelectNeutralUnits() { return mSelectNeutralUnits; }
      void SetSelectNeutralUnits(bool v) { mSelectNeutralUnits = v; }

      bool GetNeutralUnitHint() { return mNeutralUnitHint; }
      void SetNeutralUnitHint(bool v) { mNeutralUnitHint = v; }

   public: // internals (read-only)
      CIGMap *GetMap();
      void SetMap(CIGMap *value);

      CPPIG_BGManager* GetBGManager() { return mBGManager; };

      CPPIG_Doctrine* GetDoctrine() { return mDoctrine; }

      CPPIG_MissionControl* GetMissionControl() { return mMissionControl; }
      //void SetMissionControl(CPPIG_MissionControl*  _v) { mMissionControl = _v; }

   public: // used sprites interface

   protected:
      void ComputeCurrentAction();

      void ActionMove();   //not ready
      void ActionStop();   //not ready
      void ActionAttack(); //not ready
      void ActionPatrol(); //not ready
      void ActionGuard();  //not ready
      void ActionHold();   //not ready
      void ActionCapture();//not ready
      void ActionLoad();
      void ActionUnload();
      void ActionUnloadAll();
      void ActionSuppressFire();
      void ActionTakeCover();
      void ActionLand();
      void ActionTakeoff();
      void ActionBuild();
      void ActionRallyPoint();

   private:
      CPString       mMissionName, mMissionInfo;

      bool           mEnabled;         //enables/disables the mission update
      bool           mFlags[MISSION_FLAGS_COUNT];
                                       //flags

      CPCounter      mUpdateCounter;   //current update counter

      int            mSelectionMask;   //the mask of the selected objects
      CIGBaseObjectList *mObjects[MISSION_OBJECT_LISTS_COUNT];
                                       //list of all the objects
      CIGBaseObjectList *mToDeleteObjects;
                                       //list of objects to be deleted
      CIGMap        *mMap;             //map instance

      int            mAction;          //this is the wanted action (this is public)
      int            mCurrentAction;   //this is the current action (this is queried by the interface)
                                       //eg. Action is not necessarily the same as
                                       //CurrentAction : Action = MOVE and if on enemy
                                       //object CurrentAction = ATTACK
      CPPoint        mBeginPoint;      //begin point (for action) in real coord
      CPPoint        mCurrentPoint;    //current point in real coord
      CPPoint        mEndPoint;        //endpoint (for selection) in real coord

      int            mTicks, mMissionTime;

      CPPIG_MissionControl* mMissionControl; // script that controls how the mission goes
      CPPIG_Doctrine*       mDoctrine;       // collection of scripts
      CPPIG_BGManager*      mBGManager;
      //CPList<CPGISprite>*   mUsedSprites;    // these are the sprites used by all the objects
                                             // managed by the mission (map, units related)
                                             // this list is used at serialization/deserialization

      bool mNeutralUnitHint; /**< controls if neutral units produce a hint when mouse over */

      bool mSelectNeutralUnits; /**< controls if neutral units can be selected */

      // the object the camera has the focus on
      CIGBaseObject* mFocusObj;

      std::map<int, int> mValidLayers; /**< map of layers, ordered by priority, used for selecting items */

      CIGTransportRules* mTransportRules;
};

CIGMission *GetMissionInstance();

/////////////////////////////////////////////////////////////////////////////
//GET-SET and related stuff (INLINE) SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: Enabled
---------------------------------------------------------------------------*/
inline bool CIGMission::GetEnabled()
{
   return mEnabled;
}

inline void CIGMission::SetEnabled(bool value)
{
   mEnabled = value;
}

/*---------------------------------------------------------------------------
 description: Flag
---------------------------------------------------------------------------*/
inline bool CIGMission::GetFlag(int flag)
{
   return mFlags[flag];
}
inline void CIGMission::SetFlag(int flag, bool value)
{
   mFlags[flag] = value;
}
inline void CIGMission::InvertFlag(int flag)
{
   mFlags[flag] = !mFlags[flag];
}

/*---------------------------------------------------------------------------
 description: UpdateCounter
---------------------------------------------------------------------------*/
inline CPCounter& CIGMission::GetUpdateCounter()
{
   return mUpdateCounter;
}
inline void CIGMission::SetUpdateCounter(int value)
{
   mUpdateCounter = value;
}
inline void CIGMission::SetUpdateCounter(CPCounter& value)
{
   mUpdateCounter = value;
}

inline void CIGMission::SetUpdateCounterMaxValue(int value)
{
   GetUpdateCounter().SetMaxValue(value);
}

/*---------------------------------------------------------------------------
 description: Selection Mask
---------------------------------------------------------------------------*/
inline int CIGMission::GetSelectionMask()
{
   return mSelectionMask;
}
inline void CIGMission::SetSelectionMask(int value)
{
   mSelectionMask = value;
}

inline void CIGMission::AddSelectionMask(int value)
{
   mSelectionMask = mSelectionMask | value;
}

/*---------------------------------------------------------------------------
 description: Objects
---------------------------------------------------------------------------*/
inline CIGBaseObjectList *CIGMission::GetObjects(int Index)
{
   return mObjects[Index];
}
inline void CIGMission::SetObjects(int Index, CIGBaseObjectList *value)
{
   mObjects[Index] = value;
}

/*---------------------------------------------------------------------------
 description: ToDeleteObjects
---------------------------------------------------------------------------*/
inline CIGBaseObjectList *CIGMission::GetToDeleteObjects()
{
   return mToDeleteObjects;
}

inline void CIGMission::SetToDeleteObjects(CIGBaseObjectList *value)
{
   mToDeleteObjects = value;

   // as the object is about to be deleted, null the focus object reference
   for (int i=0; i<MISSION_OBJECT_LISTS_COUNT; i++)
   {
      if (GetObjects(i)->GetObject(i) == mFocusObj)
      {
        mFocusObj = 0;
        break;
      }
   }
}

inline void CIGMission::AddToDeleteObjects(CIGBaseObject *Object)
{
   // as the object is about to be deleted, null the focus object reference
   if (Object == mFocusObj)
   {
     mFocusObj = 0;
   }

   for (int i=0; i<MISSION_OBJECT_LISTS_COUNT; i++)
      GetObjects(i)->Remove(Object);

   GetToDeleteObjects()->Add(Object);
}

/*---------------------------------------------------------------------------
 description: Map
---------------------------------------------------------------------------*/
inline CIGMap *CIGMission::GetMap()
{
   return mMap;
}
inline void CIGMission::SetMap(CIGMap *value)
{
   mMap = value;
}

/*---------------------------------------------------------------------------
 description: CurrentAction
---------------------------------------------------------------------------*/
inline int CIGMission::GetCurrentAction()
{
   return mCurrentAction;
}
inline void CIGMission::SetCurrentAction(int value)
{
   mCurrentAction = value;
}

/*---------------------------------------------------------------------------
 description: Action
---------------------------------------------------------------------------*/
inline int CIGMission::GetAction()
{
   return mAction;
}

inline void CIGMission::CancelAction()
{
   if (GetAction() == MA_DEFAULT)
      CancelSelection();

   // to prevent recursive behaviour involving SetAction
   mAction = MA_DEFAULT;
   //SetAction(MA_DEFAULT);

   SetCurrentAction(MA_DEFAULT);
   ComputeCurrentAction();
}

inline void CIGMission::ResetAction()
{
   SetAction(MA_DEFAULT);
   SetCurrentAction(MA_DEFAULT);
   ComputeCurrentAction();
}

/*---------------------------------------------------------------------------
 description: BeginPoint
---------------------------------------------------------------------------*/
inline CPPoint CIGMission::GetBeginPoint()
{
   return mBeginPoint;
}


/*---------------------------------------------------------------------------
 description: CurrentPoint
---------------------------------------------------------------------------*/
inline CPPoint CIGMission::GetCurrentPoint()
{
   return mCurrentPoint;
}

inline void CIGMission::SetCurrentPoint(CPPoint value)
{
   mCurrentPoint = value;
   ComputeCurrentAction();
}

/*---------------------------------------------------------------------------
 description: EndPoint
---------------------------------------------------------------------------*/
inline CPPoint CIGMission::GetEndPoint()
{
   return mEndPoint;
}

inline void CIGMission::SetEndPoint(CPPoint value)
{
   mEndPoint = value;
   SetCurrentPoint(value);
   switch(GetCurrentAction())
   {
      case MA_SELECTING:
         MakeSelection();
         break;
   }
   ResetAction();
//   SetCurrentAction(MA_DEFAULT);
}
//---------------------------------------------------------------------------

#endif

