#include "t_control_flow_graph.h"

t_control_flow_graph::t_control_flow_graph(t_threeaddresscode *tac)
{
  this->tac = tac;
  this->cfg = new t_cfg;
}

t_cfg *t_control_flow_graph::getgraph()
{
  return this->cfg;
}

void t_control_flow_graph::start(std::vector<t_threeaddress>::iterator it_start)
{
  t_basicblock *first_bb = new t_basicblock(
      it_start, this->tac->get_code()->end(), new t_node<t_threeaddresscode>(L_BB, new t_threeaddresscode));

  this->build(first_bb);

  this->cfg->addnode(first_bb->getnode());
  this->cfg->setroot(first_bb->getnode());
}

t_basicblock *t_control_flow_graph::build(t_basicblock *bb1)
{
  std::vector<t_threeaddress>::iterator it;
  for(it = bb1->get_it_start(); it != bb1->get_it_end(); ++it)
  {
    bb1->getnode()->getvertex()->get_code()->push_back((*it));

    if((*it).gettacsymbol() == TAC_ASSIGN_IF ||
        (*it).gettacsymbol() == TAC_ASSIGN_ELSEIF)
    {	
      t_basicblock *iftrue_bb = new t_basicblock(it + 1, bb1->get_it_end(), 
          new t_node<t_threeaddresscode>(L_BB, new t_threeaddresscode));

      t_basicblock *it_ifend = this->build(iftrue_bb);


      t_basicblock *iffalse_bb = new t_basicblock(it_ifend->get_it_start() + 1, bb1->get_it_end(), 
          new t_node<t_threeaddresscode>(L_BB, new t_threeaddresscode));

      t_basicblock *it_iffalseend = this->build(iffalse_bb);


      this->cfg->addnode(iftrue_bb->getnode());
      this->cfg->addnode(iffalse_bb->getnode());

      bb1->getnode()->addchild(iftrue_bb->getnode());
      bb1->getnode()->addchild(iffalse_bb->getnode());



      t_basicblock *afterif_bb = new t_basicblock(iffalse_bb->get_it_start() + 1, bb1->get_it_end(), 
          new t_node<t_threeaddresscode>(L_BB, new t_threeaddresscode));

      //t_basicblock *it_end = this->build(afterif_bb);
      this->build(afterif_bb);

      this->cfg->addnode(afterif_bb->getnode());

      it_ifend->getnode()->addchild(afterif_bb->getnode());
      it_iffalseend->getnode()->addchild(afterif_bb->getnode());

      return afterif_bb;
    }
    else if((*it).gettacsymbol() == TAC_ASSIGN_ENDELSEIF)
    {	
      bb1->set_it_end(it);			
      return bb1;
    }
    else if((*it).gettacsymbol() == TAC_ASSIGN_ENDELSE)
    {	
      bb1->set_it_end(it);			
      return bb1;
    }
    else if((*it).gettacsymbol() == TAC_ASSIGN_NOELSE)
    {
      bb1->set_it_end(it);			
      return bb1;
    }
    else if((*it).gettacsymbol() == TAC_ASSIGN_ENDIF)
    {
      bb1->set_it_end(it);			
      return bb1;
    }
    else
    {

    }
  }

  bb1->set_it_end(it);		
  return bb1;
}

