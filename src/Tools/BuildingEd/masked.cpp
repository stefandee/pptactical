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
//---------------------------------------------------------------------------
#include <math.h>
#include <vcl.h>
#pragma hdrstop

#include <Vcl.Imaging.pngimage.hpp>

#include "FToolProp.h"
#include "PPIG_EngineDef.h"
#include "m_sless.h"
#include "masked.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ImageEx"
#pragma resource "*.dfm"
TMaskForm* MaskForm;
//---------------------------------------------------------------------------
__fastcall TMaskForm::TMaskForm(TComponent* Owner)
    : TForm(Owner)
{
    MaskTile = new TBitmap;

    MaskTile->Width = MAPCELLX;
    MaskTile->Height = MAPCELLY;
}

void TMaskForm::DarkRect(TCanvas* canvas, int xs, int ys, int xe, int ye, int darkSide)
{
    /*
  TColorRec colorRec;
  colorRec.Color = TColorRec::Aqua;
  colorRec.A = 240;

  canvas->Brush->Color = colorRec.Color;

  canvas->FillRect(TRect(xs, ys, xe, ye));
  */

    TColorRec colorRec;
    int color_r, color_g, color_b;

    for (int i = xs; i <= xe + 1; i++)
        for (int j = ys; j <= ye + 1; j++) {
            colorRec.Color = canvas->Pixels[i][j];

            color_r = colorRec.R;
            color_g = colorRec.G;
            color_b = colorRec.B;

            if (darkSide > 0) {
                if (color_r < 255 - darkSide)
                    color_r += darkSide;
                else
                    color_r = 255;

                if (color_g < 255 - darkSide)
                    color_g += darkSide;
                else
                    color_g = 255;

                if (color_b < 255 - darkSide)
                    color_b += darkSide;
                else
                    color_b = 255;
            } else {
                if (color_r > abs(darkSide))
                    color_r += darkSide;
                else
                    color_r = 0;

                if (color_g > abs(darkSide))
                    color_g += darkSide;
                else
                    color_g = 0;

                if (color_b > abs(darkSide))
                    color_b += darkSide;
                else
                    color_b = 0;
            }

            colorRec.R = color_r;
            colorRec.G = color_g;
            colorRec.B = color_b;

            canvas->Pixels[i][j] = colorRec.Color; // (TColor)((color_b << 16) | (color_g << 8) | color_r);
        }
}
//---------------------------------------------------------------------------

