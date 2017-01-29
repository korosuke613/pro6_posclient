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
#include "purchase2.h"

int g_add2WindowFlag;

void showErrorMsg4(GtkLabel *erroLabel, int errorCode);


G_MODULE_EXPORT void cb_add2_win_open(GtkButton *button, gpointer data){
  GtkBuilder      *builder;
  add2HandleData *add2hData; //ログイン画面の主要Widget保持用

  if(g_add2WindowFlag ==0){

  /* ログイン画面の主要Widget保持用構造体を作成 */
  add2hData = (add2HandleData *)malloc(sizeof(add2HandleData));

  /* login.gladeを読み込む */
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "purchase2.glade", NULL);
  /* 主要Widgetを保持 */
  add2hData->add2Window     = GTK_WIDGET( gtk_builder_get_object(builder, "add2Window") );
  add2hData->idEntry    = GTK_ENTRY( gtk_builder_get_object(builder, "idEntry") );
  add2hData->add2Button    = GTK_BUTTON( gtk_builder_get_object(builder, "add2Button") );
  add2hData->next2Button    = GTK_BUTTON( gtk_builder_get_object(builder, "next2Button") );  
 add2hData->add2Tree    = GTK_TREE_VIEW( gtk_builder_get_object(builder, "add2Tree") );

  /* シグナル、シグナルハンドラ、ユーザデータ登録 */
  gtk_builder_connect_signals(builder, add2hData);
  /* ログイン画面表示 */
  gtk_widget_show_all(add2hData->add2Window);

  g_add2WindowFlag = 1; 
 }
}


G_MODULE_EXPORT void cb_add3_cancel(GtkButton *button, gpointer data){
  add2HandleData *add2hData; //       Widget   

  add2hData = (add2HandleData *)data;
  /*                 */
  gtk_widget_hide(add2hData->add2Window);
  /*       Widget               */
  free(add2hData);
  /*               */
  g_add2WindowFlag = 0;
}


G_MODULE_EXPORT void cb_add_exec(GtkButton *button, gpointer data){
  add2HandleData *add2HData;       //メイン画面の主要Widget保持用
  const char *addStr;
  GtkTreeModel     *model;          //ツリーモデル
  GtkTreeIter      iter;            //行
  char            sendBuf[BUFSIZE], recvBuf[BUFSIZE_MAX],*records[RECORD_MAX],response[BUFSIZE],param1[BUFSIZE], param2[BUFSIZE], param3[BUFSIZE],param4[BUFSIZE], param5[BUFSIZE], param6[BUFSIZE],param7[BUFSIZE], param8[BUFSIZE],insertData[BUFSIZE];
  int             sendLen, recvLen, i;

  add2HData = (add2HandleData *)data;
 
 model = gtk_tree_view_get_model(add2HData->add2Tree);
  /* ツリービューを空にする */
  gtk_list_store_clear(GTK_LIST_STORE(model));
    
  addStr = gtk_entry_get_text(add2HData->idEntry);
  if(strlen(addStr) < 1){
    gtk_label_set_text(add2HData->resultLabel, "仕入れIDを入力してください");
    return;
  }

  if(g_soc > 0){
    sendLen = sprintf(sendBuf, "%s %s %s",PURCHASE_DT_DISP,addStr,ENTER);
    send(g_soc, sendBuf, sendLen, 0);
    recvLen = recv_data(g_soc, recvBuf, BUFSIZE_MAX);
    record_division(recvBuf, records);
    
    sscanf(records[0], "%s %s", response, param1);
    if(strcmp(response, OK_STAT) != 0){
      showErrorMsg4(add2HData->resultLabel, atoi(param1));
      return;
    }

    sprintf(insertData, "取扱商品件数　%d",atoi(param1));
    gtk_label_set_text(add2HData->resultLabel, insertData);


    
    /* ツリービューに値をセット */
    for(i=1; i<=atoi(param1); i++){
      sscanf(records[i], "%s %s %s %s %s %s %s",param2, param3, param4, param5, param6,param7,param8);
          
      gtk_list_store_append(GTK_LIST_STORE(model), &iter);
      gtk_list_store_set(GTK_LIST_STORE(model), &iter,0,atoi(param2),1,atoi(param3),2,param4,3, atoi(param5),4,atoi(param6),5,atoi(param7),6,param8,-1);
    }
  }
}

void showErrorMsg4(GtkLabel *errorLabel, int errorCode){

  switch(errorCode){
  default:
    gtk_label_set_text(errorLabel,"正しいIDを入力してください");
    break;
  }
}
