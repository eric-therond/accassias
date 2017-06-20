#ifndef T_FLOW_H
#define T_FLOW_H

#include "static_analysis/t_static_analysis.h"

class t_flow
{
  public:
    t_flow();
    t_flow(t_label, t_label);
    void set_source(t_label);
    void set_destination(t_label);
    t_label get_source();
    t_label get_destination();

  private:
    t_label source;
    t_label destination;
};

std::ostream &operator<<(std::ostream &, t_flow &);

#endif
