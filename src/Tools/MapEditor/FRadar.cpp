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

 descrition : Form Radar
 last modify: 19 11 99
        by Grab 
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FRadar.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormRadar *FormRadar=NULL;
//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
//PUBLIC AREA
/////////////////////////////////////////////////////////////////////////////
__fastcall TFormRadar::TFormRadar(TComponent* Owner)
   : TForm(Owner)
{
   itCheck = NULL;
   scaleFactor = 1;
   viewWidth = 0;
   viewHeight = 0;
}
//---------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 descrition: intercepteaza WM_ERASEBCKGND
---------------------------------------------------------------------------*/
void __fastcall TFormRadar::WndProc(Messages::TMessage &Message)
{
   switch(Message.Msg)
   {
//      case WM_ERASEBKGND:
//         return;
      default:
         TForm::WndProc(Message);
   }
}

/*---------------------------------------------------------------------------
 descrition: seteaza dimensiunea portiunii care se vede
---------------------------------------------------------------------------*/
void TFormRadar::setViewPortDim(int viewwidth, int viewheight,
                                int mapwidth, int mapheight)
{
   if (!imgRadar)
      return;
   viewWidth  = viewwidth;
   viewHeight = viewheight;
   mapWidth   = mapwidth;
   mapHeight  = mapheight;
   if (mapWidth/imgRadar->Width > mapHeight/imgRadar->Height)
      scaleFactor = mapWidth/imgRadar->Width;
   else
      scaleFactor = mapHeight/imgRadar->Height;
   paintRadar();
}

/*---------------------------------------------------------------------------
 descrition: seteaza pozitia stanga-sus a viewport
---------------------------------------------------------------------------*/
void TFormRadar::setViewPortCoord(int viewleft, int viewtop)
{
   viewLeft = viewleft;
   viewTop  = viewtop;

   if (viewLeft + viewWidth >= mapWidth)
      viewLeft = mapWidth - viewWidth;
   if (viewLeft < 0)
      viewLeft = 0;
   if (viewTop + viewHeight >= mapHeight)
      viewTop = mapHeight - viewHeight;
   if (viewTop < 0)
      viewTop = 0;

   paintRadar();
}

/*---------------------------------------------------------------------------
 descrition: deseneaza dreptunghiul dat de viewport
---------------------------------------------------------------------------*/
void TFormRadar::paintRadar()
{
   if (scaleFactor == 0)
      return;

   imgRadar->Canvas->Brush->Color = clBlack;
   imgRadar->Canvas->Pen->Color   = clWhite;

   imgRadar->Canvas->Brush->Style = bsSolid;

   imgRadar->Canvas->Rectangle(1, 1, imgRadar->Width-1, imgRadar->Height-1);
   imgRadar->Canvas->Brush->Style = bsClear;
   imgRadar->Canvas->Rectangle(viewLeft/scaleFactor, viewTop/scaleFactor,
                               (viewLeft + viewWidth)/scaleFactor,
                               (viewTop  + viewHeight)/scaleFactor);
}

/////////////////////////////////////////////////////////////////////////////
//PUBLISHED AREA
/////////////////////////////////////////////////////////////////////////////
void __fastcall TFormRadar::FormShow(TObject *Sender)
{
   if (itCheck)
      itCheck->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormRadar::FormHide(TObject *Sender)
{
   if (itCheck)
      itCheck->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadar::FormCreate(TObject *Sender)
{
   setViewPortCoord(0, 0);
}
//---------------------------------------------------------------------------



void __fastcall TFormRadar::FormResize(TObject *Sender)
{
   setViewPortDim(viewWidth, viewHeight, mapWidth, mapHeight);
}
//---------------------------------------------------------------------------

