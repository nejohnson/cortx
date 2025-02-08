/* ctxCancelEvent.c */
#include "cortx.h"

bool ctxCancelEvent(CTX_TaskId  taskId,
                    CTX_EventId eventId)
{
   CTX_QIdx idx = _ctxWaitingQ, nidx;

   if (idx == CTX_QUEUE_END)
      return false;

   if (_ctxEventQ[idx].taskId  == taskId
   &&  _ctxEventQ[idx].eventId == eventId)
   {
      CTX_EvTime t = _ctxEventQ[idx].evTime;
      _ctxWaitingQ = _ctxEventQ[idx].next;
      _ctxEventQ[_ctxWaitingQ].evTime += t;
      
      _ctxEventQ[idx].next = _ctxFreeQ;
      _ctxFreeQ    = idx;
      return true;
   }
   
   nidx = _ctxEventQ[idx].next;
   for (; nidx != CTX_QUEUE_END;
          idx = nidx, nidx = _ctxEventQ[idx].next)
   {
      if (_ctxEventQ[nidx].taskId  == taskId
      &&  _ctxEventQ[nidx].eventId == eventId)
      {
         CTX_EvTime t = _ctxEventQ[nidx].evTime;
         _ctxEventQ[idx].next  = _ctxEventQ[nidx].next;
         _ctxEventQ[idx].evTime += t;
	 
         _ctxEventQ[nidx].next = _ctxFreeQ;
         _ctxFreeQ = nidx;
         return true;
      }
   }
   return false;
}
