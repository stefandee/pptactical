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

#include <vcl.h>
#include <stdlib.h>
#pragma hdrstop

#include "FMapGen.h"
#include "MEError.h"
#include "FMain.h"
#include "basepath.h"
#include "Tools.h"
#include "PPIG_Map.h"
#include "PPIG_Mission.h"
#include "PPIG_BGManager.h"

#include "EcoSysGenRandom.h"
#include "PP_StdLib.h"
#include "PPIG_ObjStatic.h"
#include "PP_Graphic.h"
#include "FEcoSysGenRandomConfig.h"
#include "FEcoSysGenKernelConfig.h"
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMapGen *FormMapGen;
//---------------------------------------------------------------------------

__fastcall TFormMapGen::TFormMapGen(TComponent* Owner)
        : TForm(Owner)
{
  mMapChunkList = new TList;
  dialogOpen->InitialDir = AnsiString(getGamePath().c_str()) + PATH_LANDSCAPE;

  mEcoSysGen = 0;

  CreateEcoSysGen();

  switch(cbEcoSysGenAlg->ItemIndex)
  {
    case 0:
      mEcoSysGen = new EcoSysGenRandom();
      break;
  }   
}
//---------------------------------------------------------------------------

void TFormMapGen::Initialize(AnsiString landscape)
{
   TSearchRec srDesc;
   int iAttributes = faAnyFile;
   bool First = true;     //arata daca este prima citire in directorul curent
   bool End = false;      //arata daca s-a terminat citirea in directorul curent
   AnsiString   fileName;
   CAuxMapChunk *tileObj;

   removeAllTiles();

   //cautarea tile-urilor de baza din directorul landscape
   while (!End)
   {
      if (First)
      {
         fileName = getGamePath() + PATH_LANDSCAPE + /*"\\" +*/ landscape + "\\" + "*.*";
         End = !(FindFirst(getGamePath() + PATH_LANDSCAPE + /*"\\" +*/ landscape + "\\" + "*.*", iAttributes, srDesc) == 0);
         First = false;
      }
      else
         End = !(FindNext(srDesc) == 0);
      if (!End)
      if (srDesc.Name != "." && srDesc.Name != "..")
      {
         fileName = getGamePath() + PATH_LANDSCAPE + /*"\\" +*/ landscape + "\\" + srDesc.Name;
         if (LowerCase(srDesc.Name).Pos(".dat") != 0)
         {
            tileObj = getMapChunk(fileName);
            if (tileObj != NULL)
            {
               tileObj->SetPath((getGamePath() + PATH_LANDSCAPE + landscape + "\\").c_str());
               addTile(tileObj);
            }
         }
      }
   }
   FindClose(srDesc);
}

/*---------------------------------------------------------------------------
 descrition: selecteaza respectivul item atat din tile cat si din rate
---------------------------------------------------------------------------*/
void TFormMapGen::setSelectedItem(int index)
{
   listRate->ItemIndex = index;
   listTile->ItemIndex = index;
   comboRate->ItemIndex = ((CAuxMapChunk*)(mMapChunkList->Items[index]))->GetApparition() - 1;

   PreviewTile((CAuxMapChunk*)(mMapChunkList->Items[index]));

   statusMain->Panels->Items[0]->Text = ((CAuxMapChunk*)(mMapChunkList->Items[index]))->GetName().c_str();
}

/*---------------------------------------------------------------------------
 description: paint in the preview control the tile
---------------------------------------------------------------------------*/
void  TFormMapGen::PreviewTile(CAuxMapChunk* _mapChunk)
{
  // TODO: preview of tiles in generator
  imgPreview->Visible = true;

  imgPreview->Width  = _mapChunk->GetSize() * 32;
  imgPreview->Height = _mapChunk->GetSize() * 32;

  AnsiString path = _mapChunk->GetPath().c_str();
  TPngImage* lBmp = new TPngImage();

  for(int x = 0; x < _mapChunk->GetSize(); x++)
  {
    for(int y = 0; y < _mapChunk->GetSize(); y++)
    {
      if (_mapChunk->GetDepth(x, y) <= 0)
      {
        continue;
      }

      for(int depth = 0; depth < _mapChunk->GetDepth(x, y); depth++)
      {
        CAuxTileData* lTileData = _mapChunk->Get(x, y, depth);

        lBmp->LoadFromFile(path + AnsiString(lTileData->GetSprite().c_str()));
        //lBmp->Transparent = true;
        //lBmp->TransparentColor = clBlack;

        imgPreview->Canvas->Draw(x * 32, y * 32, lBmp);
      }
    }
  }

  imgPreview->Repaint();

  delete lBmp;

  //imgPreview->Picture->LoadFromFile(((CAuxMapChunk *)mMapChunkList->Items[index])->GetPath().c_str());
}

