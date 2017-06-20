#ifndef T_DOTCFGTREEVISITOR_H
#define T_DOTCFGTREEVISITOR_H

#include "data_structures/tree/t_treevisitor.h"
#include "data_structures/graph/t_node.h"
#include <sstream>

template <typename N>
class t_dotcfgtreevisitor : public t_treevisitor<N>
{
  public:
    t_dotcfgtreevisitor(){
    };

    void initialize(){
      *(this->outfile) << "digraph cfg {\nordering=out;\n";	
    };

    void initialize_node(N* node){

      std::stringstream ss;
      ss << *(node->getvertex());
      std::string s = ss.str();
      this->escape_special_char(s);

      *(this->outfile) 
        << node->getlabel()
        << node->getid()
        << " [label=\"" 
        << node->getlabel() 
        << "_"
        << s
        << "\"];\n";
    };

    void examine_node(N *node){
      (void) node;
    };
    void examine_node_target(N *node, N *targetnode){
      *(this->outfile) 
        << node->getlabel()
        << node->getid()
        << "->"
        << targetnode->getlabel()
        << targetnode->getid()
        << ";\n";
    };
    void finish_node(N *node){
      (void) node;
    };
    void finish(){
      *(this->outfile) << "\n\n}";
      this->outfile->close();
    };
    void openfile(const char *file){
      this->outfile = new std::ofstream(file, std::ios::out);
    };

    void escape_special_char(std::string &str)
    {
      std::string strtemp = str;
      unsigned int j = 0;
      for(unsigned int i = 0; i < strtemp.size(); i++)
      {	
        if(strtemp[i] == '\n')
        {
          str.resize(str.size() + 1);
          str[j++] = '\\';
          str[j] = 'n';
        }
        else
          str[j] = strtemp[i];

        j++;
      }
    };

  private:
    std::ofstream *outfile;

};

#endif


