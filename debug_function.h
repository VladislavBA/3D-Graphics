#ifndef DEBUG_FUNCTION_H
#define DEBUG_FUNCTION_H
#include "stdio.h"


template<typename T>
void print_array (T *vector, int len)
{
  for (int i = 0; i < len; i++)
    {
      if (i % 3 == 0)
        printf("}\n{");
      printf("%d = %g|", i, vector[i]);
    }

  printf("\n");
}
template<typename E>
void print_array (E *vector, int len, int key)
{
  for (int i = 0; i < len; i++)
    {
      if (i % key == 0)
        printf("}\n{");
      printf("%d = %d|", i, vector[i]);
    }

  printf("\n");
}

#endif // DEBUG_FUNCTION_H