/*---------------------------------------------------------------------------
 descrition: genereaza random tiles dupa lista selectata
---------------------------------------------------------------------------*/
bool TFormMapGen::GenerateTiles()
{
   int Sum = 0;
   int i,j,k;
   int tmp, tmpSum;
   int lValidSize;

   // select valid size
   lValidSize = ComputeSize();

   // appearance sum
   for (i=0; i<mMapChunkList->Count; i++)
   {
      if (lValidSize == ((CAuxMapChunk*)(mMapChunkList->Items[i]))->GetSize())
      {
        Sum += ((CAuxMapChunk*)(mMapChunkList->Items[i]))->GetApparition();
      }  
   }

   if (mMapChunkList->Count > 0)
   {
      // look different each time
      randomize();

      // now generate the map
      for (i=0; i < GetMissionInstance()->GetMap()->GetLogicWidth(); i+=3)
         for (j=0; j < GetMissionInstance()->GetMap()->GetLogicHeight(); j+=3)
         {
            tmp = random(Sum);
            tmpSum = 0;
            
            for (k=0; k< mMapChunkList->Count; k++)
            {
               if (((CAuxMapChunk*)(mMapChunkList->Items[k]))->GetSize() != lValidSize)
               {
                 continue;
               }

               if (tmp >= tmpSum && tmp<tmpSum + ((CAuxMapChunk*)(mMapChunkList->Items[k]))->GetApparition())
               {
                  CIGBaseTile*  lTile;
                  CAuxTileData* lTileData;

                  for(int l = 0 ; l < ((CAuxMapChunk*)(mMapChunkList->Items[k]))->GetDepth((i/3)%lValidSize, (j/3)%lValidSize); l++)
                  {
                    try
                    {
                      lTile = new CIGBaseTile();
                    }
                    catch(...)
                    {
                      printMEError("Cannot create tile object!");
                      return false;
                    }

                    lTileData = ((CAuxMapChunk*)(mMapChunkList->Items[k]))->Get((i/3)%lValidSize, (j/3)%lValidSize, l);

                    if (lTileData)
                    {
                      lTile->SetCoord(CPPoint(i * MAPCELLX, j * MAPCELLY));
                      lTile->SetType(lTileData->GetType());
                      lTile->SetSprite(GetGraphicInstance()->AddSprite(((CAuxMapChunk*)(mMapChunkList->Items[k]))->GetPath() + lTileData->GetSprite(), 1, 1, SPRL_MISSION));

                      GetMissionInstance()->NoteInLayerMap(MOL_BASETILE, lTile, i, j);
                    }
                  }
                  break;
               }
               else
               {
                 tmpSum += ((CAuxMapChunk*)(mMapChunkList->Items[k]))->GetApparition();
               }
            }   
         }
   }
   else
   {
     return false;
   }

   return true;
}

/*---------------------------------------------------------------------------
 description: adauga un tile in lista
---------------------------------------------------------------------------*/
void TFormMapGen::addTile(CAuxMapChunk *tileobj)
{
   listTile->Items->Add(tileobj->GetName().c_str());
   listRate->Items->Add(comboRate->Items->Strings[tileobj->GetApparition() - 1]);
   mMapChunkList->Add(tileobj);
   setSelectedItem(mMapChunkList->Count - 1);
   comboRate->Enabled = true;
}

