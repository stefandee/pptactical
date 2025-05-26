//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "AirSkelEd.h"
#include "FObservationPointEd.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TAirSkelEditor *AirSkelEditor;
//---------------------------------------------------------------------------

__fastcall TAirSkelEditor::TAirSkelEditor(TComponent* Owner)
        : TForm(Owner)
{
  bodyCellNo   = 0;
  DivideFactor = 2;
}
//---------------------------------------------------------------------------
void __fastcall TAirSkelEditor::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    //SkelEditor->Visible = false;
    //SkelEditor->Enabled = false;
    //MainForm->Visible   = true;
    //MainForm->Enabled   = true;
    //delete Sprite;
}
//---------------------------------------------------------------------------


void __fastcall TAirSkelEditor::FormShow(TObject *Sender)
{
  // all aircrafts have the same sprite format
  // row 1 - the body
  // row 2 - the shadows for the body
  DivideFactor = 2;

  img_body->Bitmap->Width = Sprite->Width / 8;
  img_body->Bitmap->Height = Sprite->Height / DivideFactor;

  // deseneaza corpul avionului din Sprite
  rs.Left   = 0;
  rs.Top    = 0;
  rs.Right  = img_body->Bitmap->Width;
  rs.Bottom = img_body->Bitmap->Height;
  rd        = rs;
  img_body->Bitmap->Canvas->CopyRect(rd, Sprite->Canvas, rs);

  DrawBody();
}
//---------------------------------------------------------------------------

void TAirSkelEditor::DrawBody()
{
  // umple imaginea unde se fac testele
  /*
  rg = rd;
  rg.Right *= 2;
  rg.Bottom *= 2;
  img_ground->Bitmap->Canvas->Brush->Color = clBlack;
  img_ground->Bitmap->Canvas->FillRect(rg);

  // acum deseneaza body-ul si turela peste el
  //img_ground->Canvas->CopyMode = cmSrcCopy;
  img_ground->Bitmap->Canvas->Draw(img_ground->Width / 2 - img_body->Bitmap->Width / 2,
                           img_ground->Height / 2 - img_body->Bitmap->Height / 2,
                           img_body->Bitmap);

  img_turret->Transparent = true;
  img_turret->Bitmap->TransparentMode = tmAuto;
  img_ground->Bitmap->Canvas->Draw(img_ground->Width / 2 - img_body->Bitmap->Width / 2 + tempAir->body[bodyCellNo].x  - tempAir->turret[turretCellNo].x,
                           img_ground->Height / 2 - img_body->Bitmap->Height / 2 + tempAir->body[bodyCellNo].y - tempAir->turret[turretCellNo].y,
                           img_turret->Bitmap);
  img_turret->Transparent = false;

  // paint on the test ground the primary fire hole
  int lFireX = img_ground->Width / 2 - img_body->Bitmap->Width / 2 + tempAir->body[bodyCellNo].x  - tempAir->turret[turretCellNo].x + tempAir->primary[turretCellNo].x;
  int lFireY = img_ground->Height / 2 - img_body->Bitmap->Height / 2 + tempAir->body[bodyCellNo].y - tempAir->turret[turretCellNo].y + tempAir->primary[turretCellNo].y;

  img_ground->Bitmap->Canvas->Pen->Color = clGreen;
  img_ground->Bitmap->Canvas->MoveTo(lFireX - 2, lFireY);
  img_ground->Bitmap->Canvas->LineTo(lFireX + 2, lFireY);
  img_ground->Bitmap->Canvas->MoveTo(lFireX, lFireY - 2);
  img_ground->Bitmap->Canvas->LineTo(lFireX, lFireY + 2);

  img_ground->Paint();
  */
  // deseneaza noul corp
  rd.Left = 0;
  rd.Top = 0;
  rd.Right = Sprite->Width / 8;
  rd.Bottom = Sprite->Height / DivideFactor;

  rs.Left = bodyCellNo * (Sprite->Width / 8);
  rs.Right = (bodyCellNo+1) * (Sprite->Width / 8);
  rs.Top = 0;
  rs.Bottom = Sprite->Height / DivideFactor;
  img_body->Bitmap->Canvas->CopyRect(rd, Sprite->Canvas, rs);

  int lFireX = tempAir->primary[bodyCellNo].x;
  int lFireY = tempAir->primary[bodyCellNo].y;

  img_body->Bitmap->Canvas->Pen->Color = clGreen;
  img_body->Bitmap->Canvas->MoveTo(lFireX - 2, lFireY);
  img_body->Bitmap->Canvas->LineTo(lFireX + 2, lFireY);
  img_body->Bitmap->Canvas->MoveTo(lFireX, lFireY - 2);
  img_body->Bitmap->Canvas->LineTo(lFireX, lFireY + 2);

  /*
  img_body->Bitmap->Canvas->Font->Color = (TColor)0x000000;
  img_body->Bitmap->Canvas->TextOut(0, 0, AnsiString(img_body->Bitmap->Width) + ", " + AnsiString(img_body->Bitmap->Height));
  */
  // paint the current observation point
  if (tempAir->mObsPointsCount > 0 && cbVisual->ItemIndex >= 0)
  {
    int lObsPointX = tempAir->mObsPoints[cbVisual->ItemIndex].mMountPoints[bodyCellNo].x;
    int lObsPointY = tempAir->mObsPoints[cbVisual->ItemIndex].mMountPoints[bodyCellNo].y;

    img_body->Bitmap->Canvas->Pen->Color = clYellow;
    img_body->Bitmap->Canvas->MoveTo(lObsPointX - 2, lObsPointY);
    img_body->Bitmap->Canvas->LineTo(lObsPointX + 2, lObsPointY);
    img_body->Bitmap->Canvas->MoveTo(lObsPointX    , lObsPointY - 2);
    img_body->Bitmap->Canvas->LineTo(lObsPointX    , lObsPointY + 2);
  }

  img_body->Paint();
}

