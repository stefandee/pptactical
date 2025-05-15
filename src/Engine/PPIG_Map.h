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

 description: Class In Game Map
 last modify: 21 05 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PPIG_MapH
#define PPIG_MapH

#include "PPIG_ObjBase.h"
#include "PPIG_ObjTile.h"
#include "PPIG_ObjList.h"
#include "PP_GuiDef.h"
#include "PP_Graphic.h"
#include "PPIG_EngineDef.h"
#include "PPIG_PathServer.h"
#include "Serializable.h"
#include "XmlSerializable.h"

typedef struct SIGLayers
{
   CIGBaseObjectList *mBaseTile;                   //base tile
   CIGBaseObjectList *mObjects[OBJECT_LAYERS_COUNT];
/*
   CIGObjectList     *mGroundObjects;              //ground objects - statics, infantries, vehicles
   CIGObjectList     *mGroundEffectsObjects;       //ground effects layer ????
   CIGObjectList     *mGroundDamageObjects;        //bullets
   CIGObjectList     *mAirDamageObjects;           //bullets
   CIGObjectList     *mAirObjects;                 //air layer - planes
   CIGObjectList     *mAirEffectsObjects;          //air effects layer  ????????
*/
   int               mAltitude;                     //ground square altitude
}TIGLayers; //type in game layers
//---------------------------------------------------------------------------

class CIGMapView
{
  private:
    int          mId;
    CPList<int>* mBgIdList; // a list of guests sharing this view (usualy, these are battle groups id's)
    int          mBitIndex; // bits index in an element of mVisibilityMap array

  protected:

  public: // c-d
    CIGMapView();
    CIGMapView(int _id);
    ~CIGMapView();

  public: // get-set
    int  GetId()       { return mId; }
    void SetId(int _v) { mId = _v; };

    int  GetBitIndex()       { return mBitIndex; }
    void SetBitIndex(int _v) { mBitIndex = _v; };

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  public: // list
    void AddBgId(int _bgid);
    void RemoveBgId(int _bgid);
    bool Exists(int _bgid);
    int  BgCount() { return mBgIdList->Count(); };
};
//---------------------------------------------------------------------------

class CIGMap : public CSerializable, public Serializable2
{
   friend class CIGPathServer;
   friend class CIGPathAlgorithm;
   public:
      CIGMap();
      ~CIGMap();
      void InitDimensions(int mapWidth, int mapHeight);

      void Update();
      void Paint(CPPoint ptViewport, CPPoint ptWindow, int Width, int Height, int zoomLevel = 1);

      void NoteInLayerMap(int Layer, CIGBaseObject *Object, int X, int Y);
      void RemoveFromLayerMap(int Layer, CIGBaseObject *Object, int X, int Y);
      void CleanTile(int _x, int _y);
      TTileType GetTileType(int _x, int _y);

      void NoteInGroundLogicMap(CIGBaseObject *Object, int X, int Y);
      void RemoveFromGroundLogicMap(CIGBaseObject *Object, int X, int Y);

      virtual void NotifyVisibilityMap(int _bgId, CPPoint* _pList, int _pCount, int _value);
      virtual int  GetVisibility(int _bgId, int _x, int _y);
      virtual bool AnyHasVisibility(int _bgId, int _x, int _y, int _width, int _height, int value);
      virtual void ShareView(int _bgId1, int _bgId2);
      virtual void UnshareView(int _bgId1, int _bgId2);
      virtual void RegisterView(int _bgId);
      virtual void UnregisterView(int _bgId);

      //bool IsGroundObjectOfSide(int Side, int X, int Y);
      //bool IsAirObjectOfSide(int Side, int X, int Y);

      bool IsGroundObjectInBGroup(int _bgId, int X, int Y);
      bool IsAirObjectInBGroup(int _bgId, int X, int Y);
      int  GetGroundObjectBGroup(int _x, int _y);

      bool IsGroundPlaceEmpty(CIGBaseObject *Object, int X, int Y);
      bool IsGroundObjectMoving(CIGBaseObject* Object, int X, int Y);

