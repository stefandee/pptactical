//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2025 Stefan Dicu/Piron Games & Tudor Garba             //
//                                                                           //
// This library is free software; you can redistribute it and/or             //
// modify it under the terms of the GNU Lesser General Public                //
// License as published by the Free Software Foundation; either              //
// version 2.1 of the License, or (at your option) any later version.        //
//                                                                           //
// This library is distributed in the hope that it will be useful,           //
// but WITHOUT ANY WARRANTY; without even the implied warranty of            //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         //
// Lesser General Public License for more details.                           //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public          //
// License along with this library; if not, write to the Free Software       //
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA //
//-----------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 application: PP Mission Editor

 descrition : main form
              cred ca aici am pus mai multe chestii decat ar trebui
 last modify: 17 02 00
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#include <vcl.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dir.h>
//#include <iostream.h>
#pragma hdrstop

// basic forms
#include "FMain.h"
#include "FNew.h"
#include "FInfo.h"
#include "FPreview.h"
#include "FTool.h"
#include "FRadar.h"
#include "FTree.h"
#include "FAbout.h"
#include "FCustDlg.h"
#include "FAlt.h"
#include "FPref.h"
#include "FObjProp.h"
#include "FGroup.h"
#include "FMission.h"
#include "FMapGen.h"

// mission editing forms
#include "FMissionInfo.h"
#include "FMissionScript.h"
#include "FBGEdit.h"
#include "FPolitics.h"

// engine includes
#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "PPIG_ObjInfantry.h"
#include "PPIG_ObjTank.h"
#include "PPIG_ObjRecon.h"
#include "PPIG_ObjTran.h"
#include "PPIG_ObjArtillery.h"
#include "PPIG_ObjStatic.h"
#include "PPIG_ObjFactory.h"
#include "PPIG_ObjTile.h"
#include "PPIG_ObjDropship.h"
#include "PPIG_ObjFighter.h"
#include "PPIG_ObjChopper.h"
#include "PPIG_ObjAircraft.h"
#include "PPIG_SpaceSound.h"
#include "PPIG_BGManager.h"
#include "PPIG_BattleGroup.h"
#include "PP_Sound.h"

// basic includes
#include "MEError.h"
#include "Paths.h"
#include "basepath.h"
#include "Tools.h"
#include "PPME_Cfg.h"

#include "PP_VfsSystem.h"
#include "Stream_PhysFS.h"
#include "easylogging++.h"

