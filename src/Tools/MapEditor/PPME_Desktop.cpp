//---------------------------------------------------------------------------
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
#pragma hdrstop

#include "PPME_Desktop.h"
#include "basepath.h"
#include "Paths.h"
#include "Serializable.h"
#include "CFileHistory.h"

CPPME_Desktop::CPPME_Desktop() :
    mWndPreview  (CPRect(0, 0, 100, 100)),
    mWndRadar    (CPRect(0, 100, 50, 50)),
    mWndTool     (CPRect(0, 150, 50, 50)),
    mWndTree     (CPRect(500, 0, 100, 350)),
    mWndInfo     (CPRect(0, 0, 214, 160)),
    mWndAltitude (CPRect(0, 0, 166, 170)),
    mWndTreeVisible(true),
    mWndToolVisible(false),
    mWndRadarVisible(false),
    mWndPreviewVisible(false),
    mWndInfoVisible(false),
    mWndAltitudeVisible(false),
    mSaveDesktopOnExit(false),
    mGridShown(false),
    mGridH(32), mGridV(32),
    mGridColor((TColor)0xFF0000),
    mAltitude(false),
    mAltitudeMapColor((TColor)0xFF0000),
    mAltitudeBrushColor((TColor)0xFF0000),
    mAltitudeWidth(32),
    mAltitudeHeight(32),
    mMaskMapShown(false),
    mMaskMapColor((TColor)0xFF0000),
    mFileHistory(0)
{
}
//---------------------------------------------------------------------------

CPPME_Desktop::~CPPME_Desktop()
{
}
//---------------------------------------------------------------------------

bool CPPME_Desktop::Save(CPString _fileName)
{
  try
  {
    PP::Stream_ANSI_C lA(CPString(_fileName.c_str()), true);

    // write every member
    CSerRect(mWndPreview).Serialize(lA);
    CSerRect(mWndRadar).Serialize(lA);
    CSerRect(mWndTool).Serialize(lA);
    CSerRect(mWndTree).Serialize(lA);
    CSerRect(mWndInfo).Serialize(lA);
    CSerRect(mWndAltitude).Serialize(lA);

    lA.writeBool(&mWndTreeVisible);
    lA.writeBool(&mWndToolVisible);
    lA.writeBool(&mWndRadarVisible);
    lA.writeBool(&mWndPreviewVisible);
    lA.writeBool(&mWndInfoVisible);
    lA.writeBool(&mWndAltitudeVisible);

    lA.writeBool(&mSaveDesktopOnExit);

    lA.writeBool(&mGridShown);
    lA.writeInt(&mGridH);
    lA.writeInt(&mGridV);
    //lA.writeInt(&((int)mGridColor));

    lA.writeBool(&mAltitude);
    //lA.writeInt(&(int)mAltitudeMapColor);
    //lA.writeInt(&(int)mAltitudeBrushColor);
    lA.writeInt(&mAltitudeWidth);
    lA.writeInt(&mAltitudeHeight);

    lA.writeBool(&mMaskMapShown);
    //lA.writeInt(&(int)mMaskMapColor);

    bool lHasFileHistory = (mFileHistory != 0);

    lA.writeBool(&lHasFileHistory);

    // file history is not managed by this class, it is managed by FMain
    if (mFileHistory)
    {
      mFileHistory->Serialize(lA);
    }
  }
  catch(...)
  {
    return false;
  }

  return true;
}
//---------------------------------------------------------------------------

bool CPPME_Desktop::Load(CPString _fileName)
{
  try
  {
    PP::Stream_ANSI_C lA(CPString(_fileName.c_str()));

    // read every member
    CSerRect lSerRect;

    lSerRect.DeSerialize(lA);
    mWndPreview = lSerRect.Get();

    lSerRect.DeSerialize(lA);
    mWndRadar = lSerRect.Get();

    lSerRect.DeSerialize(lA);
    mWndTool = lSerRect.Get();

    lSerRect.DeSerialize(lA);
    mWndTree = lSerRect.Get();

    lSerRect.DeSerialize(lA);
    mWndInfo = lSerRect.Get();

    lSerRect.DeSerialize(lA);
    mWndAltitude = lSerRect.Get();

    lA.readBool(&mWndTreeVisible);
    lA.readBool(&mWndToolVisible);
    lA.readBool(&mWndRadarVisible);
    lA.readBool(&mWndPreviewVisible);
    lA.readBool(&mWndInfoVisible);
    lA.readBool(&mWndAltitudeVisible);

    lA.readBool(&mSaveDesktopOnExit);

    lA.readBool(&mGridShown);
    lA.readInt(&mGridH);
    lA.readInt(&mGridV);
    //lA.readInt(&((int)mGridColor));

    lA.readBool(&mAltitude);
    //lA.readInt(&(int)mAltitudeMapColor);
    //lA.readInt(&(int)mAltitudeBrushColor);
    lA.readInt(&mAltitudeWidth);
    lA.readInt(&mAltitudeHeight);

    lA.readBool(&mMaskMapShown);
    //lA.readInt(&(int)mMaskMapColor);

    bool lHasFileHistory = false;

    lA.readBool(&lHasFileHistory);

    // file history is not managed by this class, it is managed by FMain
    if (lHasFileHistory && mFileHistory)
    {
      mFileHistory->DeSerialize(lA);
    }
  }
  catch(...)
  {
    return false;
  }

  return true;
}
//---------------------------------------------------------------------------

  /*
   int lHandle, lResult;

   if ((lHandle = open(_fileName.c_str(), O_BINARY | O_CREAT | O_WRONLY, S_IWRITE)) == -1)
   {
      close(lHandle);
      return false;
   }

   close(lHandle);
  */
  /*
   AnsiString tmp = AnsiString(getGamePath().c_str()) + PATH_MEDITOR + FILE_MEDITOR_CFG;
   if ((handle = open(AnsiString(getGamePath() + PATH_MEDITOR + FILE_MEDITOR_CFG).c_str(), O_BINARY|O_RDONLY, S_IREAD)) == -1)
   {
      setMEError(ERR_OPEN);
      close(handle);
      printMEError("Loading desktop");
      return;
   }
   if (read(handle, desktop, sizeof(TDesktop)) != sizeof(TDesktop))
   {
      setMEError(ERR_READ);
      close(handle);
      printMEError("Loading desktop");
      return;
   }
   close(handle);
   */

#pragma package(smart_init)
