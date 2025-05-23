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

 descrition : face preview object
 last modify: 29 10 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>
#include <vcl.h>
#pragma hdrstop

#include "FPreview.h"
#include "MEError.h"
#include "Paths.h"
#include "Statics.h"
#include "Tools.h"
#include "basepath.h"
#include <Vcl.Imaging.pngimage.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPreview* FormPreview;

/////////////////////////////////////////////////////////////////////////////
//PUBLIC AREA
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 descrition: constructorul
---------------------------------------------------------------------------*/
__fastcall TFormPreview::TFormPreview(TComponent* Owner)
    : TForm(Owner)
{
    itCheck = NULL;

    // Vcl::Graphics::TPicture::RegisterFileFormat("png", "Portable Network Graphics", TPngImage);
}

/*---------------------------------------------------------------------------
 descrition: seteaza selected item, adica cel pentru care trebiue sa faca preview
 parameters: pointer la selected item
---------------------------------------------------------------------------*/
void TFormPreview::setSelectedItem(TTreeItem* selecteditem)
{
    TPngImage* lBmp;

    imgPreview->Hint = "";
    selectedItem = selecteditem;

    if (selectedItem != NULL) {
        switch (selectedItem->objType) {
        case OT_NONE:
            statusPreview->Panels->Items[0]->Text = "No object selected";
            imgPreview->Picture = NULL;
            break;

        case OT_TILE:
            tileObj = getMapChunk(selectedItem->fullName);
            if (!tileObj) {
                imgPreview->Picture = NULL;
                statusPreview->Panels->Items[0]->Text = "Error reading map chunk.";
            }

            // bla bla bla - builder specific crap code
            lBmp = new TPngImage();
            imgPreview->Visible = true;
            imgPreview->Picture->Bitmap->Width = tileObj->GetSize() * TILEDIM;
            imgPreview->Picture->Bitmap->Height = tileObj->GetSize() * TILEDIM;

            // preview needs painting as in tile editor
            for (int i = 0; i < tileObj->GetSize(); i++) {
                for (int j = 0; j < tileObj->GetSize(); j++) {
                    if (tileObj->GetDepth(i, j) <= 0) {
                        continue;
                    }

                    for (int k = 0; k < tileObj->GetDepth(i, j); k++) {
                        CAuxTileData* lData = tileObj->Get(i, j, k); // :)

                        try {
                            lBmp->LoadFromFile(ExtractFilePath(selectedItem->fullName) + lData->GetSprite().c_str());
                        } catch (...) {
                            printMEError("Cannot find sprite file: " + AnsiString(lData->GetSprite().c_str()));
                        }

                        imgPreview->Canvas->Draw(i * TILEDIM, j * TILEDIM, lBmp);
                    }
                }
            }

            statusPreview->Panels->Items[0]->Text = AnsiString(tileObj->GetName().c_str()) + "(" + selectedItem->fullName + ")";
            delete lBmp;
            delete tileObj;
            break;

        case OT_STATIC:
            if (!getStaticObjectStruct(selectedItem->fullName, &staticObj)) {
                imgPreview->Picture = NULL;
                statusPreview->Panels->Items[0]->Text = "Error reading object !";
                printMEError("Cannot read building data!");
                return;
            }

            statusPreview->Panels->Items[0]->Text = selectedItem->Path + AnsiString(staticObj.sprite);

            try {
                imgPreview->Picture->LoadFromFile(selectedItem->Path + AnsiString(staticObj.sprite));
            } catch (...) {
                printMEError("Cannot load building sprite!");
            }

            break;

        case OT_INFANTRY:
            if (!getAIInfantryUnitStruct(selectedItem->Path, selectedItem->Index, &recInf)) {
                imgPreview->Picture = NULL;
                statusPreview->Panels->Items[0]->Text = "Error reading object !";
                printMEError("Cannot read infantry data!");
                return;
            }

            statusPreview->Panels->Items[0]->Text = selectedItem->Path;
            imgPreview->Picture->LoadFromFile(getGamePath() + PATH_INF_GFX + AnsiString(recInf.sprite));
            break;

        case OT_VEHICLE:
            if (!getAIVehicleUnitStruct(selectedItem->Path, selectedItem->Index, &recVeh)) {
                imgPreview->Picture = NULL;
                statusPreview->Panels->Items[0]->Text = "Error reading object !";
                printMEError("Cannot read vehicles data!");
                return;
            }

            statusPreview->Panels->Items[0]->Text = selectedItem->Path;
            imgPreview->Picture->LoadFromFile(getGamePath() + PATH_VEHICLES_GFX + AnsiString(recVeh.sprite));
            break;

        case OT_AIR:
            if (!getAIVehicleUnitStruct(selectedItem->Path, selectedItem->Index, &recVeh)) {
                imgPreview->Picture = NULL;
                statusPreview->Panels->Items[0]->Text = "Error reading object !";
                printMEError("Cannot read vehicles(aircraft) data!");
                return;
            }

            statusPreview->Panels->Items[0]->Text = selectedItem->Path;
            imgPreview->Picture->LoadFromFile(getGamePath() + PATH_VEHICLES_GFX + AnsiString(recVeh.sprite));
            break;
        }
        imgPreview->Hint = selectedItem->objName;
    } else { //selecteditem  == NULL
        statusPreview->Panels->Items[0]->Text = "No object selected";
        imgPreview->Picture = NULL;
    }
    if (mitAuto->Checked)
        mitAutoClick(NULL);
    else
        mitUserClick(NULL);
    statusPreview->Hint = statusPreview->Panels->Items[0]->Text;
}

/*---------------------------------------------------------------------------
 descrition: citeste un static dat de selectedItem
---------------------------------------------------------------------------*/
bool TFormPreview::readStatic()
{
    //TODO: modify this code! there are momory leaks all over the place
    int fileHandle;

    if ((fileHandle = open(selectedItem->fullName.c_str(), O_BINARY | O_RDONLY, S_IREAD)) == -1) {
        close(fileHandle);
        return false;
    }

    if (read(fileHandle, &staticObj, sizeof(struct _StaticObject)) == -1) {
        close(fileHandle);
        return false;
    }

    close(fileHandle);
    return true;
}

/////////////////////////////////////////////////////////////////////////////
//PUBLISHED AREA
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
void __fastcall TFormPreview::FormShow(TObject* Sender)
{
    if (itCheck)
        itCheck->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormPreview::FormHide(TObject* Sender)
{
    if (itCheck)
        itCheck->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormPreview::mitUserClick(TObject* Sender)
{
    imgPreview->Align = alClient;
    imgPreview->Stretch = true;
    imgPreview->AutoSize = false;
    mitAuto->Checked = false;
    mitUser->Checked = true;
    Constraints->MaxWidth = 0;
    Constraints->MinWidth = 0;
    Constraints->MaxHeight = 0;
    Constraints->MinHeight = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormPreview::mitAutoClick(TObject* Sender)
{
    imgPreview->Align = alNone;
    imgPreview->Stretch = false;
    imgPreview->AutoSize = true;
    mitAuto->Checked = true;
    mitUser->Checked = false;
    Constraints->MinWidth = imgPreview->Width + Width - ClientWidth + 20;
    Constraints->MaxWidth = Constraints->MinWidth;
    Constraints->MinHeight = imgPreview->Height + Height - ClientHeight + statusPreview->Height + 20;
    Constraints->MaxHeight = Constraints->MinHeight;
}
//---------------------------------------------------------------------------

