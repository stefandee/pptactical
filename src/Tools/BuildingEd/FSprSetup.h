//---------------------------------------------------------------------------

#ifndef FSprSetupH
#define FSprSetupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CSPIN.h"
//---------------------------------------------------------------------------
class TFormSprSetup : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TLabel *Label2;
        TCSpinEdit *edRows;
        TCSpinEdit *edCols;
        TButton *btnOk;
private:	// User declarations
public:		// User declarations
        __fastcall TFormSprSetup(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSprSetup *FormSprSetup;
//---------------------------------------------------------------------------
#endif
