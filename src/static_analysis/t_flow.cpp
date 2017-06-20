#include "t_flow.h"

t_flow::t_flow()
{
}

t_flow::t_flow(t_label source, t_label destination)
{
  this->set_source(source);
  this->set_destination(destination);
}

void t_flow::set_source(t_label source)
{
  this->source = source;
}

void t_flow::set_destination(t_label destination)
{
  this->destination = destination;
}

t_label t_flow::get_source()
{
  return this->source;
}

t_label t_flow::get_destination()
{
  return this->destination;
}

std::ostream &operator<<(std::ostream &out, t_flow &flow)
{
  out << "(" << flow.get_source() << ", " << flow.get_destination() << ")"; 
  return out;
}





