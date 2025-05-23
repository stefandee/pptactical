//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU General Public License as     //
// published by the Free Software Foundation; either version 2 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License  //
// for more details.                                                         //
//                                                                           //
// You should have received a copy of the GNU General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Unit              : Form Script Editor
//
//  Version           : 1.0
//
//  Description       :
//    * this form is used to write the mission script and compile (verify) it
//    * please refer to engine specifications document to learn how to write
//      mission control scripts
//
//  History           :
//    [08.12.2001] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef FMissionScriptH
#define FMissionScriptH
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>

#include "ZCsl.hpp"
#include "PPIG_Doctrine.h"
#include "ScriptPopupMenu.h"

//---------------------------------------------------------------------------
class TFormMissionScript : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *gbScript;
        TMemo *memoSrc;
        TGroupBox *gbErrors;
        TMemo *memoErrors;
        TButton *btnOk;
        TButton *Button2;
        TButton *btnCompile;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btnOkClick(TObject *Sender);
        void __fastcall btnCompileClick(TObject *Sender);
        /*
        void __fastcall Load1Click(TObject *Sender);
        void __fastcall Save1Click(TObject *Sender);
        void __fastcall Undo1Click(TObject *Sender);
        void __fastcall Cut1Click(TObject *Sender);
        void __fastcall Copy1Click(TObject *Sender);
        void __fastcall Paste1Click(TObject *Sender);
        void __fastcall SelectAll1Click(TObject *Sender);
        void __fastcall Clear1Click(TObject *Sender);
        */
private:	// User declarations
        ZCsl*                 mCompiler;
        ZString               mModuleName;
        //CPPIG_ScriptObserver* mScriptObserver;

        // these are functions and handles for the script compiler
        void addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback);
        ZCsl_callback_t  mHandleDummy;
        ZString          mcDummy(ZCsl* csl);

        /*
        ZString (__closure* mHandleMissionTime)(ZCsl* aCsl);
        ZString (__closure* mHandleInsertInfantry)(ZCsl* aCsl);
        ZString (__closure* mHandlePostMessage)(ZCsl* aCsl);

        ZString mcMissionTime(ZCsl* csl);
        ZString mcInsertInfantry(ZCsl* csl);
        ZString mcPostMessage(ZCsl* csl);
        */

public:		// User declarations
        void StringToMemo(const char* _str);
        __fastcall TFormMissionScript(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMissionScript *FormMissionScript;
//---------------------------------------------------------------------------
#endif
