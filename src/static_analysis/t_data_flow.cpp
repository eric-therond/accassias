#include "t_data_flow.h"
#include "compiler/t_compiler.h"
#include "data_structures/graph/t_depth_first_search.h"

t_data_flow::t_data_flow(t_control_flow_graph *control_flow_graph)
{
  this->flows = new std::vector<t_flow *>;
  this->blocks = new std::vector<t_basic_block *>;
  this->control_flow_graph = control_flow_graph;
}

t_label t_data_flow::init(t_statement *st)
{
  (void) st;
  t_label label_init;
  return label_init;
}

t_label *t_data_flow::final(t_statement *st)
{
  (void) st;
  t_label *label_final;
  return label_final;
}

void t_data_flow::get_blocks()
{
  std::vector<t_node<t_threeaddresscode> *> b_blocks = this->control_flow_graph->getgraph()->getnodes();
  for(std::vector<t_node<t_threeaddresscode> *>::iterator it = b_blocks.begin(); it != b_blocks.end(); ++it)
  {
    std::cout << std::endl << std::endl;
    std::cout << "block id = " << (*it)->getid() << std::endl;
    std::cout << *((*it)->getvertex()) << std::endl;
  }

}

void t_data_flow::get_flows()
{
  for(std::vector<t_flow *>::iterator it = flows->begin(); it != flows->end(); ++it)
    std::cout << "flow " << (**it) << std::endl;
}

bool t_data_flow::is_nontrivial_ae(t_tacsymbol tacsymbol)
{
  switch(tacsymbol)
  {
    case TAC_ASSIGN_ADD: break;
    case TAC_ASSIGN_SUB: break;
    case TAC_ASSIGN_DIV: break;
    case TAC_ASSIGN_MUL: break;
    case TAC_ASSIGN_INF: break;
    case TAC_ASSIGN_SUP: break;
    case TAC_ASSIGN_INFEQUAL: break;
    case TAC_ASSIGN_SUPEQUAL: break;
    default: return false;
  }

  return true;
}

bool t_data_flow::is_assignment(t_tacsymbol tacsymbol)
{
  switch(tacsymbol)
  {
    case TAC_ASSIGNV: break;
    case TAC_ASSIGNL: break;
    case TAC_ASSIGNC: break;
    case TAC_ASSIGNS: break;
    case TAC_ASSIGNCA: break;
    case TAC_ASSIGN_ADD: break;
    case TAC_ASSIGN_SUB: break;
    case TAC_ASSIGN_DIV: break;
    case TAC_ASSIGN_MUL: break;
    case TAC_ASSIGN_INF: break;
    case TAC_ASSIGN_SUP: break;
    case TAC_ASSIGN_INFEQUAL: break;
    case TAC_ASSIGN_SUPEQUAL: break;
    default: return false;
  }

  return true;
}

bool t_data_flow::is_varin_aexp(int x, int a, int b)
{
  if(x == a || x == b)
    return true;

  return false;
}

std::vector<t_threeaddress> *t_data_flow::compute_kill_ae(unsigned int label, int x)
{
  (void) label;
  std::vector<t_threeaddress> *kill_ae = new std::vector<t_threeaddress>;
  std::vector<t_threeaddress>::iterator itaexp;
  for(itaexp = this->aexp->begin(); itaexp < this->aexp->end(); itaexp++)
  {
    if(this->is_varin_aexp(x, (*itaexp).getaddrarg1(), (*itaexp).getaddrarg2()))
      kill_ae->push_back(*itaexp);
  }

  return kill_ae;
}

void t_data_flow::get_kill_ae()
{
  this->aexp = new std::vector<t_threeaddress>;
  std::vector<t_threeaddress>::iterator ittac;
  t_threeaddresscode *tac;

  std::vector<t_node<t_threeaddresscode> *> b_blocks = this->control_flow_graph->getgraph()->getnodes();
  for(std::vector<t_node<t_threeaddresscode> *>::iterator it = b_blocks .begin(); it != b_blocks .end(); ++it)
  {
    tac = (*it)->getvertex();

    for(ittac = tac->get_code()->begin(); ittac < tac->get_code()->end(); ittac++)
    {
      if(is_nontrivial_ae((*ittac).gettacsymbol()))
        this->aexp->push_back(*ittac);   
    }

    std::vector<t_node<t_threeaddresscode> *> b_blocks = this->control_flow_graph->getgraph()->getnodes();
    for(std::vector<t_node<t_threeaddresscode> *>::iterator it = b_blocks .begin(); it != b_blocks .end(); ++it)
    {
      tac = (*it)->getvertex();

      for(ittac = tac->get_code()->begin(); ittac < tac->get_code()->end(); ittac++)
      {
        if(is_assignment((*ittac).gettacsymbol()))
          compute_kill_ae((*it)->getid(), (*ittac).getaddrresult());
      }
    }
  }
}

void t_data_flow::get_gen_ae()
{

}

void t_data_flow::add_flow(t_label lb1, t_label lb2)
{
  t_flow *flow = new t_flow(lb1, lb2);
  this->flows->push_back(flow);
}

void t_data_flow::add_block(t_basicblock *bb)
{
  (void) bb;
  //  this->blocks->push_back(bb);
}

void t_data_flow::start()
{
  //this->get_flows();
  //this->get_blocks();
  //this->get_kill_ae();

  /* END CFG */
}

void available_expressions()
{

}

