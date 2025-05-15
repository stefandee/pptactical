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

//-----------------------------------------------------------------------------
//  Unit              : In-game Console Message Control
//
//  Version           : 1.0
//
//  Description       :
//    * implements a sort of a message console
//    * it should have been derived from a memo (rich text) control, but I got
//      no time to implement such a control (any volunteers? :)
//
//  History           :
//    [14.12.2001] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef PPIG_CMessageH
#define PPIG_CMessageH
//---------------------------------------------------------------------------

#include "PP_CWinControl.h"
#include "PP_Font.h"
#include "KList.h"

class CPIMessageConsole : public CPIControl
{
  private:
    CPList<CPString>* mLines;
    CPString          mText;
    bool              mWordWrap;
    CPFont*           mFont;
    CPGISprite*       mImgBkg;
    int               mAlertSound;

    void ArrangeLines();
    void SoundAlert();

  protected:

  public:
    CPIMessageConsole(CPIWinControl *Parent = NULL);
    //virtual void Update();
    virtual void Paint();

    virtual ~CPIMessageConsole();

  public:
    // get-set stuff
    virtual void     SetText(CPString _v);
    virtual CPString GetText() { return mText; };

    CPFont *GetFont();
    void  SetFont(CPFont *value);

    void AddText(CPString text);
};

inline CPFont* CPIMessageConsole::GetFont()
{
   return mFont;
}

inline void  CPIMessageConsole::SetFont(CPFont* value)
{
   delete mFont;
   mFont = value;
}

#endif