      // logic coordinates
      CIGBaseObject*     GetGroundObject(int X, int Y);
      CIGBaseObjectList* GetAirObjects(int X, int Y);
      CIGBaseObjectList* GetObjectsAt(int X, int Y);

      // real coordinates/pixel perfect
      CIGBaseObjectList* GetAirObjectsReal(int X, int Y);

   public: // serialization - deserialization
      virtual void Serialize(PP::Stream& _a);
      virtual unsigned __int32 DataSize();
      virtual void DeSerialize(PP::Stream& _a);

    virtual void Serialize(BaseSerializer& out);
    virtual void DeSerialize(BaseDeSerializer& in);

   public: //Get-Set's
      virtual int GetWidth ();
      virtual void SetWidth (int value);

      virtual int GetHeight ();
      virtual void SetHeight (int value);

      virtual int GetLogicWidth ();
      virtual void SetLogicWidth (int value);

      virtual int GetLogicHeight ();
      virtual void SetLogicHeight (int value);

      virtual int  GetAltitude(int i, int j);
      virtual void SetAltitude(int i, int j, int value);

   private:
      TIGLayers           mLayerMap[MAXMAPWIDTH][MAXMAPHEIGHT];
      CIGBaseObject*      mGroundMaskMap[MAXMAPWIDTH][MAXMAPHEIGHT];
      int                 mGroundLogicMap[MAXMAPWIDTH][MAXMAPHEIGHT];

      int                 mWidth;
      int                 mHeight;
      int                 mLogicWidth;
      int                 mLogicHeight;

      int                 mVisibilityMap[MAXMAPWIDTH][MAXMAPHEIGHT]; // Karg: used for fog-of-war
      CPList<CIGMapView>* mMapViews; // Karg: views of the map; every bgroup has one, a view may be shared by several bgroups
      int                 mPlayerIndex;
      CPSprite            *mFogSprite;    //the fog cell sprite

      CIGBaseObjectList*  mInternalList;


      int GenerateUniqueMapViewId(); // :)
      int GetBitIndex(); // Karg: a bit value in the mVisibilityMap
      int SetBitsAtIndex(int _who, int _index, int _value);
      int GetBitsAtIndex(int _who, int _index);

      void MakeInit();
      void MakeClean();
};

/////////////////////////////////////////////////////////////////////////////
//GET-SET (INLINE) SECTION
/////////////////////////////////////////////////////////////////////////////


/*---------------------------------------------------------------------------
 description: Width
---------------------------------------------------------------------------*/
inline int CIGMap::GetWidth ()
{
  return mWidth;
}

inline void CIGMap::SetWidth (int value)
{
  mWidth = value;
}

/*---------------------------------------------------------------------------
 description: Height
---------------------------------------------------------------------------*/
inline int CIGMap::GetHeight ()
{
  return mHeight;
}

inline void CIGMap::SetHeight (int value)
{
  mHeight = value;
}

/*---------------------------------------------------------------------------
 description: LogicWidth
---------------------------------------------------------------------------*/
inline int CIGMap::GetLogicWidth ()
{
  return mLogicWidth;
}

inline void CIGMap::SetLogicWidth (int value)
{
  mLogicWidth = value;
}

/*---------------------------------------------------------------------------
 description: LogicHeight
---------------------------------------------------------------------------*/
inline int CIGMap::GetLogicHeight ()
{
  return mLogicHeight;
}

inline void CIGMap::SetLogicHeight (int value)
{
  mLogicHeight = value;
}

/*---------------------------------------------------------------------------
 description: Altitude
---------------------------------------------------------------------------*/
inline int CIGMap::GetAltitude(int i, int j)
{
   if ((i < 0) || (i >= GetLogicWidth()))
   {
     return -1;
   }

   if ((j < 0) || (j >= GetLogicHeight()))
   {
     return -1;
   }

   return mLayerMap[i][j].mAltitude;
}

inline void CIGMap::SetAltitude(int i, int j, int value)
{
   if ((i < 0) || (i >= GetLogicWidth()))
   {
     return;
   }

   if ((j < 0) || (j >= GetLogicHeight()))
   {
     return;
   }

   mLayerMap[i][j].mAltitude = value;
}

//---------------------------------------------------------------------------
#endif

