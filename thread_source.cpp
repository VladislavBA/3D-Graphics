#include "thread_source.h"
#include <pthread.h>
#include "sys/time.h"
#include "sys/resource.h"

double get_time()
{
  struct rusage buf;

  getrusage(RUSAGE_THREAD, &buf);

  return (double)buf.ru_utime.tv_sec  + buf.ru_utime.tv_usec / 1000000.; // return time in seconds
}


double get_full_time()
{
  struct timeval buf;

  gettimeofday(&buf, 0);

  return (double)buf.tv_sec + buf.tv_usec / 1000000.; // return time in seconds
}
