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
#include <stdexcept>
#pragma hdrstop

#include "Tile.h"
#include "Serializable.h"

CAuxMapChunk::CAuxMapChunk()
{
  for(int i = 0; i < MAX_CHUNK_SIZE; i++)
  {
    for(int j = 0; j < MAX_CHUNK_SIZE; j++)
    {
      mLayers[i][j] = 0;
    }
  }

  mSize = 1;
  mName = "Noname mapchunk";
}
//---------------------------------------------------------------------------

CAuxMapChunk::CAuxMapChunk(int _size)
{
  for(int i = 0; i < MAX_CHUNK_SIZE; i++)
  {
    for(int j = 0; j < MAX_CHUNK_SIZE; j++)
    {
      mLayers[i][j] = 0;
    }
  }

  mSize = _size;
  mName = "Noname mapchunk";
}
//---------------------------------------------------------------------------

CAuxMapChunk::~CAuxMapChunk()
{
  // make clean
  for(int i = 0; i < MAX_CHUNK_SIZE; i++)
  {
    for(int j = 0; j < MAX_CHUNK_SIZE; j++)
    {
      if (mLayers[i][j])
      {
        while(mLayers[i][j]->Count() > 0)
        {
          mLayers[i][j]->Delete(0);
        }

        delete mLayers[i][j];
      }
    }
  }
}
//---------------------------------------------------------------------------

void CAuxMapChunk::SetSize(int _size)
{
  if ((_size > MAX_CHUNK_SIZE) || (_size < 1))
  {
    return;
  }

  if (mSize < _size)
  {
    mSize = _size;
    return;
  }

  // need to dealloc some cells
  for(int i = _size; i < mSize; i++)
  {
    for(int j = 0; j < mSize; j++)
    {
      Clear(i, j);
    }
  }

  for(int j = _size; j < mSize; j++)
  {
    for(int i = 0; i < _size; i++)
    {
      Clear(i, j);
    }
  }

  mSize = _size;
}
//---------------------------------------------------------------------------

bool CAuxMapChunk::AddToLayer(int _x, int _y, CAuxTileData* _baseTile)
{
  // are _x and _y in range 1..mSize?
  if (!CheckLimits(_x, _y))
  {
    return false;
  }

  // valid tile sent?
  if (!_baseTile)
  {
    return false;
  }

  // alloc the list if nothing was set there before
  if (!mLayers[_x][_y])
  {
    mLayers[_x][_y] = new CPList<CAuxTileData>(1);
  }

  // add logic:
  // adding a water tile will remove a ground tile but will keep a shore tile
  // adding a shore will stack upon a water tile (if any)
  // ...but adding a shore over everything else will do nothing
  // adding a ground will remove everything

  CPList<CAuxTileData>* lList = mLayers[_x][_y];

  switch(_baseTile->GetType())
  {
    case MTT_LAND:

      // clear the list if there is something to clear
      if (lList->Count() > 0)
      {
        while(lList->Count() > 0) lList->Delete(0);
      }

      lList->Add(_baseTile);
      return true;

    case MTT_SHORE:
      if ((lList->Count() == 1 && lList->Get(0)->GetType() == MTT_WATER) || lList->Count() == 0)
      {
        lList->Add(_baseTile);
        return true;
      }
      break;

    case MTT_WATER:
      if (lList->Count() == 1 && lList->Get(0)->GetType() == MTT_SHORE)
      {
          CAuxTileData* lTemp = lList->Get(0);

          lList->Remove(0);

          lList->Add(_baseTile);
          lList->Add(lTemp);
          return true;
      }
      else
      {
        // clean and add
        while(lList->Count() > 0) lList->Delete(0);
        lList->Add(_baseTile);
      }
      break;

    case MTT_UNKNOWN:
      break;
  }

  // + the a default for the switch above 
  return true;
}
//---------------------------------------------------------------------------

bool CAuxMapChunk::AddToLayer(int _x, int _y, CPString _sprite, TTileType _kind, int _altitude)
{
  if (!CheckLimits(_x, _y))
  {
    return false;
  }

  CAuxTileData* lBaseTile;

  lBaseTile = new CAuxTileData();

  lBaseTile->SetType(_kind);
  lBaseTile->SetSprite(_sprite);

  bool lResult = AddToLayer(_x, _y, lBaseTile);

  if (!lResult)
  {
    delete lBaseTile;
  }

  return lResult;
}
//---------------------------------------------------------------------------

