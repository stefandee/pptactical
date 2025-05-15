//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2004 Stefan Dicu & Tudor Girba                         //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License //
// as published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public    //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Unit              : Player Profile
//
//  Version           : 1.0
//
//  Description       :
//    * singleton
//    * holds informations about how the user set the engine (music/beep volumes,
//      scroll sensitivity and so on)
//
//  History           :
//    [05.10.2005] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef PlayerProfileH
#define PlayerProfileH
//---------------------------------------------------------------------------

#include "XmlSerializable.h"

class PlayerProfile : public Serializable2
{
  public:
    static PlayerProfile* Instance();
    static void Release();

  public: // get-set
    bool GetShowUnitTooltip() { return mShowUnitTooltip; }
    void SetShowUnitTooltip(bool v) { mShowUnitTooltip = v; }

    bool GetShowViewDir() { return mShowViewDir; }
    void SetShowViewDir(bool v) { mShowViewDir = v; }

    int  GetScrollSpeed() { return mScrollSpeed; }
    void SetScrollSpeed(int v) { mScrollSpeed = v; }

    int  GetGameSpeed() { return mGameSpeed; }
    void SetGameSpeed(int v) { mGameSpeed = v; }

    int  GetMusicVolume() { return mMusicVolume; }
    void SetMusicVolume(int v) { mMusicVolume = v; }

    int  GetSFXVolume() { return mSFXVolume; }
    void SetSFXVolume(int v) { mSFXVolume = v; }

    int  GetRepeatRate() { return mRepeatRate; }
    void SetRepeatRate(int v) { mRepeatRate = v; }

    int GetScreenshotCount() { return mScreenshotCount; }
    void SetScreenshotCount(int v);

    int  GetRepeatDelay() { return mRepeatDelay; }
    void SetRepeatDelay(int v) { mRepeatDelay = v; }

  public:
    void Save(CPString fileName);
    void Load(CPString fileName);

  public:
    virtual void Serialize(BaseSerializer& out);
    virtual void DeSerialize(BaseDeSerializer& in);

  protected:
    PlayerProfile();
    virtual ~PlayerProfile();

  private:
    static PlayerProfile* mInstance;

  private: // members
    void InitDefault();

  private: // members
    bool mShowViewDir;
    bool mShowUnitTooltip;

    int  mScrollSpeed;
    int  mGameSpeed;
    int  mMusicVolume;
    int  mSFXVolume;

    // keyboard
    int  mRepeatRate;
    int  mRepeatDelay;

    int mScreenshotCount;
};

PlayerProfile* GetProfileInstance();

#endif
