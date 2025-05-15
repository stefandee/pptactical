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
#ifndef CursorH
#define CursorH

#include "PP_Sprite.h"
#include "PP_Input.h"
#include "PPIG_InterfaceDef.h"
#include "basic2d.h"

class CCursor
 {
   public:
      static CCursor*   Instance();
      static void       Release();

      void Update();
      void Paint();

      void SetType(int type);
      void setPosition(CPPoint position);
      void setSpeedFactor(int sf);

      int  GetPosX();
      int  GetPosY();

      bool IsLeftDown();
      bool IsLeftPressed();
      bool IsLeftUp();

      bool IsRightDown();
      bool IsRightPressed();
      bool IsRightUp();

      bool IsMoved();
   protected:
      CCursor();
      ~CCursor();

   private:
      //instanta
      static CCursor       *mInstance;
      static int            mInstanceCount;

      int Type;
      int Anim;
      CSprite *Sprite;
      TMButtons Buttons, oldButtons;

      CPPoint hotSpot;
      CPPoint Position, oldPosition;

      int speedFactor;                 //folosit pentru a face mouse-ul sa 'mearga'
                                       //mai repede

      int updateCounter;
      CPRect    mRestricted;
};

typedef CCursor CPGICursor;
typedef CCursor CPIICursor;

CPGICursor* GetCursorInstance();


/////////////////////////////////////////////////////////////////////////////
//INLINE SECTION
/////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------
 Description: seteaza pozitia cursorului
 Parameters : pozitia
 Others     : se apeleaza la fiecare tick
--------------------------------------------------------------------------*/
inline void CCursor::setPosition(CPPoint position)
{
   Position = position;
}

/*--------------------------------------------------------------------------
 Description: seteaza factorul de viteza
 Others     : acesta se inmulteste cu deplasarea
--------------------------------------------------------------------------*/
inline void CCursor::setSpeedFactor(int sf)
{
   speedFactor = sf;
}

/*---------------------------------------------------------------------------
 description: returneaza pozitia X a cursorlui
---------------------------------------------------------------------------*/
inline int CCursor::GetPosX()
{
   return Position.x + hotSpot.x;
}
/*---------------------------------------------------------------------------
 description: returneaza pozitia y a cursorului
---------------------------------------------------------------------------*/
inline int CCursor::GetPosY()
{
   return Position.y + hotSpot.y;
}

/*--------------------------------------------------------------------------
 Description: arata evenimentul de mouse left down
--------------------------------------------------------------------------*/
inline bool CCursor::IsLeftDown()
{
   return (oldButtons.Left == false && Buttons.Left == true);
}

/*--------------------------------------------------------------------------
 Description: arata evenimentul de mouse left pressed
--------------------------------------------------------------------------*/
inline bool CCursor::IsLeftPressed()
{
   return (oldButtons.Left == true && Buttons.Left == true);
}

/*--------------------------------------------------------------------------
 Description: arata evenimentul de mouse left up
--------------------------------------------------------------------------*/
inline bool CCursor::IsLeftUp()
{
   return (oldButtons.Left == true && Buttons.Left == false);
}
/*--------------------------------------------------------------------------
 Description: arata evenimentul de mouse left down
--------------------------------------------------------------------------*/
inline bool CCursor::IsRightDown()
{
   return (oldButtons.Right == false && Buttons.Right == true);
}

/*--------------------------------------------------------------------------
 Description: arata evenimentul de mouse left pressed
--------------------------------------------------------------------------*/
inline bool CCursor::IsRightPressed()
{
   return (oldButtons.Right == true && Buttons.Right == true);
}

/*--------------------------------------------------------------------------
 Description: arata evenimentul de mouse left up
--------------------------------------------------------------------------*/
inline bool CCursor::IsRightUp()
{
   return (oldButtons.Right == true && Buttons.Right == false);
}

/*---------------------------------------------------------------------------
 description: arata daca cursorul s-a mutat
---------------------------------------------------------------------------*/
inline bool CCursor::IsMoved()
{
   return (oldPosition.x != Position.x || oldPosition.y != Position.y);
}

//---------------------------------------------------------------------------
#endif
