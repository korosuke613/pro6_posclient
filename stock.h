#include "pos_client.h"

#ifndef _INCLUDE_STOCK_
#define _INCLUDE_STOCK_

#define STOCK_DISP "STOCK_DISP"

extern int g_stockWindowFlag;

typedef struct _stockHandleData{
  GtkWidget *manageWindow;   //ウィンドウ
  GtkEntry  *idEntry;     //店舗ID
  GtkButton  *manageButton;     //表示ボタン
  GtkButton  *cancelButton;     //戻るボタン
  GtkLabel *resultLabel;  //取扱商品件数
  GtkTreeView *stockTree; //ツリービュー
  MainHandleData  *mainHandleData;  // メイン画面の構造体ポインタ

}stockHandleData;


#endif