#include <algorithm> 
#include <iostream>
#include <functional>
using namespace std;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 Descrition: PUBLIC AREA
---------------------------------------------------------------------------*/
__fastcall TFormMain::TFormMain(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------

Types::TRect TFormMain::TRectConv(CPRect _v)
{
  return Types::TRect(_v.left, _v.top, _v.right, _v.bottom);
}
//---------------------------------------------------------------------------

CPRect TFormMain::CPRectConv(Types::TRect _v)
{
  return CPRect(_v.Left, _v.Top, _v.Right, _v.Bottom);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: intercepteaza WM_ERASEBCKGND
---------------------------------------------------------------------------*/
void __fastcall TFormMain::WndProc(Messages::TMessage &Message)
{
   switch(Message.Msg)
   {
      case WM_ERASEBKGND:
         return;

      case WM_MOVE:
         //setScreenZone();
         return;

      case WM_SIZE:
         //setScreenZone();
         break;

      case WM_DISPLAYCHANGE:
         // must be done something!
         TForm::WndProc(Message);
         break;

      case WM_PAINT:
         //paintCanvas();
         break;

      default:
         break;
   }

   TForm::WndProc(Message);
}

/*---------------------------------------------------------------------------
 descrition: afiseaza in statusMain Application->Hint
---------------------------------------------------------------------------*/
void __fastcall TFormMain::displayHint(TObject *Sender)
{
   statusMain->Panels->Items[2]->Text = Application->Hint;
}

/*---------------------------------------------------------------------------
 descrition: update the visibility map for the player (give map/take map :)
---------------------------------------------------------------------------*/
void TFormMain::UpdateVisibility(int _value)
{
  for(int i = 0; i < GetMissionInstance()->GetMap()->GetLogicWidth(); i++)
  {
   for(int j = 0; j < GetMissionInstance()->GetMap()->GetLogicHeight(); j++)
   {
     CPPoint lTmpPoint = CPPoint(i, j);

     GetMissionInstance()->GetMap()->NotifyVisibilityMap(OS_PLAYER, &lTmpPoint, 1, _value);
   }
  }
}

void TFormMain::NoteObject(TTreeItem *treeitem, TMask mask, int x, int y, int _bg, int _id, int _scriptId)
{
   //int spriteindex;
   //TTile *tileStruct;
   struct _StaticObject lStaticStruct;
   TInfantryUnified     lInfStruct;
   TVehicleUnified      lVehStruct;
   CPGISprite*          lSprite = NULL;
   CIGInfantryUnit*     lInfUnit;
   CIGStaticObject*     lStatic;
   CAuxMapChunk*        lMapChunk;
   CIGBaseTile*         lTile;
   //CIGTankUnit*         lTankUnit;
   CIGVehicle*          lVehicleUnit;
   CIGAircraft*         lAircraftUnit;


   switch (treeitem->objType)
   {
      case OT_TILE:
         lMapChunk = getMapChunk(treeitem->fullName);

         if (!lMapChunk)
         {
           break;
         }

         for(int i = 0; i < lMapChunk->GetSize(); i++)
         {
           for(int j = 0; j < lMapChunk->GetSize(); j++)
           {
             // clean the tile
             GetMissionInstance()->GetMap()->CleanTile(i*3 + x/MAPCELLX, j*3 + y/MAPCELLY);

             for(int k =0; k < lMapChunk->GetDepth(i, j); k++)
             {
               try
               {
                 lTile = new CIGBaseTile();
               }
               catch(...)
               {
                 printMEError("Cannot create tile object!");
                 delete lMapChunk;
                 return;
               }

               CAuxTileData* lTileData = lMapChunk->Get(i, j, k);

               if (lTileData)
               {
                 lTile->SetCoord(CPPoint(x + i * 3 * MAPCELLX, y + j * 3 * MAPCELLY));
                 lTile->SetType(lTileData->GetType());
                 lTile->SetSprite(GetGraphicInstance()->AddSprite(CPString(System::UTF8String(treeitem->fullName).c_str()) + lTileData->GetSprite(), 1, 1, SPRL_MISSION));

                 GetMissionInstance()->NoteInLayerMap(MOL_BASETILE, lTile, x/MAPCELLX + i*3, y/MAPCELLY + j*3);
                 lTile->NoteInLogicMap();
               }
             }
           }
         }

         delete lMapChunk;
         break;

      case OT_STATIC:
        try
        {
          PP::Stream_ANSI_C lA(treeitem->fullName.c_str());

          CSerString lId;

          lId.DeSerialize(lA);
          
          lStaticStruct.DeSerialize(lA);
        }
        catch(...)
        {
          printMEError("Cannot read static data!");
          return;
        }

        try
        {
          //if (CPString(lStaticStruct.objName) == CPString("Dropship"))
          //   lStatic = new CIGDropship();
          //else
          switch(lStaticStruct.type)
          {
            case ST_NORMAL:
            case ST_PILLBOX:
            case ST_TURRET:
              VLOG(5) << "Creating static object.";
              lStatic = new CIGStaticObject();
              break;

            case ST_FACTORY:
              VLOG(5) << "Creating static factory object.";
              lStatic = new CIGFactoryObject();
              break;
           }
        }
        catch(...)
        {
          printMEError("Cannot alloc static instance!");
          LOG(ERROR) << "UNABLE TO CREATE STATIC !!!!!!!!!!";
        }

        // update the sprite path for layers
        // the dat and the layers bitmaps MUST be in the same folder
        for(int i = 0; i < lStaticStruct.mLayersCount; i++)
        {
          strcpy(lStaticStruct.mLayers[i].mSprite, (CPString(treeitem->Path.c_str()) + CPString(lStaticStruct.mLayers[i].mSprite)).c_str());
          //strcpy(lStaticStruct.mLayers[i].mSprite, (CPString(treeitem->Path.c_str()) + CPString(lStaticStruct.mLayers[i].mSprite)).c_str());
        }

        lStatic->SetCoord(CPPoint(x, y));
        lSprite = GetGraphicInstance()->AddSprite(CPString(treeitem->Path.c_str()) + lStaticStruct.sprite, 1, 1, SPRL_MISSION);
        lStatic->SetSprite(lSprite);
        lStatic->SetStaticData(lStaticStruct);
        lStatic->SetBattleGroupId(_bg);
        lStatic->SetId(_id);

        lStatic->NoteInLayerMap();
        lStatic->NoteInLogicMap();

        //switch (lObjDesc->mSide)
        switch (GetMissionInstance()->GetBGManager()->GetRelation(OS_PLAYER, _bg))
        {
           case REL_NEUTRAL:
              GetMissionInstance()->GetObjects(MOL_NEUTRALSTATICS)->Add(lStatic);
              break;
           case REL_OWNED:
              GetMissionInstance()->GetObjects(MOL_OWNEDSTATICS)->Add(lStatic);
              break;
           case REL_FRIENDLY:
              GetMissionInstance()->GetObjects(MOL_FRIENDLYSTATICS)->Add(lStatic);
              break;
           case REL_ENEMY:
              GetMissionInstance()->GetObjects(MOL_ENEMYSTATICS)->Add(lStatic);
              break;
           default:
              // do nothing
              LOG(ERROR) << "Static relation to player unknown, static ejected.";
              break;
        }

        GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Add(lStatic);

        // add static to the battle group it belongs
        GetMissionInstance()->GetBGManager()->AddUnit(lStatic);
        break;

      case OT_INFANTRY:
          if (!getAIInfantryUnitStruct(treeitem->Path, treeitem->Index, &lInfStruct))
          {
            printMEError("Cannot read infantry data!");
          }

          // no point to put an object which is already dead
          if (lInfStruct.hp == 0)
          {
            return;
          }

          try
          {
             lInfUnit = new CIGInfantryUnit();
          }
          catch(...)
          {
             LOG(ERROR) << "UNABLE TO CREATE INFANTRY UNIT !!!!!!!!!!";
          }

          lInfUnit->SetCoord(CPPoint(x, y));
          lSprite = GetGraphicInstance()->AddSprite(getGamePath() + PATH_INF_GFX + lInfStruct.sprite, 8, 14, SPRL_MISSION);
          lInfUnit->SetSprite(lSprite);
          lInfUnit->SetBattleGroupId(_bg);
          lInfUnit->SetId(_id);
          lInfUnit->SetScriptObserver(GetMissionInstance()->GetDoctrine()->GetScriptObserver(_scriptId));
          lInfUnit->SetSelected(false);
          lInfUnit->SetInfUnifiedData(lInfStruct);
          
          // set the orientation of the unit to default. this will also set the visual to look in the same direction as its orientation
          lInfUnit->SetSenseCounter(0);

          lInfUnit->NoteInLayerMap();
          lInfUnit->NoteInLogicMap();

          switch (GetMissionInstance()->GetBGManager()->GetRelation(OS_PLAYER, _bg))
          {
             case REL_NEUTRAL:
                GetMissionInstance()->GetObjects(MOL_NEUTRALUNITS)->Add(lInfUnit);
                break;
             case REL_OWNED:
                GetMissionInstance()->GetObjects(MOL_OWNEDUNITS)->Add(lInfUnit);
                break;
             case REL_FRIENDLY:
                GetMissionInstance()->GetObjects(MOL_FRIENDLYUNITS)->Add(lInfUnit);
                break;
             case REL_ENEMY:
                GetMissionInstance()->GetObjects(MOL_ENEMYUNITS)->Add(lInfUnit);
                break;
             default:
                // do nothing
                LOG(ERROR) << "Infantry relation to player unknown, infantry unit ejected.";
                // continue; ?
                break;
          }

          GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Add(lInfUnit);

          // add infantry to the battle group it belongs
          GetMissionInstance()->GetBGManager()->AddUnit(lInfUnit);
          break;

      case OT_VEHICLE:
          if (!getAIVehicleUnitStruct(treeitem->Path, treeitem->Index, &lVehStruct))
          {
            printMEError("Cannot read vehicle data!");
          }

          // no point to put an object which is already dead
          if (lVehStruct.hp == 0)
          {
            return;
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
          }

          lVehicleUnit->SetCoord(CPPoint(x, y));
          lSprite = GetGraphicInstance()->AddSprite(getGamePath() + PATH_VEHICLES_GFX + lVehStruct.sprite, 8, 4, SPRL_MISSION);
          lVehicleUnit->SetSprite(lSprite);
          lVehicleUnit->SetBattleGroupId(_bg);
          lVehicleUnit->SetId(_id);
          lVehicleUnit->SetScriptObserver(GetMissionInstance()->GetDoctrine()->GetScriptObserver(_scriptId));
          lVehicleUnit->SetSelected(false);
          lVehicleUnit->SetVehicleUnifiedData(lVehStruct);

          lVehicleUnit->NoteInLayerMap();
          lVehicleUnit->NoteInLogicMap();

          switch (GetMissionInstance()->GetBGManager()->GetRelation(OS_PLAYER, _bg))
          {
             case REL_NEUTRAL:
                GetMissionInstance()->GetObjects(MOL_NEUTRALUNITS)->Add(lVehicleUnit);
                break;
             case REL_OWNED:
                GetMissionInstance()->GetObjects(MOL_OWNEDUNITS)->Add(lVehicleUnit);
                break;
             case REL_FRIENDLY:
                GetMissionInstance()->GetObjects(MOL_FRIENDLYUNITS)->Add(lVehicleUnit);
                break;
             case REL_ENEMY:
                GetMissionInstance()->GetObjects(MOL_ENEMYUNITS)->Add(lVehicleUnit);
                break;
             default:
                // do nothing
                LOG(ERROR) << "Vehicle relation to player unknown, vehicle unit ejected.";
                // continue; ?
                break;
          }

          GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Add(lVehicleUnit);

          // add the vehicle to the battle group it belongs
          GetMissionInstance()->GetBGManager()->AddUnit(lVehicleUnit);
         break;

      case OT_AIR:
          if (!getAIVehicleUnitStruct(treeitem->Path, treeitem->Index, &lVehStruct))
          {
            printMEError("Cannot read aircraft data!");
          }

          // no point to put an object which is already dead
          if (lVehStruct.hp == 0)
          {
            return;
          }

          switch(lVehStruct.vType)
          {
            case 4:
              try
              {
                 lAircraftUnit = new CIGFighterUnit();
              }
              catch(...)
              {
                 LOG(ERROR) << "UNABLE TO CREATE FIGHTER UNIT !!!!!!!!!!";
              }
              break;

            case 5:
              try
              {
                 //lAircraftUnit = new CIGBomberUnit();
              }
              catch(...)
              {
                 LOG(ERROR) << "UNABLE TO CREATE BOMBER UNIT !!!!!!!!!!";
              }
              break;

            case 6:
              try
              {
                 lAircraftUnit = new CIGChopperUnit();
              }
              catch(...)
              {
                 LOG(ERROR) << "UNABLE TO CREATE FIGHTER UNIT !!!!!!!!!!";
              }
              break;

            case 7:
              try
              {
                 lAircraftUnit = new CIGDropship();
              }
              catch(...)
              {
                 LOG(ERROR) << "UNABLE TO CREATE DROPSHIP UNIT !!!!!!!!!!";
              }
              break;
          }

          lAircraftUnit->SetCoord(CPPoint(x, y));
          lSprite = GetGraphicInstance()->AddSprite(getGamePath() + PATH_VEHICLES_GFX + lVehStruct.sprite, 8, 2, SPRL_MISSION);
          lAircraftUnit->SetSprite(lSprite);
          lAircraftUnit->SetBattleGroupId(_bg);
          lAircraftUnit->SetId(_id);
          lAircraftUnit->SetScriptObserver(GetMissionInstance()->GetDoctrine()->GetScriptObserver(_scriptId));
          lAircraftUnit->SetSelected(false);
          lAircraftUnit->SetAircraftUnifiedData(lVehStruct);

          lAircraftUnit->NoteInLayerMap();
          //lAircraftUnit->NoteInLogicMap();

          switch (GetMissionInstance()->GetBGManager()->GetRelation(OS_PLAYER, _bg))
          {
             case REL_NEUTRAL:
                GetMissionInstance()->GetObjects(MOL_NEUTRALUNITS)->Add(lAircraftUnit);
                break;
             case REL_OWNED:
                GetMissionInstance()->GetObjects(MOL_OWNEDUNITS)->Add(lAircraftUnit);
                break;
             case REL_FRIENDLY:
                GetMissionInstance()->GetObjects(MOL_FRIENDLYUNITS)->Add(lAircraftUnit);
                break;
             case REL_ENEMY:
                GetMissionInstance()->GetObjects(MOL_ENEMYUNITS)->Add(lAircraftUnit);
                break;
             default:
                // do nothing
                LOG(ERROR) << "Vehicle relation to player unknown, aircraft unit ejected.";
                // continue; ?
                break;
          }

          GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Add(lAircraftUnit);

          // add the aircraft to the battle group it belongs
          GetMissionInstance()->GetBGManager()->AddUnit(lAircraftUnit);
         break;
   }
}

/*---------------------------------------------------------------------------
 descrition: salveaza in DESKTOP_FILENAME coordonatele ferestrelor
---------------------------------------------------------------------------*/
void TFormMain::SaveDesktop()
{
   // windows prop
   mDesktop.SetWndPreview (CPRectConv(FormPreview->BoundsRect));
   mDesktop.SetWndRadar   (CPRectConv(FormRadar->BoundsRect));
   mDesktop.SetWndTree    (CPRectConv(FormTree->BoundsRect));
   mDesktop.SetWndTool    (CPRectConv(FormTool->BoundsRect));
   mDesktop.SetWndInfo    (CPRectConv(FormInfo->BoundsRect));
   mDesktop.SetWndAltitude(CPRectConv(FormAltitude->BoundsRect));
   mDesktop.SetWndPreviewVisible(FormPreview->Visible);
   mDesktop.SetWndRadarVisible   (FormRadar->Visible);
   mDesktop.SetWndTreeVisible    (FormTree->Visible);
   mDesktop.SetWndToolVisible    (FormTool->Visible);
   mDesktop.SetWndInfoVisible    (FormInfo->Visible);
   mDesktop.SetWndAltitudeVisible(FormAltitude->Visible);

   // environment
   mDesktop.SetSaveDesktopOnExit(FormPreferences->checkSaveDesktop->Checked);

   //grid prop
   mDesktop.SetGridShown (FormPreferences->checkShowGrid->Checked);
   mDesktop.SetGridH     (FormPreferences->comboGridHoriz->Text.ToInt());
   mDesktop.SetGridV     (FormPreferences->comboGridVert->Text.ToInt());
   mDesktop.SetGridColor (FormPreferences->shapeGridColor->Brush->Color);

   //altitude prop
   mDesktop.SetAltitudeMapColor   (FormPreferences->lbAltitudeFont->Font->Color);
   mDesktop.SetAltitudeBrushColor (FormPreferences->shapeAltitudeBrushColor->Pen->Color);
   mDesktop.SetAltitudeWidth      (FormPreferences->comboAltitudeWidth->Text.ToInt());
   mDesktop.SetAltitudeHeight     (FormPreferences->comboAltitudeHeight->Text.ToInt());

   //mask prop
   mDesktop.SetMaskMapShown(FormPreferences->checkShowMask->Checked);
   mDesktop.SetMaskMapColor(FormPreferences->shapeMaskColor->Brush->Color);

   if (!mDesktop.Save(getGamePath() + PATH_MEDITOR + FILE_MEDITOR_DESKTOP_CFG))
   {
      setMEError(ERR_OPEN);
      printMEError("Saving desktop");
      return;
   }
}

/*---------------------------------------------------------------------------
 descrition: incarca coordonatele ferestrelor din DEFAULT_DESKTOP_FILENAME
---------------------------------------------------------------------------*/
void TFormMain::LoadDesktop()
{
  if (!mDesktop.Load(getGamePath() + PATH_MEDITOR + FILE_MEDITOR_DESKTOP_CFG))
  {
    setMEError(ERR_OPEN);
    printMEError("Loading desktop");
    return;
  }

   //screens prop
   FormPreview->BoundsRect  = TRectConv(mDesktop.GetWndPreview());
   FormRadar->BoundsRect    = TRectConv(mDesktop.GetWndRadar());
   FormTree->BoundsRect     = TRectConv(mDesktop.GetWndTree());
   FormTool->BoundsRect     = TRectConv(mDesktop.GetWndTool());
   FormInfo->BoundsRect     = TRectConv(mDesktop.GetWndInfo());
   FormAltitude->BoundsRect = TRectConv(mDesktop.GetWndAltitude());

   FormPreview->Visible = mDesktop.GetWndPreviewVisible();
   FormRadar->Visible   = mDesktop.GetWndRadarVisible();
   FormTree->Visible    = mDesktop.GetWndTreeVisible();
   FormTool->Visible    = mDesktop.GetWndToolVisible();
   FormInfo->Visible    = mDesktop.GetWndInfoVisible();
   FormAltitude->Visible= mDesktop.GetWndAltitudeVisible();

   //environment
   FormPreferences->checkSaveDesktop->Checked = mDesktop.GetSaveDesktopOnExit();

   //grid prop
   FormPreferences->checkShowGrid->Checked = mDesktop.GetGridShown();
   FormPreferences->comboGridHoriz->ItemIndex   =
                  FormPreferences->comboGridHoriz->Items->IndexOf(AnsiString(mDesktop.GetGridH()));
   FormPreferences->comboGridVert->ItemIndex    =
                  FormPreferences->comboGridHoriz->Items->IndexOf(AnsiString(mDesktop.GetGridV()));
   FormPreferences->shapeGridColor->Brush->Color = mDesktop.GetGridColor();

   //Altitude prop
   FormAltitude->btnAltitude->Down = mDesktop.GetAltitude();
   FormAltitude->editWidth->Value  = mDesktop.GetAltitudeWidth();
   FormAltitude->editHeight->Value = mDesktop.GetAltitudeHeight();
   FormPreferences->shapeAltitudeBrushColor->Pen->Color = mDesktop.GetAltitudeBrushColor();
   FormPreferences->lbAltitudeFont->Font->Color    = mDesktop.GetAltitudeMapColor();
   FormPreferences->comboAltitudeWidth->ItemIndex  = mDesktop.GetAltitudeWidth() - 1;
   FormPreferences->comboAltitudeHeight->ItemIndex = mDesktop.GetAltitudeHeight() - 1;

   //mask prop
   FormPreferences->checkShowMask->Checked       = mDesktop.GetMaskMapShown();
   FormPreferences->shapeMaskColor->Brush->Color = mDesktop.GetMaskMapColor();
   //InitMask();
}


/*---------------------------------------------------------------------------
 descrition: seteaza zoomlevel
---------------------------------------------------------------------------*/
void TFormMain::setZoomLevel(int zoomlevel)
{
   zoomLevel = zoomlevel;
   if (zoomLevel == MINZOOMLEVEL)
   {
      itZoomOut->Enabled = true;
      itZoomIn->Enabled  = false;
   }
   if (zoomLevel == MAXZOOMLEVEL)
   {
      itZoomIn->Enabled  = true;
      itZoomOut->Enabled = false;
   }
   setViewPortDim();
//   paintMap();
   paintCanvas();
}

/*---------------------------------------------------------------------------
 descrition: seteaza numele hartii
---------------------------------------------------------------------------*/
void TFormMain::setMapName(AnsiString mapname)
{
   mapName = mapname;
   Caption = "";
   Caption = Caption + APPLICATION_CAPTION + " - " + mapName;
}

/*---------------------------------------------------------------------------
 descrition: seteaza flag-ul de compilare
---------------------------------------------------------------------------*/
void TFormMain::setCompiled(bool iscompiled)
{
   isCompiled = iscompiled;
   if (isCompiled)
      statusMain->Panels->Items[1]->Text = "compiled";
   else
      statusMain->Panels->Items[1]->Text = "not compiled";
}

/*---------------------------------------------------------------------------
 descrition: seteaza flagul de isModified
             aici se pot face enable/disable save
---------------------------------------------------------------------------*/
void TFormMain::setModified(bool ismodified)
{
   isModified = ismodified;
   itSave->Enabled = isModified;
   if (isModified == true)
   {
      setCompiled(false);
      statusMain->Panels->Items[0]->Text = "modified";
   }
   else
      statusMain->Panels->Items[0]->Text = "saved";
}

/*---------------------------------------------------------------------------
 descrition: salveaza harta in fisier
             am facut o metoda separata pentru ca este apelata din mai multe locuri
 parameters: numele hartii
---------------------------------------------------------------------------*/
void TFormMain::SaveMap(AnsiString mapname)
{
   //CArchive lA;

   // TODO: add visibility map paint
   // update the visibility map for the player (see no map)
   UpdateVisibility(0);

   try
   {
     PP::Stream_ANSI_C lA(CPString(mapname.c_str()), true);

     GetMissionInstance()->Serialize(lA);
     //GetMissionInstance()->SaveToFile(CPString(mapname.c_str()));
   }
   catch(...)
   {
     UpdateVisibility(2);
     printMEError("Saving map");
     return;
   }

   UpdateVisibility(2);
   setMapName(mapname);
   setModified(false);
}
/*---------------------------------------------------------------------------
 descrition: seteaza Max pentru scroll raportat la dim hartii si
             la dimensiunea lui viewScreen
 parameters: in coord. reale
---------------------------------------------------------------------------*/
void TFormMain::setScrollBounds(int mapwidth, int mapheight)
{
   //mapWidth  = mapwidth;
   //mapHeight = mapheight;
   if (mapwidth - viewWidth>0)
      scrollHoriz->Max = mapwidth - viewWidth;
   else
      scrollHoriz->Max = 0;
   if (mapheight - viewHeight>0)
      scrollVert->Max  = mapheight - viewHeight;
   else
      scrollVert->Max  = 0;

   scrollHoriz->Hint = AnsiString(scrollHoriz->Position) + "/" + AnsiString(mapWidth);
   scrollVert->Hint  = AnsiString(scrollVert->Position) + "/" + AnsiString(mapHeight);
}

/*---------------------------------------------------------------------------
 descrition: seteaza dimensiunea portiunii vizibile din harta
             apelata la resize si la zoom level modify
             apeleaza radar screen
---------------------------------------------------------------------------*/
void TFormMain::setViewPortDim()
{
   viewWidth       = ClientWidth;
   viewHeight      = ClientHeight;
   viewWidth      *= zoomLevel;
   viewHeight     *= zoomLevel;

   setScrollBounds(mapWidth * MAPCELLX, mapHeight * MAPCELLY);

   if (isApplicationClosing || isApplicationBeginning)
      return;
   if (FormRadar)
      FormRadar->setViewPortDim(viewWidth, viewHeight, mapWidth, mapHeight);
}

/*---------------------------------------------------------------------------
 descrition: seteaza left, top al lui viewport
 parameters: coordonata stanga-sus
             stiu ca este buna coord. pentru ca scroll-urile sunt limitate mai sus
---------------------------------------------------------------------------*/
void TFormMain::setViewPortCoord(int viewleft, int viewtop)
{
   scrollHoriz->Position = viewleft;
   scrollVert->Position  = viewtop;
   viewLeft   = scrollHoriz->Position;
   viewTop    = scrollVert->Position;
//   paintMap();
   paintCanvas();

   if (isApplicationClosing || isApplicationBeginning)
      return;
   if (FormRadar)
      FormRadar->setViewPortCoord(viewleft, viewtop);
}

/*---------------------------------------------------------------------------
 descrition: centreaza viewport in x,y - coord reale
             folosita la multe chesstii
---------------------------------------------------------------------------*/
void TFormMain::centerViewPort(int x, int y)
{
   setViewPortCoord(x-viewWidth/2, y-viewHeight/2);
}

/*---------------------------------------------------------------------------
 description: seteaza screenZone in functie de zona client
 others     : screenZone in coordonate ecran
---------------------------------------------------------------------------*/
void TFormMain::setScreenZone()
{
   screenZone = GetClientRect();
   *(POINT*)&(screenZone.Left)  = ClientToScreen(Point(screenZone.Left, screenZone.Top));
   *(POINT*)&(screenZone.Right) = ClientToScreen(Point(screenZone.Right, screenZone.Bottom));
}

/*---------------------------------------------------------------------------
 descrition: pune ce-i in bmpBack in Canvas
---------------------------------------------------------------------------*/
void TFormMain::Flip()
{
   try
   {
     GetGraphicInstance()->PaintToScreen();
   }
   catch(...)
   {
     printMEError("Paint To Screen!");
     Application->Terminate();
   }  
}

/*---------------------------------------------------------------------------
 descrition: apeleaza getMask si o nterpreteaza punand un bitmap rosu peste
             patratelele respectiv
---------------------------------------------------------------------------*/
void TFormMain::paintFloat()
{
   try
   {
     GetGraphicInstance()->PaintSpriteToSurface(
       NULL, floatLeft, floatTop,
       mSpriteFloat, 0, 0, zoomLevel
       );
   }
   catch(...)
   {
     printMEError("TFormMain::paintFloat - cannot paint float surface!");
   }

   // preferences - show mask?
   /*
   if (!mDesktop.GetMaskMapShown())
   {
     return;
   }
   */

   // paint a combined mask: of the float object and of the map
   for (int i=0; i < itemMask.Width; i++)
   {
      for (int j=0; j < itemMask.Height; j++)
      {
         if (itemMask.Mask[i][j] != 0  && !GetMissionInstance()->IsGroundPlaceEmpty(NULL, floatLeft/MAPCELLX + i, floatTop/MAPCELLY + j))
         {
           GetGraphicInstance()->PaintSpriteToSurface(NULL, floatLeft + i * MAPCELLX, floatTop + j * MAPCELLY, mSpriteMask, 0, 0, zoomLevel);
         }
      }
   }
}

/*---------------------------------------------------------------------------
 descrition: apeleaza paint al hartii pentru noile dimensiuni
             si pozitii ale scrollbar-urilor
             se deseneaza in bmpMap
---------------------------------------------------------------------------*/
void TFormMain::paintMap()
{
  GetMissionInstance()->Paint(                      //punctul din harta
                        CPPoint(0, 0),
                        CPPoint(viewLeft, viewTop), //punctul unde se face tiparirea
                                                    //(pe back surface este originea)
                        viewWidth, viewHeight,      //dimensiunea lui view, cred ca ar trebui sa fie
                                                    //dimeniunea lui rcWindow
                        zoomLevel);                 //nivelul de zoom (is not used)
}

/*---------------------------------------------------------------------------
 descrition: picteaza harta de inaltimi
---------------------------------------------------------------------------*/
void TFormMain::paintAltitudeMap()
{
   int logicleft   = viewLeft   / MAPCELLX;
   int logictop    = viewTop    / MAPCELLY;
   int logicwidth  = viewWidth  / MAPCELLX + 1;
   int logicheight = viewHeight / MAPCELLY + 1;

   mAltitudeFont->SetColor(FormPreferences->lbAltitudeFont->Font->Color);

   for (int i = logicleft;  i < logicleft + logicwidth + 1; i++)
   {
      for (int j = logictop; j < logictop + logicheight + 1; j++)
      {
         GetGraphicInstance()->TextOut2(
                   i*MAPCELLX - viewLeft,
                   j*MAPCELLY - viewTop,
                   CPString(GetMissionInstance()->GetMap()->GetAltitude(i, j)),
                   TA_LEFT
                   );

         /*
         COLORREF lColor = GetMissionInstance()->GetMap()->GetAltitude(i, j) << 16;

         GetGraphicInstance()->FillRectangle(NULL,
                   CPRect(
                   i*MAPCELLX - viewLeft,
                   j*MAPCELLY - viewTop,
                   (i+1)*MAPCELLX - viewLeft,
                   (j+1)*MAPCELLY - viewTop),
                   lColor
                   //CPString(GetMissionInstance()->GetMap()->GetAltitude(i, j)),
                   //TA_LEFT
                   );
         */          
      }
   }   
}

/*---------------------------------------------------------------------------
 descrition: deseneaza grid dupa Grid Horizontal dimension si GridVert dimension
---------------------------------------------------------------------------*/
void TFormMain::paintGrid()
{
   int logicleft   = scrollHoriz->Position / mDesktop.GetGridH();
   int logictop    = scrollVert->Position  / mDesktop.GetGridV();
   int logicwidth  = viewWidth  / mDesktop.GetGridH() + 1;
   int logicheight = viewHeight / mDesktop.GetGridV() + 1;

   /*
   HPEN hPen;
   hPen = CreatePen( PS_SOLID, 0, mDesktop.GetGridColor());

   GetGraphicInstance()->SetPen(hPen);
   */

   for (int i = logicleft; i < logicleft + logicwidth + 1; i++)
   {
      GetGraphicInstance()->Line(NULL,
             CPPoint(i * mDesktop.GetGridH() - scrollHoriz->Position, 0),
             CPPoint(i * mDesktop.GetGridH() - scrollHoriz->Position, viewHeight),
             mDesktop.GetGridColor()
             );
   }

   for (int i = logictop; i < logictop + logicheight + 1; i++)
   {
      GetGraphicInstance()->Line( NULL,
             CPPoint(0,         i * mDesktop.GetGridV() - scrollVert->Position),
             CPPoint(viewWidth, i * mDesktop.GetGridV() - scrollVert->Position),
             mDesktop.GetGridColor()
             );
   }
}

/*---------------------------------------------------------------------------
 descrition: deseneaza mask map
---------------------------------------------------------------------------*/
void TFormMain::paintMaskMap()
{
  for(int x = viewLeft / MAPCELLX; x < (viewLeft + viewWidth)/MAPCELLX; x++)
  {
    for(int y = viewTop / MAPCELLY; y < (viewTop + viewHeight)/MAPCELLY; y++)
    {
      if (!GetMissionInstance()->IsGroundPlaceEmpty(NULL, x, y))
      {
        GetGraphicInstance()->PaintSpriteToSurface(NULL, x * MAPCELLX - viewLeft, y * MAPCELLY - viewTop, mSpriteMask, 0, 0, zoomLevel);
      }
    }
  }
}

/*---------------------------------------------------------------------------
 descrition: deseneaza in bmpBack harta, obiectul de pus si face flip
---------------------------------------------------------------------------*/
void TFormMain::paintCanvas()
{
   //GetMissionInstance()->Paint(CPPoint(0, 0), CPPoint(viewLeft, viewTop), viewWidth, viewHeight, zoomLevel);
   paintMap();

   if (selectedItem && mSpriteFloat)
   {
      paintFloat();
   }

   if (mDesktop.GetGridShown())
   {
      paintGrid();
   }

   if (mDesktop.GetMaskMapShown())
   {
      paintMaskMap();
   }

   if (isSelecting)
   {
      Types::TPoint tmp = ScreenToClient(Mouse->CursorPos);
      GetGraphicInstance()->Rectangle(NULL,
                                  CPRect(min(selectX - scrollHoriz->Position , (int)tmp.x),
                                  min(selectY - scrollVert->Position, (int)tmp.y),
                                  max(selectX - scrollHoriz->Position, (int)tmp.x),
                                  max(selectY - scrollVert->Position, (int)tmp.y)),
                                  0xFFFFFF
                  );
   }

   if (mDesktop.GetAltitude())
   {
      paintAltitudeMap();
      Types::TPoint tmp;
      tmp = ScreenToClient(Mouse->CursorPos);
      GetGraphicInstance()->FillRectangle(NULL,
                                          CPRect(altitudeLeft,
                                          altitudeTop,
                                          altitudeLeft + MAPCELLX * FormAltitude->editWidth->Value,
                                          altitudeTop + MAPCELLY * FormAltitude->editHeight->Value
                                          ),
                                          0x0000FF
                                          );
      //bmpBack->Canvas->Brush->Style = bsSolid;
      //bmpBack->Canvas->Brush->Color = altitudeBrushColor;
      //bmpBack->Canvas->FrameRect(Rect(altitudeLeft,
      //                                altitudeTop,
      //                                altitudeLeft + MAPCELLX * FormAltitude->editWidth->Value,
      //                                altitudeTop + MAPCELLY * FormAltitude->editHeight->Value));
   }

   Flip();
}

/*---------------------------------------------------------------------------
 descrition: creaza o harta noua data prin mapwidth, mapheight si landscape
 parameters: mapwidth si mapheight - coord. logice
---------------------------------------------------------------------------*/
void TFormMain::createNewMap(int mapwidth, int mapheight, AnsiString landscape)
{
   setModified(true);
   setMapName(NONAME_MAP);
   setScrollBounds(mapwidth * MAPCELLX, mapheight * MAPCELLY);
   mapWidth  = mapwidth;
   mapHeight = mapheight;

   // release the mission
   GetMissionInstance()->Release();

   // release the mission specific sprites
   GetGraphicInstance()->ReleaseAllSprites(SPRL_MISSION);

   //GetMissionInstance();
   GetMissionInstance()->GetMap()->InitDimensions(mapWidth * MAPCELLX, mapHeight * MAPCELLY);

   // add landscape
   CPPIG_BattleGroup* lBg = new CPPIG_BattleGroup("Landscape", "Props belong here.", OS_LANDSCAPE);
   GetMissionInstance()->GetBGManager()->AddBGroup(lBg);

   // add player battle group
   lBg = new CPPIG_BattleGroup("Player", "For units belonging to player.", OS_PLAYER);
   GetMissionInstance()->GetBGManager()->AddBGroup(lBg);

   setCurrentBGroup(OS_LANDSCAPE);

   // register a view
   GetMissionInstance()->GetMap()->RegisterView(OS_PLAYER);

   // see all map for the player
   UpdateVisibility(2);

   paintCanvas();
}

/*---------------------------------------------------------------------------
 descrition: noteaza selectia din tree
             o foloseste atunci cand se pune in harta
 parameters: pointer la structura
---------------------------------------------------------------------------*/
void TFormMain::setSelectedItem(TTreeItem *selecteditem)
{
   bool wasselected = selectedItem != NULL;
   int i,j;
   CAuxMapChunk* lMapChunk;
   AnsiString path1, path2;

   selectedItem = selecteditem;
   FormPreview->setSelectedItem(selectedItem);

   if (mSpriteFloat)
   {
      GetGraphicInstance()->ReleaseSprite(mSpriteFloat);
      mSpriteFloat = NULL;
   }

   if (selectedItem)
   {
      switch(selectedItem->objType)
      {
         case OT_TILE:
            lMapChunk = getMapChunk(selectedItem->fullName);

            if (!lMapChunk)
            {
              break;
            }

            //mSpriteFloat = new CSpriteCustom(lMapChunk->GetSize() * TILEDIM, lMapChunk->GetSize() * TILEDIM, 1, 1);
            mSpriteFloat = GetGraphicInstance()->AddSpriteCustom(lMapChunk->GetSize() * TILEDIM, lMapChunk->GetSize() * TILEDIM, 1, 1, SPRL_GENERIC);

            for(int i = 0; i < lMapChunk->GetSize(); i++)
            {
              for(int j = 0; j < lMapChunk->GetSize(); j++)
              {
                if (lMapChunk->GetDepth(i, j) <= 0)
                {
                  continue;
                }

                for(int k = 0; k < lMapChunk->GetDepth(i, j); k++)
                {
                  CAuxTileData* lData = lMapChunk->Get(i, j, k); // :)
                  path1 = selectedItem->Path + lData->GetSprite().c_str();

                  CPGISprite* lSprite = GetGraphicInstance()->AddSprite(CPString(selectedItem->Path.c_str()) + lData->GetSprite(), 1, 1, SPRL_MISSION);

                  GetGraphicInstance()->PaintSpriteToSurface(mSpriteFloat->GetSurface(), i * TILEDIM, j * TILEDIM, lSprite, 0, 0);
                }
              }
            }

            floatWidth   = mSpriteFloat->GetWidth();
            floatHeight  = mSpriteFloat->GetHeight();
            statusMain->Panels->Items[3]->Text = selectedItem->objName;

            // itemMask for map chunk - no item mask
            itemMask.Width  = 0;
            itemMask.Height = 0;

            delete lMapChunk;

            break;

         case OT_STATIC:
            mSpriteFloat = GetGraphicInstance()->AddSprite((selectedItem->Path + getObjectSpriteName(selectedItem->fullName)).c_str(), 1, 1, SPRL_MISSION);

            floatWidth   = mSpriteFloat->GetWidth();
            floatHeight  = mSpriteFloat->GetHeight();

            itemMask = getObjectMask(selectedItem->fullName);
            statusMain->Panels->Items[3]->Text = selectedItem->objName;
            break;
            
         case OT_INFANTRY:
            mSpriteFloat = GetGraphicInstance()->AddSprite(
              (getGamePath() + PATH_INF_GFX + getObjectSpriteName(AnsiString(getGamePath().c_str()) + PATH_AIINFANTRY + FILE_AIINFANTRY, OT_INFANTRY, selectedItem->Index)).c_str(),
              8, 14, SPRL_MISSION);
                                              
            floatWidth  = mSpriteFloat->GetWidth()/8;
            floatHeight = mSpriteFloat->GetHeight()/14;

            statusMain->Panels->Items[3]->Text = selectedItem->objName;
            itemMask.Width  = (mSpriteFloat->GetWidth() / 8) / MAPCELLX;
            itemMask.Height = (mSpriteFloat->GetHeight() / 14) / MAPCELLY;
            
            for (i=0; i<itemMask.Width; i++)
            {
               for (j=0; j<itemMask.Height; j++)
               {
                  itemMask.Mask[i][j] = 1;
               }
            }      
            break;

         case OT_VEHICLE:
            mSpriteFloat = GetGraphicInstance()->AddSprite(
              (getGamePath() + PATH_VEHICLES_GFX + getObjectSpriteName(AnsiString(getGamePath().c_str()) + PATH_AIVEHICLE + FILE_AIVEHICLE, OT_VEHICLE, selectedItem->Index)).c_str(),
              8, 4, SPRL_MISSION);

            floatWidth  = mSpriteFloat->GetWidth()/8;
            floatHeight = mSpriteFloat->GetHeight()/4;

            statusMain->Panels->Items[3]->Text = selectedItem->objName;
            itemMask.Width  = (mSpriteFloat->GetWidth() / 8) / MAPCELLX;
            itemMask.Height = (mSpriteFloat->GetHeight() / 4) / MAPCELLY;

            for (i = 0; i < itemMask.Width; i++)
            {
               for (j = 0; j < itemMask.Height; j++)
               {
                  itemMask.Mask[i][j] = 1;
               }
            }
            break;
            
         case OT_AIR:
            mSpriteFloat = GetGraphicInstance()->AddSprite(
              (getGamePath() + PATH_VEHICLES_GFX + getObjectSpriteName(AnsiString(getGamePath().c_str()) + PATH_AIVEHICLE + FILE_AIVEHICLE, OT_AIR, selectedItem->Index)).c_str(),
              8, 2, SPRL_MISSION);

            floatWidth  = mSpriteFloat->GetWidth()/8;
            floatHeight = mSpriteFloat->GetHeight()/1;

            statusMain->Panels->Items[3]->Text = selectedItem->objName;
            itemMask.Width  = (mSpriteFloat->GetWidth() / 8) / MAPCELLX;
            itemMask.Height = (mSpriteFloat->GetHeight() / 1) / MAPCELLY;

            for (i = 0; i < itemMask.Width; i++)
            {
               for (j = 0; j < itemMask.Height; j++)
               {
                  itemMask.Mask[i][j] = 0;
               }
            }
            break;
      }

      if(mDesktop.GetAltitude())
      {
         endAltitude();
      }
   }
   else
   {
      statusMain->Panels->Items[3]->Text = "";
   }   

   if (wasselected != (selectedItem != NULL))
   {
      paintCanvas();
      actCancel->Enabled = selectedItem != NULL;
   }
}

/*---------------------------------------------------------------------------
 descrition: incepe selectia din punctul dat
 parameters: xbegin, ybegin - in coord. reale de harta
---------------------------------------------------------------------------*/
void TFormMain::beginSelection(int xbegin, int ybegin)
{
   isSelecting = true;
   selectX = xbegin;
   selectY = ybegin;
}

/*---------------------------------------------------------------------------
 descrition: termina selectia in punctul dat
 parameters: xend, yend - in coord. reale de harta
---------------------------------------------------------------------------*/
void TFormMain::endSelection(int xend, int yend)
{
   int tmp;
   isSelecting = false;
   if (xend<selectX)
   {
      tmp      = xend;
      xend     = selectX;
      selectX  = tmp;
   }
   if (yend<selectY)
   {
      tmp      = yend;
      yend     = selectY;
      selectY  = tmp;
   }
   if (selectX < 0)
      selectX = 0;
   if (selectY < 0)
      selectY = 0;
   if (xend > mapWidth * MAPCELLX)
      xend = mapWidth;
   if (yend >mapHeight * MAPCELLY)
      yend = mapHeight;

   selectEndX = xend;
   selectEndY = yend;

   // little hack, little hack
   GetMissionInstance()->SetCurrentAction(MA_SELECT);
   GetMissionInstance()->ResetAction();
   GetMissionInstance()->SetFlag(MF_ADDSELECTION, true);
   GetMissionInstance()->SetFlag(MF_INVERTSELECTION, false);
   GetMissionInstance()->SetBeginPoint(CPPoint(selectX, selectY));
   GetMissionInstance()->SetEndPoint(CPPoint(xend, yend));
   GetMissionInstance()->MakeSelection();

   // remove units that don't match the selection criteria
   RecomputeSelection();

   bool lSelectionEmpty = (GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS)->Count() <= 0) &&
                          (GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDOTHERS)->Count() <= 0) &&
                          (GetMissionInstance()->GetObjects(MOL_SELECTEDOTHERS)->Count() <= 0);

   actCancel->Enabled           = !lSelectionEmpty;
   itDelete->Enabled            = !lSelectionEmpty;
   actObjectProperties->Enabled = !lSelectionEmpty;

   /*
   theMission->selectObjects(selectX, selectY, xend, yend);

   actCancel->Enabled = !theMission->selectedObjects->isEmpty();
   itDelete->Enabled    = !theMission->selectedObjects->isEmpty();
   actObjectProperties->Enabled = !theMission->selectedObjects->isEmpty();
   */
   
   paintCanvas();
}