void TMaskForm::InitMask()
{
    int endX, endY;

    // pune maska
    try {
        delete Image;

        TPngImage* pngImage = new TPngImage;
        pngImage->Transparent = false;

        // extrag calea de baza din fileName
        AnsiString lFileName = ExtractFilePath(MainForm->fileName) + AnsiString(MainForm->tempS->sprite);

        pngImage->LoadFromFile(lFileName);

        Image = pngImage;

        Caption = "Editing properties for " + LowerCase(ExtractFileName(MainForm->tempS->sprite)) + "(" + AnsiString(Image->Width) + "x" + AnsiString(Image->Height) + ")";
    } catch (...) {
        Application->MessageBox(L"Cannot load image", L"Error", MB_OK);
        ModalResult = mrCancel;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMaskForm::FormShow(TObject* Sender)
{
    // activate the toolprop form
    FormToolProp->Show();

    // init mask
    InitMask();
    paintBox->OnPaint(this);
}
//---------------------------------------------------------------------------

// Descriere   : pune in maska "0" (se poate trece) daca patratul 32x32 e transparent
// Param       :
// Rezultat    :
// Comentarii  :
void TMaskForm::AutoMask()
{
    int endX, endY;

    for (int i = 0; i < MainForm->tempS->maskX; i++) {
        for (int j = 0; j < MainForm->tempS->maskY; j++) {
            endX = MAPCELLX * (j + 1) - 1;
            if (endX >= Image->Width) {
                endX = Image->Width - 1;
            }

            endY = MAPCELLY * (i + 1) - 1;
            if (endY >= Image->Height) {
                endY = Image->Height - 1;
            }

            // TODO
            /*
      if (IsTransparentRegion(Image->Canvas, MAPCELLX * j, MAPCELLY * i, endX, endY))
      {
        if (MainForm->tempS->mask[i][j] == 1)
        {
          // DarkRect(Image->Canvas, MAPCELLX*j, MAPCELLY*i, endX, endY, -32);
        }

        MainForm->tempS->mask[i][j] = 0;
      }
      else
      {
        if (MainForm->tempS->mask[i][j] == 0)
        {
          // DarkRect(Image->Canvas, MAPCELLX*j, MAPCELLY*i, endX, endY, 32);
        }

        MainForm->tempS->mask[i][j] = 1;
      }
      */
        }
    }

    paintBox->OnPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TMaskForm::ExitMaskEditor1Click(TObject* Sender)
{
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------

bool TMaskForm::IsTransparentRegion(TCanvas* canvas, int xs, int ys, int xe, int ye)
{
    int transparentPixels = 0, totalPixels = (xe - xs + 1) * (ye - ys + 1);

    for (int x = xs; x <= xe; x++) {
        for (int y = ys; y <= ye; y++) {
            if (canvas->Pixels[x][y] == (TColor)0) {
                transparentPixels++;
            }
        }
    }

    if (transparentPixels >= (float)totalPixels * TRANSPARENT_PERCENT) {
        return true;
    }

    return false;
}

void __fastcall TMaskForm::AutoMask1Click(TObject* Sender)
{
    AutoMask();
}
//---------------------------------------------------------------------------

void __fastcall TMaskForm::FormHide(TObject* Sender)
{
    // hide the toolprop form
    FormToolProp->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TMaskForm::paintBoxPaint(TObject* Sender)
{
    // erase
    paintBox->Canvas->Brush->Color = clBtnFace;
    paintBox->Canvas->FillRect(Rect(0, 0, paintBox->Width, paintBox->Height));

    // paint the image
    // TransparentStretchBlt();

    paintBox->Canvas->Draw(0, 0, Image);

    // paint a frame
    TPoint points[4];

    points[0] = Point(0, 0);
    points[1] = Point(Image->Width, 0);
    points[2] = Point(Image->Width, Image->Height);
    points[3] = Point(0, Image->Height);

    paintBox->Canvas->Pen->Color = clWhite;
    paintBox->Canvas->Polyline(points, 4);

    // paint the altitude as strings
    paintBox->Canvas->Font->Color = (TColor)0xFFFFFF;
    paintBox->Canvas->Font->Name = "Small Fonts";
    paintBox->Canvas->Font->Size = -7;
    SetBkMode(paintBox->Canvas->Handle, TRANSPARENT);

    for (int x = 0; x < MainForm->tempS->maskX; x++) {
        for (int y = 0; y < MainForm->tempS->maskY; y++) {
            //MainForm->tempS->altitude[x][y] = FormToolProp->GetAlt();
            //DarkRect(Image->Bitmap->Canvas, 32*y, 32*x, 32*(y + 1), 32*(x + 1), 32);
            //paintBox->Canvas->TextOut(y * 32 + 2, x * 32 + 2, AnsiString(MainForm->tempS->altitude[x][y]));
            int alt = MainForm->tempS->altitude[x][y]; // doh!

            paintBox->Canvas->TextOut(y * MAPCELLY + 2, x * MAPCELLX + 2, AnsiString(alt));
        }
    }

    // paint the layers
    // Graphics::TBitmap* lTempBmp = new Graphics::TBitmap();

    for (int i = 0; i < MainForm->tempS->mLayersCount; i++) {
        TPngImage* lBmp = FormToolProp->GetBmpByName(AnsiString(MainForm->tempS->mLayers[i].mSprite));

        if (!lBmp) {
            continue;
        }

        TRect lSrc, lDst;

        lSrc.Left = 0;
        lSrc.Right = lBmp->Width / MainForm->tempS->mLayers[i].mSpriteCols;
        lSrc.Top = 0;
        lSrc.Bottom = lBmp->Height / MainForm->tempS->mLayers[i].mSpriteRows;

        lDst.Left = MainForm->tempS->mLayers[i].mX;
        lDst.Top = MainForm->tempS->mLayers[i].mY;
        lDst.Right = lDst.Left + lBmp->Width / MainForm->tempS->mLayers[i].mSpriteCols;
        lDst.Bottom = lDst.Top + lBmp->Height / MainForm->tempS->mLayers[i].mSpriteRows;

        paintBox->Canvas->CopyRect(lDst, lBmp->Canvas, lSrc);
    }

    // delete lTempBmp;

    // paint the points, depending on what is selected
    TColor lPointColor = (TColor)0x000000;
    int lPointX, lPointY;
    bool lAllowDrawPoint = true;

    switch (FormToolProp->GetEditPoint()) {
    case EDITPOINT_DROP:
        lPointX = MainForm->tempS->mDropPoint.x;
        lPointY = MainForm->tempS->mDropPoint.y;
        lPointColor = (TColor)0xFF0000;
        break;

    case EDITPOINT_FIRE:
        lPointX = MainForm->tempS->mFirePoint.x;
        lPointY = MainForm->tempS->mFirePoint.y;
        lPointColor = (TColor)0x00FF00;
        break;

    case EDITPOINT_VISUAL:
        if (MainForm->tempS->mObsPointsCount > 0 && FormToolProp->GetCurrentVisual() >= 0) {
            lPointX = MainForm->tempS->mObsPoints[FormToolProp->GetCurrentVisual()].mMountPoints[0].x;
            lPointY = MainForm->tempS->mObsPoints[FormToolProp->GetCurrentVisual()].mMountPoints[0].y;
            lPointColor = (TColor)0x0000FF;
        } else {
            lAllowDrawPoint = false;
        }
        break;

    default:
        break;
    }

    if (lAllowDrawPoint) {
        paintBox->Canvas->Pen->Color = lPointColor;
        paintBox->Canvas->MoveTo(lPointX - 2, lPointY);
        paintBox->Canvas->LineTo(lPointX + 2, lPointY);
        paintBox->Canvas->MoveTo(lPointX, lPointY - 2);
        paintBox->Canvas->LineTo(lPointX, lPointY + 2);
    }

    PaintMask();
}
//---------------------------------------------------------------------------

void __fastcall TMaskForm::paintBoxMouseDown(TObject* Sender,
    TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (Button != mbLeft) {
        return;
    }

    if (GetToEdit() == TOEDIT_MASK) {
        int newX, newY;

        newX = Y - (Image->Height / 2 - Image->Height / 2);
        newY = X - (Image->Width / 2 - Image->Width / 2);

        if ((newX < 0) || (newY < 0))
            return;
        if ((newY / MAPCELLY > MainForm->tempS->maskY) || (newX / MAPCELLX > MainForm->tempS->maskX))
            return;

        for (int x = newX / MAPCELLY; x < newX / MAPCELLY + FormToolProp->GetMaskBrushSize(); x++) {
            for (int y = newY / MAPCELLX; y < newY / MAPCELLX + FormToolProp->GetMaskBrushSize(); y++) {
                if (MainForm->tempS->mask[x][y] == 0) {
                    MainForm->tempS->mask[x][y] = 1;
                    // DarkRect(Image->Canvas, MAPCELLX*y, MAPCELLY*x, MAPCELLX*(y + 1), MAPCELLY*(x + 1), 32);
                } else {
                    MainForm->tempS->mask[x][y] = 0;
                    // DarkRect(Image->Canvas, MAPCELLX*y, MAPCELLY*x, MAPCELLX*(y + 1), MAPCELLY*(x + 1), -32);
                }
            }
        }
    }

    if (GetToEdit() == TOEDIT_ALT) {
        int newX, newY;

        newX = Y - (Image->Height / 2 - Image->Height / 2);
        newY = X - (Image->Width / 2 - Image->Width / 2);

        if ((newX < 0) || (newY < 0))
            return;
        if ((newY / MAPCELLX > MainForm->tempS->maskY) || (newX / MAPCELLX > MainForm->tempS->maskX))
            return;

        for (int x = newX / MAPCELLY; x < newX / MAPCELLY + FormToolProp->GetAltBrushSize(); x++) {
            for (int y = newY / MAPCELLX; y < newY / MAPCELLX + FormToolProp->GetAltBrushSize(); y++) {
                MainForm->tempS->altitude[x][y] = FormToolProp->GetAlt();
            }
        }
    }

    if (GetToEdit() == TOEDIT_POINT_FIRE) {
        MainForm->tempS->mFirePoint.x = X;
        MainForm->tempS->mFirePoint.y = Y;
    }

    if (GetToEdit() == TOEDIT_POINT_DROP) {
        MainForm->tempS->mDropPoint.x = X;
        MainForm->tempS->mDropPoint.y = Y;
    }

    if (GetToEdit() == TOEDIT_POINT_VISUAL) {
        if (MainForm->tempS->mObsPointsCount > 0) {
            MainForm->tempS->mObsPoints[FormToolProp->GetCurrentVisual()].mMountPoints[0] = CPPoint(X, Y);
        }
    }

    paintBox->OnPaint(this);
    /*
  for(int i = 0; i < tempS->maskX; i++)
    for(int j = 0; j < tempS->maskY; j++)
      Memo->Text = Memo->Text + AnsiString(tempS->mask[i][j]);
  */
}
//---------------------------------------------------------------------------

TToEdit TMaskForm::GetToEdit()
{
    if (FormToolProp->PageControl->ActivePage == FormToolProp->TabMask) {
        return TOEDIT_MASK;
    }

    if (FormToolProp->PageControl->ActivePage == FormToolProp->TabAlt) {
        return TOEDIT_ALT;
    }

    if (FormToolProp->PageControl->ActivePage == FormToolProp->TabLayers) {
        return TOEDIT_LAYER;
    }

    if (FormToolProp->PageControl->ActivePage == FormToolProp->TabPoints) {
        if (FormToolProp->rbVisualPoint->Checked) {
            return TOEDIT_POINT_VISUAL;
        }

        if (FormToolProp->rbFirePoint->Checked) {
            return TOEDIT_POINT_FIRE;
        }

        if (FormToolProp->rbDropPoint->Checked) {
            return TOEDIT_POINT_DROP;
        }
    }

    return TOEDIT_NOTHING;
}
//---------------------------------------------------------------------------

void TMaskForm::PaintMask()
{
    MaskTile->Canvas->Brush->Color = FormToolProp->MaskColor->Selected;
    MaskTile->Canvas->FillRect(TRect(0, 0, MaskTile->Width, MaskTile->Height));

    int endX, endY;

    // calculez dimensiunile mastii in functie de dimensiunile imaginii
    MainForm->tempS->maskY = Image->Width / MAPCELLX;
    MainForm->tempS->maskX = Image->Height / MAPCELLY;

    // ajustari suplimentare
    if (MainForm->tempS->maskY * MAPCELLY < Image->Width)
        MainForm->tempS->maskY++;
    if (MainForm->tempS->maskX * MAPCELLX < Image->Height)
        MainForm->tempS->maskX++;

    // fac maska plina (1 peste tot)
    for (int i = 0; i < MainForm->tempS->maskX; i++) {
        for (int j = 0; j < MainForm->tempS->maskY; j++) {
            if (MainForm->tempS->mask[i][j] == 1) {
                endX = MAPCELLX * (j + 1) - 1;
                if (endX >= Image->Width) {
                    endX = Image->Width - 1;
                }

                endY = MAPCELLY * (i + 1) - 1;
                if (endY >= Image->Height) {
                    endY = Image->Height - 1;
                }

                // sau :? pentru endX, endY
                // DarkRect(Image->Canvas, MAPCELLX*j, MAPCELLY*i, endX, endY, 32);

                paintBox->Canvas->Draw(MAPCELLX * j, MAPCELLY * i, MaskTile, FormToolProp->trackMaskOpacity->Position);
            }
        }
    }
}
//---------------------------------------------------------------------------

void TMaskForm::RepaintBox()
{
    paintBox->Repaint();
}
//---------------------------------------------------------------------------

