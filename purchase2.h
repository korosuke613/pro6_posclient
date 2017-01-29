#include "pos_client.h"

#ifndef _INCLUDE_PURCHASE_
#define _INCLUDE_PURCHASE_

extern int g_add2WindowFlag;

#define PURCHASE_DT_DISP "PURCHASE_DT_DISP"


typedef struct _add2HandleData{
  GtkWidget *add2Window;   //ウィンドウ
  GtkEntry  *idEntry;     //店舗ID
  GtkButton  *add2Button;     //OKボタン
  GtkButton  *next2Button;     //戻るボタン
  GtkButton  *cancel2Button;     //ボタン
  GtkTreeView *add2Tree;
  GtkLabel  *resultLabel; 
  MainHandleData  *mainHandleData;  // メイン画面の構造体ポインタ

}add2HandleData;

#endif
