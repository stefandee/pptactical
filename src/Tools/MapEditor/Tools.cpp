//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    // 
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU General Public License as     // 
// published by the Free Software Foundation; either version 2 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //  
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY// 
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License  //
// for more details.                                                         //
//                                                                           // 
// You should have received a copy of the GNU General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   // 
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 application: PP Mission Editor

 descrition : diferite utilitare 
 last modify: 16 11 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#include <vcl.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#pragma hdrstop

#include "Tools.h"
#include "PPIG_ObjInfantry.h"
#include "PPIG_ObjVehicle.h"
#include "PPIG_ObjTank.h"
#include "PPIG_ObjTran.h"
#include "PPIG_ObjArtillery.h"
#include "PPIG_ObjRecon.h"
#include "PP_Graphic.h"
#include "PPIG_Mission.h"
#include "MEError.h"
#include "PPIG_BGManager.h"
#include "basepath.h"

/*---------------------------------------------------------------------------
 descrition: returneaza tipul obiectului din fisierul file
             daca nu gaseste tipul returneaza OT_NONE
 parameters: numele fisierului
---------------------------------------------------------------------------*/
int getObjectType(AnsiString file)
{
   int     Handle;
   int     Length;
   __int32 lIdSize;
   char    lIdString[21];

   setMEError(ERR_NONE);

   //
   // try to open as TILE file
   //
   if ((Handle = open(file.c_str(), O_BINARY | O_RDONLY, S_IREAD)) == -1)
   {
      setMEError(ERR_OPEN, "getObjectType");
      close(Handle);
      return OT_NONE;
   }

   if (read(Handle, &lIdSize, sizeof(__int32)) == -1)
   {
     return OT_NONE;
   }

   if (read(Handle, &lIdString, 20) == -1)
   {
     return OT_NONE;
   }

   //Length = filelength(Handle);
   close(Handle);

   // check for mapchunk (tile)
   lIdString[20] = '\0';
   if (/*(lIdSize == 20) &&*/ (strcmp(lIdString, TILE_FILE_ID.c_str()) == 0))
   {
     return OT_TILE;
   }

   //
   // try to open as static file
   //
   CSerString lId;

   try
   {
     PP::Stream_ANSI_C lA(CPString(file.c_str()));

     lId.DeSerialize(lA);

     if (lId.Get() == STATIC_FILE_ID)
     {
       return OT_STATIC;
     }
   }
   catch(...)
   {
     setMEError(ERR_OPEN, "getObjectType");
   }

   return OT_NONE;
}

/*---------------------------------------------------------------------------
 descrition: returneaza numele obiectului, adica name din structura respectiva
             functioneaza pentru static si pentru tile
             returneaza spatiu daca este vreo eroare sau nu numele este vid
 parameters: numele fisierului
---------------------------------------------------------------------------*/
AnsiString getObjectName(AnsiString file)
{
   int Handle;
   int Length;
   struct _StaticObject lObjStatic;
   CAuxMapChunk* lMapChunk;
   AnsiString lName;

   setMEError(ERR_NONE);

   //
   // try to identify as tile
   //
   lMapChunk = getMapChunk(file);

   if (lMapChunk)
   {
     lName = AnsiString(lMapChunk->GetName().c_str());
     delete lMapChunk;
     return lName;
   }

   //
   // try to open as static file
   //
   CSerString lId;

   try
   {
     PP::Stream_ANSI_C lA(CPString(file.c_str()));

     lId.DeSerialize(lA);

     if (lId.Get() == STATIC_FILE_ID)
     {
       lObjStatic.DeSerialize(lA);

        // type string
        AnsiString lStrType = "(unknown)";

        switch(lObjStatic.type)
        {
          case ST_NORMAL:
            lStrType = "";
            break;

          case ST_TURRET:
            lStrType = "(turret)";
            break;

          case ST_PILLBOX:
            lStrType = "(pillbox)";
            break;

          case ST_FACTORY:
            lStrType = "(factory)";
            break;
        }

       return AnsiString(lObjStatic.objName) + " " + lStrType;
     }
   }
   catch(...)
   {
     setMEError(ERR_OPEN, "getObjectName");
   }

   return " ";
}

