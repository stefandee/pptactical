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
#include "Platform.h"

#ifdef PPT_PLATFORM_WIN
#include <io.h>
#endif

#if defined(PPT_PLATFORM_LINUX) || defined(PPT_PLATFORM_SOLARIS) || defined (PPT_COMPILER_GNU)
#include <sys/types.h>
#include <unistd.h>
#endif

#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#pragma hdrstop

#include <memory>
using namespace std;

#include "Stream_PhysFS.h"
#include "PPIG_Doctrine.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

static inline int Doctrine_random(int num)
{
    return num ? (rand()%num) : 0;
}


CPPIG_DoctrineScript::CPPIG_DoctrineScript()
{
    mObservers = new CPList<CPPIG_ScriptObserver>(2);
    //mScript    = "main()\n{\n}\n";
    mScript      = "";
}
//---------------------------------------------------------------------------

/*
CPPIG_DoctrineScript::CPPIG_DoctrineScript(int _id, CPString _script)
{
  mObservers = new CPList<CPPIG_ScriptObserver>(2);
  mScript    = _script;
  mId        = _id;
}
//---------------------------------------------------------------------------
*/

CPPIG_DoctrineScript::~CPPIG_DoctrineScript()
{
    while(mObservers->Count() > 0) mObservers->Delete(0);

    delete mObservers;
    mScript = "EndOfLine"; // doesnt matter - but if residual pointers points to it, it will
    // provide false information - debug stuff :)
    mId = -1;
}
//---------------------------------------------------------------------------

void CPPIG_DoctrineScript::AddObserver(CPPIG_ScriptObserver* _obs)
{
    if (!mObservers->Contain(_obs))
    {
        _obs->SetSubject(this);
        mObservers->Add(_obs);
    }
}
//---------------------------------------------------------------------------

void CPPIG_DoctrineScript::RemoveObserver(CPPIG_ScriptObserver* _obs)
{
    for(int i = 0; i < mObservers->Count(); i++)
    {
        if (mObservers->Get(i) == _obs)
        {
            mObservers->Delete(i);
            return;
        }
    }
}
//---------------------------------------------------------------------------

void CPPIG_DoctrineScript::Notify()
{
    for(int i = 0; i < mObservers->Count(); i++)
    {
        mObservers->Get(i)->Update(mScript);
    }
}
//---------------------------------------------------------------------------

CPPIG_ScriptObserver::CPPIG_ScriptObserver()
{
    mSubject = NULL;
}
//---------------------------------------------------------------------------

CPPIG_ScriptObserver::~CPPIG_ScriptObserver()
{
}
//---------------------------------------------------------------------------

CPPIG_ScriptObserver2::CPPIG_ScriptObserver2()
{
}
//---------------------------------------------------------------------------

CPPIG_ScriptObserver2::~CPPIG_ScriptObserver2()
{
}
//---------------------------------------------------------------------------

