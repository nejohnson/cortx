/* cortx.c */
#include "cortx.h"

int8 ctxCancelEvent(CTX_TaskId  taskId,
                    CTX_EventId eventId)
{
   CTX_QIdx idx = CtxWaitingQ, nidx;

   if (idx == QUEUE_END)
      return 0;

   if (CtxEventQ[idx].taskId  == taskId
   &&  CtxEventQ[idx].eventId == eventId)
   {
      CtxWaitingQ         = CtxEventQ[idx].next;
      CtxEventQ[idx].next = CtxFreeQ;
      CtxFreeQ            = idx;
      return -1;
   }
   
   nidx = CtxEventQ[idx].next;
   for (; nidx != CTX_QUEUE_END;
          idx = nidx, nidx = CtxEventQ[idx].next)
   {
      if (CtxEventQ[nidx].taskId  == taskId
      &&  CtxEventQ[nidx].eventId == eventId)
      {
         CtxEventQ[idx].next  = CtxEventQ[nidx].next;
         CtxEventQ[nidx].next = CtxFreeQ;
         CtxFreeQ = nidx;
         return -1;
      }
   }
   return 0;
}
