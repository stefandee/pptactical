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

#pragma hdrstop

#include "PPIG_EngineDef.h"
#include "PPIG_BGManager.h"
#include "PPIG_BattleGroup.h"
#include "PPIG_ObjReal.h"
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CPPIG_BGManager::CPPIG_BGManager()
{
    mBGroups = new CPList<CPPIG_BattleGroup>(4);
}
//---------------------------------------------------------------------------

CPPIG_BGManager::~CPPIG_BGManager()
{
    while(mBGroups->Count() > 0)
    {
        mBGroups->Delete(0);
    }

    delete mBGroups;
}
//---------------------------------------------------------------------------

void CPPIG_BGManager::Update()
{
    // update battle groups
    for(int i = 0; i < mBGroups->Count(); i++)
    {
        mBGroups->Get(i)->Update();
    }
}
//---------------------------------------------------------------------------

int  CPPIG_BGManager::GetBGroupIndexById(int _id)
{
    for(int i = 0; i < mBGroups->Count(); i++)
    {
        if (mBGroups->Get(i)->GetId() == _id)
        {
            return i;
        }
    }

    return -1;
}
//---------------------------------------------------------------------------

void CPPIG_BGManager::SetUniRelation(int _id1, int _id2, TPoliticsRelations _v)
{
    int lBg1 = GetBGroupIndexById(_id1);

    if (lBg1 == -1)
    {
        return;
    }

    int lBg2 = GetBGroupIndexById(_id2);

    if (lBg2 == -1)
    {
        return;
    }

    mRelations[lBg1][lBg2] = _v;
}
//---------------------------------------------------------------------------

void CPPIG_BGManager::SetBiRelation(int _id1, int _id2, TPoliticsRelations _v)
{
    int lBg1 = GetBGroupIndexById(_id1);

    if (lBg1 == -1)
    {
        return;
    }

    int lBg2 = GetBGroupIndexById(_id2);

    if (lBg2 == -1)
    {
        return;
    }

    mRelations[lBg1][lBg2] = _v;
    mRelations[lBg2][lBg1] = _v;
}
//---------------------------------------------------------------------------

TPoliticsRelations CPPIG_BGManager::GetRelation(int _id1, int _id2)
{
    int lBg1 = GetBGroupIndexById(_id1);

    if (lBg1 == -1)
    {
        return REL_UNKNOWN;
    }

    int lBg2 = GetBGroupIndexById(_id2);

    if (lBg2 == -1)
    {
        return REL_UNKNOWN;
    }

    return mRelations[lBg1][lBg2];
}
//---------------------------------------------------------------------------

int CPPIG_BGManager::GetRelationAsColor(int _id1, int _id2)
{
    switch(GetRelation(_id1, _id2))
    {
    case REL_ENEMY:
        return gSideColor[1];

    case REL_OWNED:
        return gSideColor[3];

    case REL_FRIENDLY:
        return gSideColor[2];

    case REL_NEUTRAL:
        return gSideColor[0];

    default:
        return 0;
    }
}
//---------------------------------------------------------------------------

void CPPIG_BGManager::AddBGroup(CPPIG_BattleGroup* _bg)
{
    if (mBGroups->Count() == POLITICS_MAX_BG)
    {
        return;
    }

    if (mBGroups->Contain(_bg))
    {
        return;
    }

    mBGroups->Add(_bg);

    // set default relation with all other battle groups
    int lIndex = mBGroups->Count() - 1;

    for(int i = 0; i < mBGroups->Count() - 1; i++)
    {
        mRelations[i][lIndex] = REL_NEUTRAL;
        mRelations[lIndex][i] = REL_NEUTRAL;
    }

    mRelations[lIndex][lIndex] = REL_OWNED;
}
//---------------------------------------------------------------------------

void CPPIG_BGManager::RemoveBGroup(CPPIG_BattleGroup* _bg)
{
    if (mBGroups->Contain(_bg))
    {
        for(int i = 0; i < mBGroups->Count(); i++)
        {
            if (_bg == mBGroups->Get(i))
            {
                // rearrange relation array
                // compact lines
                for(int j = i; j < mBGroups->Count() - 1; j++)
                {
                    mRelations[j][i] = mRelations[j+1][i];
                }

                // compact columns
                for(int j = i; j < mBGroups->Count() - 1; j++)
                {
                    mRelations[i][j] = mRelations[i][j+1];
                }

                mBGroups->Delete(i);

                return;
            }
        }
    }
}
//---------------------------------------------------------------------------

CPPIG_BattleGroup* CPPIG_BGManager::GetBGroupByIndex (int _index)
{
    if (_index >= mBGroups->Count() || (_index < 0))
    {
        return NULL;
    }

    return mBGroups->Get(_index);
}
//---------------------------------------------------------------------------

CPPIG_BattleGroup* CPPIG_BGManager::GetBGroupById (int _id)
{
    int lIndex = GetBGroupIndexById(_id);

    if (lIndex == -1)
    {
        return NULL;
    }

    return mBGroups->Get(lIndex);
}
//---------------------------------------------------------------------------

