#include "pos_client.h"

#ifndef _INCLUDE_PURCHASE_
#define _INCLUDE_PURCHASE_

extern int g_addWindowFlag;

#define PURCHASE_DISP  "PURCHASE_DISP"

typedef struct _addHandleData{
  GtkWidget *add1Window;   //ウィンドウ
  GtkEntry  *storeIdEntry;     //店舗ID
  GtkButton  *add1Button;     //ボタン
  GtkButton  *okButton;     //ボタン
  GtkButton  *cancel1Button;     //ボタン
  GtkLabel *result1Label;
  GtkTreeView *addTree;
  MainHandleData  *mainHandleData;  // メイン画面の構造体ポインタ

}addHandleData;

#endif
