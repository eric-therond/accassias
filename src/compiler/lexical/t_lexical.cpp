#include "t_lexical.h"
#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <termios.h>
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h> 

t_lexical::t_lexical()
{
  this->set_source(FROM_STDIN);
  this->init();
}

t_lexical::~t_lexical()
{
  nonblock(NB_DISABLE);
}

void t_lexical::init()
{
  erase_char = 0;
  current_line = 1;
  current_column = 0;
  pos_line_string = 0;
  nb_line_string = 0;
  current_char = CHAR_END;	
  end_up_arrow = false;
  end_down_arrow = false;
  lines_string.resize(20);
}

void t_lexical::set_file_source(const char *file)
{
  this->init();
  is.open(file, std::ifstream::in);
  set_source(FROM_FILE);
}

t_lexical_source t_lexical::get_source()
{
  return this->lexical_source;
}

void t_lexical::set_source(t_lexical_source lexical_source)
{
  this->lexical_source = lexical_source;

  switch(this->lexical_source)
  {
    case FROM_FILE:
      {
        nonblock(NB_DISABLE);
        break;
      }

    case FROM_STDIN:
      {
        nonblock(NB_ENABLE);
        break;
      }

    case FROM_LINE:
      {
        nonblock(NB_ENABLE);
        break;
      }
  }
}

void t_lexical::lexicalerror()
{
  std::cout << ERROR_LEXICAL;
}

std::string &t_lexical::get_current_lexeme()
{
  return current_lexeme;
}

std::string &t_lexical::get_current_line_string()
{
  return current_line_string;
}

t_token t_lexical::get_current_token()
{
  return current_token;
}

unsigned int t_lexical::get_current_line()
{
  return current_line;
}

unsigned int t_lexical::get_current_column()
{
  return current_column;
}

t_token t_lexical::next_token()
{
  current_token = T_ERROR;
  current_lexeme.clear();
  current_char = read_char(true); 

  switch(current_char)
  {
    case CHAR_UP_ARROW: current_token = T_UP_ARROW; break;
    case CHAR_DOWN_ARROW: current_token = T_DOWN_ARROW; break;
    case CHAR_END: current_token = T_END; break;
    case CHAR_TAB: current_token = T_TAB; break;
    case ',': current_token = T_COMMA; break;
    case ';': current_token = T_DOTCOMMA; break;
    case '=': current_token = T_EQUAL; break;
    case '$': current_token = T_VARIABLE; break;
    case '+': current_token = T_ADD; break;
    case '-': current_token = T_SUB; break;
    case '/': current_token = T_DIV; break;
    case '*': current_token = T_MUL; break;
    case ')': current_token = T_RIGHTPARENTHESIS; break;
    case '(': current_token = T_LEFTPARENTHESIS; break;
    case '}': current_token = T_RIGHTBRACE; break;
    case '{': current_token = T_LEFTBRACE; break;
    case ']': current_token = T_RIGHTBRACKET; break;
    case '[': current_token = T_LEFTBRACKET; break;
              //case '"': current_token = T_DOUBLEQUOTE; break;
    case '<': current_token = T_INF; break;
    case '>': current_token = T_SUP; break;
  }

  if(current_token == T_INF || current_token == T_SUP)
  {
    if(read_char(true) == '=')
    {
      switch(current_token)
      {
        case T_INF:
          {
            current_token = T_INFEQUAL;
            break;
          }

        case T_SUP:
          {
            current_token = T_SUPEQUAL;
            break;
          }

        default:
          break;
      }
    }
    else
      unget_char();
  }

  if(this->current_token != T_ERROR)
  {
    this->current_lexeme = this->current_char;
    return current_token;
  }

  if(read_identifier())
    current_token = this->read_keyword();

  if(read_number())
    current_token = T_NUMBER;

  if(read_string())
    current_token = T_STRING;

  return current_token;
}

