//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FObservationPointEd.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFormObservationPointEditor *FormObservationPointEditor;
//---------------------------------------------------------------------------

__fastcall TFormObservationPointEditor::TFormObservationPointEditor(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TFormObservationPointEditor::SetData(TUnitObservationPoint* _value)
{
  mData = _value;

  DataToScreen();
}
//---------------------------------------------------------------------------

void __fastcall TFormObservationPointEditor::Button1Click(TObject *Sender)
{
  if (!mData)
  {
    return;
  }

  ScreenToData();
}
//---------------------------------------------------------------------------

void TFormObservationPointEditor::DataToScreen()
{
  edName->Text          = mData->mName;
  edMinScanRange->Value = mData->mMinScanRange;
  //edScanRange->Value    = mData->mScanRange;
  edMaxScanRange->Value = mData->mMaxScanRange;
  edFOV->Value          = mData->mFOV;
  edScanAngle->Value    = mData->mScanAngle;
  cbActive->Checked     = mData->mActive;
}
//---------------------------------------------------------------------------

void TFormObservationPointEditor::ScreenToData()
{
  strcpy(mData->mName, UTF8Encode(edName->Text).c_str());

  mData->mMinScanRange = edMinScanRange->Value;
  //mData->mScanRange    = edScanRange->Value   ;
  mData->mMaxScanRange = edMaxScanRange->Value;
  mData->mFOV          = edFOV->Value         ;
  mData->mScanAngle    = edScanAngle->Value   ;
  mData->mActive       = cbActive->Checked    ;
}
//---------------------------------------------------------------------------