/*---------------------------------------------------------------------------
 descrition: opreste selectia si deselecteaza obiectele selectate
---------------------------------------------------------------------------*/
void TFormMain::cancelSelection()
{
   actCancel->Enabled           = false;
   itDelete->Enabled            = false;
   actObjectProperties->Enabled = false;
   isSelecting                  = false;

   GetMissionInstance()->CancelSelection();
   paintCanvas();
}

/*---------------------------------------------------------------------------
 descrition: sterge obiectele selectate
---------------------------------------------------------------------------*/
void TFormMain::deleteSelection()
{
   actCancel->Enabled           = false;
   itDelete->Enabled            = false;
   actObjectProperties->Enabled = false;

   // doh
   bool lSelectionEmpty = (GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS)->Count() <= 0) &&
                          (GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDOTHERS)->Count() <= 0) &&
                          (GetMissionInstance()->GetObjects(MOL_SELECTEDOTHERS)->Count() <= 0);

   if (lSelectionEmpty)
   {
     return;
   }

   // remove all selected units from: layer map, logic map and all objects
   CIGBaseObjectList* lList = GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS);
   CIGBaseObject*     lObj;

   while (lList->Count() > 0)
   {
     lObj = lList->GetObject(0);
     lObj->RemoveFromLayerMap();
     lObj->RemoveFromLogicMap();
     GetMissionInstance()->AddToDeleteObjects(lObj);
     GetMissionInstance()->GetBGManager()->RemoveUnit((CIGRealObject*)lObj);
   }

   // remove selected owned others units from logic and layer map
   lList = GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDOTHERS);

   while (lList->Count() > 0)
   {
     lObj = lList->GetObject(0);
     lObj->RemoveFromLayerMap();
     lObj->RemoveFromLogicMap();
     GetMissionInstance()->AddToDeleteObjects(lObj);
     GetMissionInstance()->GetBGManager()->RemoveUnit((CIGRealObject*)lObj);
   }

   // remove selected others units from logic and layer map
   lList = GetMissionInstance()->GetObjects(MOL_SELECTEDOTHERS);

   while (lList->Count() > 0)
   {
     lObj = lList->GetObject(0);
     lObj->RemoveFromLayerMap();
     lObj->RemoveFromLogicMap();
     GetMissionInstance()->AddToDeleteObjects(lObj);
     GetMissionInstance()->GetBGManager()->RemoveUnit((CIGRealObject*)lObj);
   }

   // now delete the objects!
   GetMissionInstance()->GetToDeleteObjects()->FreeAll();

   paintCanvas();
}

