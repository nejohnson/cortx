/* ctxInit.c */
#include "cortx.h"

void ctxInit(void)
{
   uint8 i;

   ctxWaitingQ = CTX_QUEUE_LENGTH;
   ctxFreeQ    = 0;

   for (i = 0; i < CTX_QUEUE_LENGTH;)
      ctxEventQ[i].next = ++i;

   for (i = 0; i != CTX_TASKID_LIST_END; i++)
      (ctxTaskTable[i])(CTX_EV_INIT);
}
