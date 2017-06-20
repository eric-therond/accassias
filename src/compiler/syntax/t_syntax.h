#ifndef T_SYNTAX_H 
#define T_SYNTAX_H

#include "compiler/lexical/t_lexical.h"
#include "data_structures/tree/t_tree.h"
#include "compiler/t_compiler.h"
#include "t_symbol.h"
#include "t_identifiervertex.h"
#include "t_function.h"
#include "t_class.h"
#include <map>


#define DEBUG_FACTOR "factor"
#define ERROR_FACTOR_DOUBLE_QUOTE "double quote manquant"
#define ERROR_FACTOR_IDENTIFIER_DOESNT_EXIST "cet identifiant n'existe pas"
#define ERROR_FACTOR_RIGHT_PARENTHESIS "parenthèse droite attendu"

#define DEBUG_VAR "variable"
#define ERROR_VAR_IDENTIFIER "l'identifiant manque"

#define DEBUG_VAREND "variable end"
#define ERROR_VAREND_RIGHT_PARENTHESIS "la parenthèse droite manque"
#define ERROR_VAREND_DOTCOMMA "le point virgule manque"

#define DEBUG_VARAFF "variable affectation"
#define ERROR_VARAFF_IDENTIFIER "l'identifiant manque"
#define ERROR_VARAFF_IDENTIFIER_DOESNT_EXIST "cet identifiant n'existe pas"

#define DEBUG_VARDEC "variable declaration"
#define ERROR_VARDEC_DOLLAR "le signe $ manque"
#define ERROR_VARDEC_IDENTIFIER "l'identifiant manque"
#define ERROR_VARDEC_IDENTIFIER_EXIST "cet identifiant existe déja"
#define ERROR_VARDEC_RIGHTBRACKET "le crochet droit manque"

#define DEBUG_CALL "call"
#define ERROR_CALL_IDENTIFIER_DOESNT_EXIST "cet identifiant n'existe pas"
#define ERROR_CALL_LEFT_PARENTHESIS "la parenthèse gauche manque"
#define ERROR_CALL_ARG "il manque au moins un argument"
#define ERROR_CALL_RIGHT_PARENTHESIS "la parenthèse droite manque"
#define ERROR_CALL_DOTCOMMA "le point virgule manque"

#define DEBUG_CLASS_INSTANCE "class instance"
#define ERROR_CLASS_IDENTIFIER_DOESNT_EXIST "cet identifiant n'existe pas"

#define DEBUG_BLOC "bloc"
#define ERROR_BLOC_LEFT_BRACE "l'accolade gauche manque"
#define ERROR_BLOC_RIGHT_BRACE "l'accolade droite manque"

#define DEBUG_CLASS "class"
#define ERROR_CLASS_IDENTIFIER "l'identifiant manque"
#define ERROR_CLASS_IDENTIFIER_EXIST "cet identifiant existe déja"
#define ERROR_CLASS_LEFT_PARENTHESIS "la parenthèse gauche manque"
#define ERROR_CLASS_LEFT_BRACE "l'accolade gauche manque"
#define ERROR_CLASS_RIGHT_BRACE "l'accolade droite manque"

#define DEBUG_FUNCTION "function"
#define ERROR_FUNCTION_IDENTIFIER "l'identifiant manque"
#define ERROR_FUNCTION_IDENTIFIER_EXIST "cet identifiant existe déja"
#define ERROR_FUNCTION_LEFT_PARENTHESIS "la parenthèse gauche manque"
#define ERROR_FUNCTION_LEFT_BRACE "l'accolade gauche manque"
#define ERROR_FUNCTION_RIGHT_BRACE "l'accolade droite manque"

#define DEBUG_RETURN "return"
#define ERROR_RETURN_DOTCOMMA "le point virgule manque"

#define DEBUG_FOR "for"
#define ERROR_FOR_LEFT_PARENTHESIS "la parenthèse gauche manque"
#define ERROR_FOR_FIRST_DOTCOMMA "le premier point virgule manque"
#define ERROR_FOR_SECOND_DOTCOMMA "le second point virgule manque"
#define ERROR_FOR_RIGHT_PARENTHESIS "la parenthèse droite manque"