/*---------------------------------------------------------------------------
 descrition: seteaza side al obiectulor care vor fi puse
---------------------------------------------------------------------------*/
void TFormMain::setCurrentBGroup(int _bg)
{
   currentBGroup = _bg;
   //theMission->setCurrentBGroup(_bg);
}

/*---------------------------------------------------------------------------
 descrition: adica incepe sa se lucreze pe harta de inaltimi
             se deselecteaza totul
---------------------------------------------------------------------------*/
void TFormMain::beginAltitude()
{
   setSelectedItem(NULL);
   cancelSelection();
   mDesktop.SetAltitude(true);
   paintCanvas();
}

/*---------------------------------------------------------------------------
 descrition: s-a terminat de lucrat cu harta de inaltimi
---------------------------------------------------------------------------*/
void TFormMain::endAltitude()
{
   mDesktop.SetAltitude(false);
   FormAltitude->btnAltitude->Down = false;
   paintCanvas();
}

/*---------------------------------------------------------------------------
 descrition: seteaza altitudinea data in harta
---------------------------------------------------------------------------*/
void TFormMain::setAltitude()
{
   /*
   theMission->setAltitude(altitudeLeft + scrollHoriz->Position,
                           altitudeTop + scrollVert->Position,
                           altitudeLeft + scrollHoriz->Position + CELLX * FormAltitude->editWidth->Value,
                           altitudeTop + scrollVert->Position + CELLY * FormAltitude->editHeight->Value,
                           FormAltitude->editAltitude->Value);
   */

   for(int x = (altitudeLeft + scrollHoriz->Position)/MAPCELLX; x < (altitudeLeft + scrollHoriz->Position)/MAPCELLX + FormAltitude->editWidth->Value; x++)
   {
     for(int y = (altitudeTop + scrollVert->Position)/MAPCELLY; y < (altitudeTop + scrollVert->Position)/MAPCELLY + FormAltitude->editHeight->Value; y++)
     {
       GetMissionInstance()->GetMap()->SetAltitude(x, y, FormAltitude->editAltitude->Value);
     }
   }

   setModified(false);
   setCompiled(false);
   paintCanvas();
}

