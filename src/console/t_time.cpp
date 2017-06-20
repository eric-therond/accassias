#include "t_time.h"
#include <unistd.h>

timestamp_t get_timestamp()
{
  struct timeval now;
  gettimeofday (&now, NULL);
  return  now.tv_usec / 1000;
}
