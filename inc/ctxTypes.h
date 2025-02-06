/* ctxTypes.h */
#ifndef CTXTYPES_H
#define CTXTYPES_H

typedef uint16          CTX_EvTime;
#define CTX_EV_IMMEDIATE       ( 0 )

typedef uint8           CTX_QIdx;
typedef uint8           CTX_TaskId;
typedef uint8           CTX_EventId;

typedef void(*)(CTX_EventId) CTX_Task;

#endif /* CTXTYPES_H */
