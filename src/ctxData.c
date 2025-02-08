/* ctxData.c */
#include "cortx.h"

#define CTX_USERTASK(t)	t ,
CTX_Task _ctxTaskTable[] = {
#include "ctxUserTaskList.inc"
};
#undef CTX_USERTASK

CTX_Event_T _ctxEventQ[CTX_QUEUE_LENGTH];
CTX_QIdx    _ctxWaitingQ;
CTX_QIdx    _ctxFreeQ;
