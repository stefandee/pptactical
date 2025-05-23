//---------------------------------------------------------------------------

#ifndef BayManagerObjectsH
#define BayManagerObjectsH
//---------------------------------------------------------------------------

#include <vcl.h>

class CIGDropship;
class CIGTranUnit;
class CIGInfantryUnit;
class CIGVehicle;

class BayManagerObject : public TObject
{
  public:
    virtual void Board(CIGDropship* _dropship) = 0;
    virtual void Board(CIGTranUnit* _tran) = 0;

  protected:

  private:
};
//---------------------------------------------------------------------------

class BayManagerInfantryFromFile : public BayManagerObject
{
  public:
    BayManagerInfantryFromFile(int _index);

    virtual void Board(CIGDropship* _dropship);
    virtual void Board(CIGTranUnit* _tran);

  protected:

  private:
    int mIndex;
};
//---------------------------------------------------------------------------

class BayManagerInfantryFromMap : public BayManagerObject
{
  public:
    BayManagerInfantryFromMap(CIGInfantryUnit* _unit);

    virtual void Board(CIGDropship* _dropship);
    virtual void Board(CIGTranUnit* _tran);

  protected:

  private:
    CIGInfantryUnit* mUnit;
};
//---------------------------------------------------------------------------

class BayManagerVehicleFromFile : public BayManagerObject
{
  public:
    BayManagerVehicleFromFile(int _index);

    virtual void Board(CIGDropship* _dropship);
    virtual void Board(CIGTranUnit* _tran);

  protected:

  private:
    int mIndex;
};
//---------------------------------------------------------------------------

class BayManagerVehicleFromMap : public BayManagerObject
{
  public:
    BayManagerVehicleFromMap(CIGVehicle* _unit);

    virtual void Board(CIGDropship* _dropship);
    virtual void Board(CIGTranUnit* _tran);

  protected:

  private:
    CIGVehicle* mUnit;
};
//---------------------------------------------------------------------------

#endif
