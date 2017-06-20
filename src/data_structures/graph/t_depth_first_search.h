#ifndef T_DEPTH_FIRST_SEARCH_H
#define T_DEPTH_FIRST_SEARCH_H

#include "t_node.h"
#include "t_color.h"
#include <stack>
#include <vector>
#include <map>

class t_depth_first_search
{
  public:
    static bool colorize;

    template <typename S, typename G, typename V>
      static void visit(S *visitor, G *graph){
        std::map<t_node<V> *, t_color> nodes_color;
        t_depth_first_search::visit(visitor, graph, nodes_color);
      };

    template <typename S, typename G, typename V>
      static void visit(S *visitor, G *graph, std::map<t_node<V> *, t_color> &nodes_color){
        visitor->initialize();

        typename std::vector<t_node<V> *>::iterator it;
        for(it = graph->getnodes().begin(); it < graph->getnodes().end(); it++)
        {
          visitor->initialize_node(*it);
          if(graph->getcolorize())
            nodes_color[*it] = c_white;
        }

        t_depth_first_search::search(graph->getroot(), visitor, graph, nodes_color);

        visitor->finish();
      };

  private:
    template <typename S, typename G, typename V>
      static void search(t_node<V> *node, S *visitor, G *graph, std::map<t_node<V> *, t_color> &nodes_color){
        if(node != NULL)
        {
          visitor->examine_node(node);

          for(unsigned int i = 0; i < node->getnbchildren(); i++)
          {
            t_color color = nodes_color[node->getchild(i)];
            visitor->examine_node_target(node, node->getchild(i));
            if(color == c_white)
            {
              nodes_color[node] = c_gray;
              t_depth_first_search::search(node->getchild(i), visitor, graph, nodes_color);
            }
            else
            {
              if(color == c_gray)
              {
              }
              else
              {
              }
            }
          }

          visitor->finish_node(node);
          nodes_color[node] = c_black;
        }

      };
};

//bool t_depth_first_search::colorize = true;

#endif









