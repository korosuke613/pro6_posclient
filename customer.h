#ifndef _INCLUDE_CUSTOMER_
#define _INCLUDE_CUSTOMER_

//担当: 平松 パート 二
#include "pos_client.h"

#define USER_LIST_DISP "USER_LIST_DISP" //顧客検索結果表示コマンド

//externの部分に追加
extern int g_customerManageWindowFlag; //顧客管理画面表示フラグ　1:表示 0:非表示
extern int g_customerShowWindowFlag; //検索結果画面表示フラグ 1:表示 0:非表示

extern void show_error_message(GtkLabel *errorLabel, int errorcode);

//以下構造体

//顧客管理画面のwidget
typedef struct _customerHandleData{
  GtkWidget *customerWindow;   //ウィンドウ
  GtkEntry  *startEntry;     //検索開始範囲
  GtkSpinButton  *numberSpinButton; //表示件数
  GtkLabel *resultLabel;//実行結果表示用のラベル
}CustomerHandleData; 


//検索結果表示画面のwidget
typedef struct _customerShowHandleData{
  GtkWidget *customerShowWindow; //ウィンドウ
  GtkTreeView *customerShowTreeView;
  GtkButton *customerShowFinishButton; //検索結果表示終了ボタン
}CustomerShowHandleData; 


#endif
