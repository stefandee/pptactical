//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "InfSkelEd.h"
#include "FObservationPointEd.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "ImageEx"
#pragma link "CSPIN"
#pragma resource "*.dfm"
TInfSkelEditor *InfSkelEditor;
//---------------------------------------------------------------------------

__fastcall TInfSkelEditor::TInfSkelEditor(TComponent* Owner)
        : TForm(Owner)
{
  bodyCellNo   = 0;
  DivideFactor = 2;
}
//---------------------------------------------------------------------------

void __fastcall TInfSkelEditor::FormShow(TObject *Sender)
{
  // all infantry have the same sprite format
  DivideFactor = 14;

  img_body->Bitmap->Width = Sprite->Width / 8;
  img_body->Bitmap->Height = Sprite->Height / DivideFactor;

  // deseneaza corpul din Sprite
  rd.Left   = 0;
  rd.Top    = 0;
  rd.Right  = img_body->Bitmap->Width;
  rd.Bottom = img_body->Bitmap->Height;

  rs.Left   = 0;
  rs.Top    = 9 * img_body->Bitmap->Height;
  rs.Right  = img_body->Bitmap->Width;
  rs.Bottom = rs.Top + img_body->Bitmap->Height;
  
  img_body->Bitmap->Canvas->CopyRect(rd, Sprite->Canvas, rs);

  DrawBody();
}
//---------------------------------------------------------------------------

void TInfSkelEditor::DrawBody()
{
  // deseneaza noul corp
  rd.Left = 0;
  rd.Top = 0;
  rd.Right = Sprite->Width / 8;
  rd.Bottom = Sprite->Height / DivideFactor;

  rs.Left = bodyCellNo * (Sprite->Width / 8);
  rs.Right = (bodyCellNo+1) * (Sprite->Width / 8);
  rs.Top = 9 * Sprite->Height / DivideFactor;
  rs.Bottom = 10 * Sprite->Height / DivideFactor;
  
  img_body->Bitmap->Canvas->CopyRect(rd, Sprite->Canvas, rs);

  int lFireX = tempInf->mFireHoles[bodyCellNo].x;
  int lFireY = tempInf->mFireHoles[bodyCellNo].y;

  img_body->Bitmap->Canvas->Pen->Color = clGreen;
  img_body->Bitmap->Canvas->MoveTo(lFireX - 2, lFireY);
  img_body->Bitmap->Canvas->LineTo(lFireX + 2, lFireY);
  img_body->Bitmap->Canvas->MoveTo(lFireX, lFireY - 2);
  img_body->Bitmap->Canvas->LineTo(lFireX, lFireY + 2);

  if (tempInf->mObsPointsCount > 0 && cbVisual->ItemIndex >= 0)
  {
    int lObsPointX = tempInf->mObsPoints[cbVisual->ItemIndex].mMountPoints[bodyCellNo].x;
    int lObsPointY = tempInf->mObsPoints[cbVisual->ItemIndex].mMountPoints[bodyCellNo].y;

    img_body->Bitmap->Canvas->Pen->Color = clYellow;
    img_body->Bitmap->Canvas->MoveTo(lObsPointX - 2, lObsPointY);
    img_body->Bitmap->Canvas->LineTo(lObsPointX + 2, lObsPointY);
    img_body->Bitmap->Canvas->MoveTo(lObsPointX    , lObsPointY - 2);
    img_body->Bitmap->Canvas->LineTo(lObsPointX    , lObsPointY + 2);
  }
  
  /*
  img_body->Bitmap->Canvas->Font->Color = (TColor)0x000000;
  img_body->Bitmap->Canvas->TextOut(0, 0, AnsiString(img_body->Bitmap->Width) + ", " + AnsiString(img_body->Bitmap->Height));
  */

  img_body->Paint();
}

void __fastcall TInfSkelEditor::CSpinButton2UpClick(TObject *Sender)
{
  bodyCellNo++;
  bodyCellNo %= 8;

  DrawBody();
}
//---------------------------------------------------------------------------

void __fastcall TInfSkelEditor::CSpinButton2DownClick(TObject *Sender)
{
  bodyCellNo--;
  if (bodyCellNo < 0) bodyCellNo = 7;

  DrawBody();
}
//---------------------------------------------------------------------------

void TInfSkelEditor::SetInfRecord(hInfantryUnified _value)
{
  tempInf = _value;

  DataToScreen();
}
//---------------------------------------------------------------------------

