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
//  Unit              : Battle Group Manager
//
//  Version           : 1.0
//
//  Description       :
//    * manager for battle groups
//    * has 3 interface: politics, bgroup list and units lists
//
//  History           :
//    [10.01.2002] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef PPIG_BGManagerH
#define PPIG_BGManagerH
//---------------------------------------------------------------------------

#include "Serializable.h"
#include "XmlSerializable.h"
#include "KList.h"

const int POLITICS_MAX_BG = 32;

// fwd
class CPPIG_BattleGroup;
class CIGRealObject;

class CPPIG_BGManager : public CSerializable, public Serializable2
{
  private:
    TPoliticsRelations mRelations[POLITICS_MAX_BG][POLITICS_MAX_BG];

    CPList<CPPIG_BattleGroup>* mBGroups;

    void InitRelations();
    int  GetBGroupIndexById(int _id); // :)

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

    virtual void Serialize(BaseSerializer& out);
    virtual void DeSerialize(BaseDeSerializer& in);

  public: // c-d
    CPPIG_BGManager();

    virtual void Update();

    virtual ~CPPIG_BGManager();

  public: // politics interface
    void               SetUniRelation (int _id1, int _id2, TPoliticsRelations _v);
    void               SetBiRelation  (int _id1, int _id2, TPoliticsRelations _v);
    TPoliticsRelations GetRelation    (int _id1, int _id2);
    int                GetRelationAsColor(int _id1, int _id2);

  public: // battle group management interface
    void               AddBGroup        (CPPIG_BattleGroup* _bg);
    void               RemoveBGroup     (CPPIG_BattleGroup* _bg);
    CPPIG_BattleGroup* GetBGroupByIndex (int _index);
    CPPIG_BattleGroup* GetBGroupById    (int _id);
    int                BGroupCount()    { return mBGroups->Count(); }

  public: // units lists from bgroups management interface
    void AddUnit    (CIGRealObject* _unit);
    void RemoveUnit (CIGRealObject* _unit);

    void AddUnit    (int _bgId, CIGRealObject* _unit); // dont think will ever be used
    void RemoveUnit (int _bgId, CIGRealObject* _unit); // dont think will ever be used
};

#endif