#define DEBUG_IF "if"
#define ERROR_IF_LEFT_PARENTHESIS "la parenthèse gauche manque"
#define ERROR_IF_RIGHT_PARENTHESIS "la parenthèse droite manque"

class t_syntax 
{
  public:
    t_syntax();
    t_lexical *get_lexical();
    bool start();
    void syntaxerror(const std::string &, const unsigned int, const std::string &);
    t_syntaxtree *gettree();
    /*
       t_syntaxtree *gettree_functions();
       t_syntaxtree *gettree_classes();
     */  
    std::map<std::string, t_function *> *get_functions();
    std::map<std::string, t_class *> *get_classes();
    unsigned int getnblocalvariables();

    void system_instruction(t_syntaxtree *, t_node<t_symbol> *);
    void readpol_instruction(t_syntaxtree *, t_node<t_symbol> *);

    void include_instruction(t_syntaxtree *, t_node<t_symbol> *);
    void print_instruction(t_syntaxtree *, t_node<t_symbol> *);
    void function_declaration(t_syntaxtree *, t_node<t_symbol> *);
    void fputs_instruction(t_syntaxtree *, t_node<t_symbol> *);
    void cfg_dot_instruction(t_syntaxtree *, t_node<t_symbol> *);
    void cfg_compute_instruction(t_syntaxtree *, t_node<t_symbol> *);
    void ast_dot_instruction(t_syntaxtree *, t_node<t_symbol> *);
    void class_declaration(t_syntaxtree *, t_node<t_symbol> *);
    void call_instruction(t_syntaxtree *, t_node<t_symbol> *, bool, std::map<std::string, t_function *> *);
    void instructions(t_node<t_symbol> *, t_node<t_symbol> *, t_node<t_symbol> *);
    void bloc_instructions(t_node<t_symbol> *);
    void bloc_classe(t_node<t_symbol> *,t_node<t_symbol> *);
    void classe_instance(t_syntaxtree *, t_node<t_symbol> *, t_identifier *);
    void for_instruction(t_syntaxtree *, t_node<t_symbol> *);
    void if_instruction(t_syntaxtree *, t_node<t_symbol> *);
    void variable_declaration(t_syntaxtree *, t_node<t_symbol> *, bool);


    void variable(t_syntaxtree *, t_node<t_symbol> *, bool, bool);
    void variable_affectation(t_syntaxtree *, t_node<t_symbol> *, t_identifier *, bool);
    void variable_end(t_syntaxtree *, t_node<t_symbol> *, bool);
    void return_instruction(t_syntaxtree *, t_node<t_symbol> *);
    unsigned int expression(t_syntaxtree *, t_node<t_symbol> *);
    void factor(t_syntaxtree *, t_node<t_symbol> *);
    void term(t_syntaxtree *, t_node<t_symbol> *);

    bool getexistsyntaxerror();

  private:
    t_lexical *lexical;
    t_syntaxtree *syntaxtree_prog;

    t_node<t_symbol> *prognode;
    t_node<t_symbol> *classesnode;
    t_node<t_symbol> *functions;
    t_node<t_symbol> *instructionsnode;

    t_tree<t_identifiervertex> *identifierstree;
    bool insidefunction;
    bool insideclass;
    bool existsyntaxerror;

    std::map<std::string, t_identifier *> *temp_variables;
    std::map<std::string, t_identifier *> *global_variables;
    std::map<std::string, t_function *> *global_functions;
    std::map<std::string, t_class *> *global_class;
    std::map<std::string, t_class *> *global_instances_class;
    t_function *current_function;
    t_class *current_classe;

    bool existreturn;
    unsigned int nbexprstermsfactors;
    unsigned int nbexprstermsfactors_prog;
    unsigned int nbexprstermsfactors_class;
    unsigned int nb_global_functions;
    unsigned int nb_global_class;
    unsigned int last_nb_nodes;

    unsigned int backnode_id;
    unsigned int backnode_functions_id;
    unsigned int backnode_classes_id;

};

#endif

