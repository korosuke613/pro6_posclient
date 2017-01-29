##
## POS client Makefile
##
CC		= gcc
CFLAGS		= `pkg-config --cflags gtk+-2.0`
LIBS		= `pkg-config --libs gtk+-2.0 gmodule-export-2.0`
SRCS		= $(I_SALES_C) $(RO_STOCK_C) $(RO_PURCHASE_C) $(HA_PRODUCT_C) $(HA_POINT_C) $(NI_CUSTOMER_C) $(NI_SALESTOTAL_C) $(HO_SALESANARYZE_C) $(HO_ABCANARYZE_C) pos_main.c login.c common_func.c setup_connect.c info.c
HDRS            = $(I_SALES_H) $(RO_STOCK_H) $(RO_PURCHASE_H) $(HA_PRODUCT_H) $(HA_POINT_H) $(NI_CUSTOMER_H) $(NI_SALESTOTAL_H) $(HO_SALESANARYZE_H) $(HO_ABCANARYZE_H) pos_client.h
OBJS		= $(SRCS:.c=.o)
PROGRAM		= pos

I_SALES_C	:= sales.c
I_SALES_H	:= sales.h

RO_STOCK_C	:= stock.c
RO_STOCK_H	:= stock.h
RO_PURCHASE_C	:= purchase.c purchase2.c purchase3.c
RO_PURCHASE_H	:= purchase.h purchase2.h purchase3.h

HA_PRODUCT_C	:= product.c productadd.c productinfo.c
HA_PRODUCT_H	:= product.h productadd.h productinfo.h
HA_POINT_C	:= point.c
HA_POINT_H	:= point.h

NI_CUSTOMER_C	:= customer.c
NI_CUSTOMER_H	:= customer.h
NI_SALESTOTAL_C	:= salesTotal.c
NI_SALESTOTAL_h	:= salesTotal.h

HO_SALESANARYZE_C	:= salesAnaryze.c
HO_SALESANARYZE_H	:= salesAnaryze.h
HO_ABCANARYZE_C	:= abcAnaryze.c
HO_ABCANARYZE_H	:= abcAnaryze.h

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS) $(HDRS) 
		$(CC) $(OBJS) $(LIBS) -o $(PROGRAM)

clean:
		rm -f *.o *~ $(PROGRAM)

###
