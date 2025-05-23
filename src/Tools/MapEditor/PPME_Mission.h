//---------------------------------------------------------------------------

#ifndef PPME_MissionH
#define PPME_MissionH
//---------------------------------------------------------------------------

class CPPIG_Mission;

class CPPME_Mission : public CPPIG_Mission
{
  public:
    CPPME_Mission();
    ~CPPME_Mission();

  public:
    void SaveToFile(CPString _filename);  

  protected:
  

  private:
};


#endif
