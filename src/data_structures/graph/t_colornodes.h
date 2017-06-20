#ifndef T_COLORNODES_H
#define T_COLORNODES_H

#include "t_color.h"
#include "t_node.h"
#include <map>

template <typename V>
class t_colornodes : public std::map<t_node<V> *, t_color> 
{
  public:
    t_colornodes() 
    {
    };

    template <typename G>
      colorize(G *graph)
      {
        std::vector<t_node<V> *>::iterator it;
        for(it = graph->getnodes().begin(); it < graph->getnodes().end(); it ++)
          this->[*it] = c_white;
      };
};

#endif


