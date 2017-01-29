/*
 * Copyright (c) 2016
 *
 *    Yuto Sumiyoshi.  All rights reserved.
 *
 * purchase.c
 * 最終更新日時：2016.12.12
 *
 */

#include "pos_client.h"
#include "purchase3.h"

int g_add3WindowFlag;
void showErrorMsg5(GtkLabel *resultLabel, int errorCode);

G_MODULE_EXPORT void cb_add3_win_open(GtkButton *button, gpointer data){
  GtkBuilder      *builder;
  add3HandleData *add3hData; //ログイン画面の主要Widget保持用

  if(g_add3WindowFlag ==0){

  /* ログイン画面の主要Widget保持用構造体を作成 */
  add3hData = (add3HandleData *)malloc(sizeof(add3HandleData));

  /* login.gladeを読み込む */
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "purchase3.glade", NULL);
  /* 主要Widgetを保持 */
  add3hData->add3Window     = GTK_WIDGET( gtk_builder_get_object(builder, "add3Window") );
  add3hData->storeidEntry    = GTK_ENTRY( gtk_builder_get_object(builder, "storeidEntry") );
  add3hData->proidEntry    = GTK_ENTRY( gtk_builder_get_object(builder, "proidEntry") );  
  add3hData->numEntry    = GTK_ENTRY( gtk_builder_get_object(builder, "numEntry") );
  add3hData->limEntry    = GTK_ENTRY( gtk_builder_get_object(builder, "limEntry") );
  add3hData->add3Button    = GTK_BUTTON( gtk_builder_get_object(builder, "add3Button") );
    add3hData->resultLabel    = GTK_LABEL( gtk_builder_get_object(builder, "resultLabel"));
  /* シグナル、シグナルハンドラ、ユーザデータ登録 */
  gtk_builder_connect_signals(builder, add3hData);
  /* ログイン画面表示 */
  gtk_widget_show_all(add3hData->add3Window);

  g_add3WindowFlag = 1; 
 }
}


G_MODULE_EXPORT void cb_add2_cancel(GtkButton *button, gpointer data){
  add3HandleData *add3hData; //       Widget   

  add3hData = (add3HandleData *)data;
  /*                 */
  gtk_widget_hide(add3hData->add3Window);
  /*       Widget              ) */
  free(add3hData);
  /*               */
  g_add3WindowFlag = 0;

}


G_MODULE_EXPORT void cb_purchase_exec(GtkButton *button, gpointer data){
  add3HandleData *add3hData;       //メイン画面の主要Widget保持用
  const char *storeIdStr,*proIdStr,*numStr,*limStr;
  int gender,recordCount,sendLen,recvLen;
  char *records[RECORD_MAX];
  char sendBuf[BUFSIZE],recvBuf[BUFSIZE_MAX];
  char response[BUFSIZE],param1[BUFSIZE];



  add3hData = (add3HandleData *)data;

  storeIdStr = gtk_entry_get_text(add3hData->storeidEntry);
  if(strlen(storeIdStr) < 1){
    gtk_label_set_text(add3hData->resultLabel,"ERROR:店舗IDを入力してください");
    return;
  }

  proIdStr = gtk_entry_get_text(add3hData->proidEntry);
  if(strlen(proIdStr) < 1){
    gtk_label_set_text(add3hData->resultLabel,"ERROR:商品IDを入力してください");
    return;
  }


 numStr = gtk_entry_get_text(add3hData->numEntry);
  if(strlen(numStr) < 1){
    gtk_label_set_text(add3hData->resultLabel,"ERROR:仕入れ数量を入力してください");
    return;
  }

 limStr = gtk_entry_get_text(add3hData->limEntry);
  if(strlen(limStr) < 1){
    gtk_label_set_text(add3hData->resultLabel,"ERROR:消費賞味期限日を入力してください");
    return;
  }



  if(g_soc > 0){
    sendLen = sprintf(sendBuf, "%s %s %s %s %s %s",PURCHASE,storeIdStr,proIdStr,numStr,limStr,ENTER );
    send(g_soc, sendBuf, sendLen, 0);
    recvLen = recv_data(g_soc, recvBuf,BUFSIZE_MAX);
    record_division(recvBuf,records);

   sscanf(records[0], "%s %s", response, param1);
    if(strcmp(response, OK_STAT) != 0){
      showErrorMsg5(add3hData->resultLabel, atoi(param1));
      return;
    }

    memset(response, 0, BUFSIZE);
    memset(param1, 0, BUFSIZE);


  

    gtk_label_set_text(add3hData->resultLabel, records[0]);
  

  }

  return;

}


void showErrorMsg5(GtkLabel *resultLabel, int errorCode){

  switch(errorCode){
  default:
    gtk_label_set_text(resultLabel, "致命的なエラーが発生しました");
    break;

  }

}
