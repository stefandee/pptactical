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

//-----------------------------------------------------------------------------
//  Unit              : PPME_Desktop
//
//  Version           : 1.0
//
//  Description       :
//    * holds a map editor desktop information class (e.g. windows positions and sizes)
//
//  History           :
//    [29.03.2002] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef PPME_DesktopH
#define PPME_DesktopH
//---------------------------------------------------------------------------

#include "basic2d.h"
#include "PP_String.h"
#include "Graphics.hpp"

class CFileHistory;

// map editor desktop class
class CPPME_Desktop
{
  public: // c-d
    CPPME_Desktop();
    ~CPPME_Desktop();

  public: // screens get-set
    CPRect GetWndPreview()           { return mWndPreview; };
    void   SetWndPreview(CPRect _v)  { mWndPreview = _v; };

    CPRect GetWndRadar()             { return mWndRadar; };
    void   SetWndRadar(CPRect _v)    { mWndRadar = _v; };

    CPRect GetWndTree()              { return mWndTree; };
    void   SetWndTree(CPRect _v)     { mWndTree = _v; };

    CPRect GetWndTool()              { return mWndTool; };
    void   SetWndTool(CPRect _v)     { mWndTool = _v; };

    CPRect GetWndInfo()              { return mWndInfo; };
    void   SetWndInfo(CPRect _v)     { mWndInfo = _v; };

    CPRect GetWndAltitude()          { return mWndAltitude; };
    void   SetWndAltitude(CPRect _v) { mWndAltitude = _v; };

    bool   GetWndPreviewVisible()         { return mWndPreviewVisible; };
    void   SetWndPreviewVisible(bool _v)  { mWndPreviewVisible = _v; };

    bool   GetWndRadarVisible()           { return mWndRadarVisible; };
    void   SetWndRadarVisible(bool _v)    { mWndRadarVisible = _v; };

    bool   GetWndToolVisible()            { return mWndToolVisible; };
    void   SetWndToolVisible(bool _v)     { mWndToolVisible = _v; };

    bool   GetWndTreeVisible()            { return mWndTreeVisible; };
    void   SetWndTreeVisible(bool _v)     { mWndTreeVisible = _v; };

    bool   GetWndInfoVisible()            { return mWndInfoVisible; };
    void   SetWndInfoVisible(bool _v)     { mWndInfoVisible = _v; };

    bool   GetWndAltitudeVisible()        { return mWndAltitudeVisible; };
    void   SetWndAltitudeVisible(bool _v) { mWndAltitudeVisible = _v; };

  public: // environment get-set
    void   SetSaveDesktopOnExit(bool _v)  { mSaveDesktopOnExit = _v; };
    bool   GetSaveDesktopOnExit()         { return mSaveDesktopOnExit; };

  public: // grid get-set
    void SetGridShown(bool _v)  { mGridShown = _v; };
    bool GetGridShown()         { return mGridShown; };

    void    SetGridH(__int32 _v) { mGridH = _v; };
    __int32 GetGridH()           { return mGridH; };

    void    SetGridV(__int32 _v) { mGridV = _v; };
    __int32 GetGridV()           { return mGridV; };

    void   SetGridColor(TColor _v)  { mGridColor = _v; };
    TColor GetGridColor()           { return mGridColor; };

  public: // altitude get-set
    void SetAltitude(bool _v) { mAltitude = _v; };
    bool GetAltitude()           { return mAltitude; };

    void   SetAltitudeMapColor(TColor _v)  { mAltitudeMapColor = _v; };
    TColor GetAltitudeMapColor()           { return mAltitudeMapColor; };

    void   SetAltitudeBrushColor(TColor _v)  { mAltitudeBrushColor = _v; };
    TColor GetAltitudeBrushColor()           { return mAltitudeBrushColor; };

    void    SetAltitudeWidth(__int32 _v) { mAltitudeWidth = _v; };
    __int32 GetAltitudeWidth()           { return mAltitudeWidth; };

    void    SetAltitudeHeight(__int32 _v) { mAltitudeHeight = _v; };
    __int32 GetAltitudeHeight()           { return mAltitudeHeight; };

  public: // mask get-set
    void SetMaskMapShown(bool _v) { mMaskMapShown = _v; };
    bool GetMaskMapShown()        { return mMaskMapShown; };

    void   SetMaskMapColor(TColor _v)  { mMaskMapColor = _v; };
    TColor GetMaskMapColor()           { return mMaskMapColor; };

    void SetFileHistory(CFileHistory* _v) { mFileHistory = _v; }

  public: // save-load
    bool Save(CPString _fileName);
    bool Load(CPString _fileName);

  protected:

  private:
    // screens properties
    CPRect mWndPreview;
    CPRect mWndRadar;
    CPRect mWndTool;
    CPRect mWndTree;
    CPRect mWndInfo;
    CPRect mWndAltitude;
    bool   mWndTreeVisible;
    bool   mWndToolVisible;
    bool   mWndRadarVisible;
    bool   mWndPreviewVisible;
    bool   mWndInfoVisible;
    bool   mWndAltitudeVisible;

    // environment
    bool   mSaveDesktopOnExit;

    //grid properties
    bool    mGridShown;
    __int32 mGridH, mGridV;   // distance between lines
    TColor  mGridColor;       // grid color

    //altitude prop
    bool    mAltitude;                        // working with altitude map
    TColor  mAltitudeMapColor;                // altitude text color
    TColor  mAltitudeBrushColor;              // selection brush color
    __int32 mAltitudeWidth, mAltitudeHeight;  // altitude brush size

    //mask prop
    bool   mMaskMapShown;          // the mask map is shown
    TColor mMaskMapColor;          // the mask map color

    // file history
    CFileHistory* mFileHistory;
};

#endif