/*---------------------------------------------------------------------------
 description: sterge un tile din lista
---------------------------------------------------------------------------*/
void TFormMapGen::removeTile(int index)
{
   listTile->Items->Delete(index);
   listRate->Items->Delete(index);

   delete ((CAuxMapChunk*)(mMapChunkList->Items[index]));

   mMapChunkList->Delete(index);

   mMapChunkList->Pack();

   if (mMapChunkList->Count > 0)
      setSelectedItem(0);
   else
   {
      imgPreview->Visible = false;
      comboRate->Enabled  = false;
   }
}

/*---------------------------------------------------------------------------
 descrition: sterge toate tile-urile
---------------------------------------------------------------------------*/
void TFormMapGen::removeAllTiles()
{
   listTile->Clear();
   listRate->Clear();
   mMapChunkList->Clear();
   mMapChunkList->Pack();
   comboRate->Enabled = false;
}
//---------------------------------------------------------------------------

int  TFormMapGen::ComputeSize()
{
  if (mMapChunkList->Count <= 0)
  {
    return -1;
  }

  int lSize = 0, lMapChunkSize;

  for(int i = 0; i < mMapChunkList->Count; i++)
  {
    lMapChunkSize = ((CAuxMapChunk*)(mMapChunkList->Items[i]))->GetSize();

    if (lMapChunkSize > lSize)
    {
      lSize = lMapChunkSize;
    }
  }

  return lSize;
}
//---------------------------------------------------------------------------

