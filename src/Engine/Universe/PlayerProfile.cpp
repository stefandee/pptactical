//---------------------------------------------------------------------------

#include <algorithm>
#include "tinyxml2.h"
#include "easylogging++.h"
#pragma hdrstop

#include "PlayerProfile.h"
#include "Paths.h"
#include "basepath.h"
#include "PP_Stdlib.h"
//---------------------------------------------------------------------------

PlayerProfile* PlayerProfile::mInstance = 0;

PlayerProfile* GetProfileInstance()
{
  return PlayerProfile::Instance();
}
//---------------------------------------------------------------------------

PlayerProfile* PlayerProfile::Instance()
{
  if (!mInstance)
  {
    mInstance = new PlayerProfile();
  }

  return mInstance;
}
//---------------------------------------------------------------------------

void PlayerProfile::Release()
{
  if (mInstance)
  {
    delete mInstance;
    mInstance = 0;
  }
}
//---------------------------------------------------------------------------

PlayerProfile::PlayerProfile()
{
    InitDefault();
}
//---------------------------------------------------------------------------

PlayerProfile::~PlayerProfile()
{
}
//---------------------------------------------------------------------------

void PlayerProfile::InitDefault()
{
  mShowViewDir = false;
  mShowUnitTooltip = true;

  mScrollSpeed = 5;
  mGameSpeed   = 7;
  mMusicVolume = 100;
  mSFXVolume   = 100;

  mRepeatDelay = 100;
  mRepeatRate  = 100;

  mScreenshotCount = 0;
}
//---------------------------------------------------------------------------

void PlayerProfile::SetScreenshotCount(int v)
{
    mScreenshotCount = std::max(0, v);
}
//---------------------------------------------------------------------------

void PlayerProfile::Save(CPString fileName)
{
  //PP::Stream_ANSI_C lA(getOutPath() + PATH_PLAYER_PROFILE + fileName, true);

  //mInstance->Serialize(lA);
	XmlSerializer out;

	Serialize(out);

	out.GetDoc().SaveFile(fileName.c_str());
}
//---------------------------------------------------------------------------

void PlayerProfile::Load(CPString fileName)
{
    tinyxml2::XMLDocument xmlDoc;

    if (xmlDoc.LoadFile(fileName.c_str()) != tinyxml2::XML_SUCCESS)
    {
        LOG(ERROR) << "Could not load/parse player profile! " << xmlDoc.ErrorStr();
        return;
    }

    InitDefault();

    XmlDeSerializer in(xmlDoc);

    VLOG(2) << "Deserializing player profile";
    DeSerialize(in);
}
//---------------------------------------------------------------------------

void PlayerProfile::Serialize(BaseSerializer& out)
{
    out.beginElement("Profile");

	out.createAttribute("ScrollSpeed", mScrollSpeed);
	out.createAttribute("GameSpeed", mGameSpeed);
	out.createAttribute("MusicVolume", mMusicVolume);
	out.createAttribute("SFXVolume", mSFXVolume);
	out.createAttribute("ShowViewDir", mShowViewDir);
	out.createAttribute("RepeatRate", mRepeatRate);
	out.createAttribute("RepeatDelay", mRepeatDelay);
	out.createAttribute("ScreenshotCount", mScreenshotCount);
	out.createAttribute("ShowUnitTooltip", mShowUnitTooltip);

    out.endElement();
}
//---------------------------------------------------------------------------

void PlayerProfile::DeSerialize(BaseDeSerializer& in)
{
   in.beginElement("Profile");

    in.readAttribute("ScrollSpeed", mScrollSpeed, 5);
    mScrollSpeed = clamp(mScrollSpeed, 0, 7);

    in.readAttribute("GameSpeed", mGameSpeed, 7);
    mGameSpeed = clamp(mGameSpeed, 0, 7);

    in.readAttribute("MusicVolume", mMusicVolume, 100);
    mMusicVolume = clamp(mMusicVolume, 0, 100);

    in.readAttribute("SFXVolume", mSFXVolume, 100);
    mSFXVolume = clamp(mSFXVolume, 0, 100);

    in.readAttribute("RepeatRate", mRepeatRate, 100);
    mRepeatRate = clamp(mRepeatRate, 0, 100);

    in.readAttribute("RepeatDelay", mRepeatDelay, 100);
    mRepeatDelay = clamp(mRepeatDelay, 0, 100);

    in.readAttribute("ScreenshotCount", mScreenshotCount, 0);
    mScreenshotCount = std::max(0, mScreenshotCount);

    in.readAttribute("ShowViewDir", mShowViewDir, false);

    in.readAttribute("ShowUnitTooltip", mShowUnitTooltip, true);

    in.endElement();
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
