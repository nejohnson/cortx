/* ctxPostEvent.c */
#include "cortx.h"

bool ctxPostEvent(CTX_TaskId  taskId,
                  CTX_EventId eventId,
                  CTX_EvTime  evTime)
{
   CTX_QIdx ev = _ctxFreeQ;

   if (ev == CTX_QUEUE_END)
      return false;
   _ctxFreeQ = _ctxEventQ[ev].next;
   _ctxEventQ[ev].taskId  = taskId;
   _ctxEventQ[ev].eventId = eventId;

   if (_ctxWaitingQ == CTX_QUEUE_END
   ||  evTime < _ctxEventQ[_ctxWaitingQ].evTime)
   {
      _ctxEventQ[ev].next   = _ctxWaitingQ;
      _ctxEventQ[ev].evTime = evTime;
      _ctxWaitingQ          = ev;
   }
   else
   {
      CTX_QIdx idx  = _ctxWaitingQ;
      CTX_QIdx next = _ctxEventQ[idx].next;   
      while(1)
      {
         evTime -= _ctxEventQ[idx].evTime;
         if (next == CTX_QUEUE_END
         ||  evTime < _ctxEventQ[next].evTime)
         {
            _ctxEventQ[ev].next   = next;
            _ctxEventQ[ev].evTime = evTime;
            _ctxEventQ[idx].next  = ev;
            break;
         }
         idx  = _ctxEventQ[idx].next;
         next = _ctxEventQ[idx].next;
      }
   }
   if (_ctxEventQ[ev].next != CTX_QUEUE_END)
      _ctxEventQ[_ctxEventQ[ev].next].evTime -= evTime;
   return true;
}