bool TFormMapGen::Generate()
{
  GenerateTiles();

  // for the moment
  return true;
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// PUBLISHED AREA - Tile Generation
/////////////////////////////////////////////////////////////////////////////
void __fastcall TFormMapGen::btnAddClick(TObject *Sender)
{
   CAuxMapChunk *tileObj;

   if (dialogOpen->Execute())
   {
      tileObj = getMapChunk(dialogOpen->FileName);

      if (tileObj != NULL)
      {
         tileObj->SetPath( System::UTF8String(ExtractFilePath(dialogOpen->FileName)).c_str() );
         addTile(tileObj);
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMapGen::btnDeleteClick(TObject *Sender)
{
   if (listTile->Items->Count > 0)
      removeTile(listTile->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TFormMapGen::btnClearClick(TObject *Sender)
{
   removeAllTiles();
}
//---------------------------------------------------------------------------

void __fastcall TFormMapGen::listTileClick(TObject *Sender)
{
  setSelectedItem(listTile->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TFormMapGen::listRateClick(TObject *Sender)
{
  setSelectedItem(listRate->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TFormMapGen::comboRateChange(TObject *Sender)
{
  listRate->Items->Strings[listRate->ItemIndex] = comboRate->Items->Strings[comboRate->ItemIndex];
  ((CAuxMapChunk*)(mMapChunkList->Items[listRate->ItemIndex]))->SetApparition((char)(comboRate->ItemIndex + 1));
}
//---------------------------------------------------------------------------

void __fastcall TFormMapGen::TimerTimer(TObject *Sender)
{
   if (ActiveControl == listRate)
      listTile->TopIndex = listRate->TopIndex;
   else
      listRate->TopIndex = listTile->TopIndex;
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// PUBLISHED AREA - Form
/////////////////////////////////////////////////////////////////////////////
void __fastcall TFormMapGen::btnOkGenerateClick(TObject *Sender)
{
  GenerateTiles();
}
//---------------------------------------------------------------------------

void __fastcall TFormMapGen::btnEcoSysConfigClick(TObject *Sender)
{
  switch(cbEcoSysGenAlg->ItemIndex)
  {
    case 0:
      FormEcoSysGenRandomConfig->SetData((EcoSysGenRandom*)mEcoSysGen);
      FormEcoSysGenRandomConfig->ShowModal();
      break;

    case 1:
      FormEcoSysGenKernelConfig->SetData((EcoSysGenKernel*)mEcoSysGen);
      FormEcoSysGenKernelConfig->ShowModal();
      break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMapGen::btnEcoSysPreviewClick(TObject *Sender)
{
  randomize();

  if (!mEcoSysGen)
  {
    return;
  }

  paintEcoSys->Canvas->Brush->Color = (TColor)0x000000;
  paintEcoSys->Canvas->FillRect(Types::TRect(0, 0, paintEcoSys->Width, paintEcoSys->Height));

  std::vector<EcoSysItemOut> lResult = mEcoSysGen->Generate();

  for(unsigned int i = 0; i < lResult.size(); i++)
  {
    paintEcoSys->Canvas->Brush->Color = (TColor)(lResult[i].mColorCode);
    paintEcoSys->Canvas->FillRect(
      Types::TRect(
        lResult[i].mLocation.x,
        lResult[i].mLocation.y,
        lResult[i].mLocation.x + lResult[i].mSize.x,
        lResult[i].mLocation.y + lResult[i].mSize.y
      )
    );
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMapGen::FormShow(TObject *Sender)
{
  paintEcoSys->Width  = GetMissionInstance()->GetMap()->GetLogicWidth();
  paintEcoSys->Height = GetMissionInstance()->GetMap()->GetLogicHeight();
}
//---------------------------------------------------------------------------

void __fastcall TFormMapGen::cbEcoSysGenAlgChange(TObject *Sender)
{
  CreateEcoSysGen();
}
//---------------------------------------------------------------------------

void TFormMapGen::CreateEcoSysGen()
{
  switch(cbEcoSysGenAlg->ItemIndex)
  {
    case 0:
      delete mEcoSysGen;
      mEcoSysGen = 0;

      mEcoSysGen = new EcoSysGenRandom();
      break;

    case 1:
      delete mEcoSysGen;
      mEcoSysGen = 0;

      mEcoSysGen = new EcoSysGenKernel();
      //FormEcoSysGenKernelConfig->ShowModal();
      break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMapGen::btnEcoSysGenClick(TObject *Sender)
{
  randomize();

  if (!mEcoSysGen)
  {
    return;
  }

  paintEcoSys->Canvas->Brush->Color = (TColor)0x000000;
  paintEcoSys->Canvas->FillRect(Types::TRect(0, 0, paintEcoSys->Width, paintEcoSys->Height));

  std::vector<EcoSysItemOut> lResult = mEcoSysGen->Generate();

  struct _StaticObject lStaticStruct;
  CIGStaticObject*     lStatic;
  CPGISprite*          lSprite = 0;

  for(unsigned int i = 0; i < lResult.size(); i++)
  {
    paintEcoSys->Canvas->Brush->Color = (TColor)(lResult[i].mColorCode);
    paintEcoSys->Canvas->FillRect(
      Types::TRect(
        lResult[i].mLocation.x,
        lResult[i].mLocation.y,
        lResult[i].mLocation.x + lResult[i].mSize.x,
        lResult[i].mLocation.y + lResult[i].mSize.y
      )
    );

    AnsiString lPath = ExtractFilePath(AnsiString(lResult[i].mFilePath.c_str())); 

    try
    {
      PP::Stream_ANSI_C lA(lResult[i].mFilePath);

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
      lStatic = new CIGStaticObject();
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
      strcpy(lStaticStruct.mLayers[i].mSprite, (CPString(lPath.c_str()) + CPString(lStaticStruct.mLayers[i].mSprite)).c_str());
      //strcpy(lStaticStruct.mLayers[i].mSprite, (CPString(treeitem->Path.c_str()) + CPString(lStaticStruct.mLayers[i].mSprite)).c_str());
    }

    CPPoint lLocationReal = lResult[i].mLocation;

    lLocationReal.x = lLocationReal.x * MAPCELLX;
    lLocationReal.y = lLocationReal.y * MAPCELLY;

    lStatic->SetCoord(lLocationReal);
    lSprite = GetGraphicInstance()->AddSprite(CPString(lPath.c_str()) + lStaticStruct.sprite, 1, 1, SPRL_MISSION);
    lStatic->SetSprite(lSprite);
    lStatic->SetStaticData(lStaticStruct);

    lStatic->SetBattleGroupId(OS_LANDSCAPE);
    lStatic->SetId(GetMissionInstance()->GetUniqueUnitId());

    lStatic->NoteInLayerMap();
    lStatic->NoteInLogicMap();

    //switch (lObjDesc->mSide)
    switch (GetMissionInstance()->GetBGManager()->GetRelation(OS_PLAYER, OS_LANDSCAPE))
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
  }

  FormMain->Repaint();
}
//---------------------------------------------------------------------------

