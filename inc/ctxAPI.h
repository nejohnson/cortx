/* ctxAPI.h */
#ifndef CTXAPI_H
#define CTXAPI_H

#include <stdint.h>
#include <stdbool.h>

typedef uint16_t          CTX_EvTime;
#define CTX_EV_IMMEDIATE  ( 0 )

typedef uint8_t           CTX_TaskId;
typedef uint8_t           CTX_EventId;
#define CTX_EV_INIT       ( 0 )

typedef void(*CTX_Task)(CTX_EventId);

extern void ctxInit(void);
extern void ctxRun(void);
extern bool ctxPostEvent(CTX_TaskId,
                         CTX_EventId,
                         CTX_EvTime);
extern bool ctxCancelEvent(CTX_TaskId,
                           CTX_EventId);

#define CTX_USERTASK(t)	extern void t(CTX_EventId);
#include "ctxUserTaskList.inc"
#undef CTX_USERTASK

#define CTX_USERTASK(t)	t ## _ID ,
enum {
#include "ctxUserTaskList.inc"
   CTX_TASKID_LIST_END
};
#undef CTX_USERTASK

#endif /* CTXAPI_H */
