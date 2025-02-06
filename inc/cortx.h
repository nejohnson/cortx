/* cortx.h */
#ifndef CORTX_H
#define CORTX_H

typedef uint16          CTX_EvTime;
#define CTX_EV_IMMEDIATE       ( 0 )

typedef uint8           CTX_Task;
typedef uint8           CTX_Event;

extern void ctxInit(void);
extern void ctxRun(void);
extern void ctxPostEvent( CTX_Task, CTX_Event, CTX_EvTime );
extern bool ctxCancelEvent( CTX_Task, CTX_Event );
extern void ctxIntPostEvent( CTX_Task, CTX_Event );

#endif /* CORTX_H */
