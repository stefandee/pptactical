//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License as     //
// published by the Free Software Foundation; either version 2.1 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License  //
// for more details.                                                         //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 application: Pure Power

 description: Class Cursor
              update, paint,
              mouse down, up, press
              set type, speedfactor, pos
              Singleton Pattern
 last modify: 21 04 00
        by grabX
---------------------------------------------------------------------------*/
#pragma hdrstop

#include "PPIG_Cursor.h"
#include "basepath.h"
#include "Paths.h"             //pentru a sti cum se cheama fisierul
#include "PP_Graphic.h"
#include "easylogging++.h"

CCursor *CCursor::mInstance = NULL;
int      CCursor::mInstanceCount = 0;

/*---------------------------------------------------------------------------
 description: returneaza instanta de cursor system
 others     : a aparut din cauza warningurilor
---------------------------------------------------------------------------*/
CPGICursor* GetCursorInstance()
{
   return CCursor::Instance();
}

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: returneaza o instanta a cursorului
---------------------------------------------------------------------------*/
CCursor* CCursor::Instance()
{
   if (mInstance == NULL)
   {
      mInstance = new CCursor();
   }
   mInstanceCount ++;
   return mInstance;
}

/*---------------------------------------------------------------------------
 description: elibereaza instanta cursorului
---------------------------------------------------------------------------*/
void CCursor::Release()
{
   if (mInstance != NULL)
   {
      delete mInstance;
      mInstance = NULL;
   }
}
/*--------------------------------------------------------------------------
 Description: metoda de afisare
 Parameters : none
 Others     : none
 --------------------------------------------------------------------------*/
void CCursor::Paint()
{
   if (Sprite == NULL)
      return;

   CPGIGraphicSystem *lGSI;   //adica local Graphic System Instance
   try
   {
      lGSI = GetGraphicInstance();
   }
   catch(...)
   {
      LOG(FATAL) << "Cursor::Paint - UNABLE TO QUERY GRAPHICS !!!!!!!!!!";
      throw;
   }

   try
   {
      if (Type == MC_ONOWNED || Type == MC_ONENEMY )
      {
         lGSI->PaintSprite(Position.x, Position.y, Sprite, Anim, MC_DEFAULT);
         lGSI->PaintSprite(Position.x, Position.y - 6, Sprite, Anim, Type);
      }
      else
      {
         if (Type == MC_GUARD || Type == MC_CAPTURE ||
            Type == MC_PATROL || Type == MC_LOAD ||
            Type == MC_UNLOAD || Type == MC_LAND)
         {
            lGSI->PaintSprite(Position.x, Position.y, Sprite, Anim, MC_MOVE);
            lGSI->PaintSprite(Position.x, Position.y - 6, Sprite, Anim, Type);
         }
         else
            lGSI->PaintSprite(Position.x, Position.y, Sprite, Anim, Type);
      }
   }
   catch(...)
   {
      LOG(ERROR) << "UNABLE TO PAINT CURSOR !!!!!!!!!!!!!";
   }
}

/*--------------------------------------------------------------------------
 Description: metoda care face update de animatie, pozitie a cursorului
 Parameters :
 Others     : se apeleaza la fiecare tick
              foloseste adresare directa catre Inputs
              (poate nu-i chiar bine)
--------------------------------------------------------------------------*/
void CCursor::Update()
{
   CPIIInputSystem *lISI;
   try
   {
      lISI = lISI->Instance();
   }
   catch(...)
   {
      throw;
   }
   lISI->UpdateMouse();
   oldPosition = CPPoint(Position.x, Position.y);

   Position.x += speedFactor * lISI->Mouse.x;
   Position.y += speedFactor * lISI->Mouse.y;

   if (Position.x + hotSpot.x > mRestricted.right)
      Position.x = mRestricted.right - hotSpot.x;
   if (Position.x + hotSpot.x < mRestricted.left)
      Position.x = mRestricted.left - hotSpot.x ;
   if (Position.y + hotSpot.y  > mRestricted.bottom)
      Position.y = mRestricted.bottom - hotSpot.y ;
   if (Position.y + hotSpot.y < mRestricted.top)
      Position.y = mRestricted.top - hotSpot.y ;

   oldButtons = Buttons;

   Buttons.Left   = (lISI->MouseLB != 0);
   Buttons.Right  = (lISI->MouseRB != 0);
   Buttons.Middle = (lISI->MouseMB != 0);

   if (updateCounter++ ==3)
      updateCounter = 0;
   else
      return;

   Anim++;
   Anim = Anim % 6;
}

/*--------------------------------------------------------------------------
 Description: seteaza tipul cursorului de mouse
 Parameters : tip (constante definite in GameDef)
 Others     : se apeleaza atunci cand se trece peste un screen
 -------------------------------------------------------------------------*/
void CCursor::SetType(int type)
{
   Position.x += gMouseHotSpots[Type].x - gMouseHotSpots[type].x;
   Position.y += gMouseHotSpots[Type].y - gMouseHotSpots[type].y;
   hotSpot.x = gMouseHotSpots[type].x;
   hotSpot.y = gMouseHotSpots[type].y;
   Type = type;
}


/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------
 Description: constructorul
 --------------------------------------------------------------------------*/
CCursor::CCursor()
{
   CPGIGraphicSystem *lGSI;   //adica local Graphic System Instance
   try
   {
      lGSI = lGSI->Instance();
   }
   catch(...)
   {
      throw;
   }

   Sprite = NULL;
   Sprite = lGSI->AddSprite(CPString(PATH_INGAME_GFX) + FILE_MOUSE_GFX, 6, MOUSE_CURSORS_COUNT);

   Anim = 0;

   // the initial position needs to be the position initialized in CPGIGraphicSystem::InitGraphics
   Position = CPPoint(lGSI->GetResX() / 2, lGSI->GetResY() / 2);
   oldPosition = Position;

   Type = 0;
   SetType(MC_DEFAULT);
   speedFactor = 1;
   updateCounter = 0;
   mRestricted = CPRect(0, 0, lGSI->GetResX(), lGSI->GetResY());
}

/*--------------------------------------------------------------------------
 Description: destructorul
 ---------------------------------------------------------------------------*/
CCursor::~CCursor()
{
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
