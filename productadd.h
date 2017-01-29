#ifndef _INCLUDE_PRODUCTADD_
#define _INCLUDE_PRODUCTADD_


#define PROD_INFO_APPEND "PROD_INFO_APPEND"

extern int g_prodaddWindowFlag;


typedef struct _ProdaddHandleData{
  GtkWidget *addWindow;
  GtkLabel *prod_idLabel;
  GtkLabel *prod_nameLabel;
  GtkLabel *prod_smallcateLabel;
  GtkLabel *prod_priceLabel;
  GtkEntry *prod_idEntry;
  GtkEntry *prod_nameEntry;
  GtkEntry *prod_smallcateEntry;
  GtkEntry *prod_priceEntry;
  GtkButton *okButton;
  GtkButton *cancellButton;
  GtkLabel *resultLabel;
}ProdaddHandleData;


#endif
