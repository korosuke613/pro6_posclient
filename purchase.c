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
#include "purchase.h"

int g_addWindowFlag;

void showErrorMsg6(GtkLabel *resultLabel, int errorCode);



G_MODULE_EXPORT void cb_add_win_open(GtkButton *button, gpointer data){
  GtkBuilder      *builder;
  addHandleData *addhData; //ログイン画面の主要Widget保持用
  if(g_addWindowFlag ==0){
  /* ログイン画面の主要Widget保持用構造体を作成 */
  addhData = (addHandleData *)malloc(sizeof(addHandleData));

  /* login.gladeを読み込む */
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "purchase.glade", NULL);
  /* 主要Widgetを保持 */
  addhData->add1Window     = GTK_WIDGET( gtk_builder_get_object(builder, "add1Window") );
  addhData->storeIdEntry    = GTK_ENTRY( gtk_builder_get_object(builder, "storeIdEntry") );
 addhData->result1Label    = GTK_LABEL( gtk_builder_get_object(builder, "result1Label") );
 addhData->add1Button    = GTK_BUTTON( gtk_builder_get_object(builder, "add1Button"));
 addhData->cancel1Button    = GTK_BUTTON( gtk_builder_get_object(builder, "cancel1Button"));
 addhData->okButton    = GTK_BUTTON( gtk_builder_get_object(builder, "okButton"));
 addhData->addTree    = GTK_TREE_VIEW( gtk_builder_get_object(builder, "addTree") );
  /* シグナル、シグナルハンドラ、ユーザデータ登録 */
  gtk_builder_connect_signals(builder, addhData);
  /* ログイン画面表示 */
  gtk_widget_show_all(addhData->add1Window);

  g_addWindowFlag = 1; 
 }
}


G_MODULE_EXPORT void cb_add_cancel(GtkButton *button, gpointer data){
  addHandleData *hData; //       Widget   

  hData = (addHandleData *)data;
  /*                 */
  gtk_widget_hide(hData->add1Window);
  /*       Widget              ) */
  free(hData);
  /*               */
  g_addWindowFlag = 0;

}


G_MODULE_EXPORT void cb_pur_exec(GtkButton *button, gpointer data){
  addHandleData *addhData;
  const char        *addStr;
  GtkTreeModel     *model;          //ツリーモデル
  GtkTreeIter      iter;            //行
  char              sendBuf[BUFSIZE], recvBuf[BUFSIZE_MAX],*records[RECORD_MAX],param1[BUFSIZE],param2[BUFSIZE], param3[BUFSIZE],param4[BUFSIZE], param5[BUFSIZE],response[BUFSIZE];
  int              i,sendLen, recvLen;

  addhData = (addHandleData *)data;

 model = gtk_tree_view_get_model(addhData->addTree);
  /* ツリービューを空にする */
  gtk_list_store_clear(GTK_LIST_STORE(model));


 /* テキストエントリに入力されている文字列を取得 */
  addStr = gtk_entry_get_text(addhData->storeIdEntry);
  if(strlen(addStr) < 1){
    gtk_label_set_text(addhData->result1Label, "店舗IDを入力してください");
    return;
  }


 if(g_soc > 0){
    /*サーバへプロトコルコマンド送信 */
   sendLen = sprintf(sendBuf, "%s %s %s", PURCHASE_DISP, addStr,ENTER);
   send(g_soc, sendBuf, sendLen, 0);
    /* サーバからレスポンス受信 */
    recvLen = recv_data(g_soc, recvBuf, BUFSIZE_MAX);
    record_division(recvBuf, records);
   
    sscanf(records[0], "%s %s %s", response, param1);

    if(strcmp(response, OK_STAT) != 0){
      showErrorMsg6(addhData->result1Label, atoi(param1));
      return;
    }



    /* 結果セット */
    gtk_label_set_text(addhData->result1Label , records[i]);
 
    for(i=1; i<=atoi(param1); i++){
      sscanf(records[i], "%s %s %s %s",param2, param3, param4, param5);
          
      gtk_list_store_append(GTK_LIST_STORE(model), &iter);
      gtk_list_store_set(GTK_LIST_STORE(model), &iter,0,atoi(param2),1,atoi(param3),2,param4,3, atoi(param5),-1);
    }

 }
}


void showErrorMsg6(GtkLabel *resultLabel, int errorCode){

  switch(errorCode){
  default:
    gtk_label_set_text(resultLabel, "致命的なエラーが発生しました");
    break;

  }

}
