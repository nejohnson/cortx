/* ctxProcEv.c */
#include "cortx.h"

void _ctxProcessNextEvent(void)
{
   CTX_EventId eventId;
   CTX_TaskId  taskId;
   CTX_EvTime  evTime;

   if (_ctxWaitingQ == CTX_QUEUE_END)
   {
      _ctxSysAbort();
      return;
   }

   if (evTime = _ctxEventQ[_ctxWaitingQ].evTime)
      _ctxWait(evTime);

   eventId = _ctxEventQ[_ctxWaitingQ].eventId;
   taskId  = _ctxEventQ[_ctxWaitingQ].taskId;

   ctxCancelEvent(taskId, eventId);

   (_ctxTaskTable[taskId])(eventId);
}
