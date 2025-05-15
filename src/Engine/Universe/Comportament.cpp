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
//---------------------------------------------------------------------------
#pragma hdrstop

#include "Comportament.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CComportament::CComportament()
{
  mName = "Default";
  mInfo = "Generic comportament.";

  for(int i = 0; i < 3; i++)
  {
    mOnSelect[i] = "";
    mResponse[i] = "";
    mOnAttack[i] = "";
    mOnFrag[i]   = "";
    mOnHit[i]    = "";
    mOnDie[i]    = "";
  }
}
//---------------------------------------------------------------------------

CComportament::CComportament(CComportament& _src)
{
  mName = _src.mName;
  mInfo = _src.mInfo;

  for(int i = 0; i < 3; i++)
  {
    mOnSelect[i] = _src.mOnSelect[i];
    mResponse[i] = _src.mResponse[i];
    mOnAttack[i] = _src.mOnAttack[i];
    mOnFrag[i]   = _src.mOnFrag[i];
    mOnHit[i]    = _src.mOnHit[i];
    mOnDie[i]    = _src.mOnDie[i];
  }
}
//---------------------------------------------------------------------------

CPString CComportament::GetSelect(int _pos)
{
  if (_pos > 2)
  {
    return CPString("");
  }

  return mOnSelect[_pos];
}
//---------------------------------------------------------------------------

void CComportament::SetSelect(CPString _string, int _pos)
{
  if (_pos > 2)
  {
    return;
  }

  mOnSelect[_pos] = _string;
}
//---------------------------------------------------------------------------

CPString CComportament::GetResponse(int _pos)
{
  if (_pos > 2)
  {
    return CPString("");
  }

  return mResponse[_pos];
}
//---------------------------------------------------------------------------

void CComportament::SetResponse(CPString _string, int _pos)
{
  if (_pos > 2)
  {
    return;
  }

  mResponse[_pos] = _string;
}
//---------------------------------------------------------------------------

CPString CComportament::GetAttack(int _pos)
{
  if (_pos > 2)
  {
    return CPString("");
  }

  return mOnAttack[_pos];
}
//---------------------------------------------------------------------------

void CComportament::SetAttack(CPString _string, int _pos)
{
  if (_pos > 2)
  {
    return;
  }

  mOnAttack[_pos] = _string;
}
//---------------------------------------------------------------------------

CPString CComportament::GetFrag(int _pos)
{
  if (_pos > 2)
  {
    return CPString("");
  }

  return mOnFrag[_pos];
}
//---------------------------------------------------------------------------

void CComportament::SetFrag(CPString _string, int _pos)
{
  if (_pos > 2)
  {
    return;
  }

  mOnFrag[_pos] = _string;
}
//---------------------------------------------------------------------------

CPString CComportament::GetHit(int _pos)
{
  if (_pos > 2)
  {
    return CPString("");
  }

  return mOnHit[_pos];
}
//---------------------------------------------------------------------------

void CComportament::SetHit(CPString _string, int _pos)
{
  if (_pos > 2)
  {
    return;
  }

  mOnHit[_pos] = _string;
}
//---------------------------------------------------------------------------

CPString CComportament::GetDie(int _pos)
{
  if (_pos > 2)
  {
    return CPString("");
  }

  return mOnDie[_pos];
}
//---------------------------------------------------------------------------

void CComportament::SetDie(CPString _string, int _pos)
{
  if (_pos > 2)
  {
    return;
  }

  mOnDie[_pos] = _string;
}
//---------------------------------------------------------------------------

CComportament& CComportament::operator= (const CComportament& _src)
{
  mName = _src.mName;
  mInfo = _src.mInfo;

  for(int i = 0; i < 3; i++)
  {
    mOnSelect[i] = _src.mOnSelect[i];
    mResponse[i] = _src.mResponse[i];
    mOnAttack[i] = _src.mOnAttack[i];
    mOnFrag[i]   = _src.mOnFrag[i];
    mOnHit[i]    = _src.mOnHit[i];
    mOnDie[i]    = _src.mOnDie[i];
  }

  return *this;
}
//---------------------------------------------------------------------------

void CComportament::Serialize(PP::Stream& _a)
{
  CSerString lName(mName), lInfo(mInfo);

  lName.Serialize(_a);
  lInfo.Serialize(_a);

  for(int i = 0; i < 3; i++)
  {
    CSerString(mOnSelect[i]).Serialize(_a);
    CSerString(mResponse[i]).Serialize(_a);
    CSerString(mOnAttack[i]).Serialize(_a);
    CSerString(mOnFrag[i]).Serialize(_a);
    CSerString(mOnHit[i]).Serialize(_a);
    CSerString(mOnDie[i]).Serialize(_a);
  }
}
//---------------------------------------------------------------------------

void CComportament::DeSerialize(PP::Stream& _a)
{
  CSerString(mName).DeSerialize(_a);
  CSerString(mInfo).DeSerialize(_a);

  // crap! to remake someday
  for(int i = 0; i < 3; i++)
  {
    CSerString(mOnSelect[i]).DeSerialize(_a);
    CSerString(mResponse[i]).DeSerialize(_a);
    CSerString(mOnAttack[i]).DeSerialize(_a);
    CSerString(mOnFrag[i]).DeSerialize(_a);
    CSerString(mOnHit[i]).DeSerialize(_a);
    CSerString(mOnDie[i]).DeSerialize(_a);
  }
}
//---------------------------------------------------------------------------

