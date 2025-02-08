/* ctxInit.c */
#include "cortx.h"

void ctxInit(void)
{
   _ctxWaitingQ = CTX_QUEUE_LENGTH;
   _ctxFreeQ    = 0;

   for (CTX_QIdx i = 0; i < CTX_QUEUE_LENGTH;i++)
      _ctxEventQ[i].next = i+1;
    
   for (uint8_t i = 0; i != CTX_TASKID_LIST_END; i++)
      (_ctxTaskTable[i])(CTX_EV_INIT);
}