void __fastcall TAirSkelEditor::CSpinButton2UpClick(TObject *Sender)
{
  bodyCellNo++;
  bodyCellNo %= 8;

  DrawBody();
}
//---------------------------------------------------------------------------

void __fastcall TAirSkelEditor::CSpinButton2DownClick(TObject *Sender)
{
  bodyCellNo--;
  if (bodyCellNo < 0) bodyCellNo = 7;

  DrawBody();
}
//---------------------------------------------------------------------------

void TAirSkelEditor::SetVehicleRecord(hVehicleUnified _vehicle)
{
  tempAir = _vehicle;

  DataToScreen();
}
//---------------------------------------------------------------------------

void __fastcall TAirSkelEditor::img_bodyMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
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
    case 3:
      if (cbVisual->Items->Count > 0)
      {
        tempAir->mObsPoints[cbVisual->ItemIndex].mMountPoints[bodyCellNo] = CPPoint(X, Y);
      }  
      break;  
  }

  DrawBody();
}
//---------------------------------------------------------------------------

void __fastcall TAirSkelEditor::FormCreate(TObject *Sender)
{
  try
  {
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

void TAirSkelEditor::SetSprite(AnsiString _fileName)
{
  delete Sprite;

  try
  {
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

void TAirSkelEditor::DataToScreen()
{
  cbVisual->Clear();

  for(int i = 0; i < tempAir->mObsPointsCount; i++)
  {
    cbVisual->AddItem(AnsiString(tempAir->mObsPoints[i].mName), 0);
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

void __fastcall TAirSkelEditor::btnEditVisualClick(TObject *Sender)
{
  if (tempAir->mObsPointsCount <= 0)
  {
    return;
  }

  // better safe than sorry ;)
  if (cbVisual->Items->Count <= 0 || cbVisual->Items->Count > tempAir->mObsPointsCount)
  {
    return;
  }

  if (cbVisual->ItemIndex < 0)
  {
    return;
  }

  FormObservationPointEditor->SetData(&(tempAir->mObsPoints[cbVisual->ItemIndex]));
  FormObservationPointEditor->ShowModal();

  DataToScreen();
}
//---------------------------------------------------------------------------

void __fastcall TAirSkelEditor::btnAddVisualClick(TObject *Sender)
{
  if (tempAir->mObsPointsCount >= VEHICLE_MAXVISUAL)
  {
    return;
  }

  strcpy(tempAir->mObsPoints[tempAir->mObsPointsCount].mName, "Observation Point");
  tempAir->mObsPoints[tempAir->mObsPointsCount].mMinScanRange = 6;
  //tempAir->mObsPoints[tempAir->mObsPointsCount].mScanRange    = 6;
  tempAir->mObsPoints[tempAir->mObsPointsCount].mMaxScanRange = 10;
  tempAir->mObsPoints[tempAir->mObsPointsCount].mFOV          = 90;
  tempAir->mObsPoints[tempAir->mObsPointsCount].mScanAngle    = 0;
  tempAir->mObsPoints[tempAir->mObsPointsCount].mActive       = true;

  for(int j = 0; j < 8; j++)
  {
    tempAir->mObsPoints[tempAir->mObsPointsCount].mMountPoints[j] = CPPoint(0, 0);
  }

  tempAir->mObsPointsCount++;

  DataToScreen();

}
//---------------------------------------------------------------------------

void __fastcall TAirSkelEditor::btnRemoveVisualClick(TObject *Sender)
{
  if (tempAir->mObsPointsCount <= 0)
  {
    return;
  }

  // better safe than sorry ;)
  if (cbVisual->Items->Count <= 0 || cbVisual->Items->Count > tempAir->mObsPointsCount)
  {
    return;
  }

  if (cbVisual->ItemIndex < 0)
  {
    return;
  }

  for(int i = cbVisual->ItemIndex; i < tempAir->mObsPointsCount; i++)
  {
    tempAir->mObsPoints[i] = tempAir->mObsPoints[i + 1];
  }

  tempAir->mObsPointsCount--;

  DataToScreen();
}
//---------------------------------------------------------------------------

