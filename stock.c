/*
 * Copyright (c) 2016
 *    Yuto Sumiyoshi.  All rights reserved.
 *
 * stock.c
 * 最終更新日時：2016.12.12
 *
 */

#include "pos_client.h"
#include "stock.h"

int g_stockWindowFlag;

void showErrorMsg3(GtkLabel *erroLabel, int errorCode);


G_MODULE_EXPORT void cb_stockmanage_win_open(GtkButton *button, gpointer data){
  GtkBuilder      *builder;
  stockHandleData *stockhData;
  if( g_stockWindowFlag ==0){
    /* ログイン画面の主要Widget保持用構造体を作成 */
    stockhData = (stockHandleData *)malloc(sizeof(stockHandleData));


    /* login.gladeを読み込む */
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "stock.glade", NULL);
    /* 主要Widgetを保持 */
    stockhData->manageWindow     = GTK_WIDGET( gtk_builder_get_object(builder, "manageWindow"));
    stockhData->idEntry    = GTK_ENTRY( gtk_builder_get_object(builder, "idEntry"));
    stockhData->manageButton    = GTK_BUTTON( gtk_builder_get_object(builder, "manageButton"));
    stockhData->resultLabel    = GTK_LABEL( gtk_builder_get_object(builder, "resultLabel"));
    stockhData->stockTree    = GTK_TREE_VIEW( gtk_builder_get_object(builder, "stockTree"));


    /* シグナル、シグナルハンドラ、ユーザデータ登録 */
    gtk_builder_connect_signals(builder, stockhData);
    /* ログイン画面表示 */
    gtk_widget_show_all(stockhData->manageWindow);

    g_stockWindowFlag = 1;
  }
}


G_MODULE_EXPORT void cb_stock_cancel(GtkButton *button, gpointer data){
  stockHandleData *hData; //       Widget   

  hData = (stockHandleData *)data;
  /*                 */
  gtk_widget_hide(hData->manageWindow);
  /*       Widget               */
  free(hData);
  /*               */
  g_stockWindowFlag = 0;

}

G_MODULE_EXPORT void cb_stock_exec(GtkButton *button, gpointer data){
  stockHandleData *stockHData;       //メイン画面の主要Widget保持用
  const char *storeStr;
  GtkTreeModel     *model;          //ツリーモデル
  GtkTreeIter      iter;            //行
  char            sendBuf[BUFSIZE], recvBuf[BUFSIZE_MAX],*records[RECORD_MAX],response[BUFSIZE],param1[BUFSIZE], param2[BUFSIZE], param3[BUFSIZE],param4[BUFSIZE], param5[BUFSIZE], param6[BUFSIZE],insertData[BUFSIZE];
  int             sendLen, recvLen, i;

  stockHData = (stockHandleData *)data;
 
 model = gtk_tree_view_get_model(stockHData->stockTree);
  /* ツリービューを空にする */
  gtk_list_store_clear(GTK_LIST_STORE(model));
    
  storeStr = gtk_entry_get_text(stockHData->idEntry);
  if(strlen(storeStr) < 1){
    gtk_label_set_text(stockHData->resultLabel, "店舗IDを入力してください");
    return;
  }

  if(g_soc > 0){
    sendLen = sprintf(sendBuf, "%s %s %s",STOCK_DISP,storeStr,ENTER);
    send(g_soc, sendBuf, sendLen, 0);
    recvLen = recv_data(g_soc, recvBuf, BUFSIZE_MAX);
    record_division(recvBuf, records);
    
    sscanf(records[0], "%s %s", response, param1);
    if(strcmp(response, OK_STAT) != 0){
      showErrorMsg3(stockHData->resultLabel, atoi(param1));
      return;
    }

    sprintf(insertData, "取扱商品件数　%d",atoi(param1));
    gtk_label_set_text(stockHData->resultLabel, insertData);


    
    /* ツリービューに値をセット */
    for(i=1; i<=atoi(param1); i++){
      sscanf(records[i], "%s %s %s %s %s",param2, param3, param4, param5, param6);
          
      gtk_list_store_append(GTK_LIST_STORE(model), &iter);
      gtk_list_store_set(GTK_LIST_STORE(model), &iter,0,atoi(param2),1, param3,2, atoi(param4),3, param5,4, param6,-1
          );
    }
  }
}

void showErrorMsg3(GtkLabel *errorLabel, int errorCode){

  switch(errorCode){
  default:
    gtk_label_set_text(errorLabel,"正しいIDを入力してください");
    break;
  }
}
