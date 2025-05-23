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
//  Unit              : File History Class
//
//  Version           : 1.0
//
//  Description       :
//    * maintains a file history (like an open recent)
//    * will keep a TMenuItem in sync with the internal data
//    * has a maximum hold capacity, as parameter to the constructor
//    * has a ser-deser interface, so that it may write itself to a config file
//
//  History           :
//    [07.09.2004] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef CFileHistoryH
#define CFileHistoryH
//---------------------------------------------------------------------------

#include <Menus.hpp>
#include "Serializable.h"
#include <vector>
#include <string>

class CFileHistory
{
  public:
    CFileHistory(TMenuItem* _item, TNotifyEvent _onClick, int _maxItems);

  public: // ser-deser interface
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize() { return 0; }
    virtual void DeSerialize(PP::Stream& _a);

  public: // ops
    void Add(std::string _filename);

  protected: // no impl.
    CFileHistory();
    CFileHistory(const CFileHistory&);
    CFileHistory& operator=(const CFileHistory&);

  private:
    std::vector<std::string> mFiles;
    TNotifyEvent             mOnClickEvent;
    int                      mFilesIndex;
    TMenuItem*               mMenuItem;
    int                      mMaxItems;
};

#endif
