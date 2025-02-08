
INCLUDE=-I. -I../../inc
CTX_SRC = ../ctxData.c \
          ../ctxInit.c ../ctxRun.c \
          ../ctxProcEv.c  ../ctxWait.c \
	  ../ctxCancelEvent.c  ../ctxPostEvent.c
SRC = main.c 

testapp: $(CTX_SRC) $(SRC)
	$(CC) $(INCLUDE) -g $^ -o $@
