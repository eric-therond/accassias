#ifndef T_CONTROL_FLOW_GRAPH_H
#define T_CONTROL_FLOW_GRAPH_H

#include "compiler/gencode/t_threeaddresscode.h"
#include "static_analysis/t_static_analysis.h"
#include "static_analysis/t_basicblock.h"

#define L_BB "basic_block"

class t_control_flow_graph
{
  public:
    t_control_flow_graph(t_threeaddresscode *);
    t_cfg *getgraph();
    void start(std::vector<t_threeaddress>::iterator);
    t_basicblock *build(t_basicblock *);

  private:
    t_threeaddresscode *tac;
    t_cfg *cfg;
};


#endif