bool CAuxMapChunk::Clear(int _x, int _y)
{
  if (!CheckLimits(_x, _y))
  {
    return false;
  }

  if (!mLayers[_x][_y])
  {
    return false;
  }

  while(mLayers[_x][_y]->Count() > 0)
  {
    mLayers[_x][_y]->Delete(0);
  }

  delete mLayers[_x][_y];

  mLayers[_x][_y] = 0;

  return true;
}
//---------------------------------------------------------------------------

CAuxTileData* CAuxMapChunk::Get(int _x, int _y, int _depth)
{
  if (!CheckLimits(_x, _y))
  {
    return NULL;
  }

  if (!mLayers[_x][_y])
  {
    return NULL;
  }

  for(int i = 0; i < mLayers[_x][_y]->Count(); i++)
  {
    if (i == _depth)
    {
      return mLayers[_x][_y]->Get(i);
    }
  }

  return NULL;
}
//---------------------------------------------------------------------------

int CAuxMapChunk::GetDepth(int _x, int _y)
{
  if (!CheckLimits(_x, _y))
  {
    return -1;
  }

  if (!mLayers[_x][_y])
  {
    return 0;
  }

  return mLayers[_x][_y]->Count();
}
//---------------------------------------------------------------------------

bool CAuxMapChunk::CheckLimits(int _x, int _y)
{
  return ((_x < mSize) && (_x >= 0) && (_y >= 0) && (_y < mSize));
}
//---------------------------------------------------------------------------

void CAuxMapChunk::ClearAll()
{
  for(int i = 0; i < mSize; i++)
  {
    for(int j = 0; j < mSize; j++)
    {
      Clear(i, j);
    }
  }
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//
// Tile Data serialize/deserialize section
//
//---------------------------------------------------------------------------
 
void CAuxTileData::Serialize(PP::Stream& _a)
{
  CSerString lSprite(mSprite);
  __int32    lType = (__int32)mType;

  lSprite.Serialize(_a);
  _a.write(&lType, sizeof(__int32));
}
//---------------------------------------------------------------------------

unsigned __int32 CAuxTileData::DataSize()
{
  return mSprite.Length() + sizeof(__int32);
}
//---------------------------------------------------------------------------

void CAuxTileData::DeSerialize(PP::Stream& _a)
{
  CSerString lSprite;
  __int32    lType;

  lSprite.DeSerialize(_a);
  _a.read(&lType, sizeof(__int32));

  mType   = (TTileType)lType;
  mSprite = lSprite.Get();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//
// Map Chunk serialize/deserialize section
//
//---------------------------------------------------------------------------

void CAuxMapChunk::Serialize(PP::Stream& _a)
{
  CSerString lStringId("PPTACTICAL::MAPCHUNK");
  __int8     lDepth;
  CSerString lName(mName);

  lStringId.Serialize(_a);
  lName.Serialize(_a);
  _a.write(&mSize, sizeof(__int32));
  _a.write(&mApparition, sizeof(__int32));

  for(int i = 0; i < mSize; i++)
  {
    for(int j = 0; j < mSize; j++)
    {
      if (mLayers[i][j])
      {
        lDepth = GetDepth(i, j);
        _a.write(&lDepth, sizeof(__int32));

        for(int k = 0; k < lDepth; k++)
        {
          mLayers[i][j]->Get(k)->Serialize(_a);
        }
      }
      else
      {
        lDepth = 0;
        _a.write(&lDepth, sizeof(__int32));
      }
    }
  }
}
//---------------------------------------------------------------------------

unsigned __int32 CAuxMapChunk::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void CAuxMapChunk::DeSerialize(PP::Stream& _a)
{
  CSerString    lStringId("PPTACTICAL::MAPCHUNK"), lName;
  __int8        lDepth;
  CAuxTileData* lBaseTile;

  lStringId.DeSerialize(_a);

  if (lStringId.Get() != CPString("PPTACTICAL::MAPCHUNK"))
  {
    //throw CArchiveError("Wrong or corrupted file.");
    throw std::runtime_error("Wrong or corrupted file.");
  }

  // clear the tile data
  ClearAll();

  lName.DeSerialize(_a);
  mName = lName.Get();

  _a.read(&mSize, sizeof(__int32));
  _a.read(&mApparition, sizeof(__int32));

  for(int i = 0; i < mSize; i++)
  {
    for(int j = 0; j < mSize; j++)
    {
      _a.read(&lDepth, sizeof(__int32));

      if (lDepth <= 0)
      {
        continue;
      }

      // there is some tile data to read
      for(int k = 0; k < lDepth; k++)
      {
         try
         {
           lBaseTile = new CAuxTileData();
           lBaseTile->DeSerialize(_a);
         }
         catch(...)
         {
           throw;
         }

         AddToLayer(i, j, lBaseTile);
      }
    }
  }
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
