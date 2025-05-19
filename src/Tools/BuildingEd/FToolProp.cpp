//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FToolProp.h"
#include "Statics.h"
#include "m_sless.h"
#include "masked.h"
#include "FSprSetup.h"
#include "FObservationPointEd.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFormToolProp *FormToolProp;
//---------------------------------------------------------------------------
__fastcall TFormToolProp::TFormToolProp(TComponent* Owner)
        : TForm(Owner)
{
  gridLayers->Cells[0][0] = "NAME";
  gridLayers->Cells[1][0] = "SPRITE";
  mBmpList.clear();
}
//---------------------------------------------------------------------------

int TFormToolProp::GetMaskBrushSize()
{
  return trackMaskSize->Position;
}
//---------------------------------------------------------------------------

int TFormToolProp::GetAltBrushSize()
{
  return trackAltSize->Position;
}
//---------------------------------------------------------------------------

int TFormToolProp::GetAlt()
{
  return editAlt->Value;
}
//---------------------------------------------------------------------------

void __fastcall TFormToolProp::NewLayer1Click(TObject *Sender)
{
  if (MainForm->tempS->mLayersCount >= MaxLayers)
  {
    return;
  }

  strcpy(MainForm->tempS->mLayers[MainForm->tempS->mLayersCount].mName,   "New Layer");
  strcpy(MainForm->tempS->mLayers[MainForm->tempS->mLayersCount].mSprite, "");
  MainForm->tempS->mLayers[MainForm->tempS->mLayersCount].mFrameDelay  = 5;
  MainForm->tempS->mLayers[MainForm->tempS->mLayersCount].mCycleDelay  = 0;
  MainForm->tempS->mLayers[MainForm->tempS->mLayersCount].mX     = 0;
  MainForm->tempS->mLayers[MainForm->tempS->mLayersCount].mY     = 0;
  MainForm->tempS->mLayers[MainForm->tempS->mLayersCount].mAlpha = 1.0; // means visible ;)
  MainForm->tempS->mLayers[MainForm->tempS->mLayersCount].mSpriteRows = 1;
  MainForm->tempS->mLayers[MainForm->tempS->mLayersCount].mSpriteCols = 1;

  MainForm->tempS->mLayersCount++;

  FillStringList();
}
//---------------------------------------------------------------------------

void TFormToolProp::RecToEdit(int _layer)
{
  /*
  if (_layer < 0 || _layer >= MainForm->tempS->mLayersCount)
  {
    return;
  }
  */

  edFrameDelay->Value      = MainForm->tempS->mLayers[_layer].mFrameDelay;
  edCycleDelay->Value      = MainForm->tempS->mLayers[_layer].mCycleDelay;
  cbVisible->Checked = (MainForm->tempS->mLayers[_layer].mAlpha == 1.0);
}
//---------------------------------------------------------------------------

void TFormToolProp::EditToRec(int _layer)
{
}
//---------------------------------------------------------------------------

