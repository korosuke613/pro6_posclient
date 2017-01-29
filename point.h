#ifndef _INCLUDE_POINT_
#define _INCLUDE_POINT_

#include "pos_client.h"

#define POINT_CREATE "POINT_CREATE"

extern int g_pointWindowFlag;

typedef struct _pointHandleData{
  GtkWidget  *pointWindow;           //メインウィンドウ
  GtkLabel *cardLabel;           //カードラベル
  GtkEntry *cardEntry;
  GtkLabel *nameLabel;
  GtkEntry *nameEntry;
  GtkRadioButton *menradioButton;
  GtkRadioButton *womenradioButton;
  GtkLabel *genderLabel; 
  GtkLabel *dayLabel;
  GtkEntry *dayEntry;
  GtkLabel *addLabel;
  GtkEntry *addEntry;
  GtkLabel *tellLabel;
  GtkEntry *tellEntry;
  GtkLabel *mailLabel;
  GtkEntry *mailEntry;
  GtkButton  *okButton;          //OKボタン
  GtkButton  *cancelButton;       //戻るボタン
  GtkLabel *resultLabel;
}pointHandleData;

#endif
