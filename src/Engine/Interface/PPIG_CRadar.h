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

 description: Class Piron Component Interface - Radar Control
 last modify: 19 05 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PPIG_CRadarH
#define PPIG_CRadarH

#include "PP_CWinControl.h"

class CPIRadarControl : public CPIControl  //## Inherits: <unnamed>%392541490384
{
  public:
      CPIRadarControl(CPIWinControl *Parent = NULL);
      ~CPIRadarControl();

      virtual void Update();
      virtual void Paint();

      virtual void OnMouseMove (int X, int Y);
      void OnMouseOn (int X, int Y);

      virtual void OnLeftMouseDown (int X, int Y);
      virtual void OnLeftMouseUp (int X, int Y);

      virtual void OnRightMouseDown (int X, int Y);
      virtual void OnRightMouseUp (int X, int Y);

      virtual void OnChange();

      virtual void ComputeMouseType(int X, int Y);
  public:
    //## Get and Set Operations for Class Attributes (generated)
      int GetMapWidth ();
      void SetMapWidth (int value);

      int GetMapHeight ();
      void SetMapHeight (int value);

      int GetWindowLeft ();
      void SetWindowLeft (int value);

      int GetWindowTop ();
      void SetWindowTop (int value);

      int GetWindowWidth ();
      void SetWindowWidth (int value);

      int GetWindowHeight ();
      void SetWindowHeight (int value);

      void SetWindowCenter (int x, int y);

      int GetScale();
      void SetScale(int value);
      void ComputeScale();

      bool GetRightMouseSensitive ();
      void SetRightMouseSensitive (bool value);

      virtual void SetHandleOnChange(StandardGUICallback callback);

  protected:


  private: //## implementation
      int mMapWidth;       //map width
      int mMapHeight;      //map height
      int mWindowLeft;     //window left - in real coord
      int mWindowTop;      //window top  - in real coord
      int mWindowWidth;    //window width  - in real coord
      int mWindowHeight;   //window height - in real corod
      int mScale;          //the scale factor of the radar

      bool mRightMouseSensitive;

      StandardGUICallback mHandleOnChange; // on change event
};

/////////////////////////////////////////////////////////////////////////////
//GET-SET (INLINE) SECTION
/////////////////////////////////////////////////////////////////////////////

/*---------------------------------------------------------------------------
 description: Map Width
---------------------------------------------------------------------------*/
inline int CPIRadarControl::GetMapWidth ()
{
  return mMapWidth;
}

inline void CPIRadarControl::SetMapWidth (int value)
{
  mMapWidth = value;
}

/*---------------------------------------------------------------------------
 description: Map Height
---------------------------------------------------------------------------*/
inline int CPIRadarControl::GetMapHeight ()
{
  return mMapHeight;
}

inline void CPIRadarControl::SetMapHeight (int value)
{
  mMapHeight = value;
}

/*---------------------------------------------------------------------------
 description: Window Left
---------------------------------------------------------------------------*/
inline int CPIRadarControl::GetWindowLeft ()
{
  return mWindowLeft;
}

inline void CPIRadarControl::SetWindowLeft (int value)
{
  mWindowLeft = value;
  if (mWindowLeft < 0)
    mWindowLeft = 0;
  if (mWindowLeft >= GetMapWidth() - GetWindowWidth())
    mWindowLeft = GetMapWidth() - GetWindowWidth();
}

/*---------------------------------------------------------------------------
 description: Window Top
---------------------------------------------------------------------------*/
inline int CPIRadarControl::GetWindowTop ()
{
  return mWindowTop;
}

inline void CPIRadarControl::SetWindowTop (int value)
{
  mWindowTop = value;
  if (mWindowTop < 0)
    mWindowTop = 0;
  if (mWindowTop >= GetMapHeight() - GetWindowHeight())
    mWindowTop = GetMapHeight() - GetWindowHeight() - 1;
}

/*---------------------------------------------------------------------------
 description: WindowWidth
---------------------------------------------------------------------------*/
inline int CPIRadarControl::GetWindowWidth ()
{
  return mWindowWidth;
}
inline void CPIRadarControl::SetWindowWidth (int value)
{
  mWindowWidth = value;
}

/*---------------------------------------------------------------------------
 description: WindowHeight
---------------------------------------------------------------------------*/
inline int CPIRadarControl::GetWindowHeight ()
{
  return mWindowHeight;
}
inline void CPIRadarControl::SetWindowHeight (int value)
{
  mWindowHeight = value;
}

/*---------------------------------------------------------------------------
 description: WindowCenter
---------------------------------------------------------------------------*/
inline void CPIRadarControl::SetWindowCenter (int x, int y)
{
  SetWindowLeft(x - GetWindowWidth() / 2);
  SetWindowTop(y - GetWindowHeight() / 2);
}

/*---------------------------------------------------------------------------
 description: Scale
---------------------------------------------------------------------------*/
inline int CPIRadarControl::GetScale()
{
   return mScale;
}

inline void CPIRadarControl::SetScale(int value)
{
   mScale = value;
}

inline void CPIRadarControl::ComputeScale()
{
   if (GetMapWidth() > GetMapHeight())
      mScale = GetMapWidth() / GetWidth();
   else
      mScale = GetMapHeight() / GetHeight();

   if (mScale == 0)
   {
     mScale = 1;
   }

   //sets the position of the control raported to the current pos (centered)
   SetLeft(GetLeft() + (-GetMapWidth()/mScale + GetWidth())/2);
   SetTop(GetTop() + (-GetMapHeight()/mScale + GetHeight())/2);
   //sets the dimensions of the control raported to the current pos (centered)
   SetWidth(GetMapWidth()/mScale);
   SetHeight(GetMapHeight()/mScale);
}

/*---------------------------------------------------------------------------
 description: RightMouseSensitive
---------------------------------------------------------------------------*/
inline bool CPIRadarControl::GetRightMouseSensitive ()
{
   return mRightMouseSensitive;
}

inline void CPIRadarControl::SetRightMouseSensitive (bool value)
{
   mRightMouseSensitive = value;
}

/*---------------------------------------------------------------------------
 description: Handle on change
---------------------------------------------------------------------------*/
inline void CPIRadarControl::SetHandleOnChange(StandardGUICallback callback)
{
   mHandleOnChange = callback;
}
//---------------------------------------------------------------------------

#endif

