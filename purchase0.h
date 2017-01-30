#include "pos_client.h"

#ifndef _INCLUDE_PURCHASE_
#define _INCLUDE_PURCHASE_

extern int g_addWindowFlag;


typedef struct _addHandleData{
  GtkWidget *pur0Window;   //ウィンドウ 
  GtkButton  *purinfoButton;     //ボタン
  GtkButton  *proinfoButton;     //ボタン
  GtkButton  *purButton;     //ボタン
  GtkButton  *backButton;     //ボタン
  MainHandleData  *mainHandleData;  // メイン画面の構造体ポインタ

}addHandleData;

#endif
