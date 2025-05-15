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

#include <ctype.h>

#pragma hdrstop

#include <algorithm>
using namespace std;

#include "PPIG_CMessage.h"
#include "PP_Graphic.h"
#include "Paths.h"
#include "PP_Sound.h"
#include "basepath.h"
#include "easylogging++.h"
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CPIMessageConsole::CPIMessageConsole(CPIWinControl *Parent) : CPIControl(Parent)
{
    mLines     = new CPList<CPString>(8);
    mText      = "";
    mWordWrap  = false;

    mFont  = new CPFont(PATH_FONTS + FILE_FONT_SMALL, 14, true);

    mImgBkg     = NULL;
    mImgBkg     = GetGraphicInstance()->AddSprite(CPString(PATH_INGAME_GFX) + "msg.png", 1, 1);
    mAlertSound = 0;

    CPSISoundSystem *lSSI;
    try
    {
        lSSI = GetSoundInstance();
    }
    catch(...)
    {
        LOG(ERROR) << "CPIMessageConsole::CPIMessageConsole - UNABLE TO QUERY SOUND !!!!!!!!!!";
        return;
        //throw;
    }

    mAlertSound= lSSI->LoadNew((PATH_INGAME_SFX + "snd_msgalert.wav").c_str(), 2);
}
//---------------------------------------------------------------------------

void CPIMessageConsole::Paint()
{
    if (!GetVisible())
    {
        return;
    }

    SetClipRegion();

    int lDrawAlign = PPDT_LEFT | PPDT_TOP | PPDT_SINGLELINE;
    int lL = GetAbsoluteLeft(), lT = GetAbsoluteTop();

    // acum desenez textul
    GetGraphicInstance()->SetFont(mFont);
    GetGraphicInstance()->SetColor(0xFFFFFFFF);

    // desen
    //SetClipRegion();
    if (mImgBkg != NULL)
    {
        GetGraphicInstance()->PaintSprite(lL, lT, mImgBkg, 0, 0);
    }

    if (mLines->Count() > 0)
    {
        for (int i = 0; i < min((GetHeight() - 6) / 9, mLines->Count()); i++)
        {
            /*
            GetGraphicInstance()->DrawText(
              NULL,
              mLines->Get(mLines->Count() - i - 1)->c_str(),
              CPRect(lL, lT + GetHeight() - 3 - (i + 1) * 10, lL + GetWidth() - 1, lT + GetHeight() - 3 - i * 10),
              lDrawAlign);
            */

            GetGraphicInstance()->TextOut2(
                lL + 2,
                lT + GetHeight() - 3 - (i + 1) * 10,
                mLines->Get(mLines->Count() - i - 1)->c_str());
        }
    }
}
//---------------------------------------------------------------------------

CPIMessageConsole::~CPIMessageConsole()
{
    delete mLines;
    delete mFont;
}
//---------------------------------------------------------------------------

void CPIMessageConsole::SoundAlert()
{
    CPSISoundSystem *lSSI;

    try
    {
        lSSI = GetSoundInstance();
    }
    catch(...)
    {
        LOG(ERROR) << "CPIMessageConsole::SoundAlert - UNABLE TO QUERY SOUND !!!!!!!!!!";
        //throw;
    }

    lSSI->Play(mAlertSound, 0);
}
//---------------------------------------------------------------------------

void CPIMessageConsole::SetText(CPString _v)
{
    mText = _v;

    ArrangeLines();

    SoundAlert();
}
//---------------------------------------------------------------------------

void CPIMessageConsole::AddText(CPString text)
{
    mText = mText + "\n" + text;

    ArrangeLines();

    SoundAlert();
}
//---------------------------------------------------------------------------

void CPIMessageConsole::ArrangeLines()
{
    delete mLines;

    mLines = new CPList<CPString>(8);

    if (mText == CPString(""))
    {
        return;
    }

    int lCharsWidth = GetWidth() / 4;
    char lC[2];
    CPString lStr = "", lWord = "", lTmp = "";

    lC[1] = '\0';

    //logWriteILn(GetWidth());
    //logWrite("Now arranging: "); logWriteLn(mText.c_str());

    for(int i = 0; i < mText.Length(); i++)
    {
        // will identify words; words are separated by non-alpha characters
        if (mText[i] == '\n')
        {
            lStr = lStr + lWord;
            mLines->Add(new CPString(lStr));
            //cout << lStr.c_str();
            //cout << "\n";
            //mLines->Add(new CPString(lStr));
            lStr = "";
            lWord = "";
        }
        else
        {
            lC[0] = mText[i];
            lC[1] = '\0';
            lWord = lWord + CPString(lC);

            if (!isalpha(mText[i]))
            {
                if ((lStr + lWord).Length() >= lCharsWidth)
                {
                    // lWord.Length is larger than lCharsWidth?
                    if (lWord.Length() >= lCharsWidth)
                    {
                        lTmp = lStr + lWord.SubString(0, lCharsWidth - lStr.Length());

                        mLines->Add(new CPString(lTmp));
                        //cout << lTmp.c_str();
                        //cout << "\n";

                        lTmp = lWord.SubString(lCharsWidth - lStr.Length(), lWord.Length() - 1);

                        for (int j = 0; j < lTmp.Length() / lCharsWidth; j++)
                        {
                            mLines->Add(new CPString(lTmp.SubString(j * lCharsWidth, lCharsWidth)));
                            //cout << lTmp.SubString(j * lCharsWidth, lCharsWidth);
                            //cout << "\n";
                        }

                        lWord = lTmp.SubString(lTmp.Length() - lTmp.Length() % lCharsWidth - 1, lTmp.Length() % lCharsWidth);
                        lStr = lWord;
                        lWord = "";
                    }
                    else
                    {
                        mLines->Add(new CPString(lStr));
                        //cout << lStr.c_str();
                        //cout << "\n";
                        lStr = lWord;
                        lWord = "";
                    }
                }
                else
                {
                    lStr = lStr + lWord;
                    lWord = "";
                }
            }
        }
    }

    mLines->Add(new CPString(lStr + lWord));
    //cout << lStr + lWord;
}
//---------------------------------------------------------------------------

