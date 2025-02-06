/* cortx.c */
#include "cortx.h"

int8 ctxPostEvent(CTX_EvTime  evTime,
                  CTX_TaskId  taskId,
                  CTX_EventId eventId)
{
   CTX_QIdx newIdx = ctxFreeQ;
    
   if (newIdx == CTX_QUEUE_END)
      return -1;

   ctxFreeQ = ctxEventQ[newIdx].next;

   ctxEventQ[newIdx].eventId = eventId;
   ctxEventQ[newIdx].taskId  = taskId;
   
   if (CtxEventQ[CtxWaitingQ].evTime > evTime)
   {
      CtxEventQ[CtxWaitingQ].evTime -= evTime;
      CtxEventQ[newIdx].next         = CtxWaitingQ;
      CtxWaitingQ                    = newIdx;
      return 0;
   }
   
   for (CTX_QIdx idx = ctxWaitingQ;;)
   {
      CTX_QIdx next = ctxEventQ[idx].next;
      
      TODO!!!
      
      if (next == CTX_QUEUE_END
      ||  evTime < ctxEventQ[next].evTime)
      {
	 ctxEventQ[idx].next    = newIdx;
	 ctxEventQ[newIdx].next = next;
	 ctxEventQ[idx].evTime  = evTime;
         return 0;
      }
      else
      {
         evTime -= ctxEventQ[next].evTime
         idx = next;
      }
   }
   
   return 0;
}
