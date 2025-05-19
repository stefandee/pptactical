//---------------------------------------------------------------------------

#ifndef FObservationPointEdH
#define FObservationPointEdH
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "CSPIN.h"
#include "AIUnits.h"
#include <Vcl.Mask.hpp>
//---------------------------------------------------------------------------

class TFormObservationPointEditor : public TForm
{
__published:	// IDE-managed Components
        TLabeledEdit *edName;
        TCSpinEdit *edMinScanRange;
        TCSpinEdit *edMaxScanRange;
        TCSpinEdit *edFOV;
        TCSpinEdit *edScanAngle;
        TLabel *Label1;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TButton *Button1;
        TCheckBox *cbActive;
        TButton *Button2;
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
   TUnitObservationPoint* mData;

   void DataToScreen();
   void ScreenToData();

public:		// User declarations
   __fastcall TFormObservationPointEditor(TComponent* Owner);

   void SetData(TUnitObservationPoint* _value);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormObservationPointEditor *FormObservationPointEditor;
//---------------------------------------------------------------------------
#endif
