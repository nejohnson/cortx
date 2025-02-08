/* ctxWait.c */
#include "cortx.h"

void _ctxWait(CTX_EvTime t)
{
   while (t)
   {
     CTX_EvTime n = _ctxSysDelay(t);
     if (n >= t)
        t -= n;
     else
        t = 0;
   }
}
