//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2025 Stefan Dicu/Piron Games                           //
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
 application: Tile Editor

 descrition : Form Main
 last modify: 02 09 2000
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>
#include <vcl.h>
#pragma hdrstop

#include "FMapChunk.h"

#include <System.IOUtils.hpp>
#include <Vcl.Imaging.pngimage.hpp>

#include "FAbout.h"
#include "Stream_ANSI_C.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain* FormMain;
//---------------------------------------------------------------------------

#define NONAME_TILE "Noname tile"

/////////////////////////////////////////////////////////////////////////////
// PUBLIC AREA
/////////////////////////////////////////////////////////////////////////////

__fastcall TFormMain::TFormMain(TComponent* Owner)
    : TForm(Owner)
{
    mMapChunk = new CAuxMapChunk(3);
}
//---------------------------------------------------------------------------

void TFormMain::PaintMapChunk()
{
    // TODO: optimize - cache the bitmaps, dont load them each paint
    TPngImage* lBmp = new TPngImage;
    CAuxTileData* lData;

    // int Width  = mMapChunk->GetSize() * TILEDIM;
    // int Height = mMapChunk->GetSize() * TILEDIM;

    PaintBox->Width = mMapChunk->GetSize() * TILEDIM;
    PaintBox->Height = mMapChunk->GetSize() * TILEDIM;
    PaintBox->Update();

    PaintBox->Canvas->Brush->Color = clBlack;
    PaintBox->Canvas->FillRect(TRect(
        0, 0, mMapChunk->GetSize() * TILEDIM, mMapChunk->GetSize() * TILEDIM));

    PaintBox->Canvas->Brush->Style = bsSolid;
    PaintBox->Canvas->Brush->Color = clBlack;
    PaintBox->Canvas->Font->Color = clWhite;

    for (int i = 0; i < mMapChunk->GetSize(); i++) {
        for (int j = 0; j < mMapChunk->GetSize(); j++) {
            if (mMapChunk->GetDepth(i, j) <= 0) {
                continue;
            }

            for (int k = 0; k < mMapChunk->GetDepth(i, j); k++) {
                lData = mMapChunk->Get(i, j, k); // :)

                lBmp->Transparent = true;
                // lBmp->TransparentColor = clBlack;
                auto pathSep = TPath::DirectorySeparatorChar;
                auto fullPath = ExtractFileDir(mFileName) + pathSep + lData->GetSprite().c_str();
                lBmp->LoadFromFile(fullPath.c_str());
                PaintBox->Canvas->Draw(i * TILEDIM, j * TILEDIM, lBmp);
            }

            PaintBox->Canvas->TextOut(
                i * TILEDIM, j * TILEDIM, AnsiString(mMapChunk->GetDepth(i, j)));
        }
    }

    delete lBmp;
}

void TFormMain::SetTileName(AnsiString tilename)
{
    Caption = "PP Tile Editor - " + tilename;
}

/*---------------------------------------------------------------------------
 descrition: serialize the mapchunk member to file
---------------------------------------------------------------------------*/
void TFormMain::SaveMapChunk(AnsiString filename)
{
    try {
        PP::Stream_ANSI_C lA(CPString(filename.c_str()), true);
        mMapChunk->Serialize(lA);
    } catch (...) {
        // message
        return;
    }

    SetTileName(filename);
}

