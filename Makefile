# MPLAB IDE generated this makefile for use with GNU make.
# Project: lcd.mcp
# Date: Fri Jun 18 17:41:52 2010

AS = MPASMWIN.exe
CC = mcc18.exe
LD = mplink.exe
AR = mplib.exe
RM = rm

lcd.cof : my_xlcd.o main.o config.o actions.o
	$(LD) /l"C:\MCC18\lib" "rm18f4550.lkr" "my_xlcd.o" "main.o" "config.o" "actions.o" /u_CRUNTIME /u_DEBUG /z__MPLAB_BUILD=1 /z__MPLAB_DEBUG=1 /o"lcd.cof" /M"lcd.map" /W

my_xlcd.o : my_xlcd.c ../../MCC18/h/delays.h my_xlcd.h my_xlcd.c ../../MCC18/h/p18cxxx.h ../../MCC18/h/p18f4550.h
	$(CC) -p=18F4550 "my_xlcd.c" -fo="my_xlcd.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

main.o : main.c ../../MCC18/h/stdlib.h ../../MCC18/h/string.h ../../MCC18/h/timers.h ../../MCC18/h/delays.h my_xlcd.h actions.h main.c ../../MCC18/h/p18cxxx.h ../../MCC18/h/p18f4550.h ../../MCC18/h/stddef.h ../../MCC18/h/pconfig.h config.h const.h
	$(CC) -p=18F4550 "main.c" -fo="main.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

config.o : config.c ../../MCC18/h/timers.h ../../MCC18/h/delays.h my_xlcd.h config.c ../../MCC18/h/p18cxxx.h ../../MCC18/h/p18f4550.h ../../MCC18/h/pconfig.h const.h config.h
	$(CC) -p=18F4550 "config.c" -fo="config.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

actions.o : actions.c ../../MCC18/h/delays.h my_xlcd.h actions.h ../../MCC18/h/stdio.h ../../MCC18/h/timers.h actions.c const.h ../../MCC18/h/p18cxxx.h ../../MCC18/h/p18f4550.h ../../MCC18/h/stdarg.h ../../MCC18/h/stddef.h ../../MCC18/h/pconfig.h
	$(CC) -p=18F4550 "actions.c" -fo="actions.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

clean : 
	$(RM) "my_xlcd.o" "main.o" "config.o" "actions.o" "lcd.cof" "lcd.hex"

