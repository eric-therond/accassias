#ifndef T_DOTTREEVISITOR_H
#define T_DOTTREEVISITOR_H

#include "data_structures/tree/t_treevisitor.h"
#include "data_structures/graph/t_node.h"
#include <sstream>
#include <fstream>
//#include "compiler/t_syntaxvertex.h"

// METRE CETTE CLASSE DANS UN REPERTOIRE "DRAW"
// first parameter = type node
template <typename N>
class t_dottreevisitor : public t_treevisitor<N>
{
  public:
    t_dottreevisitor(){
    };

    void initialize(){
      *(this->outfile) << "digraph ats {\nordering=out;\n";	
    };
    void initialize_node(N* node){
      if(node->getvertex()->getlexeme().empty() || node->getvertex()->getlexeme() == "\"")
      {
        *(this->outfile) 
          << node->getlabel()
          << node->getid()
          << " [label=\"" 
          << node->getlabel()
          << "_"
          << node->getid()
          << "\"];\n";
      }
      else
      {
        *(this->outfile) 
          << node->getlabel()
          << node->getid()
          << " [label=\"" 
          << node->getlabel() 
          << "_"
          << node->getvertex()->getlexeme()
          << "_"
          << node->getid()
          << "\"];\n";
      }
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

  private:
    std::ofstream *outfile;

};

#endif


