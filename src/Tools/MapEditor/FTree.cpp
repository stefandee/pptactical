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
#include <vcl.h>
#pragma hdrstop

#include "FTree.h"
#include "basePath.h"
#include "PPME_Defines.h" 
#include "Tools.h"
#include "FMain.h"
#include "FPreview.h"
#include "MEError.h"
#include "Paths.h"
#include "AIUnits.h"
#include "PPIG_Mission.h"
#include "PPIG_BGManager.h"
#include "PPIG_BattleGroup.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormTree *FormTree;
//---------------------------------------------------------------------------


/*---------------------------------------------------------------------------
 descrition: incercare
---------------------------------------------------------------------------*/
int __fastcall CompareFunc(TTreeNode *Node1,TTreeNode *Node2, int Reverse)

{
  int GT = AnsiStrIComp(Node1->Text.c_str(), Node2->Text.c_str());
  if (Reverse)
    return -GT;
  return GT;

}

/////////////////////////////////////////////////////////////////////////////
//PUBLIC AREA
/////////////////////////////////////////////////////////////////////////////

__fastcall TFormTree::TFormTree(TComponent* Owner)
   : TForm(Owner)
{
   itCheck = NULL;
   comboSide->ItemIndex = 0;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: recreaza tree-urile
---------------------------------------------------------------------------*/
void TFormTree::refreshTree()
{
   treeLandscape->Items->Clear();
   treeUnits->Items->Clear();
   buildLandscape();
   buildUnits();
}

/*---------------------------------------------------------------------------
 LANDSCAPE AREA
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
 descrition: creaza tree-ul de landscape
---------------------------------------------------------------------------*/
void TFormTree::buildLandscape()
{
   TTreeNode *nodeCurrent;
   treeLandscape->Items->Clear();
   nodeCurrent = treeLandscape->Items->Add(treeLandscape->Selected, "Landscapes");
   recursiveLandscape(treeLandscape->Items->Item[0], (getGamePath() + PATH_LANDSCAPE).c_str());
   treeLandscape->Items->Item[0]->Expand(false);

   try
   {
      nodeCurrent->Data = new TLandscapeTreeItem;
   }
   catch(...)
   {
      setMEError(ERR_ALLOCATE);
      Application->Terminate();
   }


   ((TLandscapeTreeItem *)nodeCurrent->Data)->Path =
                  AnsiString(getGamePath().c_str()) + PATH_LANDSCAPE;
   ((TLandscapeTreeItem *)nodeCurrent->Data)->fullName = AnsiString(getGamePath().c_str()) + PATH_LANDSCAPE;
   ((TLandscapeTreeItem *)nodeCurrent->Data)->isData = false;
   ((TLandscapeTreeItem *)nodeCurrent->Data)->objType = OT_NONE;
   ((TLandscapeTreeItem *)nodeCurrent->Data)->objName = " ";
   treeLandscape->AlphaSort();
}

/*---------------------------------------------------------------------------
 descrition: parcurge recursiv arborele de directoare si creaza tree
---------------------------------------------------------------------------*/
void TFormTree::recursiveLandscape(TTreeNode* parentnode, AnsiString path)
{
   TSearchRec srDesc;      //descrierea fisierului
   TTreeNode *nodeCurrent;
   int iAttributes = faDirectory | faAnyFile;
   int eExtensionPos;
   bool bFirst = true;     //arata daca este prima citire in directorul curent
   bool bEnd = false;      //arata daca s-a terminat citirea in directorul curent
   int objType;
   AnsiString fileName;

   while (!bEnd)
   {
      if (bFirst)
      {
         bEnd = !(FindFirst(path + "*.*", iAttributes, srDesc) == 0);
         bFirst = false;
      }
      else
         bEnd = !(FindNext(srDesc) == 0);
      if (!bEnd)
      if (srDesc.Name != "." && srDesc.Name != "..")
      {
         fileName = path + srDesc.Name;

         eExtensionPos = srDesc.Name.Pos(".dat");
         objType = OT_NONE;

         if (eExtensionPos != 0)
         {
           objType = getObjectType(fileName);
         }

         if ((srDesc.Attr & faDirectory) || (eExtensionPos != 0 && objType != OT_NONE))
         {
            if (eExtensionPos != 0)
               nodeCurrent = treeLandscape->Items->AddChild(parentnode,
                             "_" + LowerCase(getObjectName(fileName)));
            else
            {
               nodeCurrent = treeLandscape->Items->AddChild(parentnode,
                                                   LowerCase(srDesc.Name));
            }
            nodeCurrent->Text[1] = UpCase(nodeCurrent->Text[1]);
            try
            {
               nodeCurrent->Data = new TLandscapeTreeItem;
            }
            catch(...)
            {
               setMEError(ERR_ALLOCATE);
               printMEError();
               Application->Terminate();
            }

            ((TLandscapeTreeItem *)nodeCurrent->Data)->Path =
                         path /*+ "\\"*/;
            ((TLandscapeTreeItem *)nodeCurrent->Data)->fullName =
                         path /*+ "\\"*/ + srDesc.Name;


            ((TLandscapeTreeItem *)nodeCurrent->Data)->objType = objType;
            ((TLandscapeTreeItem *)nodeCurrent->Data)->isData =
                           (eExtensionPos != 0) &&
                           (((TLandscapeTreeItem *)nodeCurrent->Data)->objType != OT_NONE);

            if (objType == OT_NONE)
            {
              ((TLandscapeTreeItem *)nodeCurrent->Data)->objName = "";
            }
            else
            {
              ((TLandscapeTreeItem *)nodeCurrent->Data)->objName = getObjectName(fileName);
            }

            if (srDesc.Attr & faDirectory)
            {
               if (srDesc.Name == "Buildings")
               {
                 int k = 0;
                 k++;
               }

               recursiveLandscape(nodeCurrent, path + srDesc.Name + "\\");
            }
            else
            {
               nodeCurrent->ImageIndex = 2;
               nodeCurrent->SelectedIndex = 2;
            }
         }
      }
   }
   FindClose(srDesc);
}
/*---------------------------------------------------------------------------
 LANDSCAPE AREA - done
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 UNITS AREA
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
 descrition: creaza tree-ul de landscape
---------------------------------------------------------------------------*/
void TFormTree::buildUnits()
{
   TSearchRec sr;
   TTreeNode *nodeCurrent;
   treeUnits->Items->Clear();
   nodeCurrent = treeUnits->Items->AddChild(treeUnits->Selected, "Units");
   try
   {
      nodeCurrent->Data = new TUnitsTreeItem;
   }
   catch(...)
   {
      setMEError(ERR_ALLOCATE);
      Application->Terminate();
   }

   ((TUnitsTreeItem *)nodeCurrent->Data)->fullName = "Units";
   ((TUnitsTreeItem *)nodeCurrent->Data)->isData = false;
   ((TUnitsTreeItem *)nodeCurrent->Data)->objType = OT_NONE;
   ((TUnitsTreeItem *)nodeCurrent->Data)->objName = " ";

   buildInfantry(nodeCurrent);
   buildVehicle(nodeCurrent);
   buildAir(nodeCurrent);

   treeUnits->Items->Item[0]->Expand(false);
   treeUnits->AlphaSort();
}

/*---------------------------------------------------------------------------
 descrition: citeste aiinfantry.dat
---------------------------------------------------------------------------*/
void TFormTree::buildInfantry(TTreeNode *nodeParent)
{
   int i;
   AnsiString fileAIInf = AnsiString(getGamePath().c_str()) + PATH_AIINFANTRY + FILE_AIINFANTRY;
   TTreeNode *nodeOrdinary, *nodeSpecial, *nodeCurrent;
   hInfantryUnified recAIInf = new TInfantryUnified;

   nodeParent   = treeUnits->Items->AddChild(nodeParent, "Infantry units");
   nodeOrdinary = treeUnits->Items->AddChild(nodeParent, "Ordinary units");
   nodeSpecial  = treeUnits->Items->AddChild(nodeParent, "Special units");
   try
   {
      nodeParent->Data     = new TUnitsTreeItem;
      nodeOrdinary->Data   = new TUnitsTreeItem;
      nodeSpecial->Data    = new TUnitsTreeItem;
   }
   catch(...)
   {
      setMEError(ERR_ALLOCATE);
      Application->Terminate();
   }

   ((TUnitsTreeItem *)nodeParent->Data)->fullName  = "Infantry units";
   ((TUnitsTreeItem *)nodeParent->Data)->isData    = false;
   ((TUnitsTreeItem *)nodeParent->Data)->objType   = OT_NONE;
   ((TUnitsTreeItem *)nodeParent->Data)->objName   = " ";
   ((TUnitsTreeItem *)nodeOrdinary->Data)->fullName= "Ordinary infantry units";
   ((TUnitsTreeItem *)nodeOrdinary->Data)->isData  = false;
   ((TUnitsTreeItem *)nodeOrdinary->Data)->objType = OT_NONE;
   ((TUnitsTreeItem *)nodeOrdinary->Data)->objName = " ";
   ((TUnitsTreeItem *)nodeSpecial->Data)->fullName = "Special infantry units";
   ((TUnitsTreeItem *)nodeSpecial->Data)->isData   = false;
   ((TUnitsTreeItem *)nodeSpecial->Data)->objType  = OT_NONE;
   ((TUnitsTreeItem *)nodeSpecial->Data)->objName  = " ";

   for (i=0; i<getAIInfRecords(fileAIInf.c_str()); i++)
      if (getAIInfRecord(fileAIInf.c_str(), i, recAIInf))
      {
         if (recAIInf->type == 0)
            //adica special
            nodeCurrent = treeUnits->Items->AddChild(nodeSpecial,
                                                     AnsiString(recAIInf->name) +
                                                     " " +
                                                     AnsiString(recAIInf->surname));
         else
            //adica ordinary
            nodeCurrent = treeUnits->Items->AddChild(nodeOrdinary,
                                                     AnsiString(recAIInf->kind));
         try
         {
            nodeCurrent->Data = new TUnitsTreeItem;
         }
         catch(...)
         {
            setMEError(ERR_ALLOCATE);
            Application->Terminate();
         }

         ((TUnitsTreeItem *)nodeCurrent->Data)->Path     = fileAIInf.c_str();
         ((TUnitsTreeItem *)nodeCurrent->Data)->fullName = fileAIInf.c_str();
         ((TUnitsTreeItem *)nodeCurrent->Data)->objType  = OT_INFANTRY;
         ((TUnitsTreeItem *)nodeCurrent->Data)->isData   = true;
         ((TUnitsTreeItem *)nodeCurrent->Data)->objName  = nodeCurrent->Text;
         ((TUnitsTreeItem *)nodeCurrent->Data)->Index    = i;

         nodeCurrent->ImageIndex = 2;
         nodeCurrent->SelectedIndex = 2;
      }
      else
      {
         setMEError(ERR_READ);
         Application->Terminate();
      }
   delete recAIInf;
}

/*---------------------------------------------------------------------------
 descrition: citeste aivehicle.dat
---------------------------------------------------------------------------*/
void TFormTree::buildVehicle(TTreeNode *nodeParent)
{
   int i;
   AnsiString fileAIVeh = AnsiString(getGamePath().c_str()) + PATH_AIVEHICLE + FILE_AIVEHICLE;
   TTreeNode *nodeCurrent;
   hVehicleUnified recAIVeh = new TVehicleUnified;

   nodeParent   = treeUnits->Items->AddChild(nodeParent, "Vehicles");

   try
   {
      nodeParent->Data     = new TUnitsTreeItem;
   }
   catch(...)
   {
      setMEError(ERR_ALLOCATE);
      Application->Terminate();
   }

   ((TUnitsTreeItem *)nodeParent->Data)->fullName  = "Vehicles";
   ((TUnitsTreeItem *)nodeParent->Data)->isData    = false;
   ((TUnitsTreeItem *)nodeParent->Data)->objType   = OT_NONE;
   ((TUnitsTreeItem *)nodeParent->Data)->objName   = " ";

   for (i = 0; i < getAIVehRecords(fileAIVeh.c_str()); i++)
   {
      if (getAIVehRecord(fileAIVeh.c_str(), i, recAIVeh))
      {
         if (
              recAIVeh->vType != 4 &&
              recAIVeh->vType != 5 &&
              recAIVeh->vType != 6 &&
              recAIVeh->vType != 7
            ) // aircrafts are dealt with in the next method
         {
           nodeCurrent = treeUnits->Items->AddChild(nodeParent, AnsiString(recAIVeh->name) + "/" + AnsiString(recAIVeh->callsign) + "(" + GetVehicleType(recAIVeh->vType) + ")");

           try
           {
              nodeCurrent->Data = new TUnitsTreeItem;
           }
           catch(...)
           {
              setMEError(ERR_ALLOCATE);
              Application->Terminate();
           }

           ((TUnitsTreeItem *)nodeCurrent->Data)->Path     = fileAIVeh.c_str();
           ((TUnitsTreeItem *)nodeCurrent->Data)->fullName = fileAIVeh.c_str();
           ((TUnitsTreeItem *)nodeCurrent->Data)->objType  = OT_VEHICLE;
           ((TUnitsTreeItem *)nodeCurrent->Data)->isData   = true;
           ((TUnitsTreeItem *)nodeCurrent->Data)->objName  = nodeCurrent->Text;
           ((TUnitsTreeItem *)nodeCurrent->Data)->Index    = i;

           nodeCurrent->ImageIndex = 2;
           nodeCurrent->SelectedIndex = 2;
         }
      }
      else
      {
         setMEError(ERR_READ);
         Application->Terminate();
      }
   }

   delete recAIVeh;
}

/*---------------------------------------------------------------------------
 descrition: citeste aiair.dat
---------------------------------------------------------------------------*/
void TFormTree::buildAir(TTreeNode *nodeParent)
{
   int i;
   AnsiString fileAIVeh = AnsiString(getGamePath().c_str()) + PATH_AIVEHICLE + FILE_AIVEHICLE;
   TTreeNode *nodeCurrent;
   hVehicleUnified recAIVeh = new TVehicleUnified; // aircrafts are still vehicles

   nodeParent   = treeUnits->Items->AddChild(nodeParent, "Aircrafts");

   try
   {
      nodeParent->Data     = new TUnitsTreeItem;
   }
   catch(...)
   {
      setMEError(ERR_ALLOCATE);
      Application->Terminate();
   }

   ((TUnitsTreeItem *)nodeParent->Data)->fullName  = "Aircrafts";
   ((TUnitsTreeItem *)nodeParent->Data)->isData    = false;
   ((TUnitsTreeItem *)nodeParent->Data)->objType   = OT_NONE;
   ((TUnitsTreeItem *)nodeParent->Data)->objName   = " ";

   for (i = 0; i < getAIVehRecords(fileAIVeh.c_str()); i++)
   {
      if (getAIVehRecord(fileAIVeh.c_str(), i, recAIVeh))
      {
         if (recAIVeh->vType >= 4 && recAIVeh->vType <= 7)  
         {
           nodeCurrent = treeUnits->Items->AddChild(nodeParent, AnsiString(recAIVeh->name) + "/" + AnsiString(recAIVeh->callsign) + "(" + GetVehicleType(recAIVeh->vType) + ")");

           try
           {
              nodeCurrent->Data = new TUnitsTreeItem;
           }
           catch(...)
           {
              setMEError(ERR_ALLOCATE);
              Application->Terminate();
           }

           ((TUnitsTreeItem *)nodeCurrent->Data)->Path     = fileAIVeh.c_str();
           ((TUnitsTreeItem *)nodeCurrent->Data)->fullName = fileAIVeh.c_str();
           ((TUnitsTreeItem *)nodeCurrent->Data)->objType  = OT_AIR;
           ((TUnitsTreeItem *)nodeCurrent->Data)->isData   = true;
           ((TUnitsTreeItem *)nodeCurrent->Data)->objName  = nodeCurrent->Text;
           ((TUnitsTreeItem *)nodeCurrent->Data)->Index    = i;

           nodeCurrent->ImageIndex = 2;
           nodeCurrent->SelectedIndex = 2;
         }
      }
      else
      {
         setMEError(ERR_READ);
         Application->Terminate();
      }
   }

   delete recAIVeh;
}

/*---------------------------------------------------------------------------
 descrition: deselecteaza selectia
             folosita din exterior (ex. right click)
---------------------------------------------------------------------------*/
void TFormTree::unSelect()
{
   treeLandscape->Selected = treeLandscape->Items->Item[0];
   treeUnits->Selected     = treeUnits->Items->Item[0];
   FormMain->setSelectedItem(NULL);
}
/*---------------------------------------------------------------------------
 UNITS AREA - done
---------------------------------------------------------------------------*/

/////////////////////////////////////////////////////////////////////////////
//PUBLISHED AREA
/////////////////////////////////////////////////////////////////////////////
void __fastcall TFormTree::FormCreate(TObject *Sender)
{
   refreshTree(); //pentru construirea initiala a listei
   comboSide->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormTree::FormShow(TObject *Sender)
{
   if (itCheck)
      itCheck->Checked = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormTree::FormHide(TObject *Sender)
{
   if (itCheck)
      itCheck->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormTree::mitRefreshClick(TObject *Sender)
{
   refreshTree();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: activeaza chestia sselectata
---------------------------------------------------------------------------*/
void __fastcall TFormTree::pcObjectsChange(TObject *Sender)
{
   if (pcObjects->ActivePage == tsUnits)
   {
      if (treeUnits->Selected)
         sbTree->Panels->Items[0]->Text = ((TUnitsTreeItem *)treeUnits->Selected->Data)->fullName;
   }
   else
   {
      if (treeLandscape->Selected)
         sbTree->Panels->Items[0]->Text = ((TLandscapeTreeItem *)treeLandscape->Selected->Data)->fullName;
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: pune poza care trebuie
---------------------------------------------------------------------------*/
void __fastcall TFormTree::treeLandscapeCollapsed(TObject *Sender,
      TTreeNode *Node)
{
   Node->ImageIndex = 0;
   Node->SelectedIndex = 0;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: pune poza care trebuie
---------------------------------------------------------------------------*/
void __fastcall TFormTree::treeLandscapeExpanded(TObject *Sender,
      TTreeNode *Node)
{
   Node->ImageIndex = 1;
   Node->SelectedIndex = 1;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: scrie in statusbar fullname din node->data
             transmite obiectul selectat catre preview form
---------------------------------------------------------------------------*/
void __fastcall TFormTree::treeLandscapeChange(TObject *Sender,
      TTreeNode *Node)
{
   if (Node->Data)
   {
      sbTree->Panels->Items[0]->Text = ((TLandscapeTreeItem *)Node->Data)->fullName;
      if (((TLandscapeTreeItem *)Node->Data)->isData)
         FormPreview->setSelectedItem((TLandscapeTreeItem *)Node->Data);
      else
         FormPreview->setSelectedItem(NULL);
   }
   else
      FormPreview->setSelectedItem(NULL);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: transmite obiectul selectat catre form main
---------------------------------------------------------------------------*/
void __fastcall TFormTree::treeLandscapeDblClick(TObject *Sender)
{
   if (treeLandscape->Selected->Data)
   {
      sbTree->Panels->Items[0]->Text = ((TLandscapeTreeItem *)treeLandscape->Selected->Data)->fullName;
      if (((TLandscapeTreeItem *)treeLandscape->Selected->Data)->isData)
         FormMain->setSelectedItem((TLandscapeTreeItem *)treeLandscape->Selected->Data);
      else
         FormMain->setSelectedItem(NULL);
   }
   else
      FormMain->setSelectedItem(NULL);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: pune in sbTree
---------------------------------------------------------------------------*/
void __fastcall TFormTree::treeUnitsChange(TObject *Sender,
      TTreeNode *Node)
{
   if (Node->Data)
   {
      sbTree->Panels->Items[0]->Text = ((TUnitsTreeItem *)Node->Data)->fullName;
      if (((TUnitsTreeItem *)Node->Data)->isData)
         FormPreview->setSelectedItem((TUnitsTreeItem *)Node->Data);
      else
         FormPreview->setSelectedItem(NULL);
   }
   else
      FormPreview->setSelectedItem(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TFormTree::treeUnitsDblClick(TObject *Sender)
{
   if (treeUnits->Selected->Data)
      if (((TUnitsTreeItem *)treeUnits->Selected->Data)->isData)
         FormMain->setSelectedItem((TUnitsTreeItem *)treeUnits->Selected->Data);
      else
         FormMain->setSelectedItem(NULL);
   else
      FormMain->setSelectedItem(NULL);
}
//---------------------------------------------------------------------------


/*---------------------------------------------------------------------------
 description: deselecteaza selectia
/---------------------------------------------------------------------------*/
void __fastcall TFormTree::mitUnselectClick(TObject *Sender)
{
   unSelect();
}
//---------------------------------------------------------------------------

void __fastcall TFormTree::comboSideChange(TObject *Sender)
{
   FormMain->setCurrentBGroup(GetMissionInstance()->GetBGManager()->GetBGroupByIndex(comboSide->ItemIndex)->GetId());
}
//---------------------------------------------------------------------------


void __fastcall TFormTree::comboSideEnter(TObject *Sender)
{
   // fill the combobox with battle group names
   comboSide->Clear();

   for(int i = 0; i < GetMissionInstance()->GetBGManager()->BGroupCount(); i++)
   {
     comboSide->Items->Add(AnsiString(GetMissionInstance()->GetBGManager()->GetBGroupByIndex(i)->GetName().c_str()));
   }

   comboSide->ItemIndex = 0;
}
//---------------------------------------------------------------------------


