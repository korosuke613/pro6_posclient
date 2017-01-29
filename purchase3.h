#include "pos_client.h"

#ifndef _INCLUDE_PURCHASE_
#define _INCLUDE_PURCHASE_

extern int g_add3WindowFlag;


#define PURCHASE "PURCHASE"


typedef struct _add3HandleData{
  GtkWidget *add3Window;   //ウィンドウ
  GtkEntry  *storeidEntry;     //店舗ID
  GtkEntry  *proidEntry;     //店舗ID
  GtkEntry  *numEntry;     //店舗ID
  GtkEntry  *limEntry;     //店舗ID
  GtkButton  *add3Button;     //OKボタン
  GtkButton  *endButton;     //戻るボタン
  GtkButton  *cancel3Button;     //ボタン
  GtkLabel	*resultLabel;
  MainHandleData  *mainHandleData;  // メイン画面の構造体ポインタ

}add3HandleData;

#endif