/*---------------------------------------------------------------------------
 descrition: returneaza numele obiectului, adica name din structura respectiva
             functioneaza pentru static
             returneaza spatiu daca este vreo eroare sau nu numele este vid
 parameters: numele fisierului
---------------------------------------------------------------------------*/
AnsiString getObjectSpriteName(AnsiString file)
{
   int Handle;
   int Length;
   struct _StaticObject lObjStatic;
   AnsiString Name;

   setMEError(ERR_NONE);

   //
   // try to open as static
   //
   CSerString lId;

   try
   {
     PP::Stream_ANSI_C lA(CPString(file.c_str()));

     lId.DeSerialize(lA);

     if (lId.Get() == STATIC_FILE_ID)
     {
       lObjStatic.DeSerialize(lA);

       return AnsiString(lObjStatic.sprite);
     }
   }
   catch(...)
   {
     setMEError(ERR_OPEN, "getObjectName");
   }

   return " ";
}

/*---------------------------------------------------------------------------
 descrition: returneaza numele sprite-ului
 parameters: numele fisierului, tipul unitatii si index in fisier
---------------------------------------------------------------------------*/
AnsiString getObjectSpriteName(AnsiString filename, int objtype, int index)
{
   hInfantryUnified tmpinf;
   hVehicleUnified  tmpveh;
   AnsiString tmpsprite;

   switch (objtype)
   {
      case OT_TILE:
         return " ";
      case OT_INFANTRY:
         tmpinf = new TInfantryUnified;
         if (getAIInfRecord(filename.c_str(), index, tmpinf))
         {
            tmpsprite = tmpinf->sprite;
            delete tmpinf;
            return tmpsprite;
         }
         else
         {
            delete tmpinf;
            return " ";
         }

      case OT_VEHICLE:
         tmpveh = new TVehicleUnified;
         if (getAIVehRecord(filename.c_str(), index, tmpveh))
         {
            if (tmpveh->vType < 4)
            {
              tmpsprite = tmpveh->sprite;
              delete tmpveh;
              return tmpsprite;
            }  
         }

         delete tmpveh;
         return " ";

      case OT_AIR:
         tmpveh = new TVehicleUnified;
         if (getAIVehRecord(filename.c_str(), index, tmpveh))
         {
            if (tmpveh->vType >= 4 && tmpveh->vType <= 7)
            {
              tmpsprite = tmpveh->sprite;
              delete tmpveh;
              return tmpsprite;
            }  
         }

         delete tmpveh;
         return " ";
   }

   return " ";
}

/*---------------------------------------------------------------------------
 descrition: returneaza mask din dat-ul dat de file
             functioneaza pentru static
---------------------------------------------------------------------------*/
TMask getObjectMask(AnsiString file)
{
   TMask tmpMask;
   int Handle;
   int Length;
   struct _StaticObject lObjStatic;

   tmpMask.Width = -1;
   setMEError(ERR_NONE);

   //
   // try to open as static
   //
   CSerString lId;

   try
   {
     PP::Stream_ANSI_C lA(CPString(file.c_str()));

     lId.DeSerialize(lA);

     if (lId.Get() == STATIC_FILE_ID)
     {
       lObjStatic.DeSerialize(lA);

       tmpMask.Width  = lObjStatic.maskY;
       tmpMask.Height = lObjStatic.maskX;
       for (int i=0; i < lObjStatic.maskX; i++)
       {
         for (int j=0; j < lObjStatic.maskY; j++)
         {
           tmpMask.Mask[i][j] = lObjStatic.mask[i][j];
         }
       }
     }
   }
   catch(...)
   {
     setMEError(ERR_OPEN, "getObjectMask");
   }

   return tmpMask;
}

/*---------------------------------------------------------------------------
 description: returneaza mask din structura staticului
---------------------------------------------------------------------------*/
TMask getObjectMask(hStaticObject objStatic)
{
   TMask tmpMask;

   tmpMask.Width  = objStatic->maskY;
   tmpMask.Height = objStatic->maskX;
   for (int i=0; i<objStatic->maskX; i++)
      for (int j=0; j<objStatic->maskY; j++)
         tmpMask.Mask[j][i] = objStatic->mask[i][j];
   return tmpMask;
}

