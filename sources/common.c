#include "common.h"

#include <stdlib.h>
#include <string.h>

void* custom_memdup(const void *src, size_t size)
{
  void *res;

  if (NULL == src)
  {
    return NULL;
  }

  res = malloc(size);

  if (NULL == res)
  {
    return NULL;
  }

  memcpy(res, src, size);

  return res;
}