/////////////////////////////////////////////////////////////////////////////
// PUBLISHED AREA
/////////////////////////////////////////////////////////////////////////////
void __fastcall TFormMain::itExitClick(TObject* Sender)
{
    Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itNewClick(TObject* Sender)
{
    // form components initializations
    mModified = true;
    comboAppearance->ItemIndex = 2; // adica normal
    comboSize->ItemIndex = 2;
    edName->Text = mMapChunk->GetName().c_str();
    // imgSprite->Picture         = NULL;
    PaintBox->ShowHint = false;
    mFileName = NONAME_TILE;

    // map chunk initializations
    mMapChunk->SetSize(3);
    mMapChunk->SetApparition(2);
    mMapChunk->SetName("Noname mapchunk");

    // form re-init
    SetTileName(mFileName);
    edName->Text = "Noname mapchunk";

    // clear the tile
    for (int i = 0; i < mMapChunk->GetSize(); i++) {
        for (int j = 0; j < mMapChunk->GetSize(); j++) {
            mMapChunk->Clear(i, j);
        }
    }

    PaintMapChunk();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itLoadClick(TObject* Sender)
{
    if (dialogOpen->Execute()) {
        try {
            PP::Stream_ANSI_C lA(UTF8Encode(dialogOpen->FileName).c_str());
            mMapChunk->DeSerialize(lA);

            // lA.Open(CPString(dialogOpen->FileName.c_str()));
            // mMapChunk->DeSerialize(lA);
            // lA.Close();
        } catch (...) {
        }

        mFileName = dialogOpen->FileName;
        SetTileName(mFileName);

        comboAppearance->ItemIndex = mMapChunk->GetApparition() - 1;
        comboSize->ItemIndex = mMapChunk->GetSize() - 1;
        edName->Text = mMapChunk->GetName().c_str();

        PaintBox->ShowHint = true;

        PaintMapChunk();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itSaveClick(TObject* Sender)
{
    if (mFileName == NONAME_TILE) {
        itSaveAsClick(Sender);
    } else {
        SaveMapChunk(mFileName);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itSaveAsClick(TObject* Sender)
{
    if (dialogSave->Execute()) {
        SaveMapChunk(dialogSave->FileName);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itHelpClick(TObject* Sender)
{
    //
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itAboutClick(TObject* Sender)
{
    FormAbout->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnSpriteClick(TObject* Sender)
{
    //
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormCreate(TObject* Sender)
{
    itNewClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Addshore1Click(TObject* Sender)
{
    // do ground add
    if (dialogOpenPicture->Execute()) {
        CAuxTileData* lData;

        try {
            lData = new CAuxTileData();
        } catch (...) {
        }

        PaintBox->Hint = ExtractFileName(dialogOpenPicture->FileName);
        PaintBox->ShowHint = true;

        lData->SetSprite(
            UTF8Encode(ExtractFileName(dialogOpenPicture->FileName)).c_str());
        lData->SetType(MTT_SHORE);

        mMapChunk->AddToLayer(mSquare.x, mSquare.y, lData);

        PaintMapChunk();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Addwater1Click(TObject* Sender)
{
    // do water add
    if (dialogOpenPicture->Execute()) {
        CAuxTileData* lData;

        try {
            lData = new CAuxTileData();
        } catch (...) {
        }

        PaintBox->Hint = ExtractFileName(dialogOpenPicture->FileName);
        PaintBox->ShowHint = true;

        lData->SetSprite(
            UTF8Encode(ExtractFileName(dialogOpenPicture->FileName)).c_str());
        lData->SetType(MTT_WATER);

        mMapChunk->AddToLayer(mSquare.x, mSquare.y, lData);

        PaintMapChunk();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Addground1Click(TObject* Sender)
{
    // do ground add
    if (dialogOpenPicture->Execute()) {
        CAuxTileData* lData;

        try {
            lData = new CAuxTileData();
        } catch (...) {
        }

        PaintBox->Hint = ExtractFileName(dialogOpenPicture->FileName);
        PaintBox->ShowHint = true;

        lData->SetSprite(
            UTF8Encode(ExtractFileName(dialogOpenPicture->FileName)).c_str());
        lData->SetType(MTT_LAND);

        mMapChunk->AddToLayer(mSquare.x, mSquare.y, lData);

        PaintMapChunk();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ClearTile1Click(TObject* Sender)
{
    // clear the current tile
    mMapChunk->Clear(mSquare.x, mSquare.y);

    PaintMapChunk();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::comboSizeChange(TObject* Sender)
{
    mMapChunk->SetSize(comboSize->ItemIndex + 1);
    PaintMapChunk();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::edNameExit(TObject* Sender)
{
    mMapChunk->SetName(UTF8Encode(edName->Text).c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Import1Click(TObject* Sender)
{
    // load a the specified bitmap, split the bitmap according to size then
    // fill the mapchunk

    if (!dialogOpenPicture->Execute()) {
        return;
    }

    Graphics::TBitmap* lBmp = new Graphics::TBitmap;

    try {
        lBmp->LoadFromFile(dialogOpenPicture->FileName);
    } catch (...) {
        delete lBmp;
        return;
    }

    delete mMapChunk;

    mMapChunk = new CAuxMapChunk(lBmp->Width / TILEDIM);

    lBmp->Canvas->CopyMode = cmSrcCopy;

    for (int i = 0; i < lBmp->Width / TILEDIM; i++) {
        for (int j = 0; j < lBmp->Height / TILEDIM; j++) {
            CAuxTileData* lData;

            try {
                lData = new CAuxTileData();
            } catch (...) {
                return;
            }

            // lBmpToSave->Canvas->Brush->Color = clBlack;
            // lBmpToSave->Canvas->FillRect(TRect(0, 0, TILEDIM, TILEDIM));

            Graphics::TBitmap* lBmpToSave = new Graphics::TBitmap;

            // split the bitmap loaded
            lBmpToSave->Width = TILEDIM;
            lBmpToSave->Height = TILEDIM;
            lBmpToSave->Canvas->CopyMode = cmSrcCopy;
            lBmpToSave->Canvas->CopyRect(
                TRect(0, 0, TILEDIM, TILEDIM),
                lBmp->Canvas,
                TRect(i * TILEDIM, j * TILEDIM, (i + 1) * TILEDIM, (j + 1) * TILEDIM));

            AnsiString lFullFileName = ExtractFileName(dialogOpenPicture->FileName);
            AnsiString lFileExt = ExtractFileExt(lFullFileName);
            int lExtPos = lFullFileName.Pos(lFileExt);
            // int lCount = lFullFileName.Length() - lExtPos + 1;
            AnsiString lFileName = lFullFileName.SubString(1, lExtPos - 1);

            lFileName = lFileName + "_" + AnsiString(i + 1) + AnsiString(j + 1) + lFileExt;

            // and save the cell
            lBmpToSave->SaveToFile(lFileName);
            delete lBmpToSave;

            // set data for mapchunk
            lData->SetSprite(CPString(lFileName.c_str()));
            lData->SetType(MTT_LAND);

            mMapChunk->AddToLayer(i, j, lData);
        }
    }

    delete lBmp;

    PaintMapChunk();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::comboSizeCloseUp(TObject* Sender)
{
    mMapChunk->SetSize(comboSize->ItemIndex + 1);
    PaintMapChunk();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxMouseUp(TObject* Sender,
    TMouseButton Button,
    TShiftState Shift,
    int X,
    int Y)
{
    mSquare.x = X / TILEDIM;
    mSquare.y = Y / TILEDIM;
    popupMain->Popup(Left + PaintBox->Left + X, Top + PaintBox->Top + Y);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxMouseMove(TObject* Sender,
    TShiftState Shift,
    int X,
    int Y)
{
    statusMain->SimpleText = AnsiString(X / TILEDIM + 1) + "," + AnsiString(Y / TILEDIM + 1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxPaint(TObject* Sender)
{
    PaintMapChunk();
}
//---------------------------------------------------------------------------

