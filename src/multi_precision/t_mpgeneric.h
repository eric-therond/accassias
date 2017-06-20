#ifndef T_MPGENERIC_H 
#define T_MPGENERIC_H

#include <vector>
#include <string>

enum mp_type
{
  MP_STRING,
  MP_INTEGER,
  MP_ARRAY
};

class t_mpgeneric 
{
  public:
    t_mpgeneric();
    t_mpgeneric(mp_type);

    void setbase(unsigned int);
    unsigned int getbase();

    void settype(mp_type);
    mp_type gettype();

    void setsize(unsigned int);
    unsigned int getsize();

    std::vector<int> &getdigits();
    void setdigits(std::vector<int> &, unsigned int);
    void setdigits(std::vector<int> &, unsigned int, unsigned int);
    void setdigits(std::vector<int> &, unsigned int, unsigned int, unsigned int);
    void setdigits(std::string &);

    unsigned int toint();
    void setcases(std::vector<int> &, unsigned int, std::vector<t_mpgeneric *> &);
    void setcase(unsigned int, t_mpgeneric *);
    t_mpgeneric *getcase(unsigned int);
    std::vector<t_mpgeneric *> getcases();

    bool istrue();
    static bool equal(t_mpgeneric *, t_mpgeneric *);
    static bool sup(t_mpgeneric *, t_mpgeneric *);
    static bool supequal(t_mpgeneric *, t_mpgeneric *);
    static bool inf(t_mpgeneric *, t_mpgeneric *);
    static bool infequal(t_mpgeneric *, t_mpgeneric *);

    std::vector<int> digits;

  private:
    std::vector<t_mpgeneric *> cases;
    unsigned int base;
    mp_type type;
};

std::ostream &operator<<(std::ostream &, t_mpgeneric &);

#endif

