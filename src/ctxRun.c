/* ctxRun.c */
#include "cortx.h"

void ctxRun(void)
{
   while(1)
   {
      CTX_Time ttne = _ctxProcessNextEvent();

      if (ttne)
          _ctxWait(ttne);
    }
}
