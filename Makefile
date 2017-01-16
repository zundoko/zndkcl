#
# @file     Makefile
# @brief    Makefile for zndkcl
#
# @note     https://github.com/zundoko/zndkcmdline
#
# @date     2017-01-16
# @authr    zundoko
#

PRJNAME =   zndkcl

TARGET  = $(PRJNAME)
SRCS    = $(TARGET:%=%.c)
OBJS    = $(SRCS:%.c=%.o)
DEPEND  =   Makefile.depend

CC      = $(CROSS_COMPILE)gcc
INC     =  -I.
CFLAGS  = $(INC) -c -Wall -Werror
LDFLAGS = 
LIBS    = 

.PHONY: all
all: $(TARGET)

.PHONY: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $< $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $<

.PHONY: clean
clean:
	-rm -rf $(OBJS) $(TARGET) *~

.PHONY: depend
depend:
	-rm -rf $(DEPEND)
	$(CC) -MM -MG $(CFLAGS) $(SRCS) > $(DEPEND)

-include  $(DEPEND)

# end
