#ifndef MULTITHREAD_H
#define MULTITHREAD_H

int synchronize (int p);

double reduce_sum (double *sum, int my_rank, int p);

double reduce_max_sum (double *max, int my_rank, int p);

#endif // MULTITHREAD_H
