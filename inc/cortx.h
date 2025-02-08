/* cortx.h */
#ifndef CORTX_H
#define CORTX_H

#include "ctxAPI.h"

#define CTX_QUEUE_LENGTH    ( 3 )
#define CTX_QUEUE_END       ( CTX_QUEUE_LENGTH )

typedef uint8_t CTX_QIdx;

typedef struct {
   CTX_QIdx     next;
   CTX_EvTime   evTime;
   CTX_EventId  eventId;
   CTX_TaskId   taskId;
} CTX_Event_T;

extern CTX_Task    _ctxTaskTable[];
extern CTX_Event_T _ctxEventQ[];
extern CTX_QIdx    _ctxWaitingQ;
extern CTX_QIdx    _ctxFreeQ;

extern void        _ctxProcessNextEvent(void);
extern void        _ctxWait(CTX_EvTime);
extern CTX_EvTime  _ctxSysDelay(CTX_EvTime);
extern void        _ctxSysAbort(void);

#endif /* CORTX_H */
