//---------------------------------------------------------------------------


#pragma hdrstop

#include "CFileHistory.h"

//---------------------------------------------------------------------------

CFileHistory::CFileHistory(TMenuItem* _item, TNotifyEvent _onClick, int _maxItems)
{
  mMenuItem     = _item;
  mMaxItems     = _maxItems;
  mOnClickEvent = _onClick;
  mFilesIndex   = 0;

  mFiles.clear();

  // as the file history is empty, disable the menu item
  mMenuItem->Enabled = false;
}
//---------------------------------------------------------------------------

void CFileHistory::Serialize(PP::Stream& _a)
{
  _a.writeInt(&mFilesIndex);

  int lFilesSize = mFiles.size();

  _a.writeInt(&lFilesSize);

  for(unsigned int i = 0; i < mFiles.size(); i++)
  {
    CSerString lFile(CPString(mFiles[i].c_str()));

    lFile.Serialize(_a);
  }
}
//---------------------------------------------------------------------------

void CFileHistory::DeSerialize(PP::Stream& _a)
{
  _a.readInt(&mFilesIndex);

  int lFilesSize;

  _a.readInt(&lFilesSize);

  for(int i = 0; i < lFilesSize; i++)
  {
    CSerString lFile;

    lFile.DeSerialize(_a);

    Add(lFile.Get().c_str());
  }
}
//---------------------------------------------------------------------------

void CFileHistory::Add(std::string _fileName)
{
  mMenuItem->Enabled = true;

  // check if the file is in the history list
  bool lFound = false;

  for(unsigned int i = 0; i < mFiles.size(); i++)
  {
    if (mFiles[i] == _fileName)
    {
      lFound = true;
    }
  }

  if (!lFound)
  {
    if (mFiles.size() < (unsigned)mMaxItems)
    {
      mFiles.push_back(_fileName);
    }
    else
    {
      mFiles[mFilesIndex] = _fileName;

      mFilesIndex += 1;
      mFilesIndex %= mMaxItems;
    }
  }

  mMenuItem->Clear();

  // reset the hot keys autoassign, otherwise we'll get wrong file names
  // (with "&" character inside)
  mMenuItem->AutoHotkeys = maManual;

  for(unsigned int i = 0; i < mFiles.size(); i++)
  {
    TMenuItem* lMenuItem = new TMenuItem(mMenuItem);

    lMenuItem->OnClick  = mOnClickEvent;
    lMenuItem->ShortCut = TextToShortCut(AnsiString("Ctrl+") + AnsiString(i));
    lMenuItem->Caption  = AnsiString(mFiles[i].c_str());

    mMenuItem->Add(lMenuItem);
  }
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