/*---------------------------------------------------------------------------
 description: returneaza CAuxMapChunk* sau NULL
 parameters: filename
---------------------------------------------------------------------------*/
CAuxMapChunk* getMapChunk(AnsiString file)
{
   int           Handle;
   int           Length;
   CAuxMapChunk* lMapChunk;

   // verifying the mapchunk file
   if ((Handle = open(file.c_str(), O_BINARY | O_RDONLY, S_IREAD)) == -1)
   {
      setMEError(ERR_OPEN);
      close(Handle);
      return NULL;
   }

   // reading the first 4 + 20 bytes
   __int32 lIdSize;

   if (read(Handle, &lIdSize, sizeof(__int32)) == -1)
   {
     close(Handle);
     return NULL;
   }

   if (lIdSize != 20)
   {
     close(Handle);
     return NULL;
   }

   char lIdString[21];

   if (read(Handle, &lIdString, 20) == -1)
   {
     close(Handle);
     return NULL;
   }

   lIdString[20] = '\0';

   if (strcmp(lIdString, "PPTACTICAL::MAPCHUNK") != 0)
   {
     close(Handle);
     return NULL;
   }
   close(Handle);

   //CArchive lA;

   try
   {
      lMapChunk = new CAuxMapChunk();
   }
   catch(...)
   {
      setMEError(ERR_ALLOCATE);
      return NULL;
   }

   try
   {
     //lA.Open(CPString(file.c_str()));
     PP::Stream_ANSI_C lA(CPString(file.c_str()));
     lMapChunk->DeSerialize(lA);
   }
   catch(...)
   {
     delete lMapChunk;
     //lA.Close();
     return NULL;
   }

   //lA.Close();
   return lMapChunk;
}

/*---------------------------------------------------------------------------
 descrition: returneaza hStaticObject sau NULL
 parameters: numele fisierului
---------------------------------------------------------------------------*/
bool getStaticObjectStruct(AnsiString filename, struct _StaticObject* _static)
{
  setMEError(ERR_NONE);

  if (!_static)
  {
   setMEError(ERR_OPEN, "getStaticObjectStruct");
   return false;
  }

  //
  // try to open as static
  //
  CSerString lId;

  try
  {
   PP::Stream_ANSI_C lA(CPString(filename.c_str()));

   lId.DeSerialize(lA);

   if (lId.Get() == STATIC_FILE_ID)
   {
     _static->DeSerialize(lA);

     return true;
   }
  }
  catch(...)
  {
   setMEError(ERR_OPEN, "getStaticObjectStruct");
  }

  return false;
}

/*---------------------------------------------------------------------------
 descrition: returneaza structura index din filename
---------------------------------------------------------------------------*/
bool getAIInfantryUnitStruct(AnsiString filename, int index, TInfantryUnified* _inf)
//TInfantryUnified getAIInfantryUnitStruct(AnsiString filename, int index)
{
   //TInfantryUnified tmp;
   if (getAIInfRecord(filename.c_str(), index, _inf))
      return true;
   else
      return false;
}

/*---------------------------------------------------------------------------
 descrition: returneaza structura index din filename
---------------------------------------------------------------------------*/
bool getAIVehicleUnitStruct(AnsiString filename, int index, TVehicleUnified* _veh)
//TInfantryUnified getAIInfantryUnitStruct(AnsiString filename, int index)
{
   //TInfantryUnified tmp;
   if (getAIVehRecord(filename.c_str(), index, _veh))
      return true;
   else
      return false;
}