void CPPIG_ScriptObserver2::Update(CPString _script)
{
    if (mHandleScript)
    {
        mHandleScript(_script);
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  The flyweight factory implementation follows
//---------------------------------------------------------------------------

CPPIG_Doctrine::CPPIG_Doctrine()
{
    mDoctrineScripts = new CPList<CPPIG_DoctrineScript>(2);
}
//---------------------------------------------------------------------------

CPPIG_Doctrine::~CPPIG_Doctrine()
{
    // clear all doctrines
    while (mDoctrineScripts->Count() > 0) mDoctrineScripts->Delete(0);

    // delete the list
    try
    {
        delete mDoctrineScripts;
    }
    catch(...)
    {
        LOG(ERROR) << "CPPIG_Doctrine::~CPPIG_Doctrine - cannot delete scripts";
    }
}
//---------------------------------------------------------------------------

CPPIG_DoctrineScript* CPPIG_Doctrine::GetSubjectByIndex(int _index)
{
    if (_index >= mDoctrineScripts->Count())
    {
        return NULL;
    }

    return mDoctrineScripts->Get(_index);
}
//---------------------------------------------------------------------------

void CPPIG_Doctrine::Clear()
{
    while (mDoctrineScripts->Count()>0)
    {
        mDoctrineScripts->Delete(0);
    }

    delete mDoctrineScripts;

    try
    {
        mDoctrineScripts = new CPList<CPPIG_DoctrineScript>(2);
    }
    catch(...)
    {
        LOG(ERROR) << "CPPIG_Doctrine::Clear - cannot realloc scripts list";
    }
}
//---------------------------------------------------------------------------

int CPPIG_Doctrine::GenerateUniqueScriptId()
{
    int lId;
    bool lUnique;

    do
    {
        lUnique = true;
        lId = Doctrine_random(16384);

        for(int i = 0; i < mDoctrineScripts->Count(); i++)
        {
            if (lId == mDoctrineScripts->Get(i)->GetId())
            {
                lUnique = false;
                break;
            }
        }
    }
    while(!lUnique);

    return lId;
}
//---------------------------------------------------------------------------

CPPIG_DoctrineScript* CPPIG_Doctrine::GetSubjectOf(CPPIG_ScriptObserver* _obs)
{
    // find what this observer is observing, and remove it
    for(int i = 0; i < mDoctrineScripts->Count(); i++)
    {
        if (mDoctrineScripts->Get(i)->ContainsObserver(_obs))
        {
            return mDoctrineScripts->Get(i);
        }
    }

    return NULL;
}
//---------------------------------------------------------------------------

// Description : add script, without regard to unicity
// Params      : _id = id, _script = script body
// Result      : n/a
// Comments    : the purpose of this method is solely to add
void CPPIG_Doctrine::AddScript(int _id, CPString _script)
{
    CPPIG_DoctrineScript* lScript;

    try
    {
        lScript = new CPPIG_DoctrineScript();
    }
    catch(...)
    {
        LOG(ERROR) << "CPPIG_Doctrine::AddScript - cannot alloc script, aborting.";
        return;
    }

    lScript->SetScript(_script);
    lScript->SetId(_id);
    mDoctrineScripts->Add(lScript);
}
//---------------------------------------------------------------------------

// Description : add a script, taking care of the unicity
// Params      : _obs    = the current observer the client is using, if any;
//               _script = he body of script
// Result      : the new observer, if appropriate
// Comments    : - the code isn't well optimized, there are removal that should be done
//               only in some cases, not all the time, but this code is to be called once
//               while loading map from file and in map editor
//               - the method is used: (1) loading map from file, precisely when loading objects
//                 and identifying their scripts. (2) in map editor, when changing scripts
CPPIG_ScriptObserver* CPPIG_Doctrine::AddScript(CPPIG_ScriptObserver* _obs, CPString _script)
{
    CPPIG_DoctrineScript* lScript    = NULL;
    CPPIG_DoctrineScript* lNewScript = NULL;
    CPPIG_ScriptObserver* lObs       = NULL;

    // body of script cannot be empty
    // if it is empty, and there is a valid observer, remove the observer
    if (_script == CPString(""))
    {
        if (_obs != NULL)
        {
            lScript = GetSubjectOf(_obs);

            if (lScript)
            {
                lScript->RemoveObserver(_obs);
                MakeClean();
            }
        }

        return NULL;
    }

    if (_obs != NULL)
    {
        lScript = GetSubjectOf(_obs);

        if (lScript)
        {
            lScript->RemoveObserver(_obs);
            MakeClean();
        }
        else
        {
            // got an observer that doesn't observer our scripts
            // can't dealloc this observer, so just return
            return NULL;
        }
    }

    // alloc an observer
    try
    {
        lObs = new CPPIG_ScriptObserver();
    }
    catch(...)
    {
        LOG(ERROR) << "CPPIG_Doctrine::AddScript - cannot alloc script, aborting.";
        return NULL;
    }

    // test if script body is unique
    // CSL is case-sensitive, so we can't make smart things
    CPString lTmpScript;

    /*
    for(int i = 0; i < mDoctrineScripts->Count(); i++)
    {
      lTmpScript = mDoctrineScripts->Get(i)->GetScript();

      if (CompactString(lTmpScript) == CompactString(_script))
      //if (lTmpScript == _script)
      {
        // is not unique, so return an observer for this script
        mDoctrineScripts->Get(i)->AddObserver(lObs);
        return lObs;
      }
    }
    */

    // is not unique, so return an observer for this script
    // mDoctrineScripts->Get(i)->AddObserver(lObs);

    // the script body is unique
    try
    {
        lNewScript = new CPPIG_DoctrineScript();
    }
    catch(...)
    {
        LOG(ERROR) << "CPPIG_Doctrine::AddScript - cannot alloc script, aborting.";
        return NULL;
    }

    lNewScript->SetId(GenerateUniqueScriptId());
    lNewScript->AddObserver(lObs);
    lNewScript->SetScript(_script);
    mDoctrineScripts->Add(lNewScript);

    return lObs;
}
//---------------------------------------------------------------------------

CPPIG_ScriptObserver* CPPIG_Doctrine::AddScriptFromFile(CPPIG_ScriptObserver* _obs, CPString _fileName)
{
    /*
    // open the script file and read it into a string
    int lHandle = open(_fileName, O_BINARY | O_RDONLY);

    if (lHandle == -1)
    {
        return 0;
    }

    int lFileSize = lseek(lHandle, 0, SEEK_END);

    lseek(lHandle, 0, SEEK_SET);

    char* lString;

    // should use an auto_ptr
    try
    {
        lString = new char[lFileSize + 1];
    }
    catch(...)
    {
        close(lHandle);
        return 0;
    }

    if (read(lHandle, lString, lFileSize) == -1)
    {
        delete lString;
        close(lHandle);
        return 0;
    }

    lString[lFileSize] = '\0';

    CPString lScriptString = CPString(lString);

    delete lString;
    close(lHandle);

    return AddScript(_obs, CPString(lScriptString));
    */

    CPPIG_ScriptObserver* result = NULL;

    try
    {
        PP::Stream_PhysFS inputStream(_fileName.c_str());

        size_t dataLength = inputStream.length();
        char* dataString = new char[dataLength + 1];

        inputStream.read(dataString, dataLength);
        dataString[dataLength] = '\0';

        result = AddScript(_obs, CPString(dataString));
    }
    catch(...)
    {
    }

    return result;
}
//---------------------------------------------------------------------------

CPPIG_ScriptObserver* CPPIG_Doctrine::GetScriptObserver(int _id)
{
    CPPIG_ScriptObserver* lObs       = NULL;

    for(int i = 0; i < mDoctrineScripts->Count(); i++)
    {
        if (mDoctrineScripts->Get(i)->GetId() == _id)
        {
            // alloc an observer
            try
            {
                lObs = new CPPIG_ScriptObserver();
            }
            catch(...)
            {
                LOG(ERROR) << "CPPIG_Doctrine::AddScript - cannot alloc script, aborting.";
                return NULL;
            }

            mDoctrineScripts->Get(i)->AddObserver(lObs);
            return lObs;
        }
    }

    return NULL;
}
//---------------------------------------------------------------------------

void CPPIG_Doctrine::RemoveScriptObserver(CPPIG_ScriptObserver* _obs)
{
    CPPIG_DoctrineScript* lSubject = GetSubjectOf(_obs);

    if (lSubject)
    {
        lSubject->RemoveObserver(_obs);
        MakeClean();
    }
}
//---------------------------------------------------------------------------

void CPPIG_Doctrine::MakeClean()
{
    for(int i = 0; i < mDoctrineScripts->Count(); i++)
    {
        if (mDoctrineScripts->Get(i)->IsEmpty())
        {
            mDoctrineScripts->Delete(i);
            i = 0;
        }
    }
}
//---------------------------------------------------------------------------

CPString CPPIG_Doctrine::CompactString(CPString _string)
{
    CPString lCStr = "";

    for(int i = 0; i < _string.Length(); i++)
    {
        if (_string[i] != '\n' && _string[i] != ' ')
        {
            lCStr = lCStr + CPString(_string[i]);
        }
    }

    return lCStr;
}
//---------------------------------------------------------------------------

void CPPIG_Doctrine::Serialize(PP::Stream& _a)
{
    CSerInt    lScriptId;
    int        lScriptNo(mDoctrineScripts->Count());
    CSerString lStreamId("DOCTRINE");

    lStreamId.Serialize(_a);

    //lScriptNo.Serialize(_a);
    //logWrite(".Scripts to be written: "); logWriteILn(lScriptNo.Get());
    //lScriptNo.Serialize(_a);
    _a.writeInt(&lScriptNo);
    VLOG(9) << ".Doctrine scripts to be written: " << lScriptNo;

    // now read the scripts from stream
    for(int i = 0; i < mDoctrineScripts->Count(); i++)
    {
        CSerInt lScriptId(mDoctrineScripts->Get(i)->GetId());

        try
        {
            lScriptId.Serialize(_a);
        }
        catch(...)
        {
            //throw CArchiveError("UNABLE TO WRITE SCRIPT ID !!!!!!!!!!");
        }

        CSerString lScript(mDoctrineScripts->Get(i)->GetScript());

        try
        {
            lScript.Serialize(_a);
        }
        catch(...)
        {
            //throw CArchiveError("UNABLE TO WRITE SCRIPT STRING !!!!!!!!!!");
        }
    }
}
//---------------------------------------------------------------------------

unsigned __int32 CPPIG_Doctrine::DataSize()
{
    return 0;
}
//---------------------------------------------------------------------------

void CPPIG_Doctrine::DeSerialize(PP::Stream& _a)
{
    // deserialization temporary
    CSerInt    lScriptId/*, lScriptId*/;
    __int32    lScriptNo;
    CSerString lScript;
    CSerString lStreamId;

    // clear all scripts
    Clear();

    // read the stream id
    lStreamId.DeSerialize(_a);

    if (lStreamId.Get() != CPString("DOCTRINE"))
    {
        //throw CArchiveError("CPPIG_Doctrine::DeSerialize - stream desync.");
    }

    // read the scripts number
    /*
    try
    {
      lScriptNo.DeSerialize(_a);
    }
    catch(...)
    {
      //throw CArchiveError("UNABLE TO READ NUMBER OF SCRIPTS !!!!!!!!!!");
    }
    */
    _a.readInt(&lScriptNo); //, sizeof(__int32));

    VLOG(9) << ".Doctrine Scripts to read: " << lScriptNo;

    // now read the scripts from stream
    for(int i = 0; i < lScriptNo; i++)
    {
        try
        {
            lScriptId.DeSerialize(_a);
        }
        catch(...)
        {
            //throw CArchiveError("UNABLE TO READ SCRIPT ID !!!!!!!!!!");
        }

        try
        {
            lScript.DeSerialize(_a);
        }
        catch(...)
        {
            //throw CArchiveError("UNABLE TO READ SCRIPT STRING !!!!!!!!!!");
        }

        // add the script to script pool
        AddScript(lScriptId.Get(), lScript.Get());
    }
}
//---------------------------------------------------------------------------

void CPPIG_Doctrine::Serialize(BaseSerializer& out)
{
    out.beginElement("doctrine");

    out.createAttribute("id", "DOCTRINE");

    int scriptsCount = mDoctrineScripts->Count();
    out.rawWrite(scriptsCount);

    VLOG(9) << ".Doctrine scripts to be written: " << scriptsCount;

    out.beginElement("scripts");

    // now read the scripts from stream
    for(int i = 0; i < scriptsCount; i++)
    {
        out.beginElement("script");

        out.createAttribute("id", mDoctrineScripts->Get(i)->GetId());
        out.createAttribute("data", mDoctrineScripts->Get(i)->GetScript());

        out.endElement();
    }

    out.endElement();
    out.endElement();
}
//---------------------------------------------------------------------------

void CPPIG_Doctrine::DeSerialize(BaseDeSerializer& in)
{
}
//---------------------------------------------------------------------------