void TFormToolProp::FillStringList()
{
  if (MainForm->tempS->mLayersCount == 0)
  {
    cbVisible->Enabled = false;
    edFrameDelay->Enabled = false;
    edCycleDelay->Enabled = false;
    btnUp->Enabled = false;
    btnDown->Enabled = false;
    btnLeft->Enabled = false;
    btnRight->Enabled = false;
    DeleteLayer1->Enabled = false;
    RenameLayer1->Enabled = false;
    Sprite1->Enabled = false;
    SpriteSetup1->Enabled = false;
  }
  else
  {
    cbVisible->Enabled = true;
    edFrameDelay->Enabled = true;
    edCycleDelay->Enabled = true;
    btnUp->Enabled = true;
    btnDown->Enabled = true;
    btnLeft->Enabled = true;
    btnRight->Enabled = true;
    DeleteLayer1->Enabled = true;
    RenameLayer1->Enabled = true;
    Sprite1->Enabled = true;
    SpriteSetup1->Enabled = true;
  }

  // clear the list
  if (MainForm->tempS->mLayersCount == 0)
  {
    gridLayers->RowCount = 2;
    gridLayers->Cells[0][1] = "No layers - right click to add.";
  }
  else
  {
    gridLayers->RowCount = MainForm->tempS->mLayersCount + 1; // +1 for the fixed row
  }

  gridLayers->Cells[0][0] = "NAME";
  gridLayers->Cells[1][0] = "SPRITE";
  gridLayers->FixedRows = 1;

  // fill it with info - name and sprite
  for(int i = 0; i < MainForm->tempS->mLayersCount; i++)
  {
    gridLayers->Cells[0][i + 1] = AnsiString(MainForm->tempS->mLayers[i].mName);
    gridLayers->Cells[1][i + 1] = AnsiString(MainForm->tempS->mLayers[i].mSprite);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormToolProp::FormShow(TObject *Sender)
{
  for(int i = 0; i < (int)mBmpList.size(); i++)
  {
    delete mBmpList[i].Bitmap;
    mBmpList[i].Bitmap = 0;
  }

  mBmpList.clear();

  for(int j = 0; j < MainForm->tempS->mLayersCount; j++)
  {
    AddBmp(AnsiString(MainForm->tempS->mLayers[j].mSprite));
  }

  FillStringList();

  RecToEdit(gridLayers->Selection.Top - 1);
  DataToScreen();
}
//---------------------------------------------------------------------------

void __fastcall TFormToolProp::DeleteLayer1Click(TObject *Sender)
{
  for(int i = gridLayers->Selection.Top; i < MainForm->tempS->mLayersCount; i++)
  {
    MainForm->tempS->mLayers[i-1] = MainForm->tempS->mLayers[i];
  }

  if (MainForm->tempS->mLayersCount > 0)
  {
    MainForm->tempS->mLayersCount--;
  }

  FillStringList();
  RecToEdit(gridLayers->Selection.Top - 1);
  MaskForm->RepaintBox();
}
//---------------------------------------------------------------------------

void __fastcall TFormToolProp::Sprite1Click(TObject *Sender)
{
  if (OpenDialog->Execute())
  {
    DeleteBmp(AnsiString(MainForm->tempS->mLayers[gridLayers->Selection.Top - 1].mSprite));
    strncpy(MainForm->tempS->mLayers[gridLayers->Selection.Top - 1].mSprite, UTF8Encode(ExtractFileName(OpenDialog->FileName)).c_str(), 512);
    FillStringList();

    AddBmp(OpenDialog->FileName);

    if (FormSprSetup->ShowModal() == mrOk)
    {
      MainForm->tempS->mLayers[gridLayers->Selection.Top - 1].mSpriteRows = FormSprSetup->edRows->Value;
      MainForm->tempS->mLayers[gridLayers->Selection.Top - 1].mSpriteCols = FormSprSetup->edCols->Value;
    }

    MaskForm->RepaintBox();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormToolProp::cbVisibleClick(TObject *Sender)
{
  MainForm->tempS->mLayers[gridLayers->Selection.Top - 1].mAlpha = (float)cbVisible->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormToolProp::edFrameDelayChange(TObject *Sender)
{
  MainForm->tempS->mLayers[gridLayers->Selection.Top - 1].mFrameDelay = edFrameDelay->Value;
}
//---------------------------------------------------------------------------

void __fastcall TFormToolProp::gridLayersClick(TObject *Sender)
{
  RecToEdit(gridLayers->Selection.Top - 1);
}
//---------------------------------------------------------------------------

void TFormToolProp::ManageBmpList()
{
  for(int i = 0; i < (int)mBmpList.size(); i++)
  {
    bool lFound = false;
    int j;

    for(j = 0; j < MainForm->tempS->mLayersCount; j++)
    {
      AnsiString lSprite = AnsiString(MainForm->tempS->mLayers[j].mSprite);

      if (lSprite == mBmpList[i].Name && !lSprite.IsEmpty())
      {
        lFound = true;
        break;
      }
    }

    // the sprite exists in layers list, but not in bmp list. add it to bmp list
    if (!lFound)
    {
      CBmpName lBmp;

      lBmp.Name = AnsiString(MainForm->tempS->mLayers[j].mSprite);
      lBmp.Bitmap = new TPngImage();

      try
      {
        lBmp.Bitmap->LoadFromFile(lBmp.Name);
      }
      catch(...)
      {
        Application->MessageBox(L"Loading bitmap failed", L"Error", MB_OK);
        return;
      }

      mBmpList.push_back(lBmp);
    }
  }

  // removal of the bitmaps that do not exist anymore in layer list
  int lAllOk = true;

  do
  {
    for(int j = 0; j < MainForm->tempS->mLayersCount && lAllOk; j++)
    {
      bool lFound = false;
      int i;

      for(i = 0; i < (int)mBmpList.size(); i++)
      {
        if (AnsiString(MainForm->tempS->mLayers[j].mSprite) == mBmpList[i].Name)
        {
          lFound = true;
          break;
        }

        // remove the entry from bmp list
        if (!lFound)
        {
          lAllOk = false;

          std::vector<CBmpName>::iterator lIt = mBmpList.begin();
          lIt += i;

          mBmpList.erase(lIt);
        }
      }
    }
  }
  while (!lAllOk);
}
//---------------------------------------------------------------------------

TPngImage* TFormToolProp::GetBmpByName(AnsiString _name)
{
  for(int i = 0; i < (int)mBmpList.size(); i++)
  {
    if (mBmpList[i].Name == _name)
    {
      return mBmpList[i].Bitmap;
    }
  }

  return 0;
}
//---------------------------------------------------------------------------

void TFormToolProp::AddBmp(AnsiString _name)
{
  for(int i = 0; i < (int)mBmpList.size(); i++)
  {
    if (mBmpList[i].Name == _name)
    {
      return;
    }
  }

  CBmpName lBmp;

  lBmp.Name = _name;
  lBmp.Bitmap = new TPngImage();

  try
  {
    lBmp.Bitmap->LoadFromFile(ExtractFilePath(MainForm->fileName) + lBmp.Name);
  }
  catch(...)
  {
    UnicodeString message = L"Loading bitmap failed " + _name;
    Application->MessageBox(message.c_str(), L"Error", MB_OK);
    return;
  }

  lBmp.Bitmap->Transparent = true;
  //lBmp.Bitmap->TransparentColor = (TColor)0x000000;
  //lBmp.Bitmap->TransparentMode = tmAuto;

  mBmpList.push_back(lBmp);
}
//---------------------------------------------------------------------------

void TFormToolProp::DeleteBmp(AnsiString _name)
{
}
//---------------------------------------------------------------------------

void __fastcall TFormToolProp::btnUpClick(TObject *Sender)
{
  MainForm->tempS->mLayers[gridLayers->Selection.Top - 1].mY -= edStep->Value;
  MaskForm->RepaintBox();
}
//---------------------------------------------------------------------------

void __fastcall TFormToolProp::btnDownClick(TObject *Sender)
{
  MainForm->tempS->mLayers[gridLayers->Selection.Top - 1].mY += edStep->Value;
  MaskForm->RepaintBox();
}
//---------------------------------------------------------------------------

void __fastcall TFormToolProp::btnLeftClick(TObject *Sender)
{
  MainForm->tempS->mLayers[gridLayers->Selection.Top - 1].mX -= edStep->Value;
  MaskForm->RepaintBox();
}
//---------------------------------------------------------------------------

void __fastcall TFormToolProp::btnRightClick(TObject *Sender)
{
  MainForm->tempS->mLayers[gridLayers->Selection.Top - 1].mX += edStep->Value;
  MaskForm->RepaintBox();
}
//---------------------------------------------------------------------------

void __fastcall TFormToolProp::SpriteSetup1Click(TObject *Sender)
{
  FormSprSetup->ShowModal();

  MainForm->tempS->mLayers[gridLayers->Selection.Top - 1].mSpriteRows = FormSprSetup->edRows->Value;
  MainForm->tempS->mLayers[gridLayers->Selection.Top - 1].mSpriteCols = FormSprSetup->edCols->Value;

  MaskForm->RepaintBox();
}
//---------------------------------------------------------------------------

void __fastcall TFormToolProp::edCycleDelayChange(TObject *Sender)
{
  MainForm->tempS->mLayers[gridLayers->Selection.Top - 1].mCycleDelay = edCycleDelay->Value;
}
//---------------------------------------------------------------------------

void TFormToolProp::DataToScreen()
{
  cbVisual->Clear();

  for(int i = 0; i < MainForm->tempS->mObsPointsCount; i++)
  {
    cbVisual->AddItem(AnsiString(MainForm->tempS->mObsPoints[i].mName), 0);
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

void __fastcall TFormToolProp::btnEditVisualClick(TObject *Sender)
{
  if (MainForm->tempS->mObsPointsCount <= 0)
  {
    return;
  }

  // better safe than sorry ;)
  if (cbVisual->Items->Count <= 0 || cbVisual->Items->Count > MainForm->tempS->mObsPointsCount)
  {
    return;
  }

  if (cbVisual->ItemIndex < 0)
  {
    return;
  }

  FormObservationPointEditor->SetData(&(MainForm->tempS->mObsPoints[cbVisual->ItemIndex]));
  FormObservationPointEditor->ShowModal();

  DataToScreen();
}
//---------------------------------------------------------------------------

void __fastcall TFormToolProp::btnAddVisualClick(TObject *Sender)
{
  if (MainForm->tempS->mObsPointsCount >= VEHICLE_MAXVISUAL)
  {
    return;
  }

  strcpy(MainForm->tempS->mObsPoints[MainForm->tempS->mObsPointsCount].mName, "Observation Point");
  MainForm->tempS->mObsPoints[MainForm->tempS->mObsPointsCount].mMinScanRange = 6;
  //MainForm->tempS->mObsPoints[MainForm->tempS->mObsPointsCount].mScanRange    = 6;
  MainForm->tempS->mObsPoints[MainForm->tempS->mObsPointsCount].mMaxScanRange = 10;
  MainForm->tempS->mObsPoints[MainForm->tempS->mObsPointsCount].mFOV          = 90;
  MainForm->tempS->mObsPoints[MainForm->tempS->mObsPointsCount].mScanAngle    = 0;
  MainForm->tempS->mObsPoints[MainForm->tempS->mObsPointsCount].mActive       = true;

  for(int j = 0; j < 8; j++)
  {
    MainForm->tempS->mObsPoints[MainForm->tempS->mObsPointsCount].mMountPoints[j] = CPPoint(0, 0);
  }

  MainForm->tempS->mObsPointsCount++;

  DataToScreen();
}
//---------------------------------------------------------------------------

void __fastcall TFormToolProp::btnRemoveVisualClick(TObject *Sender)
{
  if (MainForm->tempS->mObsPointsCount <= 0)
  {
    return;
  }

  // better safe than sorry ;)
  if (cbVisual->Items->Count <= 0 || cbVisual->Items->Count > MainForm->tempS->mObsPointsCount)
  {
    return;
  }

  if (cbVisual->ItemIndex < 0)
  {
    return;
  }

  for(int i = cbVisual->ItemIndex; i < MainForm->tempS->mObsPointsCount; i++)
  {
    MainForm->tempS->mObsPoints[i] = MainForm->tempS->mObsPoints[i + 1];
  }

  MainForm->tempS->mObsPointsCount--;

  DataToScreen();
  
  MaskForm->RepaintBox();
}
//---------------------------------------------------------------------------

TEditPoint TFormToolProp::GetEditPoint()
{
  if (rbVisualPoint->Checked)
  {
    return EDITPOINT_VISUAL;
  }

  if (rbFirePoint->Checked)
  {
    return EDITPOINT_FIRE;
  }

  if (rbDropPoint->Checked)
  {
    return EDITPOINT_DROP;
  }

  return EDITPOINT_NOTHING;
}
//---------------------------------------------------------------------------

int TFormToolProp::GetCurrentVisual()
{
  return cbVisual->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TFormToolProp::cbVisualChange(TObject *Sender)
{
  MaskForm->RepaintBox();
}
//---------------------------------------------------------------------------

void __fastcall TFormToolProp::rbDropPointClick(TObject *Sender)
{
  MaskForm->RepaintBox();
}
//---------------------------------------------------------------------------

void __fastcall TFormToolProp::MaskColorChange(TObject *Sender)
{
  MaskForm->RepaintBox();
}
//---------------------------------------------------------------------------

void __fastcall TFormToolProp::trackMaskOpacityChange(TObject *Sender)
{
  MaskForm->RepaintBox();
}
//---------------------------------------------------------------------------

