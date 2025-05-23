//---------------------------------------------------------------------------

#ifndef FEcoSysGenKernelConfigH
#define FEcoSysGenKernelConfigH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "CSPIN.h"
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include "EcoSysGenKernel.h"
//---------------------------------------------------------------------------

class TFormEcoSysGenKernelConfig : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TTabSheet *TabSheet2;
        TButton *btnOk;
        TButton *Button2;
        TCSpinEdit *edSteps;
        TLabel *Label1;
        TLabel *Label2;
        TPaintBox *paintZone;
        TDrawGrid *DrawGrid;
        TPopupMenu *PopupMenu1;
        TMenuItem *Add1;
        TMenuItem *Remove1;
        TMenuItem *Edit1;
        TMenuItem *Clear1;
        TPopupMenu *PopupMenu2;
        TMenuItem *SelectAll1;
        TMenuItem *LeftTopQuarter1;
        TMenuItem *LeftBottomQuarter1;
        TMenuItem *RightTopQuarter1;
        TMenuItem *RightBottomQuarter1;
        TColorDialog *ColorDialog;
        TOpenDialog *OpenDialog;
        TMenuItem *Deformationfunction1;
        void __fastcall paintZonePaint(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall SelectAll1Click(TObject *Sender);
        void __fastcall paintZoneMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall paintZoneMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall paintZoneMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall LeftTopQuarter1Click(TObject *Sender);
        void __fastcall LeftBottomQuarter1Click(TObject *Sender);
        void __fastcall RightTopQuarter1Click(TObject *Sender);
        void __fastcall RightBottomQuarter1Click(TObject *Sender);

        void __fastcall DrawGridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall DrawGridDblClick(TObject *Sender);
        void __fastcall Add1Click(TObject *Sender);
        void __fastcall Edit1Click(TObject *Sender);
        void __fastcall Remove1Click(TObject *Sender);
        void __fastcall Clear1Click(TObject *Sender);
        void __fastcall btnOkClick(TObject *Sender);
        void __fastcall Deformationfunction1Click(TObject *Sender);
private:	// User declarations
        EcoSysGenKernel*            mEditData;
        CPRect                      mZone;
        bool                        mSelection;
        std::vector<EcoSysItemIn*>  mItems;

        void DataToScreen();
        void ScreenToData();

        void ItemsToScreen();

public:		// User declarations
        __fastcall TFormEcoSysGenKernelConfig(TComponent* Owner);

        void SetData(EcoSysGenKernel* _data);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormEcoSysGenKernelConfig *FormEcoSysGenKernelConfig;
//---------------------------------------------------------------------------
#endif
