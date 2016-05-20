#include "multithread.h"

#include "stdio.h"
#include "pthread.h"

int synchronize (int p)
{
  static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
  static pthread_cond_t c_in = PTHREAD_COND_INITIALIZER;
  static pthread_cond_t c_out = PTHREAD_COND_INITIALIZER;
  static int t_in = 0;
  static int t_out = 0;

  pthread_mutex_lock (&m);

  t_in++;

  if (t_in >= p)
    {
      t_out = 0;
      pthread_cond_broadcast (&c_in);
    }
  else
    {
      while (t_in < p)
        pthread_cond_wait (&c_in,&m);
    }
  t_out++;
  if (t_out >= p)
    {
      t_in = 0;
      pthread_cond_broadcast (&c_out);
    }
  else
    {
      while (t_out < p)
        pthread_cond_wait (&c_out,&m);
    }
  pthread_mutex_unlock (&m);

  return 0;
}

double reduce_sum (double *sum, int my_rank, int p)
{
  static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
  static pthread_cond_t c_in = PTHREAD_COND_INITIALIZER;
  static pthread_cond_t c_out = PTHREAD_COND_INITIALIZER;
  static int t_in = 0;
  static int t_out = 0;

  pthread_mutex_lock (&m);

  t_in++;

  if (t_in >= p)
    {
      t_out = 0;
      pthread_cond_broadcast (&c_in);
    }
  else
    {
      while (t_in < p)
        pthread_cond_wait (&c_in,&m);
    }
  if (my_rank == 0)
    {
      for (int i = 1; i < p; i++)
        sum[0] += sum[i];
      for (int i = 1; i < p; i++)
        sum[i] = sum[0];
    }
  t_out++;
  if (t_out >= p)
    {
      t_in = 0;
      pthread_cond_broadcast (&c_out);
    }
  else
    {
      while (t_out < p)
        pthread_cond_wait (&c_out,&m);
    }
  pthread_mutex_unlock (&m);

  return sum[my_rank];
}

double reduce_max_sum (double *max, int my_rank, int p)
{
  static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
  static pthread_cond_t c_in = PTHREAD_COND_INITIALIZER;
  static pthread_cond_t c_out = PTHREAD_COND_INITIALIZER;
  static int t_in = 0;
  static int t_out = 0;

  pthread_mutex_lock (&m);

  t_in++;

  if (t_in >= p)
    {
      t_out = 0;
      pthread_cond_broadcast (&c_in);
    }
  else
    {
      while (t_in < p)
        pthread_cond_wait (&c_in,&m);
    }
  if (my_rank == 0)
    {
      double reduce_max = 0.;
      for (int i = 0; i < p; i++)
        {
          if (reduce_max < max[i])
            reduce_max = max[i];
        }
      for (int i = 0; i < p; i++)
        max[i] = reduce_max;
    }
  t_out++;

  if (t_out >= p)
    {
      t_in = 0;
      pthread_cond_broadcast (&c_out);
    }
  else
    {
      while (t_out < p)
        pthread_cond_wait (&c_out,&m);
    }
  pthread_mutex_unlock (&m);

  return max[my_rank];
}
