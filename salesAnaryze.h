/*
 * Copyright (c) 2016
 *    Yuki Taguchi.  All rights reserved.                    
 *
 * salesAnaryze.h
 *        2016.12.12 16:00
 *
 */



#include "pos_client.h"

#ifndef _INCLUDE_ANARYZE_
#define _INCLUDE_ANARYZE_

extern int g_sellWindowFlag;
extern int g_anaryzeWindowFlag;


typedef struct _AnaryzeHandleData{
  GtkWidget *anaryzewindow1;
  GtkTreeView  *treeView;
  GtkLabel *disp_label;
}AnaryzeHandleData;



typedef struct _SellHandleData{
  GtkWidget *anaryzewindow;
  GtkLabel *Showlabel;
  GtkEntry *startentry;
  GtkEntry *endentry;
  GtkRadioButton *radioBtn[3];
  GtkComboBox *Weatherbox;
  GtkComboBox *Oldminbox;
  GtkComboBox *Oldmaxbox;
  GtkEntry *Showentry;
  GtkButton *ShowButton;
  GtkButton *BackButton;
  //AnaryzeHandleData *AnaryzeHandleData;
}SellHandleData;



/*
typedef struct _UserInfo{
  gint    id;
  const gchar   *id;
  const gchar   *name;
  const gchar   *sellmass;
}UserInfo;
*/
#endif

