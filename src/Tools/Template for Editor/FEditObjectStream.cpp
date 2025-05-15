//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FEditObjectStream.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEditObjectStream *FormEditObjectStream;
//---------------------------------------------------------------------------
__fastcall TFormEditObjectStream::TFormEditObjectStream(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
