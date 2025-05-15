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
 application: Pure Power - in game

 description: Class Piron Component Interface - pure power battle screen
 last modify: 28 05 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PPIG_CBattleControlH
#define PPIG_CBattleControlH

#include "PP_CWinControl.h"

class CPIBattleControl : public CPIControl
{
  public:
    //## Constructors (generated)
      CPIBattleControl(CPIWinControl *Parent = NULL);

    //## Destructor (generated)
      ~CPIBattleControl();

      virtual void Update();
      virtual void Paint();

      virtual void OnLeftMouseDown(int X, int Y);
      virtual void OnLeftMouseUp(int X, int Y);
      virtual void OnRightMouseClick();
      virtual void OnMouseMove(int X, int Y);
      virtual void OnMouseOn(int X, int Y);

      virtual void ComputeMouseType(int X, int Y);
  public:
  //protected:
    //## Get and Set Operations for Class Attributes (generated)

      int GetMapWidth ();
      void SetMapWidth (int value);

      int GetMapHeight ();
      void SetMapHeight (int value);

      int GetWindowLeft ();
      void SetWindowLeft (int value);

      int GetWindowTop ();
      void SetWindowTop (int value);

      CPPoint GetObjectHintPos() { return mObjectHintPos; }

  private:
  private: //## implementation
      int mMapWidth;
      int mMapHeight;
      //window - is the visible portion of the map
      int mWindowLeft;
      int mWindowTop;
      CPPoint mObjectHintPos;
};

/////////////////////////////////////////////////////////////////////////////
//GET-SET (INLINE) SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: MapWidth
---------------------------------------------------------------------------*/
inline int CPIBattleControl::GetMapWidth ()
{
  return mMapWidth;
}

inline void CPIBattleControl::SetMapWidth (int value)
{
  mMapWidth = value;
}

/*---------------------------------------------------------------------------
 description: MapHeight
---------------------------------------------------------------------------*/
inline int CPIBattleControl::GetMapHeight ()
{
  return mMapHeight;
}

inline void CPIBattleControl::SetMapHeight (int value)
{
  mMapHeight = value;
}

/*---------------------------------------------------------------------------
 description: WindowLeft
---------------------------------------------------------------------------*/
inline int CPIBattleControl::GetWindowLeft ()
{
  return mWindowLeft;
}

/*---------------------------------------------------------------------------
 description: WindowTop
---------------------------------------------------------------------------*/
inline int CPIBattleControl::GetWindowTop ()
{
  return mWindowTop;
}

#endif

