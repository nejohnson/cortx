#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "ctxAPI.h"

CTX_EvTime _ctxSysDelay(CTX_EvTime t)
{
   sleep (1);
   return 1;
}

void _ctxSysAbort(void)
{
   printf("!! SYSTEM ABORTING !!\n");
   exit(1);
}

void Button_Task(CTX_EventId id)
{
   switch (id)
   {
case CTX_EV_INIT:
      printf("%s: INIT\n", __func__);
      ctxPostEvent(Button_Task_ID, 1, 1);
      break;
      
case 1:
      printf("%s: TICK\n", __func__);
      ctxPostEvent(Button_Task_ID, 2, 1);
      break;

case 2:
      printf("%s: TOCK\n", __func__);
      ctxPostEvent(Button_Task_ID, 1, 1);
      ctxPostEvent(UI_Task_ID, 1, CTX_EV_IMMEDIATE);
      break;
   }
}

void UI_Task(CTX_EventId id)
{
   switch (id)
   {
case CTX_EV_INIT:
      printf("%s: INIT\n", __func__);
      break;
      
case 1:
      printf("%s: BOOP\n", __func__);
      break;
   }
}

void Display_Task(CTX_EventId id) {}
void Serial_Task(CTX_EventId id) {}

int main(int argc, char *argv[])
{
   ctxInit();
   ctxRun();   

   return 0;
}
