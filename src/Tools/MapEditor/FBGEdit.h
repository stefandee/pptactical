//---------------------------------------------------------------------------

#ifndef FBGEditH
#define FBGEditH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------

#include "ZCsl.hpp"
#include <Dialogs.hpp>
#include <Menus.hpp>
#include "ScriptPopupMenu.h"
//---------------------------------------------------------------------------

class TFormBGEdit : public TForm
{
__published:	// IDE-managed Components
        TButton *btnOk;
        TGroupBox *GroupBox1;
        TGroupBox *gbScript;
        TMemo *memoSrc;
        TButton *btnCompile;
        TMemo *memoErrors;
        TLabel *Label3;
        TSpeedButton *btnPrev;
        TSpeedButton *btnNext;
        TLabel *lProgress;
        TButton *btnDelete;
        TButton *btnAdd;
        TGroupBox *gbData;
        TLabel *Label1;
        TEdit *edName;
        TEdit *edId;
        TLabel *Label2;
        TMemo *memoDesc;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall edNameExit(TObject *Sender);
        void __fastcall memoDescExit(TObject *Sender);
        void __fastcall memoSrcExit(TObject *Sender);
        void __fastcall btnPrevClick(TObject *Sender);
        void __fastcall btnNextClick(TObject *Sender);
        void __fastcall btnAddClick(TObject *Sender);
        void __fastcall btnDeleteClick(TObject *Sender);
        void __fastcall edIdExit(TObject *Sender);
        void __fastcall btnCompileClick(TObject *Sender);
private:	// User declarations
        int             mCurrentBg;

        void ShowCurrentBGroup();
        void ScriptToMemo(const char* _str);
        void UpdateIndex();
        int  GenerateUniqueId();
        bool IdUnique(int _id);

private:
        ZCsl*                 mCompiler;
        ZString               mModuleName;

        void addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback);
        ZCsl_callback_t  mHandleDummy;
        ZString          mcDummy(ZCsl* csl);

        /*
        ZString (__closure* mHandleMissionTime)(ZCsl* aCsl);
        ZString (__closure* mHandleIsUnitDead)(ZCsl* aCsl);
        ZString (__closure* mHandleZoneEvent)(ZCsl* aCsl);
        ZString (__closure* mHandleZoneEvaluateThreat)(ZCsl* aCsl);
        */

        /*
        ZString mcMissionTime(ZCsl* csl);
        ZString mcIsUnitDead(ZCsl* csl);
        ZString mcZoneEvent(ZCsl* csl);
        ZString mcZoneEvaluateThreat(ZCsl* csl);
        */
public:		// User declarations
        __fastcall TFormBGEdit(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormBGEdit *FormBGEdit;
//---------------------------------------------------------------------------
#endif
