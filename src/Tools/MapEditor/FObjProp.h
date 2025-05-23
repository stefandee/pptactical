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
/*---------------------------------------------------------------------------
 application: PP Mission Editor

 descrition : form objects manager
 last modify: 26 12 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef FObjPropH
#define FObjPropH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "PPIG_ObjList.h"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Mask.hpp>
#include <ExtCtrls.hpp>

#include "ZCsl.hpp"
#include "CSPIN.h"
#include "ScriptPopupMenu.h"
#include <Menus.hpp>

//---------------------------------------------------------------------------
class TFormObjManager : public TForm
{
__published:	// IDE-managed Components
   TPageControl *pageMain;
   TTabSheet *sheetStatic;
   TTabSheet *sheetUnit;
   TLabel *Label2;
   TLabel *lbStaticName;
   TMaskEdit *editStaticCurrentHP;
   TLabel *Label4;
   TMemo *memoStaticStory;
   TLabel *Label6;
   TLabel *lbInfName;
   TLabel *Label8;
        TMemo *memoInfSrc;
   TLabel *lbInfHP;
   TLabel *lbStaticHP;
        TTabSheet *sheetVehicle;
        TTabSheet *sheetAir;
        TPanel *Panel1;
        TSpeedButton *btnCenterView;
        TUpDown *updownObjects;
        TLabel *lbObject;
        TLabel *Label1;
        TMemo *memoInfErrors;
        TButton *btnInfCompile;
        TEdit *edInfHp;
        TLabel *Label3;
        TEdit *edInfId;
        TLabel *Label5;
        TLabel *lbVName;
        TLabel *Label9;
        TEdit *edVHp;
        TLabel *lbVHp;
        TEdit *edVId;
        TLabel *Label11;
        TMemo *memoVSrc;
        TButton *btnVCompile;
        TMemo *memoVErrors;
        TLabel *Label7;
        TLabel *Label10;
        TCSpinEdit *edInfBodyAngle;
        TCSpinEdit *edInfScanAngle;
        TCSpinEdit *edVBodyAngle;
        TLabel *Label12;
        TLabel *Label13;
        TCSpinEdit *edVScanAngle;
        TLabel *Label14;
        TCSpinEdit *edVTurretAngle;
        TLabel *Label15;
        TLabel *lbAirName;
        TMemo *memoAirSrc;
        TButton *btnAirCompile;
        TMemo *memoAirErrors;
        TLabel *Label17;
        TEdit *edAirHp;
        TLabel *Label18;
        TEdit *edAirId;
        TLabel *Label19;
        TCSpinEdit *edAirBodyAngle;
        TLabel *lbAirHp;
        TLabel *Label20;
        TCSpinEdit *edAirAltitude;
        TButton *btnStaticBay;
        TButton *btnVBay;
        TButton *btnAirBay;
        TMemo *memoStaticErrors;
        TButton *btnStaticCompile;
        TMemo *memoStaticSrc;
        TLabel *Label16;
        TEdit *edStaticId;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall updownObjectsChanging(TObject *Sender,
          bool &AllowChange);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall edInfHpExit(TObject *Sender);
        void __fastcall edInfIdExit(TObject *Sender);
        void __fastcall memoInfSrcExit(TObject *Sender);
        void __fastcall btnInfCompileClick(TObject *Sender);
        void __fastcall edVHpExit(TObject *Sender);
        void __fastcall edVIdExit(TObject *Sender);
        void __fastcall memoVSrcExit(TObject *Sender);
        void __fastcall btnVCompileClick(TObject *Sender);
        void __fastcall edInfScanAngleChange(TObject *Sender);
        void __fastcall edInfBodyAngleChange(TObject *Sender);
        void __fastcall edVBodyAngleChange(TObject *Sender);
        void __fastcall edVTurretAngleChange(TObject *Sender);
        void __fastcall edVScanAngleChange(TObject *Sender);
        void __fastcall edAirAltitudeChange(TObject *Sender);
        void __fastcall edAirBodyAngleChange(TObject *Sender);
        void __fastcall edAirIdExit(TObject *Sender);
        void __fastcall edAirHpExit(TObject *Sender);
        void __fastcall memoAirSrcExit(TObject *Sender);
        void __fastcall btnAirCompileClick(TObject *Sender);
        void __fastcall btnVBayClick(TObject *Sender);
        void __fastcall btnAirBayClick(TObject *Sender);
        void __fastcall memoStaticSrcExit(TObject *Sender);
        void __fastcall btnStaticCompileClick(TObject *Sender);
        void __fastcall edStaticIdExit(TObject *Sender);
private:	// User declarations
    ZCsl*   mCompiler;
    ZString mModuleName;

    //ZString (__closure* mHandleDummy)(ZCsl* aCsl);

    void addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback);
    ZCsl_callback_t  mHandleDummy;
    ZString          mcDummy(ZCsl* csl);

    void ScriptToMemo(TMemo* _memo, const char* _str);

public:		// User declarations
   CIGBaseObjectList *objList;
   int currentObj;

   void setList();
   void setCurrentObject(int currentobj);
   void setStaticData();
   void setInfData();
   void setVData();
   void setAirData();
   void componentUpdate();

   __fastcall TFormObjManager(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormObjManager *FormObjManager;
//---------------------------------------------------------------------------
#endif
