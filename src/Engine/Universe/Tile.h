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
 application: more than one

 descrition : structura de tile
 last modify: 09 02 2000
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef TileH
#define TileH

#define MAX_CHUNK_SIZE 5
#define TILEDIM        96

//class CPString;
#include "KList.h"
#include "PPIG_MapData.h"
#include "PP_String.h"

#include "Stream.h"

const CPString TILE_FILE_ID = "PPTACTICAL::MAPCHUNK";

//#define MAXFILELENGTH   50
//#define TILEDIM         96
//#define TILESQUARES     3

class CAuxTileData
{
  public:
    CPString GetSprite() { return mSprite; };
    void     SetSprite(CPString _v) { mSprite = _v; };

    TTileType GetType() { return mType; };
    void SetType(TTileType _type) { mType = _type; };

  public: // serialize-deserialize
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  private:
    CPString  mSprite;
    TTileType mType;
};

class CAuxMapChunk
{
  public:
    CAuxMapChunk();
    CAuxMapChunk(int _size);
    ~CAuxMapChunk();

  public: // serialize-deserialize
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  public:
    void SetSize(int _size);
    int  GetSize() { return mSize; };

    void SetApparition(int _v) { mApparition = _v; };
    int  GetApparition() { return mApparition; };

    void SetName(CPString _v) { mName = _v; }
    CPString GetName() { return mName; }

    void     SetPath(CPString _v) { mPath = _v; }
    CPString GetPath() { return mPath; }

  public: // the interface
    bool AddToLayer(int _x, int _y, CAuxTileData* _baseTile);
    bool AddToLayer(int _x, int _y, CPString _sprite, TTileType _kind, int _altitude);

    bool          Clear(int _x, int _y);
    CAuxTileData* Get(int _x, int _y, int _depth);
    int           GetDepth(int _x, int _y);

  protected:
    virtual bool CheckLimits(int _x, int _y);
    void ClearAll();

  private:
    int                   mSize;
    int                   mApparition;
    CPString              mName;
    CPString              mPath; // path to the sprites, full, only used in map editor
    CPList<CAuxTileData>* mLayers[MAX_CHUNK_SIZE][MAX_CHUNK_SIZE];
};
//---------------------------------------------------------------------------

#endif
