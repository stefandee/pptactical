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

 descrition : Form Tool
              tool bar-uri intr-o singura forma
              butoanele apeleaza actiuni din FormMain
 last modify: 21 11 99 
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef FToolH
#define FToolH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormTool : public TForm
{
__published:	// IDE-managed Components
        TToolBar *toolAll;
        TToolButton *btnInf;
        TToolButton *btnVehicle;
        TToolButton *btnStatic;
        TToolButton *btnAir;
        TToolButton *sep1;
        TToolButton *btnEnemy;
        TToolButton *btnNeutral;
        TToolButton *btnOwned;
        TToolButton *btnFriendly;
        TToolButton *ToolButton5;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormHide(TObject *Sender);
private:	// User declarations
public:		// User declarations
      TMenuItem *itCheck;           //pentru item-ul de meniu corespunzator
   __fastcall TFormTool(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTool *FormTool;
//---------------------------------------------------------------------------
#endif