void __fastcall TInfSkelEditor::img_bodyMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  /*
  switch(rg_tpoints->ItemIndex)
  {
    case 0 :
      tempAir->bodyLightsNo %= 4;
      tempAir->bodyLights[tempAir->bodyLightsNo].x = X;
      tempAir->bodyLights[tempAir->bodyLightsNo].y = Y;
      tempAir->bodyLightsNo++;
      break;
    case 1:
      tempAir->primary[bodyCellNo].x = X;
      tempAir->primary[bodyCellNo].y = Y;
      break;
    case 2:
      tempAir->secondary[bodyCellNo].x = X;
      tempAir->secondary[bodyCellNo].y = Y;
      break;
  }
  */

  if (rbWeapon->Checked)
  {
    tempInf->mFireHoles[bodyCellNo].x = X;
    tempInf->mFireHoles[bodyCellNo].y = Y;
  }

  if (rbVisual->Checked && cbVisual->Items->Count > 0)
  {
    tempInf->mObsPoints[cbVisual->ItemIndex].mMountPoints[bodyCellNo] = CPPoint(X, Y);
  }

  DrawBody();
}
//---------------------------------------------------------------------------

void __fastcall TInfSkelEditor::FormCreate(TObject *Sender)
{
  try
  {
    //Sprite = new Graphics::TBitmap();
    Sprite = new TPngImage();
    //Sprite->Width = 16;
    //Sprite->Height = 16;
  }
  catch(...)
  {
    Sprite = NULL;
  }
}
//---------------------------------------------------------------------------

void TInfSkelEditor::SetSprite(AnsiString _fileName)
{
  delete Sprite;

  try
  {
    //Sprite = new Graphics::TBitmap();
    Sprite = new TPngImage();
    //Sprite->Width = 16;
    //Sprite->Height = 16;
  }
  catch(...)
  {
    Sprite = NULL;
  }

  Sprite->LoadFromFile(_fileName);
}
//---------------------------------------------------------------------------

void TInfSkelEditor::DataToScreen()
{
  cbVisual->Clear();

  for(int i = 0; i < tempInf->mObsPointsCount; i++)
  {
    cbVisual->AddItem(AnsiString(tempInf->mObsPoints[i].mName), 0);
  }

  if (cbVisual->Items->Count > 0)
  {
    cbVisual->ItemIndex = 0;
  }
  else
  {
    cbVisual->ItemIndex = -1;
  }
}
//---------------------------------------------------------------------------

void __fastcall TInfSkelEditor::btnAddVisualClick(TObject *Sender)
{
  if (tempInf->mObsPointsCount >= INFANTRY_MAXVISUAL)
  {
    return;
  }

  strcpy(tempInf->mObsPoints[tempInf->mObsPointsCount].mName, "Observation Point");
  tempInf->mObsPoints[tempInf->mObsPointsCount].mMinScanRange = 6;
  //tempInf->mObsPoints[tempInf->mObsPointsCount].mScanRange    = 6;
  tempInf->mObsPoints[tempInf->mObsPointsCount].mMaxScanRange = 10;
  tempInf->mObsPoints[tempInf->mObsPointsCount].mFOV          = 90;
  tempInf->mObsPoints[tempInf->mObsPointsCount].mScanAngle    = 0;
  tempInf->mObsPoints[tempInf->mObsPointsCount].mActive       = true;

  for(int j = 0; j < 8; j++)
  {
    tempInf->mObsPoints[tempInf->mObsPointsCount].mMountPoints[j] = CPPoint(0, 0);
  }

  tempInf->mObsPointsCount++;

  DataToScreen();
}
//---------------------------------------------------------------------------

void __fastcall TInfSkelEditor::btnRemoveVisualClick(TObject *Sender)
{
  if (tempInf->mObsPointsCount <= 0)
  {
    return;
  }

  // better safe than sorry ;)
  if (cbVisual->Items->Count <= 0 || cbVisual->Items->Count > tempInf->mObsPointsCount)
  {
    return;
  }

  if (cbVisual->ItemIndex < 0)
  {
    return;
  }

  for(int i = cbVisual->ItemIndex; i < tempInf->mObsPointsCount; i++)
  {
    tempInf->mObsPoints[i] = tempInf->mObsPoints[i + 1];
  }

  tempInf->mObsPointsCount--;

  DataToScreen();
}
//---------------------------------------------------------------------------

void __fastcall TInfSkelEditor::btnEditVisualClick(TObject *Sender)
{
  if (tempInf->mObsPointsCount <= 0)
  {
    return;
  }

  // better safe than sorry ;)
  if (cbVisual->Items->Count <= 0 || cbVisual->Items->Count > tempInf->mObsPointsCount)
  {
    return;
  }

  if (cbVisual->ItemIndex < 0)
  {
    return;
  }

  FormObservationPointEditor->SetData(&(tempInf->mObsPoints[cbVisual->ItemIndex]));
  FormObservationPointEditor->ShowModal();

  DataToScreen();
}
//---------------------------------------------------------------------------

