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

 description: Input Form Class
              asta este un fel de formular
              accepta tipurile:
               INT, LONG, BOOL, TEXT, RELATION, FILE - relatie matematica
              format:
               TIP1%TIP2...
              initial values:
               VALUE1%VALUE2...
 
 last modify: 16 03 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FInput.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFormInput *FormInput;

/////////////////////////////////////////////////////////////////////////////
//VALUE HANDLERS SECTION
/////////////////////////////////////////////////////////////////////////////
const char *getMAPWIDTH()
{
   return "";
}
const char *getMAPHEIGHT()
{
   return "";
}

const char *getMAXEVENTS()
{
   return "10";
}

//indicatie - a se pune $ in fata identificatorului, pentru a nu fi incurcat
struct SValueHandle
{
   const char* valueName;
   const char* (*getValue)();
}
valueHandlers[]=
{
   {"$MAPWIDTH",     getMAPWIDTH},
   {"$MAPHEIGHT",    getMAPHEIGHT},
   {"$MAXEVENTS",   getMAXEVENTS}
};

/////////////////////////////////////////////////////////////////////////////
//OK HANDLERS
/////////////////////////////////////////////////////////////////////////////
void setEvent()
{
}

void setAction()
{
}


/////////////////////////////////////////////////////////////////////////////
//PRIVATE SECTION
/////////////////////////////////////////////////////////////////////////////
int TFormInput::getValueHandler(AnsiString valuename)
{
   int i,count=sizeof(valueHandlers)/sizeof(struct SValueHandle);
   for (i=0; i<count; i++)
      if (valuename == valueHandlers[i].valueName)
         return i;
   return -1;
}

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
__fastcall TFormInput::TFormInput(TComponent* Owner)
   : TForm(Owner)
{
   inputControlCount = 0;
   for (int i=0; i<MAXINPUTVALUES; i++)
   {
      inputValues[i] = "";
      tagObjects[i] = NULL;
      tagRelatedObjects[i] = 0;
   }
   nextControlTop = 15;
   tagRelatedCount = 0;
   valueCount = 0;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: formateaza input form
 parameters : title   - textul labelurilor
              format  - tipurile ce trebuie introduse
              initial - valorile initiale, sau "DEFAULT"
---------------------------------------------------------------------------*/
void TFormInput::Format(AnsiString title,
                        AnsiString format,
                        AnsiString initial)
{
   TControl *tmpControl;
   int i,j,k;
   bool defaultValues = (initial == "DEFAULT");
   AnsiString tmpValue;    //valoarea intiala
   AnsiString tmpRange;
   AnsiString tmpFormat;

   if (defaultValues)
      tmpValue = "DEFAULT";
   for (i=FormInput->ControlCount-1; i>=0; i--)
   {
      tmpControl = FormInput->Controls[i];
      if (String(tmpControl->ClassName()) != "TBitBtn")
      {
         FormInput->RemoveControl(tmpControl);
         delete tmpControl;
      }
   }
   for (i=0; i<MAXINPUTVALUES; i++)
   {
      inputValues[i] = "";
      tagObjects[i] = NULL;
      tagRelatedObjects[i] = 0;
   }
   inputControlCount = 0;
   tagRelatedCount = 0;
   nextControlTop = 15;
   valueCount = 0;
   if (format.IsEmpty() || format == "NONE")
   {
      ModalResult = mrOk;
      return;
   }

   while ((i=format.Pos("%")) != 0)
   {
      valueCount ++;
      k = title.Pos("%");
      if (!defaultValues)
      {
         j = initial.Pos("%");
         tmpValue = initial.SubString(0, j-1);
         initial.Delete(1,j);
      }
      tmpFormat = format.SubString(0, i-1);
      format.Delete(1, i);
      tmpRange = "NONE";

      if ((j = tmpFormat.Pos("[")) != 0)
      {
         if (tmpFormat[tmpFormat.Length() - 1] == ']')
            tmpRange = tmpFormat.SubString(j+1, tmpFormat.Length() - j - 1);
         tmpFormat.Delete(j, tmpFormat.Length());
      }
      createInputControl(title.SubString(1, k-1),
                         tmpFormat,
                         tmpRange,
                         tmpValue);
      title.Delete(1, k);
   }
}

/*---------------------------------------------------------------------------
 description: creaza controlul si label-ul corespunzator
---------------------------------------------------------------------------*/
void TFormInput::createInputControl(AnsiString title,
                                    AnsiString type,
                                    AnsiString range,
                                    AnsiString initvalue)
{
   TControl *control = NULL;

   //prelucrez sirurile => inlocuiesc variabilele definite in valueHandlers
   //                      cu valorile aferente
   int i,j;
   int count=sizeof(valueHandlers)/sizeof(struct SValueHandle);
   for (i=0; i<count; i++)
   {
      while ((j = title.Pos(valueHandlers[i].valueName)) != 0)
      {
         title.Delete(j, strlen(valueHandlers[i].valueName));
         title.Insert((valueHandlers[i].getValue)(), j);
      }
      while ((j = type.Pos(valueHandlers[i].valueName)) != 0)
      {
         type.Delete(j, strlen(valueHandlers[i].valueName));
         type.Insert((valueHandlers[i].getValue)(), j);
      }

      while ((j = range.Pos(valueHandlers[i].valueName)) != 0)
      {
         range.Delete(j, strlen(valueHandlers[i].valueName));
         range.Insert((valueHandlers[i].getValue)(), j);
      }
      while ((j = initvalue.Pos(valueHandlers[i].valueName)) != 0)
      {
         initvalue.Delete(j, strlen(valueHandlers[i].valueName));
         initvalue.Insert((valueHandlers[i].getValue)(), j);
      }
   }

   if (type == "INT")
      createINT(&control, title, range, initvalue);
   //--------------------------------------------------------------

   if (type == "LONG")
      createLONG(&control, title, range, initvalue);
   //--------------------------------------------------------------

   if (type == "BOOL")
      createBOOL(&control, title, range, initvalue);
   //--------------------------------------------------------------

   if (type == "TEXT")
      createTEXT(&control, title, range, initvalue);
   //--------------------------------------------------------------

   if (type == "RELATION")
      createRELATION(&control, title, range, initvalue);
   //--------------------------------------------------------------

   if (type == "FILE")
      createFILE(&control, title, range, initvalue);
   //--------------------------------------------------------------

   if (control)
   {
      TLabel *label;
      label = new TLabel(this);
      label->Caption = title;
      label->Visible = true;
      label->Left = 15;
      label->Top = nextControlTop + 4;
      InsertControl(label);

      control->Left = 100;
      control->Top = nextControlTop;
      control->Width = 150;
      control->Visible = true;
      nextControlTop += control->Height + FREESPACE;

      controlValueChange(control);
   }
}

/*---------------------------------------------------------------------------
 description: tratarea schimbarii valorii unui input control
---------------------------------------------------------------------------*/
void __fastcall TFormInput::controlValueChange(TObject *Sender)
{
   AnsiString tmpStr;

   if (String(Sender->ClassName()) == "TEdit")
      tmpStr = ((TEdit *)Sender)->Text;
   if (String(Sender->ClassName()) == "TComboBox")
      tmpStr = ((TComboBox *)Sender)->Text;
   if (String(Sender->ClassName()) == "TMemo")
      tmpStr = ((TMemo *)Sender)->Lines->Text;
   if (String(Sender->ClassName()) == "TCSpinEdit")
      tmpStr = ((TCSpinEdit *)Sender)->Text;

   inputValues[((TControl *)Sender)->Tag] = tmpStr;
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//TYPES SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: INT type
 others     : range accepta - si ,
---------------------------------------------------------------------------*/
void TFormInput::createINT(TControl **control, AnsiString title,
                           AnsiString range, AnsiString initvalue)
{
   int i,j;
   AnsiString tmpValue1, tmpValue2;
   TLabel *rangeLabel;
/*   try
   {
      initvalue.ToInt();
   }
   catch(...)
   {
      return;
   }
*/
   if ((range == "NONE") || (range.Pos("-") != 0))
   {
      tmpValue1 = "0";
      tmpValue2 = "10000";
      if ((i = range.Pos("-")) != 0)
      {//range - domeniu
         tmpValue1 = range.SubString(1, i-1);
         tmpValue2 = range.SubString(i+1, range.Length() - i);
         if ((j = getValueHandler(tmpValue1)) != -01)
            tmpValue1 = (valueHandlers[j].getValue)();
         if ((j = getValueHandler(tmpValue2)) != -1)
            tmpValue2 = (valueHandlers[j].getValue)();
         try
         {
            tmpValue1.ToInt();
            tmpValue2.ToInt();
         }
         catch(...)
         {
            return;
         }
         rangeLabel = new TLabel(this);
         rangeLabel->Caption = "(" + tmpValue1 + "-" + tmpValue2 + ")";
         rangeLabel->Visible = true;
         rangeLabel->Left = 260;
         rangeLabel->Top = nextControlTop + 4;
         InsertControl(rangeLabel);
      }
      *control = new TCSpinEdit(this);
      InsertControl(*control);
      (*control)->Tag = inputControlCount++;
      ((TCSpinEdit*)(*control))->OnChange = FormInput->controlValueChange;
      ((TCSpinEdit*)(*control))->MinValue = tmpValue1.ToInt();
      ((TCSpinEdit*)(*control))->MaxValue = tmpValue2.ToInt();
      if (initvalue == "DEFAULT")
         ((TCSpinEdit*)(*control))->Text = "0";
      else
         ((TCSpinEdit*)(*control))->Value = initvalue.ToInt();
      (*control)->Height = 20;
   }
   if ((i = range.Pos(",")) != 0)
   {//range - enumerare de valori
      rangeLabel = new TLabel(this);
      rangeLabel->Caption = "(";
      rangeLabel->Visible = true;
      rangeLabel->Left = 260;
      rangeLabel->Top = nextControlTop + 4;
      InsertControl(rangeLabel);
      (* control) = new TComboBox(this);
      InsertControl((* control));
      (* control)->Tag = inputControlCount++;
      ((TComboBox*)(* control))->OnChange = FormInput->controlValueChange;
      ((TComboBox*)(* control))->Style = csDropDownList;

      while ((i = range.Pos(",")) != 0)
      {
         tmpValue1 = range.SubString(1, i-1);
         try
         {
            tmpValue1.ToInt();
            if ((j = getValueHandler(tmpValue1)) != -1)
               tmpValue1 = (valueHandlers[j].getValue)();
            if (rangeLabel->Caption.Length() < 8)
            {
               if (rangeLabel->Caption.Length() < 5)
                  rangeLabel->Caption = rangeLabel->Caption + tmpValue1 + ",";
               else
                  rangeLabel->Caption = rangeLabel->Caption + "...";
            }
           ((TComboBox*)(* control))->Items->Add(tmpValue1);
         }
         catch(...)
         {
            //
         }
         range.Delete(1, i);
      }
      rangeLabel->Caption = rangeLabel->Caption + range + ")";
      ((TComboBox*)(* control))->Items->Add(range);

      ((TComboBox*)(* control))->ItemIndex =
               ((TComboBox*)(* control))->Items->IndexOf(initvalue);
      if (((TComboBox*)(* control))->ItemIndex == -1)
         ((TComboBox*)(* control))->ItemIndex = 0;  //adica default
      (* control)->Height = 20;
   }
}
/* INT - gata
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 description: LONG type - deocamdata apeleaza INT
 others     : range accepta - si ,
---------------------------------------------------------------------------*/
void TFormInput::createLONG(TControl **control, AnsiString title,
                            AnsiString range, AnsiString initvalue)
{
   createINT(control, title, range, initvalue);
}
/* LONG - gata
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 description: BOOL type
 others     : range este ignorat
---------------------------------------------------------------------------*/
void TFormInput::createBOOL(TControl **control, AnsiString title,
                            AnsiString range, AnsiString initvalue)
{
   (* control) = new TComboBox(this);
   InsertControl((* control));
   (* control)->Tag = inputControlCount++;
   ((TComboBox*)(* control))->OnChange = FormInput->controlValueChange;
   ((TComboBox*)(* control))->Style = csDropDownList;
   ((TComboBox*)(* control))->Items->Add("TRUE");
   ((TComboBox*)(* control))->Items->Add("FALSE");
   ((TComboBox*)(* control))->ItemIndex = 0;  //adica default
   if (initvalue == "FALSE")
      ((TComboBox*)(* control))->ItemIndex = 1;
   (* control)->Height = 20;
}
//BOOL - gata
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: TEXT type
 others     : range este ignorat
---------------------------------------------------------------------------*/
void TFormInput::createTEXT(TControl **control, AnsiString title,
                            AnsiString range, AnsiString initvalue)
{
   (* control) = new TMemo(this);
   InsertControl((* control));
   (* control)->Tag = inputControlCount++;
   ((TMemo*)(* control))->OnChange = FormInput->controlValueChange;
   if (initvalue == "DEFAULT")
      ((TMemo*)(* control))->Lines->Text = "";
   else
      ((TMemo*)(* control))->Lines->Text = initvalue;
   (* control)->Height = 100;
}
/* TEXT - gata
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 description: RELATION type
 others     : adica <, >,
              range este ignorat
---------------------------------------------------------------------------*/
void TFormInput::createRELATION(TControl **control, AnsiString title,
                                AnsiString range, AnsiString initvalue)
{
   (* control) = new TComboBox(this);
   InsertControl((* control));
   (* control)->Tag = inputControlCount++;
   ((TComboBox*)(* control))->OnChange = FormInput->controlValueChange;
   ((TComboBox*)(* control))->Style = csDropDownList;
   ((TComboBox*)(* control))->Items->Add(">");
   ((TComboBox*)(* control))->Items->Add("<");
   ((TComboBox*)(* control))->Items->Add("=");
   ((TComboBox*)(* control))->Items->Add(">=");
   ((TComboBox*)(* control))->Items->Add("<=");
   ((TComboBox*)(* control))->ItemIndex = 0;  //adica default
   ((TComboBox*)(* control))->ItemIndex =
            ((TComboBox*)(* control))->Items->IndexOf(initvalue);
   if (((TComboBox*)(* control))->ItemIndex == -1)
      ((TComboBox*)(* control))->ItemIndex = 0;  //adica default
   (* control)->Height = 20;
}
/*RELATION - gata
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 description: FILE type
 others     : range - extensiile posibile
---------------------------------------------------------------------------*/
void TFormInput::createFILE(TControl **control, AnsiString title,
                            AnsiString range, AnsiString initvalue)
{
   TSpeedButton *btnOpenDialog;
   TOpenDialog  *openDialog;

   openDialog = new TOpenDialog(this);
   (* control) = new TEdit(this);
   InsertControl((* control));
   tagObjects[inputControlCount] = openDialog;
   (* control)->Tag = inputControlCount++;
   ((TEdit*)(* control))->OnChange = FormInput->controlValueChange;
   if (initvalue == "DEFAULT")
      ((TEdit*)(* control))->Text = "";
   else
      ((TEdit*)(* control))->Text = ExtractFileName(initvalue);
   (* control)->Height = 20;
   btnOpenDialog = new TSpeedButton(this);
   btnOpenDialog->Caption = "...";
   btnOpenDialog->Left = 260;
   btnOpenDialog->Top = nextControlTop;
   btnOpenDialog->OnClick = getFileFromDialog;
   InsertControl(btnOpenDialog);
   btnOpenDialog->Tag = tagRelatedCount++;
   tagRelatedObjects[tagRelatedCount-1] = *control;

   openDialog->Title = "Open";
   openDialog->InitialDir = ExtractFilePath(initvalue);
   if (range != "NONE")
      openDialog->Filter = range;
}
void __fastcall TFormInput::getFileFromDialog(TObject *Sender)
{
   TOpenDialog *tmpDialog;
   TEdit *tmpEdit;
   tmpDialog = (TOpenDialog *)tagObjects[((TComponent *)tagRelatedObjects[((TComponent *)Sender)->Tag])->Tag];
   tmpEdit = ((TEdit *)tagRelatedObjects[((TComponent *)Sender)->Tag]);
   tmpDialog->FileName = ExtractFileName(tmpEdit->Text);
   if (tmpDialog->Execute())
   {
      tmpEdit->Text = ExtractFileName(tmpDialog->FileName);
   }
}
/* FILE - gata
---------------------------------------------------------------------------*/


