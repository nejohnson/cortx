/* ctxRun.c */
#include "cortx.h"

void ctxRun(void)
{
   while(1)
      _ctxProcessNextEvent();
}