void CPPIG_BGManager::AddUnit(CIGRealObject* _unit)
{
    if (!_unit)
    {
        return;
    }

    CPPIG_BattleGroup* lBg = GetBGroupById(_unit->GetBattleGroupId());

    if (!lBg)
    {
        return;
    }

    lBg->AddUnit(_unit);
}
//---------------------------------------------------------------------------

void CPPIG_BGManager::RemoveUnit(CIGRealObject* _unit)
{
    if (!_unit)
    {
        return;
    }

    CPPIG_BattleGroup* lBg = GetBGroupById(_unit->GetBattleGroupId());

    if (!lBg)
    {
        return;
    }

    lBg->RemoveUnit(_unit);
}
//---------------------------------------------------------------------------

void CPPIG_BGManager::AddUnit(int _bgId, CIGRealObject* _unit)
{
    if (!_unit)
    {
        return;
    }

    CPPIG_BattleGroup* lBg = GetBGroupById(_bgId);

    if (!lBg)
    {
        return;
    }

    _unit->SetBattleGroupId(_bgId);

    lBg->AddUnit(_unit);
}
//---------------------------------------------------------------------------

void CPPIG_BGManager::RemoveUnit (int _bgId, CIGRealObject* _unit)
{
    if (!_unit)
    {
        return;
    }

    CPPIG_BattleGroup* lBg = GetBGroupById(_bgId);

    if (!lBg)
    {
        return;
    }

    lBg->RemoveUnit(_unit);
}
//---------------------------------------------------------------------------

void CPPIG_BGManager::Serialize(PP::Stream& _a)
{
    CSerInt            lBgNo(mBGroups->Count());
    CSerString         lBgHeader("CPPIG_BGManager");

    // write stream sync string (in case of a mess)
    lBgHeader.Serialize(_a);

    // write number of battlegroups
    lBgNo.Serialize(_a);

    // deser battlegroups
    for(int i = 0; i < mBGroups->Count(); i++)
    {
        mBGroups->Get(i)->Serialize(_a);
    }

    // ser politics matrix
    for(int i = 0; i < mBGroups->Count(); i++)
    {
        for(int j = 0; j < mBGroups->Count(); j++)
        {
            CSerInt lRelation((int)mRelations[i][j]);
            lRelation.Serialize(_a);
        }
    }
}
//---------------------------------------------------------------------------

unsigned __int32 CPPIG_BGManager::DataSize()
{
    return 0;
}
//---------------------------------------------------------------------------

void CPPIG_BGManager::DeSerialize(PP::Stream& _a)
{
    CSerInt            lBgNo, lRelation;
    CSerString         lBgHeader;
    CPPIG_BattleGroup* lBg;

    // clean-up
    while(mBGroups->Count() > 0)
    {
        mBGroups->Delete(0);
    }

    // stream sync
    lBgHeader.DeSerialize(_a);

    if (lBgHeader.Get() != CPString("CPPIG_BGManager"))
    {
        //throw CArchiveError("CPPIG_BGManager - stream skew!");
    }

    // number of battlegroups
    lBgNo.DeSerialize(_a);

    // deser battlegroups
    for(int i = 0; i < lBgNo.Get(); i++)
    {
        try
        {
            lBg = new CPPIG_BattleGroup();
        }
        catch(...)
        {
            //throw CArchiveError("CPPIG_BGManager - battlegroup instance alloc error.");
        }

        lBg->DeSerialize(_a);

        AddBGroup(lBg);
    }

    // deser politics matrix
    for(int i = 0; i < lBgNo.Get(); i++)
    {
        for(int j = 0; j < lBgNo.Get(); j++)
        {
            lRelation.DeSerialize(_a);

            mRelations[i][j] = (TPoliticsRelations)lRelation.Get();
        }
    }
}
//---------------------------------------------------------------------------

void CPPIG_BGManager::Serialize(BaseSerializer& out)
{
    out.beginElement("bgmanager");

    // sync string, useful for binary serialization
    out.createAttribute("id", "CPPIG_BGManager");

    // write the battle groups count
    int groupsCount = mBGroups->Count();
    out.rawWrite(groupsCount);

    out.beginElement("battlegroups");

    // serialize battlegroups
    for(int i = 0; i < groupsCount; i++)
    {
        mBGroups->Get(i)->Serialize(out);
    }

    out.endElement(); /**< battlegroups */

    out.beginElement("politics");

    // serialize politics matrix
    for(int i = 0; i < groupsCount; i++)
    {
        for(int j = 0; j < groupsCount; j++)
        {
            out.beginElement("relation");
            out.createAttribute("i", i);
            out.createAttribute("j", j);
            out.createAttribute("v", (int)mRelations[i][j]);
            out.endElement(); /**< relation */
        }
    }

    out.endElement(); /**< politics matrix */

    out.endElement(); /**< battle group manager */
}
//---------------------------------------------------------------------------

void CPPIG_BGManager::DeSerialize(BaseDeSerializer& in)
{

}
//---------------------------------------------------------------------------
