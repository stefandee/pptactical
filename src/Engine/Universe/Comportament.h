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
//  Unit              : Comportament (Comportament.h)
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * definitii pentru comportamentul sonor al unui om
//    * tipul TUComportament contine membrii mOnxxx, care sint nume de fisiere
//      de sunet
//
//  Istorie           :
//    [08.10.2000] - [Karg] - a fost creat unitul
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef ComportamentH
#define ComportamentH
//---------------------------------------------------------------------------

#include "Serializable.h"
#include "PP_String.h"
//#define MAX_COMPORTAMENTS 8

class CComportament : public CSerializable
{
  private :
    CPString mName;
    CPString mInfo;

    CPString mOnSelect[3];
    CPString mResponse[3];
    CPString mOnAttack[3];
    CPString mOnFrag[3];
    CPString mOnHit[3];
    CPString mOnDie[3];

  protected :

  public :
    CComportament();
    CComportament(CComportament& _src);

    void SetName(CPString _string) { mName = _string; };
    CPString GetName() { return mName; };

    void SetInfo(CPString _string) { mInfo = _string; };
    CPString GetInfo() { return mInfo; };

    // sunetele
    CPString GetSelect(int _pos);
    void SetSelect(CPString _string, int _pos);

    CPString GetResponse(int _pos);
    void SetResponse(CPString _string, int _pos);

    CPString GetAttack(int _pos);
    void SetAttack(CPString _string, int _pos);

    CPString GetFrag(int _pos);
    void SetFrag(CPString _string, int _pos);

    CPString GetHit(int _pos);
    void SetHit(CPString _string, int _pos);

    CPString GetDie(int _pos);
    void SetDie(CPString _string, int _pos);

    CComportament& operator= (const CComportament& _src);

    virtual void Serialize(PP::Stream& _a);
    virtual void DeSerialize(PP::Stream& _a);

    ~CComportament() {};
};

#endif
