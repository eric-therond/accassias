#ifndef T_LEXICAL_H 
#define T_LEXICAL_H 

#include "t_token.h"
#include <string>
#include <fstream>
#include <vector>

#define ERROR_LEXICAL "erreur lexicale"

#define CHAR_END 0
#define CHAR_UP_ARROW 1
#define CHAR_DOWN_ARROW 2
#define CHAR_TAB 3

#define NB_DISABLE 1
#define NB_ENABLE 1

enum t_lexical_source
{
  FROM_STDIN,
  FROM_LINE,
  FROM_FILE
};

class t_lexical 
{
  public:
    t_lexical();
    ~t_lexical();
    t_lexical_source get_source();
    void init();
    void set_source(t_lexical_source);
    void set_file_source(const char *);
    void lexicalerror();
    int kbhit();
    void nonblock(int);

    void read_next_line();

    void unget_char();
    void get_char();
    char read_char(bool);
    t_token read_keyword();
    bool read_identifier();
    bool read_string();
    bool read_number();

    std::string &get_current_lexeme();
    std::string &get_current_line_string();
    t_token next_token();
    t_token get_current_token();
    unsigned int get_current_line();
    unsigned int get_current_column();

  private:
    char prec_current_char;
    char current_char;
    std::string current_lexeme;
    t_token current_token;
    unsigned int current_line;

    unsigned int nb_line_string;
    unsigned int pos_line_string;
    std::string current_line_string;
    std::vector<std::string> lines_string;

    unsigned int current_pos_line;
    unsigned int current_column;
    t_lexical_source lexical_source;
    std::ifstream is;

    int erase_char;
    bool end_up_arrow;
    bool end_down_arrow;
};

#endif


