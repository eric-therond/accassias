#ifndef T_DATA_FLOW_H
#define T_DATA_FLOW_H

#include "compiler/gencode/t_threeaddresscode.h"
#include "static_analysis/t_static_analysis.h"
#include "static_analysis/t_basicblock.h"
#include "static_analysis/t_flow.h"
#include "static_analysis/t_control_flow_graph.h"

class t_data_flow
{
  public:
    t_data_flow(t_control_flow_graph *);
    t_label init(t_statement *);
    t_label *final(t_statement *);

    void get_blocks();
    void add_block(t_basicblock *);

    void get_flows();
    void add_flow(t_label, t_label);

    void get_kill_ae();
    void get_gen_ae();

    bool is_varin_aexp(int, int, int);
    std::vector<t_threeaddress> *compute_kill_ae(unsigned int, int);

    bool is_assignment(t_tacsymbol);
    bool is_nontrivial_ae(t_tacsymbol);
    void available_expressions();

    void start();

  private:
    std::vector<t_threeaddress> *aexp;
    std::vector<t_flow *> *flows;
    std::vector<t_basic_block *> *blocks;
    t_control_flow_graph *control_flow_graph;
};


#endif
