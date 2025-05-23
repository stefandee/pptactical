//---------------------------------------------------------------------------

#ifndef FBayManagerH
#define FBayManagerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------

class CIGUnit;

class TFormBayManager : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TGroupBox *GroupBox2;
        TButton *btnAdd;
        TButton *btnRemove;
        TButton *btnClearBay;
        TStringGrid *gridBay;
        TStringGrid *gridAvail;
        TStatusBar *statusBay;
        TButton *Button4;
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall btnAddClick(TObject *Sender);
        void __fastcall btnRemoveClick(TObject *Sender);
        void __fastcall btnClearBayClick(TObject *Sender);
private:	// User declarations
        CIGUnit* mUnit;

        void UpdateBayGrid();
        void UpdateAvailGrid();
        void UpdateAvailGridForFileVehicles();
        void UpdateAvailGridForFileInfantry();
        void UpdateAvailGridForMapVehicles();
        void UpdateAvailGridForMapInfantry();

        void CreateBayGridFixed();
        void CreateAvailGridFixed();

        void DeleteObjects();

public:		// User declarations
        __fastcall TFormBayManager(TComponent* Owner);
        void SetTransport(CIGUnit* _tran);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormBayManager *FormBayManager;
//---------------------------------------------------------------------------
#endif