t_token t_lexical::read_keyword()
{	
  if(this->current_lexeme == "t_fputs")
    return T_FPUTS;

  if(this->current_lexeme == "t_cfg_dot")
    return T_CFG_DOT;

  if(this->current_lexeme == "t_cfg_compute")
    return T_CFG_COMPUTE;

  if(this->current_lexeme == "t_ast_dot")
    return T_AST_DOT;

  if(this->current_lexeme == "t_print")
    return T_PRINT;

  if(this->current_lexeme == "t_readpol")
    return T_READPOL;

  if(this->current_lexeme == "t_system")
    return T_SYSTEM;

  if(this->current_lexeme == "t_include")
    return T_INCLUDE;

  if(this->current_lexeme == "declr")
    return T_DECLR;

  if(this->current_lexeme == "function")
    return T_FUNCTION;

  if(this->current_lexeme == "class")
    return T_CLASS;

  if(this->current_lexeme == "new")
    return T_NEW;

  if(this->current_lexeme == "return")
    return T_RETURN;

  if(this->current_lexeme == "if")
    return T_IF;

  if(this->current_lexeme == "elseif")
    return T_ELSEIF;

  if(this->current_lexeme == "else")
    return T_ELSE;

  if(this->current_lexeme == "for")
    return T_FOR;

  if(this->current_lexeme == "do")
    return T_DO;

  if(this->current_lexeme == "while")
    return T_WHILE;

  if(this->current_lexeme == "this")
    return T_THIS;

  return T_IDENTIFIER;
}

bool t_lexical::read_string()
{
  if(current_char == '"')
  {
    current_char = read_char(false);

    while(current_char != '"')
    {
      current_lexeme.append(1, current_char);
      current_char = read_char(false);
    }

    //ungetchar();
    return true;
  }

  return false;
}

bool t_lexical::read_identifier()
{
  if((current_char >= 'a' && current_char <= 'z') || current_char == '_')
  {
    while((current_char >= 'a' && current_char <= 'z') || current_char == '_')
    {
      current_lexeme.append(1, current_char);
      current_char = read_char(false);
    }

    unget_char();
    return true;
  }

  return false;
}

bool t_lexical::read_number()
{
  if(current_char >= '0' && current_char <= '9')
  {
    while(current_char >= '0' && current_char <= '9')
    {
      current_lexeme.append(1, current_char);
      current_char = read_char(false);
    }

    unget_char();
    return true;
  }

  return false;
}

int t_lexical::kbhit()
{
  struct timeval tv;
  fd_set fds;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
  select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &fds);
}

void t_lexical::nonblock(int state)
{
  struct termios ttystate;

  //get the terminal state
  tcgetattr(STDIN_FILENO, &ttystate);

  if (state==NB_ENABLE)
  {
    //turn off canonical mode
    ttystate.c_lflag &= ~ICANON;
    ttystate.c_lflag &= (~ECHO);
    //minimum of number input read.
    ttystate.c_cc[VMIN] = 1;
    erase_char = ttystate.c_cc[VERASE];
  }
  else if (state==NB_DISABLE)
  {
    //turn on canonical mode
    ttystate.c_lflag |= ICANON;
  }
  //set the terminal attributes.
  tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

}

void t_lexical::read_next_line()
{
  unsigned int i;
  current_pos_line = 0;
  current_line_string = "";
  current_column = 0;

  do
  {
    current_char = read_char(true); 

    if(current_char == CHAR_TAB)
      break;

    if(current_char == CHAR_UP_ARROW || current_char == CHAR_DOWN_ARROW)
    {
      if(nb_line_string)
      {
        for(i = 0; i < current_column; i ++)
          std::cout << "\b \b";

        switch(current_char)
        {
          case CHAR_UP_ARROW :
            {
              if(end_down_arrow)
                pos_line_string --;

              std::cout << lines_string[pos_line_string % 20];
              current_line_string = lines_string[pos_line_string % 20];
              current_column = current_line_string.size();

              if(pos_line_string > 0)
                pos_line_string --;

              end_up_arrow = true;
              end_down_arrow = false;

              break;
            }

          case CHAR_DOWN_ARROW :
            {
              if(end_up_arrow)
                pos_line_string ++;

              std::cout << lines_string[pos_line_string % 20];
              current_line_string = lines_string[pos_line_string % 20];
              current_column = current_line_string.size();

              if(pos_line_string < (nb_line_string - 1))
                pos_line_string ++;

              end_up_arrow = false;
              end_down_arrow = true;

              break;
            }
        }
      }
    }


  }
  while(current_char != CHAR_END);

  if(current_line_string != "")
  {
    lines_string[nb_line_string % 20] = current_line_string;	
    pos_line_string = nb_line_string;	
    nb_line_string ++;
  }
}