/*---------------------------------------------------------------------------
---------------------------------------------------------------------------
 Descrition: PUBLISHED AREA
---------------------------------------------------------------------------
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
 descrition: aici se initializeaza mission si alte chestiuni mai marunte dar
             nu mai putin importante :)
---------------------------------------------------------------------------*/
void __fastcall TFormMain::FormCreate(TObject *Sender)
{
  initGamePath();

  setMapName(NONAME_MAP);
  isModified = true;
  isCompiled = false;
  selectedItem = NULL;

  isCaptured = false;     //nefolosit inca se refera la mouse

  floatLeft = -1;
  floatTop  = -1;
  zoomLevel = MINZOOMLEVEL;

  currentBGroup = OS_LANDSCAPE;

  isSelecting = false;
  selectX = selectY = selectEndX = selectEndX = 0;
  selectionColor = clWhite;

  try
  {
    mAltitudeFont = new CPFont("Small Font");
    mAltitudeFont->SetSize(8);
  }
  catch(...)
  {
  }

  // desktop settings
  mDesktop.SetAltitude(false);
  mDesktop.SetAltitudeWidth(0);
  mDesktop.SetAltitudeHeight(0);
  mDesktop.SetAltitudeMapColor(clGray);
  mDesktop.SetAltitudeBrushColor(clBlack);
  mDesktop.SetGridShown(false);
  mDesktop.SetGridColor(clBlack);
  mDesktop.SetGridH(32);
  mDesktop.SetGridV(32);
  mDesktop.SetMaskMapShown(false);
  mDesktop.SetMaskMapColor(clRed);
  mDesktop.SetSaveDesktopOnExit(true);

  // application settings
  isApplicationClosing   = false;
  isApplicationBeginning = true;

  // surfaces
  mSpriteFloat = NULL;
  mSpriteMask  = NULL;
  mTest        = NULL;

  Application->OnHint    = displayHint;

  dialogOpen->InitialDir = (getGamePath() + PATH_MISSIONS).c_str();
  dialogSave->InitialDir = (getGamePath() + PATH_MISSIONS).c_str();

   /*
   try
   {
     GetMissionInstance();
   }
   catch(...)
   {
      setMEError(ERR_ALLOCATE);
      Application->Terminate();
   }
   */

  ControlStyle << csCaptureMouse;        //asta ca sa captureze mouse-ul
  isApplicationBeginning = false;

  mFileHistory = new CFileHistory(OpenRecent1, &HistoryClick, HISTORY_MENU_MAX_ITEMS);

  // file history is not managed by the desktop class, it is managed by FMain
  // it is enough to set it once
  mDesktop.SetFileHistory(mFileHistory);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::HistoryClick(TObject *Sender)
{
  AnsiString lFileName = ((TMenuItem*)Sender)->Caption;

  try
  {
    // TODO integrate VFS
    PP::Stream_ANSI_C lA(CPString(lFileName.c_str()));

    GetMissionInstance()->DeSerialize(lA);

    UpdateVisibility(2);
  }
  catch(...)
  {
     printMEError("Loading map");
     Application->Terminate();
  }

  setModified(false);
  setMapName(lFileName);
  paintCanvas();

  mapWidth  = GetMissionInstance()->GetMap()->GetLogicWidth();
  mapHeight = GetMissionInstance()->GetMap()->GetLogicHeight();

  setScrollBounds(mapWidth * MAPCELLX, mapHeight * MAPCELLY);
}
//---------------------------------------------------------------------------

void TFormMain::MakeClean()
{
   isApplicationClosing = true;
   LOG(INFO) << "MISSION OVER";

   //Path Server
   try
   {
      CIGPathServer *lPSI;
      lPSI->Release();
   }
   catch(...)
   {
      LOG(ERROR) << "UNABLE TO REMOVE PATH SERVER !!!!!!!!!!!";
   }
   //Path Server - over

   //Mission
   try
   {
      CIGMission *lMI;
      lMI->Release();
   }
   catch(...)
   {
     LOG(ERROR) << "UNABLE TO REMOVE MISSION !!!!!!!!!!!";
   }
   //Mission - over

   //Space Sound System
   try
   {
      CIGSpaceSoundSystem *lSSSI;
      lSSSI->Release();
   }
   catch(...)
   {
      LOG(ERROR) << "UNABLE TO REMOVE SPACE SOUND SYSTEM !!!!!!!!!!!";
   }
   //Space Sound System - over

   //Direct Sound
   try
   {
      CPSISoundSystem *lSSI;
      lSSI->Release();
   }
   catch(...)
   {
      LOG(ERROR) << "UNABLE TO REMOVE DIRECT SOUND !!!!!!!!!!!";
   }
   //Direct Sound - over

   //Direct Draw
   try
   {
      CPGIGraphicSystem *lGSI;
      lGSI->Release();
   }
   catch(...)
   {
      LOG(ERROR) << "UNABLE TO REMOVE DIRECT DRAW !!!!!!!!!!!";
   }
   //Direct Draw - over

   LOG(INFO) << "ALL MISSION DATA REMOVED FROM MEMORY";
   LOG(INFO) << "\nPP IN-GAME LOG FILE OVER";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormDestroy(TObject *Sender)
{
  // bailing out!
  delete mFileHistory;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: apeleaza new map dupa care face load dekstop
 others    : daca nu se sel. un landscape => app. over
---------------------------------------------------------------------------*/
void __fastcall TFormMain::FormShow(TObject *Sender)
{
   if (FormNewMap->ShowModal() == mrOk)
   {
      InitMapEditorSession();

      int mapwidth, mapheight; //variabile temporare
      AnsiString landscape;
      mapwidth  = atoi(AnsiString(FormNewMap->edMapWidth->Text).c_str());
      mapheight = atoi(AnsiString(FormNewMap->edMapHeight->Text).c_str());
      landscape = FormNewMap->comboLandscape->Text;
      createNewMap(mapwidth, mapheight, landscape);

      FormMapGen->Initialize(landscape);
      
      FormMapGen->Generate();  //asta ii un fel de carpeala, da' merge :)

      LoadDesktop();
   }
   else
      Application->Terminate();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: apeleaza chestia cu dialogul
---------------------------------------------------------------------------*/
void __fastcall TFormMain::FormClose(TObject *Sender, TCloseAction &Action)
{
   if (isModified)
      switch (Application->MessageBox(L"The map is not saved. Do you want to save it ?",
                                      L"Warning", MB_YESNOCANCEL))
      {
         case IDYES: //save the file
            if (mDesktop.GetSaveDesktopOnExit())
            {
               SaveDesktop();
            }

            itSaveClick(Sender);
            Action = caFree;
            MakeClean();
            break;
            
         case IDNO:
            if (mDesktop.GetSaveDesktopOnExit())
            {
               SaveDesktop();
            }

            MakeClean();
            Action = caFree;
            break;

         case IDCANCEL:
            Action = caNone;
            break;
      }
    else
    {
      if (mDesktop.GetSaveDesktopOnExit())
      {
         SaveDesktop();
      }

      MakeClean();
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 Descrition: MENU FUNCTIONS
---------------------------------------------------------------------------*/
void __fastcall TFormMain::itExitClick(TObject *Sender)
{
   if (isModified)
   {
      switch (Application->MessageBox(L"The map is not saved. Do you want to save it ?",
                                      L"Warning", MB_YESNOCANCEL))
      {
         case IDYES: //saves the file and exits
            if (mDesktop.GetSaveDesktopOnExit())
            {
               SaveDesktop();
            }

            itSaveClick(Sender);
            Application->Terminate();
            break;
         case IDNO:  //exitx
            if (mDesktop.GetSaveDesktopOnExit())
            {
               SaveDesktop();
            }

            Application->Terminate();
            break;
         case IDCANCEL:
            return;
      }
   }
   else
   {
      if (mDesktop.GetSaveDesktopOnExit())
      {
         SaveDesktop();
      }

      MakeClean();
      
      Application->Terminate();
   }   
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itNewClick(TObject *Sender)
{
   if (isModified)
   {//trebuie anuntat utilizatorul
      switch (Application->MessageBox(L"The map is not saved. Do you want to save it ?",
                                      L"Warning", MB_YESNOCANCEL))
      {
         case IDYES: //save the file
            itSaveClick(Sender);
            break;
         case IDCANCEL:
            return;
      }
   }

   if (FormNewMap->ShowModal() == mrOk)
   {
      int mapwidth, mapheight; //variabile temporare
      AnsiString landscape;
      mapwidth  = atoi(AnsiString(FormNewMap->edMapWidth->Text).c_str());
      mapheight = atoi(AnsiString(FormNewMap->edMapHeight->Text).c_str());
      landscape = FormNewMap->comboLandscape->Text;
      createNewMap(mapwidth, mapheight, landscape);
      FormMapGen->Initialize(landscape);
      FormMapGen->Generate(); //asta ii un fel de carpeala, da' merge :)
      paintCanvas();
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itOpenClick(TObject *Sender)
{
   dialogOpen->InitialDir = AnsiString(getGamePath().c_str()) + PATH_MISSIONS;

   if (dialogOpen->Execute())
   {
      try
      {
#if PPT_USE_VFS
        PP::Stream_PhysFS lA(CPString(PATH_MISSIONS) + CPString(System::UTF8String(ExtractFileName(dialogOpen->FileName)).c_str()));
#else
        PP::Stream_ANSI_C lA(CPString(System::UTF8String(dialogOpen->FileName).c_str()));
#endif

        GetMissionInstance()->DeSerialize(lA);
        //GetMissionInstance()->LoadFromFile(dialogOpen->FileName.c_str());
        UpdateVisibility(2);

        mFileHistory->Add(System::UTF8String(dialogOpen->FileName).c_str());
      }
      catch(...)
      {
         printMEError("Loading map");
         Application->Terminate();
      }

      setModified(false);
      setMapName(dialogOpen->FileName);
      paintCanvas();

      mapWidth  = GetMissionInstance()->GetMap()->GetLogicWidth();
      mapHeight = GetMissionInstance()->GetMap()->GetLogicHeight();

      setScrollBounds(mapWidth * MAPCELLX, mapHeight * MAPCELLY);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itSaveClick(TObject *Sender)
{
   if (mapName == NONAME_MAP)
   {
      itSaveAsClick(Sender);
   }
   else
   {
//      SaveMap(dialogSave->FileName);
      SaveMap(mapName);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itSaveAsClick(TObject *Sender)
{
   if (dialogSave->Execute())
   {
      SaveMap(dialogSave->FileName);
   }   
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itZoomInClick(TObject *Sender)
{
   setZoomLevel(MINZOOMLEVEL);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itZoomOutClick(TObject *Sender)
{
   setZoomLevel(MAXZOOMLEVEL);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itEventManagerClick(TObject *Sender)
{
//   FormEventManager->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itMissionDatasClick(TObject *Sender)
{
   FormMissionInfo->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itTestClick(TObject *Sender)
{
   //CArchive lA;

   SetCurrentDir(AnsiString(getGamePath().c_str()) + PATH_MEDITOR);

   try
   {
     PP::Stream_ANSI_C lA(getGamePath() + PATH_MEDITOR);
     GetMissionInstance()->Serialize(lA);
     //GetMissionInstance()->SaveToFile(getGamePath() + PATH_MEDITOR + "Test.tpm");
   }
   catch(...)
   {
     printMEError("Saving test map");
     return;
   }

   WinExec((AnsiString(getGamePath().c_str()) + PATH_MEDITOR + "PPTactical_SDL_mingw.exe").c_str(), SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itRefreshTreeClick(TObject *Sender)
{
   FormTree->refreshTree();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itInfoClick(TObject *Sender)
{
   if (itInfo->Checked)
      FormInfo->Hide();
   else
      FormInfo->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itPreviewClick(TObject *Sender)
{
   if (itPreview->Checked)
      FormPreview->Hide();
   else
      FormPreview->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itToolClick(TObject *Sender)
{
   if (itTool->Checked)
      FormTool->Hide();
   else
      FormTool->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itRadarClick(TObject *Sender)
{
   if (itRadar->Checked)
      FormRadar->Hide();
   else
      FormRadar->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itTreeClick(TObject *Sender)
{
   if (itTree->Checked)
      FormTree->Hide();
   else
      FormTree->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itAltitudeClick(TObject *Sender)
{
   if (itAltitude->Checked)
      FormAltitude->Hide();
   else
      FormAltitude->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itSaveDesktopClick(TObject *Sender)
{
   SaveDesktop();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itLoadDesktopClick(TObject *Sender)
{
   LoadDesktop();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itSoulessEditorClick(TObject *Sender)
{
   SetCurrentDir(AnsiString(getGamePath().c_str()) + PATH_PPTOOLS);
   WinExec(AnsiString(getGamePath() + PATH_PPTOOLS + "BuildingEd.exe").c_str(), SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itTileEditorClick(TObject *Sender)
{
   SetCurrentDir(AnsiString(getGamePath().c_str()) + PATH_PPTOOLS);
   WinExec(AnsiString(getGamePath() + PATH_PPTOOLS + "MapChunkEd.exe").c_str(), SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itUnitsEditorClick(TObject *Sender)
{
   SetCurrentDir(AnsiString(getGamePath().c_str()) + PATH_PPTOOLS);
   WinExec(AnsiString(getGamePath() + PATH_PPTOOLS + "UnitsEd.exe").c_str(), SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itAboutClick(TObject *Sender)
{
   FormAbout->ShowModal();
}
//---------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 Descrition: MENU FUNCTIONS end
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 Descrition: ACTIONS FUNCTIONS
---------------------------------------------------------------------------*/
void __fastcall TFormMain::actCancelExecute(TObject *Sender)
{
   if (selectedItem)
   {
      setSelectedItem(NULL);
   }
   else
   {
     if (isSelecting)
     {
        cancelSelection();
     }   
     else
     {
       if (mDesktop.GetAltitude())
       {
          endAltitude();
       }
       else
       {
         bool lSelectionEmpty = (GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS)->Count() <= 0) &&
                                (GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDOTHERS)->Count() <= 0) &&
                                (GetMissionInstance()->GetObjects(MOL_SELECTEDOTHERS)->Count() <= 0);

         if (!lSelectionEmpty)
         {
           cancelSelection();
         }
       }
     }
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::actScrollLeftExecute(TObject *Sender)
{
   scrollHoriz->Position -= 2*MAPCELLX;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::actScrollRightExecute(TObject *Sender)
{
   scrollHoriz->Position += 2*MAPCELLX;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::actScrollUpExecute(TObject *Sender)
{
   scrollVert->Position -= 4*MAPCELLY;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::actScrollDownExecute(TObject *Sender)
{
   scrollVert->Position += 4*MAPCELLY;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::actDeleteExecute(TObject *Sender)
{
   deleteSelection();
   setModified(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::actSelectNeutralExecute(TObject *Sender)
{
   actSelectNeutral->Checked = !actSelectNeutral->Checked;
   RecomputeSelection();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::actSelectOwnedExecute(TObject *Sender)
{
   actSelectOwned->Checked = !actSelectOwned->Checked;
   RecomputeSelection();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::actSelectFriendlyExecute(TObject *Sender)
{
   actSelectFriendly->Checked = !actSelectFriendly->Checked;
   RecomputeSelection();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::actSelectEnemyExecute(TObject *Sender)
{
   actSelectEnemy->Checked = !actSelectEnemy->Checked;
   RecomputeSelection();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::actSelectStaticExecute(TObject *Sender)
{
   actSelectStatic->Checked = !actSelectStatic->Checked;
   FormTool->btnStatic->Down = actSelectStatic->Checked;
   RecomputeSelection();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::actSelectInfantryExecute(TObject *Sender)
{
   actSelectInfantry->Checked = !actSelectInfantry->Checked;
   FormTool->btnInf->Down = actSelectInfantry->Checked;
   RecomputeSelection();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::actSelectVehicleExecute(TObject *Sender)
{
   actSelectVehicle->Checked = !actSelectVehicle->Checked;
   FormTool->btnVehicle->Down = actSelectVehicle->Checked;
   RecomputeSelection();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::actSelectAirExecute(TObject *Sender)
{
   actSelectAir->Checked = !actSelectAir->Checked;
   FormTool->btnAir->Down = actSelectAir->Checked;
   RecomputeSelection();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::actSelectScreenExecute(TObject *Sender)
{
   beginSelection(viewLeft, viewTop);
   endSelection(viewLeft + viewWidth, viewTop + viewHeight);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::actSelectAllExecute(TObject *Sender)
{
   beginSelection(0, 0);
   endSelection(mapWidth * MAPCELLX, mapHeight * MAPCELLY);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::actObjectPropertiesExecute(TObject *Sender)
{
   FormObjManager->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::actGroupOrdersExecute(TObject *Sender)
{
   FormGroup->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::actCompileExecute(TObject *Sender)
{                                           
   //TODO: reintegrate mission verification/compile
   /*
   if (theMission->Compile(FormDialog->memoMsg->Lines))
      setCompiled(true);
   MEMsg("Compile finished", CUST_OK, "Ok", "Cancel");
   */
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::actPreferencesExecute(TObject *Sender)
{
   if (FormPreferences->ShowModal() == mrOk)
   {
      //Environment
      mDesktop.SetSaveDesktopOnExit(FormPreferences->checkSaveDesktop->Checked);
      //Grid
      mDesktop.SetGridColor (FormPreferences->shapeGridColor->Brush->Color);
      mDesktop.SetGridShown (FormPreferences->checkShowGrid->Checked);
      mDesktop.SetGridH     (FormPreferences->comboGridHoriz->Text.ToInt());
      mDesktop.SetGridV     (FormPreferences->comboGridVert->Text.ToInt());

      //Altitude
      mDesktop.SetAltitudeMapColor   (FormPreferences->lbAltitudeFont->Font->Color);
      mDesktop.SetAltitudeBrushColor (FormPreferences->shapeAltitudeBrushColor->Pen->Color);

      FormAltitude->editWidth->Value  =
                              FormPreferences->comboAltitudeWidth->Text.ToInt();
      FormAltitude->editHeight->Value =
                              FormPreferences->comboAltitudeHeight->Text.ToInt();

      mDesktop.SetAltitudeWidth(FormAltitude->editWidth->Value);
      mDesktop.SetAltitudeHeight(FormAltitude->editHeight->Value);

      //Mask
      mDesktop.SetMaskMapShown (FormPreferences->checkShowMask->Checked);
      mDesktop.SetMaskMapColor (FormPreferences->shapeMaskColor->Brush->Color);
      
      //InitMask();

      paintCanvas();
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::actCenterViewExecute(TObject *Sender)
{
  CIGBaseObject* lObj = NULL;

  if (GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS)->Count() >= 0)
  {
    lObj = GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS)->GetObject(0);
  }
  else
  {
    if (GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDOTHERS)->Count() >= 0)
    {
      lObj = GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDOTHERS)->GetObject(0);
    }
    else
    {
      if (GetMissionInstance()->GetObjects(MOL_SELECTEDOTHERS)->Count() >= 0)
      {
        lObj = GetMissionInstance()->GetObjects(MOL_SELECTEDOTHERS)->GetObject(0);
      }
    }
  }

  centerViewPort(lObj->GetCoord().x, lObj->GetCoord().y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 Descrition: ACTIONS FUNCTIONS end
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 descrition: se testeaza daca isCapture si daca X,Y este in bmpMain
---------------------------------------------------------------------------*/
void __fastcall TFormMain::FormMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   int tempCELLX, tempCELLY;

   tempCELLX = MAPCELLX;
   tempCELLY = MAPCELLY;

   if (selectedItem != NULL)
   {
      if (selectedItem->objType == OT_TILE)
      {
         tempCELLX *= 3;
         tempCELLY *= 3;
         floatLeft = ((scrollHoriz->Position + X)/tempCELLX)*tempCELLX - scrollHoriz->Position;
         floatTop  = ((scrollVert->Position + Y)/tempCELLY)*tempCELLY - scrollVert->Position;
      }
      else
      {
         floatLeft = ((scrollHoriz->Position + X - floatWidth/2)/tempCELLX)*tempCELLX - scrollHoriz->Position;
         floatTop  = ((scrollVert->Position + Y - floatHeight/2)/tempCELLY)*tempCELLY - scrollVert->Position;
      }
      if (floatLeft + scrollHoriz->Position < 0)
         floatLeft = - scrollHoriz->Position;
         
      if (floatTop + scrollVert->Position < 0)
         floatTop = - scrollVert->Position;

      if (floatLeft + scrollHoriz->Position + floatWidth > mapWidth * MAPCELLX)
         floatLeft = MAPCELLX * mapWidth - floatWidth - scrollHoriz->Position;

      if (floatTop + scrollVert->Position + floatHeight > mapHeight * MAPCELLY)
         floatTop = MAPCELLY * mapHeight - floatHeight - scrollVert->Position;

      paintCanvas();
      statusMain->Panels->Items[3]->Text =
                              selectedItem->objName + " : " +
                              AnsiString(floatLeft*zoomLevel + scrollHoriz->Position) +
                              "," + AnsiString(floatTop*zoomLevel + scrollVert->Position);
   }
   else
   if (isSelecting)
   {
      statusMain->Panels->Items[3]->Text =
                            "Selection : " +
                             AnsiString(selectX + scrollHoriz->Position) +
                              "," + AnsiString(selectY + scrollVert->Position) +
                              "->" +
                             AnsiString(X + scrollHoriz->Position) +
                              "," + AnsiString(Y + scrollVert->Position);

      paintCanvas();
   }
   else
   {
     if (mDesktop.GetAltitude())
     {

        //altitudeLeft = ((scrollHoriz->Position + X)/tempCELLX)*tempCELLX - scrollHoriz->Position;
        //altitudeTop  = ((scrollVert->Position + Y)/tempCELLY)*tempCELLY - scrollVert->Position;
        
        altitudeLeft = scrollHoriz->Position + X - scrollHoriz->Position;
        altitudeTop  = scrollVert->Position  + Y - scrollVert->Position;

        if (altitudeLeft + scrollHoriz->Position< 0)
           altitudeLeft = - scrollHoriz->Position;

        if (altitudeTop + scrollVert->Position< 0)
           altitudeTop = - scrollVert->Position;

        if (altitudeLeft + scrollHoriz->Position + MAPCELLX * FormAltitude->editWidth->Value > MAPCELLX * mapWidth)
           altitudeLeft = MAPCELLX * mapWidth - scrollHoriz->Position - FormAltitude->editWidth->Value;

        if (altitudeTop + scrollVert->Position + MAPCELLY * FormAltitude->editHeight->Value > MAPCELLY * mapHeight)
           altitudeTop = MAPCELLY * mapHeight - scrollVert->Position - FormAltitude->editHeight->Value;

        statusMain->Panels->Items[3]->Text =
                              "Altitude brush : " +
                               AnsiString(altitudeLeft + scrollHoriz->Position) +
                                "," +
                               AnsiString(altitudeTop + scrollVert->Position) +
                               "->" +
                               AnsiString(altitudeLeft + scrollHoriz->Position +
                                          MAPCELLX * FormAltitude->editWidth->Value) +
                               "," +
                               AnsiString(altitudeTop + scrollVert->Position +
                                          MAPCELLY * FormAltitude->editHeight->Value);
        paintCanvas();
        
     }
     else
     {
        statusMain->Panels->Items[3]->Text =
                               AnsiString(X*zoomLevel + scrollHoriz->Position) +
                                "," + AnsiString(Y*zoomLevel + scrollVert->Position);
     }
   }

   FormInfo->setInfo(GetMissionInstance()->GetMap()->GetGroundObject(
                                                       (X + scrollHoriz->Position)/MAPCELLX,
                                                       (Y + scrollVert->Position)/MAPCELLY));

   //TODO: setInfo for air objects!
   CIGBaseObjectList* lList = GetMissionInstance()->GetMap()->GetAirObjects(
                                                       (X + scrollHoriz->Position)/MAPCELLX,
                                                       (Y + scrollVert->Position)/MAPCELLY);

   if (lList)
   {
     FormInfo->setInfo(lList->GetObject(0));
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: left click  => pune obiectul
                            incepe select
                            sau
                            seteaza altitudinea
             right click => anuleaza actiunea curenta
                            sau porneste popup
---------------------------------------------------------------------------*/
void __fastcall TFormMain::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (Button == mbLeft)
   {
      if (selectedItem != NULL)
      {
         setModified(true);

         NoteObject(selectedItem,
                    itemMask,
                    scrollHoriz->Position + floatLeft,
                    scrollVert->Position  + floatTop,
                    currentBGroup,
                    GetMissionInstance()->GetUniqueUnitId(),
                    -1);
         paintCanvas();
      }
      else
      {
        if (mDesktop.GetAltitude())
        {
           setAltitude();
        }
        else
        {
           //adica daca nu se intampla nimic
           beginSelection(scrollHoriz->Position + X, scrollVert->Position + Y);
        }
      }
   }
/*
   if (Button == mbRight)
      if (selectedItem)
         setSelectedItem(NULL);
      else
      if (isSelecting)
         cancelSelection();
      else
      if (isAltitude)
         endAltitude();
      else
      if (!theMission->selectedObjects->isEmpty())
         cancelSelection();
*/
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: verifica daca s-a terminat selectia si dac da atunci ia act
---------------------------------------------------------------------------*/
void __fastcall TFormMain::FormMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (Button == mbLeft && isSelecting)
   {
      endSelection(scrollHoriz->Position + X, scrollVert->Position + Y);
      FormObjManager->FormActivate(this);
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: deseneaza doar canvas, adica doar ceea ce este in bmpBack
---------------------------------------------------------------------------*/
void __fastcall TFormMain::FormPaint(TObject *Sender)
{
   paintCanvas();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: este necesar pentru ca nu stiu sa fac altfel resize pe bmpMain si bmpBack
---------------------------------------------------------------------------*/
void __fastcall TFormMain::FormResize(TObject *Sender)
{
   setViewPortDim();
   setScreenZone();
// paintMap();
//   paintCanvas();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: seteaza hint
---------------------------------------------------------------------------*/
void __fastcall TFormMain::scrollHorizChange(TObject *Sender)
{
   setViewPortCoord(scrollHoriz->Position, scrollVert->Position);
   scrollHoriz->Hint = AnsiString(scrollHoriz->Position) + "/" + AnsiString(mapWidth * MAPCELLX);
//   if (FormRadar)
//      FormRadar->setViewPortCoord(rcViewport.left, rcViewport.top);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: seteaza hint
---------------------------------------------------------------------------*/
void __fastcall TFormMain::scrollVertChange(TObject *Sender)
{
   setViewPortCoord(scrollHoriz->Position, scrollVert->Position);
   scrollVert->Hint = AnsiString(scrollVert->Position) + "/" + AnsiString(mapHeight * MAPCELLY);
//   if (FormRadar)
//      FormRadar->setViewPortCoord(rcViewport.left, rcViewport.top);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: folisit pentru a nu lasa float image in plus pe ecran atunci cand
             mouse-ul nu se afla in cadrul lui map editor
 others    : deocamdata este nefolosit
---------------------------------------------------------------------------*/
void __fastcall TFormMain::timerMainTimer(TObject *Sender)
{
//   if (Mouse->CursorPos.x < )
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ControlScript1Click(TObject *Sender)
{
  FormMissionScript->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BattleGroupScript1Click(TObject *Sender)
{
  FormBGEdit->ShowModal();

  // update the side
  FormTree->comboSideEnter(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Politics1Click(TObject *Sender)
{
  FormPolitics->ShowModal();        
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: Initiates the mission data
---------------------------------------------------------------------------*/
void TFormMain::InitMapEditorSession()
{
#if PPT_USE_VFS
  GetVfsInstance();
#endif // PPT_USE_VFS

   InitLog();

#ifdef PP_TEST_VERSION
   initGamePath();
#endif

   //Config - init begin
   CCfgTxt *lConfig = NULL;
   try
   {
      lConfig = new CCfgTxt();

      CPString lCfg = getGamePath() + FILE_MEDITOR_CFG;
      if (lConfig->Open(lCfg.c_str()) == false)
      {
         LOG(FATAL) << "---------- UNABLE TO OPEN CONFIG FILE ( " << lCfg << " ) !!!!!!!!!!!";
         delete lConfig;
         lConfig = NULL;
      }
   }
   catch(...)
   {
      LOG(FATAL) << "---------- UNABLE TO INITIALIZE CONFIG CLASS !!!!!!!!!!!";
   }
   //Config - init end
   SetLogDetail(lConfig);

   //Direct Draw - init begin
   if (InitGraphics(lConfig) == false)
      PostQuitMessage(0);
   //Direct Draw - init end

   //Direct Sound - init begin
   if (InitSound(lConfig) == false)
   {
   //nimic
   }
   //Direct Sound - init end

   //Mission - init begin
   if (InitMission(lConfig) == false)
      PostQuitMessage(0);
   //Mission - init end

   //Path Server - init begin
   if (InitPathServer(lConfig) == false)
      PostQuitMessage(0);
   //Path Server - init end

   if (lConfig != NULL)
      delete lConfig;

   InitMask();
}

/*---------------------------------------------------------------------------
 description: initiaza log file
---------------------------------------------------------------------------*/
void TFormMain::InitLog()
{
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.setGlobally(el::ConfigurationType::Filename, "./logs/pptactical_editor.log");
    defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime %level %msg");
    el::Loggers::reconfigureLogger("default", defaultConf);

    LOG(INFO) << "Starting Engine Log";
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: seteaza lowest log detail level
 parameters : Config - pointer la clasa unde s-a deschis fisierul de config
---------------------------------------------------------------------------*/
void TFormMain::SetLogDetail(CCfgTxt *Config)
{
   int level = 0;

   if (Config != NULL)
   {
      Config->GetResource(CFGRES_DEBUGLEVEL, &level);

      // clamp int
      if (level < 1)
      {
          level = 1;
      }

      if (level > 9)
      {
          level = 9;
      }
   }

    if (level > 0)
    {
        el::Loggers::setVerboseLevel(level);
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: initiaza DDraw cu rezolutia citita din Config
---------------------------------------------------------------------------*/
bool TFormMain::InitGraphics(CCfgTxt *Config)
{

   VLOG(5) << "DIRECT DRAW INITIALIZING ... ";

#if PPT_USE_SDL
    if (SDL_Init(SDL_INIT_EVERYTHING & ~SDL_INIT_HAPTIC) != 0)
    {
        return false;
    }
#endif

   try
   {
      int rx = GetSystemMetrics(SM_CXSCREEN), ry = GetSystemMetrics(SM_CYSCREEN);

      GetGraphicInstance(Handle, rx, ry, PGI_WINDOWED);
   }
   catch(...)
   {
      LOG(ERROR) << "UNABLE TO INITIALIZE DIRECT DRAW !!!!!!!!!!!";
      return false;
   }

   VLOG(5) << "DIRECT DRAW INITIALIZED\n-------------------- ";

   return true;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: initiaza SoundSystem
---------------------------------------------------------------------------*/
bool TFormMain::InitSound(CCfgTxt *Config)
{
   VLOG(5) << "DIRECT SOUND INITIALIZING ...";
   try
   {
      GetSoundInstance(Handle);
   }
   catch(...)
   {
      LOG(ERROR) << "UNABLE TO INITIALIZE DIRECT SOUND !!!!!!!!!!!";
      return false;
   }

   try
   {
      CIGSpaceSoundSystem *lSSSI;
      lSSSI->Instance();
   }
   catch(...)
   {
      LOG(ERROR) << "UNABLE TO INITIALIZE SPACE SOUND SYSTEM !!!!!!!!!!!";
      return false;
   }

   VLOG(5) << "DIRECT SOUND INITIALIZED\n-------------------- ";
   return true;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: initiaza suprafata mask
 others     : deseneaza un mic dreptunghi cu diagcross si culoarea maskMapcolor
---------------------------------------------------------------------------*/
void TFormMain::InitMask()
{
   try
   {
     mSpriteMask = GetGraphicInstance()->AddSprite(getGamePath() + PATH_INGAME_GFX + "mask.png", 1, 1);
   }
   catch(...)
   {
     printMEError("TFormMain::InitMask - cannot load from file!");
   }  
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: initiaza misiunea
 others     : daca PPIG_TESTMAP => citeste din Config
---------------------------------------------------------------------------*/
bool TFormMain::InitMission(CCfgTxt *Config)
{
   VLOG(5) << "MISSION INITIALIZING ...";

   try
   {
      GetMissionInstance();
   }
   catch(...)
   {
      LOG(ERROR) << "UNABLE TO INITIALIZE MISSION !!!!!!!!!!!";
      return false;
   }

   VLOG(5) << "MISSION INITIALIZED\n-------------------- ";
   
   return true;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: initiate the path server
---------------------------------------------------------------------------*/
bool TFormMain::InitPathServer(CCfgTxt *Config)
{
   VLOG(5) << "PATH SERVER INITIALIZING ...";
   try
   {
      GetPathServerInstance();
   }
   catch(...)
   {
      LOG(ERROR) << "UNABLE TO CREATE PATH SERVER !!!!!!!!!!!";
      return false;
   }
   VLOG(5) << "PATH SERVER INITIALIZED\n-------------------- ";
   return true;
}
//---------------------------------------------------------------------------

// Description : Recomputes the selection based on the options in the Edit menu
// Param       :
// Result      :
// Comments    : Is called after the selection ended
void TFormMain::RecomputeSelection()
{
  CIGBaseObjectList* lList[3], *lNotValidList;

  lNotValidList = new CIGBaseObjectList();

  // unselect all units from MOL_SELECTEDOWNEDUNITS that don't match the "Edit" menu criterias
  lList[0] = GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS);
  lList[1] = GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDOTHERS);
  lList[2] = GetMissionInstance()->GetObjects(MOL_SELECTEDOTHERS);

  for(int j = 0; j < 3; j++)
  {
    lNotValidList->RemoveAll();

    for(int i = 0; i < lList[j]->Count(); i++)
    {
      CIGBaseObject* lObj = lList[j]->GetObject(i);

      // remove selected vehicles
      if (lObj->IsDerivedFrom(IGCN_VEHICLEUNIT) && !actSelectVehicle->Checked)
      {
        lNotValidList->AddUnique(lObj);
      }

      // remove selected infantries
      if (lObj->IsDerivedFrom(IGCN_INFANTRYUNIT) && !actSelectInfantry->Checked)
      {
        lNotValidList->AddUnique(lObj);
      }

      // remove selected statics
      if (lObj->IsDerivedFrom(IGCN_STATICOBJECT) && !actSelectStatic->Checked)
      {
        lNotValidList->AddUnique(lObj);
      }

      // remove selected aircrafts
      if (lObj->IsDerivedFrom(IGCN_AIRUNIT) && !actSelectAir->Checked)
      {
        lNotValidList->AddUnique(lObj);
      }

      // check object relation with the player
      TPoliticsRelations lRelation = GetMissionInstance()->GetBGManager()->GetRelation(OS_PLAYER, lObj->GetBattleGroupId());

      if (lRelation == REL_NEUTRAL && !actSelectNeutral->Checked)
      {
        lNotValidList->AddUnique(lObj);
      }

      if (lRelation == REL_OWNED && !actSelectOwned->Checked)
      {
        lNotValidList->AddUnique(lObj);
      }

      if (lRelation == REL_FRIENDLY && !actSelectFriendly->Checked)
      {
        lNotValidList->AddUnique(lObj);
      }

      if (lRelation == REL_ENEMY && !actSelectEnemy->Checked)
      {
        lNotValidList->AddUnique(lObj);
      }
    }

    // iterate the notvalid list and remove from the original list what is to be removed
    for(int i = 0; i < lNotValidList->Count(); i++)
    {
      CIGBaseObject* lObj = lNotValidList->GetObject(i);

      lObj->SetSelected(false);

      lList[j]->Remove(lObj);
    }
  }

  delete lNotValidList;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MapGenerator1Click(TObject *Sender)
{
  if (FormMapGen->ShowModal() == mrOk)
  {
    paintCanvas();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormMouseWheel(TObject *Sender,
      TShiftState Shift, int WheelDelta, Types::TPoint &MousePos, bool &Handled)
{
  if (Shift.Contains(ssShift))
  {
    scrollHoriz->Position = scrollHoriz->Position - WheelDelta;
  }
  else
  {
    scrollVert->Position = scrollVert->Position - WheelDelta;
  }

  Handled = true;
}
//---------------------------------------------------------------------------


