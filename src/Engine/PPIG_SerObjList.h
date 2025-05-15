//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2004 Stefan Dicu                                       //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License //
// as published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public   //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Unit              : Serializable Base Object List
//
//  Version           : 1.0
//
//  Description       :
//    * wraps a CIGBaseObjectList and serialize/deserialize it
//    * the class only holds a pointer to the list but does not
//      manage it (create/delete)
//
//  History           :
//    [01.09.2004] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef PPIG_SerObjListH
#define PPIG_SerObjListH
//---------------------------------------------------------------------------

#include "Serializable.h"

class CIGBaseObjectList;

class CIGSerBaseObjectList : public CSerializable
{
  public:
    CIGSerBaseObjectList(CIGBaseObjectList* _objList) : mBaseObjectList(_objList) {}

  public: // ser/deser
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  public: // accesor
    CIGBaseObjectList* Get() { return mBaseObjectList; }

  protected:

  private: // private ctors
    CIGSerBaseObjectList();
    CIGSerBaseObjectList(const CIGSerBaseObjectList&);
    CIGSerBaseObjectList& operator = (const CIGSerBaseObjectList&);

  private:
    CIGBaseObjectList* mBaseObjectList;
};

#endif
