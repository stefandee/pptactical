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
//  Unit              : PP In-game Doctrine
//
//  Version           : 2.0
//
//  Description       :
//    * it is mostly used in the design/test of AI and scripting
//    * contains all the scripts used in the game
//    * it contains the mission control script, the battle groups scripts and
//      the unit scripts, so that the script will not be duplicated
//    * of course, a 1-2KBytes script may not count alone, but if stored by
//      100 units, starts to go up;
//    * plus, a script is loaded only once by the csl object of a (unit, mission control,
//      battle group), at initialization, so storing the script inside of an object is
//      pointless
//    * the Doctrine class will implement a flyweight pattern combined with an observer
//      pattern; the observer pattern is needed when editing scripts in editor or
//      dynamically editing a script while the engine runs
//
//  History           :
//    [12.12.2001] - [Karg] - unit created
//    [18.12.2001] - [Karg] - added ChangeScriptName method
//    [29.01.2002] - [Karg] - added the observer stuff, which implied heavy structural
//                            changes
//-----------------------------------------------------------------------------

#ifndef PPIG_DoctrineH
#define PPIG_DoctrineH
//---------------------------------------------------------------------------

#include "PP_String.h"
#include "KList.h"
#include "Serializable.h"
#include "XmlSerializable.h"

/*
// this class is a ConcreteFlyweight
class CPPIG_DoctrineScript
{
  private:
    CPString mScript;
    CPString mName;

  protected:

  public: // c-d
    CPPIG_DoctrineScript();
    CPPIG_DoctrineScript(CPString _name, CPString _script);
    ~CPPIG_DoctrineScript();

  public: // get-set
    CPString GetName() { return mName; }
    void SetName(CPString _name) { mName = _name; }

    CPString GetScript() { return mScript; }
    void SetScript(CPString _script) { mScript = _script; }
};
//---------------------------------------------------------------------------
*/

// forward decl.
class CPPIG_ScriptObserver;

// this class is a ConcreteFlyweight
// the contents of this class are _subject_ to observers
class CPPIG_DoctrineScript
{
  private:
    CPString                      mScript;    // this is the content
    __int16                       mId;
    //CPList<CPString>*             mNames;      // a script may be known under several names (facets)
    CPList<CPPIG_ScriptObserver>* mObservers; // these are the subscribers/observers

  protected:


  public: // c-d
    CPPIG_DoctrineScript();
    CPPIG_DoctrineScript(__int16 _id, CPString _script);
    ~CPPIG_DoctrineScript();

  public: // get-set
    void AddObserver(CPPIG_ScriptObserver* _obs);
    void RemoveObserver(CPPIG_ScriptObserver* _obs);
    bool ContainsObserver(CPPIG_ScriptObserver* _obs) { return mObservers->Contain(_obs); };
    bool IsEmpty() { return (mObservers->Count() == 0); };

    CPString  GetScript()                 { return mScript; }
    void      SetScript(CPString _script) { mScript = _script; }

    void SetId(__int16 _v) { mId = _v; };
    __int16  GetId() { return mId; };

    void Notify();
};
//---------------------------------------------------------------------------

// this is a default observer class
class CPPIG_ScriptObserver
{
  private:

  protected:
    CPPIG_DoctrineScript* mSubject;

  public: // c-d
    CPPIG_ScriptObserver();
    virtual ~CPPIG_ScriptObserver();

  public:
    virtual void Update(CPString _script) {};

    void SetSubject(CPPIG_DoctrineScript* _subject) { mSubject = _subject; };
    //CPPIG_DoctrineScript* GetSubject() { return mSubject; } // useless

    //void SetName(CPString _name) { mName = _name; }
    //CPString GetName() { return mName; }

    CPString GetScript()   { return mSubject->GetScript(); }
    int  GetScriptId() { return mSubject->GetId(); }
};
//---------------------------------------------------------------------------

// a script observer with a handler - useful if you want a class to contain
// an observer rather than inherit one
typedef std::function <void(CPString)> ScriptObserver2Callback; /**< Scroll bar related events */

class CPPIG_ScriptObserver2 : public CPPIG_ScriptObserver
{
  protected:
    ScriptObserver2Callback mHandleScript;

  public: // c-d
    CPPIG_ScriptObserver2();
    virtual ~CPPIG_ScriptObserver2();

  public:
    void SetHandleScript(ScriptObserver2Callback _value) { mHandleScript = _value; };
    virtual void Update(CPString _script);
};
//---------------------------------------------------------------------------

// this class is a FlyweightFactory (the script pool)
// this class assures that scripts are unique
class CPPIG_Doctrine : public CSerializable, public Serializable2
{
  private:
    CPList<CPPIG_DoctrineScript>* mDoctrineScripts;

  private: // util private interface
    CPPIG_DoctrineScript* GetSubjectByIndex(int _index);
    int                   SubjectCount() { return mDoctrineScripts->Count(); }
    void                  Clear();
    void                  AddScript(int _id, CPString _script);

  protected:
    int                   GenerateUniqueScriptId();
    CPPIG_DoctrineScript* GetSubjectOf(CPPIG_ScriptObserver* _obs);
    void                  MakeClean();
    CPString              CompactString(CPString _string);

  public: // c-d
    CPPIG_Doctrine();
    ~CPPIG_Doctrine();

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

    virtual void Serialize(BaseSerializer& out);
    virtual void DeSerialize(BaseDeSerializer& in);

  public: // actual factory interface
    CPPIG_ScriptObserver* AddScript(CPPIG_ScriptObserver* _obs, CPString _script);
    CPPIG_ScriptObserver* AddScriptFromFile(CPPIG_ScriptObserver* _obs, CPString _fileName);
    CPPIG_ScriptObserver* GetScriptObserver(int _id);
    void                  RemoveScriptObserver(CPPIG_ScriptObserver* _obs); // redundant
};

/*
class CPPIG_Doctrine
{
  private:
    CPList<CPPIG_DoctrineScript>* mDoctrineScripts;

  protected:

  public:
    CPPIG_Doctrine();
    ~CPPIG_Doctrine();

    CPPIG_DoctrineScript* GetScriptByName(CPString _name);
    CPPIG_DoctrineScript* GetScriptByIndex(int _index);
    int                   Count() { return mDoctrineScripts->Count(); }

    bool                  RemoveScript(CPString _name);

    void SetScript(CPString _name, CPString _string);
    //bool ChangeScriptName(CPString _oldName, CPString _newName);

    void Clear();
};
*/

#endif