/*---------------------------------------------------------------------------
 description: returneaza un string dintr'un tip de vehicul
---------------------------------------------------------------------------*/
AnsiString GetVehicleType(int _type)
{
  switch(_type)
  {
    case 0:
      return "tank";

    case 1:
      return "artillery";

    case 2:
      return "transport";

    case 3:
      return "recon";

    case 4:
      return "fighter";

    case 5:
      return "bomber";

    case 6:
      return "chopper";

    case 7:
      return "dropship";
  }

  return "unknown";
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: creates an engine class infantry based on parameters
---------------------------------------------------------------------------*/
CIGInfantryUnit* createInfantryUnit(CIGMission* _mission, AnsiString filename, int index, int _unitId, int _bgId, int _scriptId)
{
  CIGInfantryUnit* lInfUnit;
  TInfantryUnified lInfStruct;
  CPGISprite*      lSprite = 0;

  if (!getAIInfantryUnitStruct(filename, index, &lInfStruct))
  {
    return 0;
    //printMEError("Cannot read infantry data!");
  }

  try
  {
    lInfUnit = new CIGInfantryUnit();
  }
  catch(...)
  {
    return 0;
  }

  lSprite = GetGraphicInstance()->AddSprite(getGamePath() + PATH_INF_GFX + lInfStruct.sprite, 8, 14, SPRL_MISSION);
  lInfUnit->SetSprite(lSprite);
  lInfUnit->SetBattleGroupId(_bgId);
  lInfUnit->SetId(_unitId);
  lInfUnit->SetScriptObserver(_mission->GetDoctrine()->GetScriptObserver(_scriptId));
  lInfUnit->SetSelected(false);
  lInfUnit->SetInfUnifiedData(lInfStruct);

  /*
  switch(_mission->GetBGManager()->GetRelation(OS_PLAYER, _bgId))
  {
     case REL_NEUTRAL:
        _mission->GetObjects(MOL_NEUTRALUNITS)->Add(lInfUnit);
        break;
     case REL_OWNED:
        _mission->GetObjects(MOL_OWNEDUNITS)->Add(lInfUnit);
        break;
     case REL_FRIENDLY:
        _mission->GetObjects(MOL_FRIENDLYUNITS)->Add(lInfUnit);
        break;
     case REL_ENEMY:
        _mission->GetObjects(MOL_ENEMYUNITS)->Add(lInfUnit);
        break;
     default:
        break;
  }

  _mission->GetObjects(MOL_ALLOBJECTS)->Add(lInfUnit);
  */

  // add infantry to the battle group it belongs
  _mission->GetBGManager()->AddUnit(lInfUnit);

  return lInfUnit;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: creates an engine class infantry based on parameters
---------------------------------------------------------------------------*/
CIGVehicle*      createVehicleUnit(CIGMission* _mission, AnsiString filename, int index, int _unitId, int _bgId, int _scriptId)
{
  TVehicleUnified lVehStruct;
  CIGVehicle*     lVehicleUnit;
  CPGISprite*     lSprite = 0;

  if (!getAIVehicleUnitStruct(filename, index, &lVehStruct))
  {
    return 0;
    //printMEError("Cannot read vehicle data!");
  }

  switch(lVehStruct.vType)
  {
    case 0:
      try
      {
         lVehicleUnit = new CIGTankUnit();
      }
      catch(...)
      {
         LOG(ERROR) << "UNABLE TO CREATE TANK UNIT !!!!!!!!!!";
      }
      break;

    case 1:
      try
      {
         lVehicleUnit = new CIGArtilleryUnit();
      }
      catch(...)
      {
         LOG(ERROR) << "UNABLE TO CREATE ARTILLERY UNIT !!!!!!!!!!";
      }
      break;

    case 2:
      try
      {
         lVehicleUnit = new CIGTranUnit();
      }
      catch(...)
      {
         LOG(ERROR) << "UNABLE TO CREATE TRAN UNIT !!!!!!!!!!";
      }
      break;

    case 3:
      try
      {
         lVehicleUnit = new CIGReconUnit();
      }
      catch(...)
      {
         LOG(ERROR) << "UNABLE TO CREATE RECON UNIT !!!!!!!!!!";
      }
      break;

    default:
      return 0;  
  }

  lSprite = GetGraphicInstance()->AddSprite(getGamePath() + PATH_VEHICLES_GFX + lVehStruct.sprite, 8, 4, SPRL_MISSION);
  lVehicleUnit->SetSprite(lSprite);
  lVehicleUnit->SetBattleGroupId(_bgId);
  lVehicleUnit->SetId(_unitId);
  lVehicleUnit->SetScriptObserver(_mission->GetDoctrine()->GetScriptObserver(_scriptId));
  lVehicleUnit->SetSelected(false);
  lVehicleUnit->SetVehicleUnifiedData(lVehStruct);

  /*
  switch (_mission->GetBGManager()->GetRelation(OS_PLAYER, _bgId))
  {
     case REL_NEUTRAL:
        _mission->GetObjects(MOL_NEUTRALUNITS)->Add(lVehicleUnit);
        break;
     case REL_OWNED:
        _mission->GetObjects(MOL_OWNEDUNITS)->Add(lVehicleUnit);
        break;
     case REL_FRIENDLY:
        _mission->GetObjects(MOL_FRIENDLYUNITS)->Add(lVehicleUnit);
        break;
     case REL_ENEMY:
        _mission->GetObjects(MOL_ENEMYUNITS)->Add(lVehicleUnit);
        break;
     default:
        // do nothing
        //logWriteLn("Vehicle relation to player unknown, vehicle unit ejected.");
        // continue; ?
        return 0;
  }

  _mission->GetObjects(MOL_ALLOBJECTS)->Add(lVehicleUnit);
  */

  // add the vehicle to the battle group it belongs
  _mission->GetBGManager()->AddUnit(lVehicleUnit);

  return lVehicleUnit;
}

//---------------------------------------------------------------------------
#pragma package(smart_init)
