/* cortx.c */

#include "cortx.h"

/*****************************************************************************
 **
 **     Private Defines
 **
 *****************************************************************************/

#define QUEUE_LENGTH    ( 13 )  /* Maximum 15 */
#define QUEUE_END       ( QUEUE_LENGTH )

/*****************************************************************************
 **
 **     Private Data Types
 **
 *****************************************************************************/

typedef struct {
    uint16 time;
    uint8  event;
    uint8  next:4;
    uint8  taskId:4;
} CtxEvent_T;

/*****************************************************************************
 **
 **     Private Data
 **
 *****************************************************************************/

/*
 * List of task handlers
 */
 
static void (* TaskTable[])( T_Event ) = {
    Button_Task,
    UI_Task,
    Display_Task,
    MIDI_Task
};

static T_EventRec EventQ[QUEUE_LENGTH];
static uint8 FreeQ;
static uint8 WaitingQ;

/*****************************************************************************
 **
 **     Private Function Prototypes
 **
 *****************************************************************************/

static T_EvTime ProcessNextEvent( void );
static void     Wait( T_EvTime time );

/*****************************************************************************
 **
 **     Public Functions
 **
 *****************************************************************************/

/*===========================================================================*
 *
 * Function
 *    ctxInit
 *   
 * Synopsis
 *    void ctxInit( void )
 *
 * Description
 *    Initialise scheduler
 *
 * Returns
 *    none
 *
 *===========================================================================*/

void ctxInit( void )
{
    uint8 i, j;

    WaitingQ = QUEUE_END;
    FreeQ = 0;

    for ( j = 0, i = 1; i < ( QUEUE_END + 1 ); i++, j++)
    {
        EventQ[j].Next = i;
    }
}

/*===========================================================================*
 *
 * Function
 *    ctxRun
 *   
 * Synopsis
 *    void ctxRun( void )
 *
 * Description
 *    Main scheduler loop - never quits.
 *
 * Returns
 *    none
 *
 *===========================================================================*/
 
void ctxRun( void )
{
    T_EvTime    TimeToNextEvent;

    while ( WaitingQ != QUEUE_END )
    {
        TimeToNextEvent = ProcessNextEvent();

        if ( TimeToNextEvent )
        {
            uint8 i;
            
            /* Need to wait until time of next event */
            Wait( TimeToNextEvent );

            /* Run through all events subtracting time just waited */
            for ( i = WaitingQ; i != QUEUE_END; i = EventQ[i].Next )
                EventQ[i].Time -= TimeToNextEvent;
        }
    }
}

/*===========================================================================*
 *
 * Function
 *    ctxPostEvent
 *   
 * Synopsis
 *    void ctxPostEvent( T_EvTime Time, T_Task Task, T_Event Event )
 *
 * Description
 *    Posts a new message into the message queue.  If no slots free reset proc.
 *
 * Returns
 *    void
 *
 *===========================================================================*/

void ctxPostEvent( T_EvTime Time, T_Task Task, T_Event Event )
{
    uint8 newidx;
    uint8 idx;
    
    /* Check if any free slots */
    if ( FreeQ == QUEUE_END )
    {
        /* None, so reset */
        ( (void (*)(void) ) 0x0000 )();
    }

    /* Get new slot from free queue and put in values */
    newidx = FreeQ;
    FreeQ = EventQ[FreeQ].Next;

    EventQ[newidx].Time  = Time;
    EventQ[newidx].Task  = Task;
    EventQ[newidx].Event = Event;

    /* Now find time-ordered place for event in waiting queue */

    /* See if it goes at the front */
    if ( EventQ[WaitingQ].Time > Time )
    {
        /* Yes, so insert and return */
        EventQ[newidx].Next = WaitingQ;
        WaitingQ = newidx;
        return;
    }

    /* No, so scan through queue */
    idx = WaitingQ;
    for (;;)
    {
        uint8 nextidx = EventQ[idx].Next;

        if ( nextidx == QUEUE_END ||
             EventQ[nextidx].Time > Time )
        {
            /* Place found, insert queue item */
            EventQ[newidx].Next = nextidx;
            EventQ[idx].Next = newidx;
            return;
        }

        idx = nextidx;  /* Move on to next one */
    }
}

/*===========================================================================*
 *
 * Function
 *    ctxCancelEvent
 *   
 * Synopsis
 *    bool ctxCancelEvent( T_Task Task, T_Event Event )
 *
 * Description
 *    Cancels the specified event.  Returns TRUE if event found, else FALSE.
 *
 * Returns
 *    TRUE - event found and removed.
 *    FALSE - no event.
 *
 *===========================================================================*/

bool ctxCancelEvent( T_Task Task, T_Event Event )
{
    uint8   idx;
    uint8   nidx;

    idx = WaitingQ;

    if ( idx == QUEUE_END )     /* Safety check */
    {
        return FALSE;
    }

    /* Check first event */
    if ( EventQ[idx].Task  == Task &&
         EventQ[idx].Event == Event )
    {
        WaitingQ = EventQ[idx].Next;
        EventQ[idx].Next = FreeQ;
        FreeQ = idx;
        return TRUE;
    }
    else
    {
        /* No match with first event, so scan rest of queue */
        nidx = EventQ[idx].Next;
        for ( ; nidx != QUEUE_END; idx = nidx, nidx = EventQ[idx].Next)
        {
            if ( EventQ[nidx].Task  == Task &&
                 EventQ[nidx].Event == Event )
            {
                /* Found match, remove from queue */
                EventQ[idx].Next = EventQ[nidx].Next;
                EventQ[nidx].Next = FreeQ;
                FreeQ = nidx;
                return TRUE;
            }
        }
        return FALSE;
    }
}

/*****************************************************************************
 **
 **     Private Functions
 **
 *****************************************************************************/

/*===========================================================================*
 *
 * Function
 *    Wait
 *   
 * Synopsis
 *    void Wait( T_EvTime time )
 *
 * Description
 *    Waits for the specified period of time.
 *
 * Returns
 *    none
 *
 *===========================================================================*/

static void Wait( T_EvTime time )
{
    T_EvTime n;

    while ( time )
    {
        n = 65;     /* longest timer can wait is 65ms */
        if ( time < 66 )
            n = time;

        time -= n;
            
        n *= 1000;  /* convert into microseconds */

        n = ~n;     /* invert because counter counts UP */

        TR0 = LOW;  /* stop timer */
        TL0 = ( n & 0xFF );
        TH0 = ( ( n >> 8 ) & 0xFF );

        TF0 = LOW;      /* clear timer overflow flag */
        TR0 = HIGH;     /* start the timer */
        
        while( TF0 == LOW );  /* wait for timer to timeout */
    }
}

/*===========================================================================*
 *
 * Function
 *    ProcessNextEvent
 *   
 * Synopsis
 *    T_EvTime ProcessNextEvent( void )
 *
 * Description
 *    Processes next immediate message, otherwise returns time to next message.
 *
 * Returns
 *    Time to next waiting event, or zero if event processed.
 *
 *===========================================================================*/

static T_EvTime ProcessNextEvent( void )
{
    T_Event     Event;
    T_Task      Task;

    if ( EventQ[WaitingQ].Time != 0 )
    {
        return EventQ[WaitingQ].Time;
    }

    Event = EventQ[WaitingQ].Event;
    Task  = EventQ[WaitingQ].Task;

    Sched_CancelEvent( Task, Event );   /* Remove event from queue */

    (TaskTable[Task])( Event );     /* Execute event handler */

    return 0;    
}

/*****************************************************************************
 **********************************  END  ************************************
 *****************************************************************************/

