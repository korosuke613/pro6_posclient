#ifndef _INCLUDE_PRODUCTINFO_
#define _INCLUDE_PRODUCTINFO_
#include "pos_client.h"


#define CATE_DISP "CATE_DISP"
#define CATE_DETAIL_DISP "CATE_DETAIL_DISP"
#define CATE_INFO_DISP "CATE_INFO_DISP"
#define PROD_INFO_DISP "PROD_INFO_DISP"


extern int g_prodinfoWindowFlag;


typedef struct _ProdinfoHandleData{
  GtkWidget *cateWindow;
  GtkLabel *bigcateLabel;
  GtkComboBox *bcateCombobox;
  GtkLabel *smallcateLabel;
  GtkComboBox *scateCombobox;
  GtkLabel *cateLabel;
  GtkComboBox *cateCombobox;
  GtkTreeView *pro_maneTreeview;
  GtkButton *cancellButton;
  GtkLabel *resultLabel;
}ProdinfoHandleData;


#endif
