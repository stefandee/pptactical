//---------------------------------------------------------------------------
#ifndef AirSkelEdH
#define AirSkelEdH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "AIUnits.h"
#include "ImageEx.h"
#include "CSPIN.h"
#include <Buttons.hpp>
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------

class TAirSkelEditor : public TForm
{
__published:	// IDE-managed Components
        TRadioGroup *rg_tpoints;
        TGroupBox *GroupBox1;
        TImageEx *img_body;
        TCSpinButton *CSpinButton2;
        TGroupBox *GroupBox2;
        TSpeedButton *btnEditVisual;
        TSpeedButton *btnAddVisual;
        TSpeedButton *btnRemoveVisual;
        TComboBox *cbVisual;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        
        void __fastcall FormShow(TObject *Sender);
        void __fastcall CSpinButton2UpClick(TObject *Sender);
        void __fastcall CSpinButton2DownClick(TObject *Sender);
        void __fastcall img_bodyMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall btnEditVisualClick(TObject *Sender);
        void __fastcall btnAddVisualClick(TObject *Sender);
        void __fastcall btnRemoveVisualClick(TObject *Sender);
private:	// User declarations
        void DataToScreen();
public:		// User declarations
        __fastcall TAirSkelEditor(TComponent* Owner);
        TPngImage* Sprite;
        TRect rs, rd, rg;
        int  bodyCellNo, DivideFactor;
        void DrawBody();
        hVehicleUnified tempAir;

        void SetVehicleRecord(hVehicleUnified _vehicle);
        void SetSprite(AnsiString _fileName);
};
//---------------------------------------------------------------------------
extern PACKAGE TAirSkelEditor *AirSkelEditor;
//---------------------------------------------------------------------------
#endif
