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
#pragma hdrstop

#include <algorithm>
using namespace std;

#include "PPIG_Map.h"
#include "basepath.h"
#include "Paths.h"
#include "PP_Stdlib.h"

#ifdef _MSC_VER
#define for if(0) {} else for
#endif

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CIGMap::CIGMap()
{
    MakeInit();
    mFogSprite = GetGraphicInstance()->AddSprite(CPString(PATH_INGAME_GFX) + "fogcell.png", 1, 1, SPRL_GENERIC);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CIGMap::~CIGMap()
{
    MakeClean();
}
//---------------------------------------------------------------------------

void CIGMap::MakeInit()
{
    for (int i = 0; i < MAXMAPWIDTH; i++)
    {
        for (int j = 0; j < MAXMAPHEIGHT; j++)
        {
            mGroundLogicMap[i][j]     = 0;
            mGroundMaskMap[i][j]      = NULL;
            mLayerMap[i][j].mBaseTile = 0;
            for (int k = 0; k<OBJECT_LAYERS_COUNT; k++)
            {
                //mLayerMap[i][j].mObjects[k] = new CIGBaseObjectList();
                mLayerMap[i][j].mObjects[k] = 0;
            }

            mLayerMap[i][j].mAltitude = 0;

            mVisibilityMap[i][j] = 0; // Karg: fog-of-war
        }
    }

    mMapViews = new CPList<CIGMapView>(4);

    mInternalList = new CIGBaseObjectList();
}
//---------------------------------------------------------------------------

void CIGMap::MakeClean()
{
    delete mInternalList;

    delete mMapViews;

    VLOG(9) << "Deleting map[" << MAXMAPWIDTH << "," << MAXMAPHEIGHT << "] with " << OBJECT_LAYERS_COUNT << " layers";

    // TMN: Yep, this is ugly, but we need to make sure we don't try to
    // delete a CIGBaseObject twice. We should probably ref-count it, but
    // this is a quick (and ugly) fix in the meantime.
    /*
    CIGBaseObject* rgObjects[MAXMAPWIDTH][MAXMAPHEIGHT];
    for (int x=0; x<MAXMAPWIDTH; ++x) {
      for (int y=0; y<MAXMAPHEIGHT; ++y) {
         rgObjects[x][y] = mLayerMap[x][y].mBaseTile;
      }
    }
    CIGBaseObject** pBegin = &rgObjects[0][0];
    CIGBaseObject** pEnd   = &rgObjects[0][0] + MAXMAPWIDTH*MAXMAPHEIGHT;
    std::sort(pBegin, pEnd);
    CIGBaseObject** pEnd2 = std::unique(pBegin, pEnd);
    for (CIGBaseObject** p = pBegin; p != pEnd2; ++p)
    {
      if (!*p)
      {
         continue;
      }
      try
      {
         delete *p;
      }
      catch(...)
      {
         logWriteLn("Failed to delete basetile.");
      }
    }
    */

    // Karg: TMN did a very smart hack; an alternative would be like this:
    CPList<CIGBaseObjectList> *mTileList;

    try
    {
        mTileList = new CPList<CIGBaseObjectList>((MAXMAPWIDTH + 1) * (MAXMAPHEIGHT + 1) / 9);
    }
    catch(...)
    {
        LOG(ERROR) << "CIGMap::~CIGMap - Failed to alloc base tile list, aborting deletion.";
        return;
    }

    for (int x=0; x<MAXMAPWIDTH; ++x)
    {
        for (int y=0; y<MAXMAPHEIGHT; ++y)
        {
            if (mLayerMap[x][y].mBaseTile != NULL)
            {
                if (!mTileList->Contain(mLayerMap[x][y].mBaseTile))
                {
                    mTileList->Add(mLayerMap[x][y].mBaseTile);

                    // Karg - failure to repeately save
                    mLayerMap[x][y].mBaseTile->RemoveAll();
                }
            }
        }
    }

    VLOG(9) << "Tiles to delete: " << mTileList->Count();

    while (mTileList->Count() > 0)
    {
        // delete elements from the lists; this may be list with one element - basetile -
        // (water or land) or a list with two elements (water and shore)
        while (mTileList[0].Count() > 0)
        {
            mTileList[0].Delete(0);
        }

        // now delete the list
        mTileList->Delete(0);
    }

    delete mTileList;

    // delete the other layers
    for (int i=0; i<MAXMAPWIDTH; i++)
    {
        for (int j=0; j<MAXMAPHEIGHT; j++)
        {
            //logWrite("Deleting lists in layers "); logWriteI(i); logWrite(","); logWriteILn(j);
            for (int k=0; k<OBJECT_LAYERS_COUNT; k++)
            {
                //logWrite("Object: "); logWriteILn(k);
                try
                {
                    delete mLayerMap[i][j].mObjects[k];
                }
                catch(...)
                {
                    LOG(ERROR) << "Failed to delete.";
                }
            }
        }
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sets the dimensions of the map
 parameters : mapWidth, mapHeight - real dimensions
 others     : it is called only once
---------------------------------------------------------------------------*/
void CIGMap::InitDimensions(int mapWidth, int mapHeight)
{
    SetWidth(mapWidth);
    SetHeight(mapHeight);
    SetLogicWidth(mapWidth/MAPCELLX);
    SetLogicHeight(mapHeight/MAPCELLY);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the tick event
---------------------------------------------------------------------------*/
void CIGMap::Update()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the map
 parameters : ptViewport - the zone of the screen where the paint is wanted
              rcWindow - the visible portion of the map
              zoomLevel - is 1 by default
---------------------------------------------------------------------------*/
void CIGMap::Paint(CPPoint ptViewport, CPPoint ptWindow, int Width, int Height, int zoomLevel)
{
    int i,j,k;
    int lLogicLeft, lLogicTop, lLogicWidth, lLogicHeight;
    CIGBaseObjectList *lTmpList = new CIGBaseObjectList();

    VLOG(5) << "CIGMap::Paint - begin ...";
    lLogicLeft   = max((long)0, (long)(ptWindow.x / MAPCELLX - 5));
    lLogicTop    = min((long)0, (long)(ptWindow.y / MAPCELLY));
    // TODO: what the hell?! lLogicTop    = max((long)0, (long)(ptWindow.y / MAPCELLY));

    lLogicWidth  = min(Width  / MAPCELLX + 20, GetLogicWidth() - lLogicLeft);
    lLogicHeight = max(Height / MAPCELLY + 12, GetLogicHeight()- lLogicTop);
    //lLogicHeight = min(Height / MAPCELLY + 12, GetLogicHeight()- lLogicTop);

    //Base Tile Layer
    VLOG(5) << "CIGMap::Paint - base tile paint begins...";
    for (i=lLogicLeft; i<lLogicLeft + lLogicWidth; i++)
    {
        for (j=lLogicTop; j<lLogicTop + lLogicHeight; j++)
        {
            CIGBaseObjectList *mBaseTileList = mLayerMap[i][j].mBaseTile;

            if (mBaseTileList != NULL)
            {
                int baseTileCount = mBaseTileList->Count();
                for(k = 0; k < baseTileCount; k++)
                {
                    mBaseTileList->GetObject(k)->Paint(ptViewport, ptWindow, Width, Height, zoomLevel);
                }
            }
        }
    }
    VLOG(5) << "CIGMap::Paint - base tile paint over";
    //Base Tile Layer - over

    //Objects Layers
    VLOG(5) << "CIGMap::Paint - object paint begins...";
    for (k = 0; k < OBJECT_LAYERS_COUNT; k++)
    {
// was modified by Karg - changed TList to KList     lTmpList->Clear();
        lTmpList->RemoveAll();

        int topaint = 0;

        for (j=lLogicTop + lLogicHeight - 1; j>=lLogicTop; j--)
        {
            for (i=lLogicLeft; i<lLogicLeft + lLogicWidth; i++)
            {
                if (mLayerMap[i][j].mObjects[k] && mLayerMap[i][j].mObjects[k]->IsEmpty() == false)
                {
                    topaint++;
                    lTmpList->AddUnique(mLayerMap[i][j].mObjects[k]);
                }
            }
        }

        lTmpList->Paint(ptViewport, ptWindow, Width, Height, zoomLevel, NULL);

        // paint over the visibility map
        if (k == OBJECT_LAYERS_COUNT - 1)
        {
            for (j=lLogicTop + lLogicHeight - 1; j>=lLogicTop; j--)
            {
                for (i=lLogicLeft; i<lLogicLeft + lLogicWidth; i++)
                {
                    if (GetBitsAtIndex(mVisibilityMap[i][j], mPlayerIndex) == 0)
                    {
                        if (mFogSprite)
                        {
                            GetGraphicInstance()->PaintSprite(
                                    i * MAPCELLX + ptViewport.x - ptWindow.x - 16,
                                    j * MAPCELLY + ptViewport.y - ptWindow.y - 16,
                                    mFogSprite, 0, 0);
                        }
                    }
                }
            }
        }
    }

    //
    // test code - paint the mask map
    // use it to see if the vehicles dont left any artifact while moving
    //
    /*
    for (i=lLogicLeft; i<lLogicLeft + lLogicWidth; i++)
    {
       for (j=lLogicTop; j<lLogicTop + lLogicHeight; j++)
       {
         if (mGroundLogicMap[i][j])
         {
           int lX = i * MAPCELLX + ptViewport.x - ptWindow.x;
           int lY = j * MAPCELLY + ptViewport.y - ptWindow.y;

           GetGraphicInstance()->FillRectangle(0, CPRect(lX, lY, lX + 32, lY + 32), 0xFF0000);
         }
       }
    }
    */

    VLOG(5) << "CIGMap::Paint - object paint over";
    //Objects Layers - over
// was modified by Karg - changed TList to KList     lTmpList->Clear();
    lTmpList->RemoveAll();
    delete lTmpList;

    /*
    for(int x = 0; x < 50; x++)
    {
      for(int y = 0; y < 50; y++)
        logWriteI(mVisibilityMap[x][y]);
      logWriteLn(" ");
    }
    */

    VLOG(5) << "CIGMap::Paint - over";
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: notes an object to the specified layer (in the layer map)
 parameters : Layer  - the wanted layer
              Object - the object to be noted
              X, Y   - logic coordonates
---------------------------------------------------------------------------*/
void CIGMap::NoteInLayerMap(int Layer, CIGBaseObject *Object, int X, int Y)
{
    if (X<0 || Y<0 || X>=GetLogicWidth() || Y>= GetLogicHeight())
    {
        return;
    }

    switch(Layer)
    {
    case MOL_BASETILE:
        if (!mLayerMap[X][Y].mBaseTile)
        {
            mLayerMap[X][Y].mBaseTile = new CIGBaseObjectList;
        }
        mLayerMap[X][Y].mBaseTile->Add(Object);
        break;

    default:
        //logWrite("Adding: "); logWriteI(Object->GetId()); logWrite(" at "); logWriteI(X); logWrite(","); logWriteILn(Y);

        if (!mLayerMap[X][Y].mObjects[Layer])
        {
            mLayerMap[X][Y].mObjects[Layer] = new CIGBaseObjectList;
        }
        mLayerMap[X][Y].mObjects[Layer]->Add(Object);
        break;
    }

    /*
    if (X<0 || Y<0 || X>=GetLogicWidth() || Y>= GetLogicHeight())
       return;
    switch(Layer)
    {
       case MOL_BASETILE:
          mLayerMap[X][Y].mBaseTile = Object;
          break;
       default:
          mLayerMap[X][Y].mObjects[Layer]->Add(Object);
          break;
    }
    */
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: removes an object from the specified layer (in the layer map) (if it exists)
 parameters : Layer  - the wanted layer
              Object - the object to be noted
              X, Y   - logic coordonates
---------------------------------------------------------------------------*/
void CIGMap::RemoveFromLayerMap(int Layer, CIGBaseObject *Object, int X, int Y)
{
    if (X<0 || Y<0 || X>=GetLogicWidth() || Y>= GetLogicHeight())
    {
        return;
    }

    // Karg: fool-proof?
    if (!mLayerMap[X][Y].mObjects[Layer])
    {
        return;
    }

    //logWrite("Removing: "); logWriteI(Object->GetId()); logWrite(" at "); logWriteI(X); logWrite(","); logWriteILn(Y);

    CIGBaseObjectList*& objList = mLayerMap[X][Y].mObjects[Layer];
    objList->Remove(Object);
    if (objList->IsEmpty())
    {
        delete objList;
        objList = 0;
    }

    /*
    if (X<0 || Y<0 || X>=GetLogicWidth() || Y>= GetLogicHeight())
       return;
    mLayerMap[X][Y].mObjects[Layer]->Remove(Object);
    */
}
//---------------------------------------------------------------------------

void CIGMap::CleanTile(int _x, int _y)
{
    if (_x < 0 || _y <0 || _x >= GetLogicWidth() || _y >= GetLogicHeight())
    {
        return;
    }

    // fool-proof
    if (!mLayerMap[_x][_y].mBaseTile)
    {
        return;
    }

    mLayerMap[_x][_y].mBaseTile->FreeAll();
}
//---------------------------------------------------------------------------

TTileType CIGMap::GetTileType(int _x, int _y)
{
    if (_x < 0 || _y <0 || _x >= GetLogicWidth() || _y >= GetLogicHeight())
    {
        return MTT_UNKNOWN;
    }

    // fool-proof
    /*
    if (!mLayerMap[_x][_y].mBaseTile)
    {
      return MTT_UNKNOWN;
    }
    */

    // this can't happen normally
    /*
    if (mLayerMap[_x][_y].mBaseTile->Count() <= 0)
    {
      logWriteLn("CIGMap::GetTileType - error in map.");
      return MTT_UNKNOWN;
    }
    */

    // it is supposed that in a certain location we can have these situations
    // (when a map is correctly constructed):
    // 1. a water tile
    // 2. a ground tile
    // 3. a water and a shore tile
    // 4. a shore
    // in all cases the first object (0) in the list holds the definitory tile type

    // search the locations to the left and up for the tile that contains the
    // wanted location (_x, _y)

    // this is because tiles do not cover in layer map all locations - otherwise
    // it would hinder the paint and since paint is a time consuming operation,
    // it is better to make this case a little bit slower

    int lMinX = max(0, _x - 5);
    int lMinY = max(0, _y - 5);

    for (int lX = _x; lX >= lMinX; lX--)
    {
        for (int lY = _y; lY >= lMinY; lY--)
        {
            if (mLayerMap[lX][lY].mBaseTile)
            {
                // well, we found a location that contains a tile; check if it
                // covers the location (_x, _y)

                // this can't happen normally
                if (mLayerMap[lX][lY].mBaseTile->Count() <= 0)
                {
                    LOG(FATAL) << "CIGMap::GetTileType - error in map.";
                    return MTT_UNKNOWN;
                }

                CIGBaseTile* lBaseTile = (CIGBaseTile*)(mLayerMap[lX][lY].mBaseTile->GetObject(0));

                if (lX >= lBaseTile->GetLogicCoord().x && lX <= lBaseTile->GetLogicCoord().x + lBaseTile->GetLogicDimX() &&
                        lY >= lBaseTile->GetLogicCoord().y && lY <= lBaseTile->GetLogicCoord().y + lBaseTile->GetLogicDimY())
                {
                    return lBaseTile->GetType();
                }
            }
        }
    }

    //CIGBaseTile* lBaseTile = (CIGBaseTile*)(mLayerMap[_x][_y].mBaseTile->GetObject(0));

    //return lBaseTile->GetType();
    return MTT_UNKNOWN;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: notes an object to the logic map
 parameters : Object - the object to be noted
              X, Y   - logic coordonates
---------------------------------------------------------------------------*/
void CIGMap::NoteInGroundLogicMap(CIGBaseObject *Object, int X, int Y)
{
    if (X<0 || Y<0 || X>=GetLogicWidth() || Y>=GetLogicHeight())
        return;
    if (Object != NULL)
    {
        mGroundMaskMap[X][Y] = Object;
        if (Object->GetMoving() == true)
            mGroundLogicMap[X][Y] = -5; // MOVELOCK was -1
        else
            mGroundLogicMap[X][Y] = -5;
    }

    /*
    if (Object != NULL)
    {
       mGroundMaskMap[X][Y] = Object;
       if (Object->GetMoving() == true)
          mGroundLogicMap[X][Y] = -1; // MOVELOCK was -1
       else
          mGroundLogicMap[X][Y] = -5;
    }
    */
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: removes an object from the logic map (if it exists)
 parameters : Object - the object to be noted
              X, Y   - logic coordonates
---------------------------------------------------------------------------*/
void CIGMap::RemoveFromGroundLogicMap(CIGBaseObject *Object, int X, int Y)
{
    if (X<0 || Y<0 || X>=GetLogicWidth() || Y>= GetLogicHeight())
        return;
    if (Object != NULL)
    {
        if (mGroundMaskMap[X][Y] == Object)
        {
            mGroundMaskMap[X][Y] = 0;//NULL;
            mGroundLogicMap[X][Y] = 0;
        }
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the ground object in X,Y is of Side
 parameters : Side - the queried side
              X, Y - the logic coord of the object
 others     : it only looks in the logic map
---------------------------------------------------------------------------*/
bool CIGMap::IsGroundObjectInBGroup(int _bgId, int X, int Y)
{
    if (X<0 || Y<0 || X>=GetLogicWidth() || Y>= GetLogicHeight())
        return false;
    if (mGroundMaskMap[X][Y] != NULL)
        return (mGroundMaskMap[X][Y]->GetBattleGroupId() == _bgId);
    return false;
}
//---------------------------------------------------------------------------
/*
bool CIGMap::IsGroundObjectOfSide(int Side, int X, int Y)
{
   if (X<0 || Y<0 || X>=GetLogicWidth() || Y>= GetLogicHeight())
      return false;
   if (mGroundMaskMap[X][Y] != NULL)
      return (mGroundMaskMap[X][Y]->GetSide() == Side);
   return false;
}
//---------------------------------------------------------------------------
*/

/*---------------------------------------------------------------------------
 description: returns true if the ground object in X,Y is of Side
 parameters : Side - the queried side
              X, Y - the logic coord of the object
 others     : it only looks in the logic map
---------------------------------------------------------------------------*/
bool CIGMap::IsAirObjectInBGroup(int _bgId, int X, int Y)
{
    if (X<0 || Y<0 || X>=GetLogicWidth() || Y>= GetLogicHeight())
        return false;
    return false;
}
//---------------------------------------------------------------------------

int  CIGMap::GetGroundObjectBGroup(int _x, int _y)
{
    if (_x < 0 || _y < 0 || _x >= GetLogicWidth() || _y >= GetLogicHeight())
    {
        return -1;
    }

    if (mGroundMaskMap[_x][_y] != NULL)
    {
        return mGroundMaskMap[_x][_y]->GetBattleGroupId();
    }

    return -1;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the place is empty or is the specified object
 parameters : Object - the ignored object
              X, Y   - the logic coord of the queried place
---------------------------------------------------------------------------*/
bool CIGMap::IsGroundPlaceEmpty(CIGBaseObject* Object, int X, int Y)
{
    if (X<0 || Y<0 || X>=GetLogicWidth() || Y>= GetLogicHeight())
        return false;
    if (mGroundMaskMap[X][Y] != NULL)
        return (mGroundMaskMap[X][Y] == Object);
    return true;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the object in X,Y is moving
 parameters : Object - the ignored object
              X, Y   - the logic coord of the queried place
---------------------------------------------------------------------------*/
bool CIGMap::IsGroundObjectMoving(CIGBaseObject* Object, int X, int Y)
{
    if (X<0 || Y<0 || X>=GetLogicWidth() || Y>= GetLogicHeight())
        return false;
    if (mGroundMaskMap[X][Y] != NULL)
        return ((mGroundMaskMap[X][Y] == Object) ||
                (mGroundMaskMap[X][Y]->GetMoving() == true));
    return false;
}
//---------------------------------------------------------------------------


/*---------------------------------------------------------------------------
 description: gets the object from the GroundMaskMap
 parameters : X, Y - logic coord
---------------------------------------------------------------------------*/
CIGBaseObject *CIGMap::GetGroundObject(int X, int Y)
{
    if (X<0 || Y<0 || X>=GetLogicWidth() || Y>= GetLogicHeight())
        return NULL;
    return mGroundMaskMap[X][Y];
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets a list of all air units from the layermap
 parameters : X, Y - logic coord
---------------------------------------------------------------------------*/
CIGBaseObjectList *CIGMap::GetAirObjects(int X, int Y)
{
    if (X<0 || Y<0 || X>=GetLogicWidth() || Y>= GetLogicHeight())
    {
        return 0;
    }

    return mLayerMap[X][Y].mObjects[MOL_AIROBJECTS];
}
//---------------------------------------------------------------------------

// Description : List of all units in the air, pixel-perfect
// Param       : X, Y - coordinates as real (logic * MAPCELLSIZE)
// Result      :
// Comments    : to be tested, may be slow if many units
CIGBaseObjectList* CIGMap::GetAirObjectsReal(int X, int Y)
{
    int logicX = X / MAPCELLX;
    int logicY = Y / MAPCELLY;

    if (logicX<0 || logicY<0 || logicX>=GetLogicWidth() || logicY>= GetLogicHeight())
    {
        return 0;
    }

    mInternalList->RemoveAll();

    // TODO: parse all the map - this is a generic approach
    // a locality query (quad trees, whatever) would have been more appropriate
    for(int x = 0; x < GetLogicWidth(); x++)
    {
        for(int y = 0; y < GetLogicHeight(); y++)
        {
            CIGBaseObjectList* list = mLayerMap[x][y].mObjects[MOL_AIROBJECTS];

            if (!list)
            {
                continue;
            }

            // process the list and check if the point (X, Y) is "inside" the unit
            for(int i = 0; i < list->Count(); i++)
            {
                if (list->GetObject(i)->Pick(CPPoint(X, Y)))
                {
                    // add to the internal&temp list
                    mInternalList->Add(list->GetObject(i));
                }
            }
        }
    }

    return mInternalList;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets a list of all units from the layermap at the specified point
 parameters : X, Y - logic coord
---------------------------------------------------------------------------*/
CIGBaseObjectList* CIGMap::GetObjectsAt(int X, int Y)
{
    if (X<0 || Y<0 || X>=GetLogicWidth() || Y>= GetLogicHeight())
    {
        return 0;
    }

    CIGBaseObjectList* lResult = new CIGBaseObjectList();

    if (mLayerMap[X][Y].mObjects[MOL_AIROBJECTS])
    {
        lResult->AddUnique(mLayerMap[X][Y].mObjects[MOL_AIROBJECTS]);
    }

    if (mGroundMaskMap[X][Y])
    {
        lResult->Add(mGroundMaskMap[X][Y]);
    }

    return lResult;
}
//---------------------------------------------------------------------------

void CIGMap::NotifyVisibilityMap(int _bgId, CPPoint* _pList, int _pCount, int _value)
{
    bool lFound = false;
    int  lBitIndex;

    // caut view'ul de care apartine _bgId
    for(int i = 0; i < mMapViews->Count(); i++)
    {
        if (mMapViews->Get(i)->Exists(_bgId))
        {
            lFound    = true;
            lBitIndex = mMapViews->Get(i)->GetBitIndex();
            break;
        }
    }

    if (!lFound)
    {
        return;
    }

    for(int i = 0; i < _pCount; i++)
    {
        int lX = _pList[i].x;
        int lY = _pList[i].y;

        if (lX >= 0 && lY >= 0 && lX < GetLogicWidth() && lY < GetLogicHeight())
        {
            mVisibilityMap[lX][lY] = SetBitsAtIndex(mVisibilityMap[lX][lY], lBitIndex, _value);
        }
    }
}
//---------------------------------------------------------------------------

void CIGMap::ShareView(int _bgId1, int _bgId2)
{
}
//---------------------------------------------------------------------------

void CIGMap::UnshareView(int _bgId1, int _bgId2)
{
}
//---------------------------------------------------------------------------

void CIGMap::RegisterView(int _bgId)
{
    for(int i = 0; i < mMapViews->Count(); i++)
    {
        if (mMapViews->Get(i)->Exists(_bgId))
        {
            return;
        }
    }

    CIGMapView* lMapView;

    try
    {
        lMapView = new CIGMapView(GenerateUniqueMapViewId());
    }
    catch(...)
    {
        LOG(ERROR) << "CIGMap::RegisterView - cannot allocate map view";
        //throw CErrorGeneric();
        return;
    }

    lMapView->AddBgId(_bgId);

    int lBitIndex = GetBitIndex();

    lMapView->SetBitIndex(lBitIndex);

    mMapViews->Add(lMapView);

    if (_bgId == OS_PLAYER)
    {
        mPlayerIndex = lBitIndex;
        VLOG(5) << "The player has the bit index of " << mPlayerIndex;
    }

    VLOG(5) << "RegisterView - a view was notified for battle-group " << _bgId;
}
//---------------------------------------------------------------------------

void CIGMap::UnregisterView(int _bgId)
{
    for(int i = 0; i < mMapViews->Count(); i++)
    {
        if (mMapViews->Get(i)->Exists(_bgId))
        {
            mMapViews->Get(i)->RemoveBgId(_bgId);

            if (mMapViews->Get(i)->BgCount() <= 0)
            {
                mMapViews->Delete(i);
            }

            // clear the player bit index
            if (_bgId == OS_PLAYER)
            {
                mPlayerIndex = -1;
            }

            return;
        }
    }
}
//---------------------------------------------------------------------------

int CIGMap::GenerateUniqueMapViewId()
{
    bool lUnique;
    int  lId;

    do
    {
        lId = gfRandom(128);
        lUnique = true;

        for(int i = 0; i < mMapViews->Count(); i++)
        {
            if (mMapViews->Get(i)->GetId() == lId)
            {
                lUnique = false;
                break;
            }
        }
    }
    while(lUnique == false);

    return lId;
}
//---------------------------------------------------------------------------

int CIGMap::GetBitIndex()
{
    bool lUnique;

    // only 16 positions are available
    for(int i = 0; i < 16; i+=1)
    {
        lUnique = true;

        for(int j = 0; j < mMapViews->Count(); j++)
        {
            if (mMapViews->Get(j)->GetBitIndex() == i * 2)
            {
                lUnique = false;
                break;
            }
        }

        if (lUnique)
        {
            return i*2;
        }
    }

    return -1;
}
//---------------------------------------------------------------------------

int CIGMap::SetBitsAtIndex(int _who, int _index, int _value)
{
    _value = _value << _index;

    // create a mask to erase the value of the 2 bits
    int lMask = 3 << _index;
    lMask = ~lMask;

    // clear the 2 bits
    _who = _who & lMask;

    // now them to the new value
    _who = _who | _value;

    return _who;
}
//---------------------------------------------------------------------------

int CIGMap::GetBitsAtIndex(int _who, int _index)
{
    int lMask = (3 << _index);

    _who = (_who & lMask);

    _who = (_who >> _index);

    return _who;
}
//---------------------------------------------------------------------------

int  CIGMap::GetVisibility(int _bgId, int _x, int _y)
{
    if (_x < 0 || _y < 0 || _x > GetLogicWidth() || _y > GetLogicHeight())
    {
        return -1;
    }

    if (_bgId == OS_PLAYER)
    {
        return GetBitsAtIndex(mVisibilityMap[_x][_y], mPlayerIndex);
    }

    for(int i = 0; i < mMapViews->Count(); i++)
    {
        if (mMapViews->Get(i)->Exists(_bgId))
        {
            int lBitIndex = mMapViews->Get(i)->GetBitIndex();

            return GetBitsAtIndex(mVisibilityMap[_x][_y], lBitIndex);
        }
    }

    return -1;
}
//---------------------------------------------------------------------------

bool CIGMap::AnyHasVisibility(int _bgId, int _x, int _y, int _width, int _height, int value)
{
    int startX = max(0, _x);
    int endX = min(GetLogicWidth(), _x + _width);
    int startY = max(0, _y);
    int endY = min(GetLogicHeight(), _y + _height);

    if (endX < 0 || endY < 0 || startX > GetLogicWidth() || startY > GetLogicHeight())
    {
        return false;
    }

    if (_bgId == OS_PLAYER)
    {
        for(int x = startX; x <= endX; x++)
        {
            for(int y = startY; y <= endY; y++)
            {
                if (GetBitsAtIndex(mVisibilityMap[x][y], mPlayerIndex) == value)
                {
                    return true;
                }
            }
        }
    }
    else
    {
        for(int i = 0; i < mMapViews->Count(); i++)
        {
            if (mMapViews->Get(i)->Exists(_bgId))
            {
                int lBitIndex = mMapViews->Get(i)->GetBitIndex();

                for(int x = startX; x <= endX; x++)
                {
                    for(int y = startY; y <= endY; y++)
                    {
                        if (GetBitsAtIndex(mVisibilityMap[x][y], lBitIndex) == value)
                        {
                            return true;
                        }
                    }
                }
                //return GetBitsAtIndex(mVisibilityMap[_x][_y], lBitIndex);
            }
        }
    }

    return false;
}
//---------------------------------------------------------------------------

// Class CIGMapView implementation

CIGMapView::CIGMapView()
{
    mId       = -1;
    mBgIdList = new CPList<int>(2);
    mBitIndex    = -1;
}
//---------------------------------------------------------------------------

CIGMapView::CIGMapView(int _id)
{
    mId       = _id;
    mBgIdList = new CPList<int>(2);
    mBitIndex    = -1;
}
//---------------------------------------------------------------------------

CIGMapView::~CIGMapView()
{
    delete mBgIdList;
}
//---------------------------------------------------------------------------

void CIGMapView::AddBgId(int _bgid)
{
    //int* lToAdd = new int[1];

    //*lToAdd = _bgid;

    mBgIdList->Add(new int(_bgid));
    //mBgIdList->Add(lToAdd);
}
//---------------------------------------------------------------------------

void CIGMapView::RemoveBgId(int _bgid)
{
    for(int i = 0; i < mBgIdList->Count(); i++)
    {
        if (*(mBgIdList->Get(i)) == _bgid)
        {
            // remove, return
            mBgIdList->Delete(i);
            return;
        }
    }
}
//---------------------------------------------------------------------------

bool CIGMapView::Exists(int _bgid)
{
    for(int i = 0; i < mBgIdList->Count(); i++)
    {
        if (*(mBgIdList->Get(i)) == _bgid)
        {
            return true;
        }
    }

    return false;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//
// Map view serialize/deserialize section
//
//---------------------------------------------------------------------------

void CIGMapView::Serialize(PP::Stream& _a)
{
    CSerInt lId(mId), lBitIndex(mBitIndex), lBgCount(mBgIdList->Count());

    // view id, bit index, bg count
    lId      .Serialize(_a);
    lBitIndex.Serialize(_a);
    lBgCount .Serialize(_a);

    for(int i = 0; i < mBgIdList->Count(); i++)
    {
        CSerInt lBgId(*(mBgIdList->Get(i)));
        lBgId.Serialize(_a);
    }
}
//---------------------------------------------------------------------------

unsigned __int32 CIGMapView::DataSize()
{
    return 0;
}
//---------------------------------------------------------------------------

void CIGMapView::DeSerialize(PP::Stream& _a)
{
    CSerInt lId, lBitIndex, lBgId, lBgCount;

    // clean-up
    mBgIdList->Clear();

    // map view id, bit index and bgroup id list deserialization
    lId.DeSerialize(_a);
    lBitIndex.DeSerialize(_a);
    lBgCount.DeSerialize(_a);

    for(int i = 0; i < lBgCount.Get(); i++)
    {
        lBgId.DeSerialize(_a);
        AddBgId(lBgId.Get());
    }

    mId       = lId.Get();
    mBitIndex = lBitIndex.Get();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//
// Map serialize/deserialize section
//
//---------------------------------------------------------------------------

void CIGMap::Serialize(PP::Stream& _a)
{
    VLOG(5) << "Writing map (" << mLogicWidth << "x" << mLogicHeight << ")";

    // first serialize map dimensions (physic)
    CSerInt lWidth(mWidth), lHeight(mHeight);

    lWidth .Serialize(_a);
    lHeight.Serialize(_a);

    // serialize the tile structure
    VLOG(5) << ".Writing tile structure";
    int lWritten = 0;

    for (int i = 0; i < GetLogicWidth(); i++)
    {
        for (int j = 0; j < GetLogicHeight(); j++)
        {
            _a.writeInt(&mLayerMap[i][j].mAltitude);

            if (!mLayerMap[i][j].mBaseTile)
            {
                CSerInt lTileCount(0);
                lTileCount.Serialize(_a);

                //CSerInt lAltitude(mLayerMap[i][j].mAltitude);
                //lAltitude.Serialize(_a);
                continue;
            }

            // map extension - although no more than two sprites may share the same cell
            // (a water and a shore); however, maybe it is useful to support multiple
            // layered tiles
            CSerInt lTileCount = mLayerMap[i][j].mBaseTile->Count();
            lTileCount.Serialize(_a);

            for(int k = 0; k < mLayerMap[i][j].mBaseTile->Count(); k++)
            {
                CIGBaseTile* lBaseTile = (CIGBaseTile*)(mLayerMap[i][j].mBaseTile->GetObject(k));

                // the data saved are index of sprites
                CSerInt lSpriteIndex(GetGraphicInstance()->GetSpriteIndex(lBaseTile->GetSprite(), SPRL_MISSION));
                lSpriteIndex.Serialize(_a);

                CSerInt lType((int)lBaseTile->GetType());
                lType.Serialize(_a);

                //logWriteI(lSpriteIndex.Get()); logWrite(" ");
                lWritten++;
            }

            lWritten++;

            // the altitude is serialized only if there are tiles
            // this will lead to smaller map files
            //if (mLayerMap[i][j].mBaseTile->Count() > 0)
            //{
            //_a.Write(&mLayerMap[i][j].mAltitude, sizeof(int));
            //CSerInt lAltitude(mLayerMap[i][j].mAltitude);
            //lAltitude.Serialize(_a);
            //}
        }

        //logWriteLn(" ");
    }
    VLOG(5) << ".Tile structure written " << lWritten;
    // tile Structure - over

    // serialize the visibility map
    for (int i = 0; i < GetLogicWidth(); i++)
    {
        for (int j = 0; j < GetLogicHeight(); j++)
        {
            _a.writeInt(&mVisibilityMap[i][j]);
        }
    }

    // serialize the map views list
    int     lViewCount = mMapViews->Count();
    CIGMapView* lMapView;

    _a.writeInt(&lViewCount);

    for(int i = 0; i < lViewCount; i++)
    {
        lMapView = mMapViews->Get(i);
        lMapView->Serialize(_a);
    }

    // serialize the player map index
    _a.writeInt(&mPlayerIndex);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGMap::DataSize()
{
    return 0;
}
//---------------------------------------------------------------------------

void CIGMap::DeSerialize(PP::Stream& _a)
{
    // make clean
    MakeClean();
    MakeInit();

    // deserialize map dimensions (physic)
    CSerInt lWidth, lHeight;

    lWidth .DeSerialize(_a);
    lHeight.DeSerialize(_a);

    VLOG(5) << "Reading map (" << lWidth.Get()/MAPCELLX << "x" << lHeight.Get()/MAPCELLY << ")";

    InitDimensions(lWidth.Get(), lHeight.Get());

    // deserialize the tile structure
    CIGBaseTile* lBaseTile;

    int lRead = 0;

    for (int i = 0; i < GetLogicWidth(); i++)
    {
        for (int j = 0; j < GetLogicHeight(); j++)
        {
            _a.readInt(&mLayerMap[i][j].mAltitude);

            CSerInt lTileCount;
            lTileCount.DeSerialize(_a);

            //if (lTileCount.Get() <= 0)
            //{
            //  continue;
            //}

            if (lTileCount.Get() > 0)
            {
                for(int k = 0; k < lTileCount.Get(); k++)
                {
                    // allocate tile
                    VLOG(5) << "..Loading base tile ...";

                    try
                    {
                        lBaseTile = new CIGBaseTile();
                    }
                    catch(...)
                    {
                        //throw StreamError("Unable to allocate memory for basetile object");
                    }

                    // reading the sprite index
                    CSerInt lSpriteIndex;

                    lSpriteIndex.DeSerialize(_a);

                    // read the tile type
                    CSerInt lType;

                    lType.DeSerialize(_a);

                    // settings for the tile
                    lBaseTile->SetSprite(GetGraphicInstance()->GetSpriteByIndex(lSpriteIndex.Get(), SPRL_MISSION));
                    lBaseTile->SetCoord(CPPoint(i * MAPCELLX, j * MAPCELLY));
                    lBaseTile->SetType((TTileType)lType.Get());
                    lBaseTile->NoteInLayerMap();
                    /*lBaseTile->*///NoteInLayerMap(MOL_BASETILE, lBaseTile, i, j);
                    lBaseTile->NoteInLogicMap();

                    //logWriteI(lSpriteIndex.Get()); logWrite(" ");

                    //logWriteLn("..Base tile loaded", LOGDET_LOWEST);

                    lRead++;
                }
            }

            // and read the altitude
            //CSerInt lAltitude;
            //lAltitude.DeSerialize(_a);
            //mLayerMap[i][j].mAltitude = lAltitude.Get();
        }

        //logWriteLn("");
    }

    VLOG(5) << ".Tile structure loaded" << lRead;
    // tile Structure - over

    /*for (int i = 0; i < GetLogicWidth(); i++)
    {
      for (int j = 0; j < GetLogicHeight(); j++)
      {
        switch (GetTileType(i, j))
        {
          case MTT_LAND:
            logWrite("L");
            break;
          case MTT_WATER:
            logWrite("W");
            break;
          case MTT_SHORE:
            logWrite("S");
            break;
          case MTT_UNKNOWN:
            logWrite("U");
            break;
          default:
            logWrite("?");
            break;
        }
      }

      logWriteLn(" ");
    }*/

    // deserialize the visibility map
    for (int i = 0; i < GetLogicWidth(); i++)
    {
        for (int j = 0; j < GetLogicHeight(); j++)
        {
            _a.readInt(&mVisibilityMap[i][j]);
        }
    }

    // deserialize the map views list
    CSerInt     lViewCount;
    CIGMapView* lMapView;

    // clean-up the views
    mMapViews->Clear();

    lViewCount.DeSerialize(_a);

    for(int i = 0; i < lViewCount.Get(); i++)
    {
        // alloc
        try
        {
            lMapView = new CIGMapView();
        }
        catch(...)
        {
            //throw StreamError("Unable to allocate memory for a mapview object.");
        }

        // read
        lMapView->DeSerialize(_a);

        // add to list of views - it may be done with a register view, but we want
        // to keep the same shit from the previous saved game
        mMapViews->Add(lMapView);
    }

    // deserialize the player map index
    CSerInt lPlayerMapIndex;

    lPlayerMapIndex.DeSerialize(_a);
    mPlayerIndex = lPlayerMapIndex.Get();
}
//---------------------------------------------------------------------------

void CIGMap::Serialize(BaseSerializer& out)
{
    VLOG(5) << "Writing map (" << mLogicWidth << "x" << mLogicHeight << ")";

    // serialize
    out.beginElement("map");

    // map size (physical)
    out.createAttribute("w", mWidth);
    out.createAttribute("h", mHeight);

    // serialize the tile structure
    VLOG(5) << ".Writing layers structure";
    int lWritten = 0;

    out.beginElement("layers");

    for (int i = 0; i < GetLogicWidth(); i++)
    {
        for (int j = 0; j < GetLogicHeight(); j++)
        {
            out.beginElement("layer");

            out.createAttribute("x", i);
            out.createAttribute("y", j);
            out.createAttribute("z", mLayerMap[i][j].mAltitude);

            if (!mLayerMap[i][j].mBaseTile)
            {
                out.rawWrite(0);
                out.endElement(); /**< layer */

                continue;
            }

            // map extension - although no more than two sprites may share the same cell
            // (a water and a shore); however, maybe it is useful to support multiple
            // layered tiles
            int tileCount = mLayerMap[i][j].mBaseTile->Count();
            out.rawWrite(tileCount);

            out.beginElement("tiles");

            for(int k = 0; k < tileCount; k++)
            {
                out.beginElement("tile");

                CIGBaseTile* lBaseTile = (CIGBaseTile*)(mLayerMap[i][j].mBaseTile->GetObject(k));

                // the data saved are index of sprites
                out.createAttribute("spriteIndex", GetGraphicInstance()->GetSpriteIndex(lBaseTile->GetSprite(), SPRL_MISSION));
                out.createAttribute("type", (int)lBaseTile->GetType());

                out.endElement(); /**< tile */

                lWritten++;
            }
            out.endElement(); /**< tiles */

            lWritten++;

            // the altitude is serialized only if there are tiles
            // this will lead to smaller map files
            //if (mLayerMap[i][j].mBaseTile->Count() > 0)
            //{
            //_a.Write(&mLayerMap[i][j].mAltitude, sizeof(int));
            //CSerInt lAltitude(mLayerMap[i][j].mAltitude);
            //lAltitude.Serialize(_a);
            //}

            out.endElement(); /**< layer */
        }

        //logWriteLn(" ");
    }

    out.endElement(); /**< layers */

    VLOG(5) << ".Layers structure written " << lWritten;
    // tile Structure - over

    // serialize the visibility map
    out.beginElement("visibility");

    for (int i = 0; i < GetLogicWidth(); i++)
    {
        for (int j = 0; j < GetLogicHeight(); j++)
        {
            out.beginElement("entry");
            out.createAttribute("i", i);
            out.createAttribute("j", j);
            out.createAttribute("v", &mVisibilityMap[i][j]);
            out.endElement();
        }
    }

    out.endElement(); /**< visibility */

    // serialize the map views list
    int     lViewCount = mMapViews->Count();
    CIGMapView* lMapView;

    out.beginElement("views");
    out.rawWrite(lViewCount);

    for(int i = 0; i < lViewCount; i++)
    {
        lMapView = mMapViews->Get(i);
        //lMapView->Serialize(out);
    }

    out.endElement(); /**< views */

    // serialize the player map index
    out.createAttribute("playerIndex", mPlayerIndex);

    out.endElement(); /**< map */
}
//---------------------------------------------------------------------------

void CIGMap::DeSerialize(BaseDeSerializer& in)
{
    // TODO
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

