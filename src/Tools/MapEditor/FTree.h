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

 descrition : implementeaza interfata pentru arborele de obiecte
 last modify: 30 10 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef FTreeH
#define FTreeH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <ImgList.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------

class TFormTree : public TForm
{
   __published:	// IDE-managed Components
      TPageControl *pcObjects;
      TTabSheet *tsLandscape;
      TTabSheet *tsUnits;
      TTreeView *treeLandscape;
      TTreeView *treeUnits;
      TPopupMenu *pmTree;
      TMenuItem *mitRefresh;
      TImageList *ilTree;
      TStatusBar *sbTree;
      TMenuItem *N1;
      TMenuItem *mitUnselect;
      TPanel *plSide;
      TComboBox *comboSide;
      TLabel *lbSide;
      void __fastcall FormShow(TObject *Sender);
      void __fastcall FormHide(TObject *Sender);
      void __fastcall FormCreate(TObject *Sender);
      void __fastcall mitRefreshClick(TObject *Sender);
      void __fastcall treeLandscapeCollapsed(TObject *Sender,
                                             TTreeNode *Node);
      void __fastcall treeLandscapeExpanded(TObject *Sender, TTreeNode *Node);
      void __fastcall treeLandscapeChange(TObject *Sender, TTreeNode *Node);
      void __fastcall pcObjectsChange(TObject *Sender);
      void __fastcall treeUnitsChange(TObject *Sender, TTreeNode *Node);
      void __fastcall mitUnselectClick(TObject *Sender);
      void __fastcall treeLandscapeDblClick(TObject *Sender);
      void __fastcall comboSideChange(TObject *Sender);
   void __fastcall treeUnitsDblClick(TObject *Sender);
        void __fastcall comboSideEnter(TObject *Sender);
   private:	   // User declarations
   public:		// User declarations
      TMenuItem *itCheck;           //pentru item-ul de meniu corespunzator

      __fastcall TFormTree(TComponent* Owner);

      void refreshTree();

      void buildLandscape();
      void recursiveLandscape(TTreeNode* Node, AnsiString path);
      void buildUnits();
      void buildInfantry(TTreeNode *nodeParent);
      void buildVehicle(TTreeNode *nodeParent);
      void buildAir(TTreeNode *nodeParent);

      void unSelect();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTree *FormTree;
//---------------------------------------------------------------------------
#endif
