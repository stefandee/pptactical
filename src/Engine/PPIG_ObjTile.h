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

 description: Class In Game Base Tile
 last modify: 07 05 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PPIG_ObjTileH
#define PPIG_ObjTileH

#include "PPIG_ObjBase.h"
#include "PP_String.h"
#include "PPIG_EngineDef.h"
#include "PPIG_MapData.h"

class CIGBaseTile: public CIGBaseObject
{
   public:
      CIGBaseTile();

      virtual CPString ClassName() {return IGCN_BASETILE;}
      virtual bool IsDerivedFrom(CPString className);

      virtual void NoteInLogicMap();
      virtual void NoteInLogicMap(CPPoint Point);
      virtual void NoteInLayerMap ();
      virtual void RemoveFromLayerMap ();

      virtual void Paint(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel = 1);
   public: // get-set
     TTileType GetType()             { return mType; };
     void      SetType(TTileType _v) { mType = _v; };

   /*
   public: // serialize-deserialize
     virtual void Serialize(Stream& _a) {};
     virtual unsigned __int32 DataSize() {};
     virtual void DeSerialize(Stream& _a) {};
   */
   protected:
      virtual bool CanBePlaced(CPPoint _point) { return true; }

   private:
     TTileType mType;
};

//---------------------------------------------------------------------------
#endif

