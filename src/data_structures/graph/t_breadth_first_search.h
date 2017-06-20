#ifndef T_BREADTH_FIRST_SEARCH_H
#define T_BREADTH_FIRST_SEARCH_H

#include "t_node.h"
#include "t_color.h"
#include <stack>
#include <vector>
#include <map>

class t_breadth_first_search
{
  public:
    template <typename S, typename G, typename V>
      static void visit(S *visitor, G *graph)
      {
        std::map<t_node<V> *, t_color> nodes_color;
        t_breadth_first_search::visit<S, G, V>(visitor, graph, nodes_color);
      }

    template <typename S, typename G, typename V>
      static void visit(S *visitor, G *graph, std::map<t_node<V> *, t_color> &nodes_color)
      {
        visitor->initialize();

        std::vector<t_node<t_symbol> *>::iterator it;
        for(it = graph->getnodes().begin(); it < graph->getnodes().end(); it++)
        {
          visitor->initialize_node(*it);
          nodes_color[*it] = c_white;
        }

        if(graph->getroot() == NULL)
          return;

        t_breadth_first_search::search(visitor, graph, nodes_color);

        visitor->finish();
      };

  private:
    template <typename S, typename G, typename V>
      static void search(S *visitor, G *graph, std::map<t_node<V> *, t_color> &nodes_color)
      {
        std::stack<t_node<V> *> Q;
        Q.push(graph->getroot());
        while(!Q.empty())
        {
          t_node<V> *node = Q.top();
          Q.pop();
          visitor->examine_node(node);

          for(unsigned int i = 0; i < node->getnbchildren(); i++)
          {
            t_color color = nodes_color[node->getchild(i)];
            if(color == c_white)
            {
              visitor->examine_node_target(node, node->getchild(i));
              nodes_color[node] = c_gray;
              Q.push(node->getchild(i));
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

#endif









