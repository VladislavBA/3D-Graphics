#ifndef MULTITHREAD_H
#define MULTITHREAD_H

#include "pthread.h"

int synchronize (int total_thread);

template <typename T>
T reduce_max (T &sum, const int total_thread);

template <typename T>
T reduce_sum (T &sum, const int total_thread);

template <typename T>
T reduce_sum (T &sum, const int total_thread)
{
    static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t c_in = PTHREAD_COND_INITIALIZER;
    static pthread_cond_t c_out = PTHREAD_COND_INITIALIZER;
    static int t_in = 0;
    static int t_out = 0;
    static T result_sum = 0.;

    pthread_mutex_lock (&m);

    if (t_in == 0)
        result_sum = sum;
    else
        result_sum += sum;

    t_in++;

    if (t_in >= total_thread)
      {
        t_out = 0;
        pthread_cond_broadcast (&c_in);
      }
    else
      {
        while (t_in < total_thread)
          pthread_cond_wait (&c_in,&m);
      }

    sum = result_sum;

    t_out++;
    if (t_out >= total_thread)
      {
        t_in = 0;
        pthread_cond_broadcast (&c_out);
      }
    else
      {
        while (t_out < total_thread)
          pthread_cond_wait (&c_out,&m);
      }
    pthread_mutex_unlock (&m);

    return sum;
}

template <typename T>
T reduce_max (T &max, const int total_thread)
{
    static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t c_in = PTHREAD_COND_INITIALIZER;
    static pthread_cond_t c_out = PTHREAD_COND_INITIALIZER;
    static int t_in = 0;
    static int t_out = 0;
    static T result_max = 0.;

    pthread_mutex_lock (&m);

    if (t_in == 0)
        result_max = max;
    else
        if (result_max < max)
            result_max = max;

    t_in++;

    if (t_in >= total_thread)
      {
        t_out = 0;
        pthread_cond_broadcast (&c_in);
      }
    else
      {
        while (t_in < total_thread)
          pthread_cond_wait (&c_in,&m);
      }

    max = result_max;

    t_out++;
    if (t_out >= total_thread)
      {
        t_in = 0;
        pthread_cond_broadcast (&c_out);
      }
    else
      {
        while (t_out < total_thread)
          pthread_cond_wait (&c_out,&m);
      }
    pthread_mutex_unlock (&m);

    return max;
}
#endif // MULTITHREAD_H
