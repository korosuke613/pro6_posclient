/*
 * Copyright (c) 2016
 *    Yuki Taguchi.  All rights reserved.←複数人の場合は複数人の名前を並べること
 *
 * abcAnaryze.h
 * 最終更新日時：2016.12.12 16:00
 *
 */


#include "pos_client.h"

#ifndef _INCLUDE_ABC_
#define _INCLUDE_ABC_
#define ABC "ABC_DISP"

extern int g_abcWindowFlag;
extern int g_abc1WindowFlag;

typedef struct _Abc0HandleData{
  GtkWidget *abc0window;
  GtkRadioButton *noradio;
  GtkRadioButton *genderradio;
  GtkRadioButton *weatherradio;
  GtkRadioButton *ageradio;
  GtkButton *nextbutton;
  GtkButton *backbutton;
}Abc0HandleData;


typedef struct _Abc1HandleData{
  GtkWidget *weatherwindow;
  GtkEntry *startentry;
  GtkComboBox *weatherbox;
  GtkButton *showbutton;
  GtkButton *backbutton;
}Abc1HandleData;

typedef struct _Abc2HandleData{
  GtkWidget *genderwindow;
  GtkEntry *startentry;
  GtkComboBox *genderbox;
  GtkButton *showbutton;
  GtkButton *backbutton;
}Abc2HandleData;


typedef struct _Abc3HandleData{
  GtkWidget *agewindow;
  GtkEntry *startentry;
  GtkComboBox *Age1box;
  GtkComboBox *Age2box;
  GtkButton *showbutton;
  GtkButton *backbutton;
}Abc3HandleData;

typedef struct _Abc4HandleData{
  GtkWidget *daywindow;
  GtkEntry *startentry;
  GtkButton *showbutton;
  GtkButton *backbutton;
}Abc4HandleData;

typedef struct _Abc5HandleData{
  GtkWidget *abcwindow1;
  GtkTreeView  *treeView;
  GtkLabel    *disp_Label;
}Abc5HandleData;






#endif