char t_lexical::read_char(bool space)
{

  this->prec_current_char = current_char;

  while(true)
  {
    this->get_char();

    switch(current_char)
    {
      //case CHAR_END:
      //return CHAR_END;

      case ' ': if(space) break;
      case '\r': if(space) break;
      case '\t': if(space) break;
      case '/':
                 {
                   if(space)
                   {
                     this->get_char();
                     if(current_char == '*')
                     {
                       while(current_char != CHAR_END)
                       {
                         this->get_char();
                         if(current_char == '*')
                         {
                           this->get_char();
                           if(current_char == '/')
                             break;
                         }
                       }
                     }
                     else
                       unget_char();

                     break;
                   }
                 }

      default: return current_char;
    }
  }

  return current_char;
}

void t_lexical::unget_char()
{
  this->current_char = this->prec_current_char;

  switch(this->lexical_source)
  {
    case FROM_FILE:
      {
        is.unget();
        break;
      }

    case FROM_STDIN:
      {
        std::cin.unget();
        break;
      }

    case FROM_LINE:
      {
        current_pos_line --;
        break;
      }
  }
}

void t_lexical::get_char()
{
  switch(this->lexical_source)
  {
    case FROM_FILE:
      {
        current_char = is.get();

        if(!is.good())
        {
          this->set_source(FROM_STDIN);
          current_char = CHAR_END;
          is.close();
          break;
        }

        if(current_char == '\n')
        { 
          current_char = ' ';
          current_line ++; 
          current_column = 0;
          break;
        }

        current_column ++;

        break;
      }

    case FROM_STDIN:
      {
        usleep(1);
        if(!kbhit())
          current_char = std::cin.get();

        if(current_char == erase_char && current_column > 0)
        {
          std::cout << "\b \b";
          current_line_string.erase(current_line_string.length() - 1, 1);
          current_column --;
          get_char();
          break;
        }

        if(current_char == '\n') 
        {		
          this->set_source(FROM_LINE);
          current_char = CHAR_END;
          current_column = 0;
          break;
        }	

        if(current_char ==  9)
        {
          current_char = CHAR_TAB;
          current_token = T_TAB;
          break;
        }

        if(current_char ==  27)
        {
          if(!kbhit() && std::cin.get() == 91)
          {
            if(!kbhit())
            {
              current_char = std::cin.get();
              if(current_char == 65)
              {
                current_char = CHAR_UP_ARROW;
                current_token = T_UP_ARROW;
                //this->set_source(FROM_LINE);
              }
              else if(current_char == 66)
              {
                current_char = CHAR_DOWN_ARROW;
                current_token = T_DOWN_ARROW;
                //this->set_source(FROM_LINE);
              }
            }
          }
          break;
        }

        if(current_char != 127)
        {
          current_line_string += current_char; 
          std::cout << current_char;
          current_column ++;
        }

        break;
      }

    case FROM_LINE:
      {
        if(current_pos_line == current_line_string.size())
        {
          this->set_source(FROM_STDIN);
          current_char = CHAR_END;
          current_line_string = ""; 
          current_pos_line = 0;
          current_column = 0;
          break;
        }

        current_char = current_line_string[current_pos_line ++];
        current_column ++;

        break;
      }
  }
}

