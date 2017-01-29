/*
 * Copyright (c) 2016
 *    Yuki Taguchi.  All rights reserved.                    
 *
 * salesAnaryze.c
 *        2016.12.12 16:00
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include <gtk/gtk.h>
#include "salesAnaryze.h"
#include "pos_client.h"

int g_sellWindowFlag = 0;
int g_anaryzeWindowFlag = 0;

void showSAErrorMsg(GtkLabel *erroLabel, int errorCode);


G_MODULE_EXPORT void cb_sell_win_open(GtkButton *button, gpointer data){
  GtkBuilder        *builder;
  SellHandleData *hData; //残高照会画面の主要Widget保持用
  GtkListStore *model1,*model2,*model3; //GtkListStoreオブジェクト
  GtkTreeIter iter1,iter2,iter3;
  const gchar *Weather[] = {"晴れ","雨","曇り","雪"};
  const gint age1[] = {10,20,30,40,50};
  const gint age2[] = {19,29,39,49,59};
  int i;


  /* 販売分析画面が表示されていない場合 */
  if(g_sellWindowFlag == 0){
    /* 販売分析画面の主要Widget保持用構造体を作成 */
    hData = (SellHandleData *)malloc(sizeof(SellHandleData));
    /* salesAnaryze.gladeを読み込む */
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "salesAnaryze.glade", NULL);

    /* 扱う必要のあるWidgetを構造体hDataのメンバに保持 */
    /* メンバ"anaryzewindow"には残高照会画面のウィジェット */
    /* メンバ"idEntry"には口座IDを入力するテキストエントリウィジェット */
    /* メンバ"resultLabel"には結果を表示するラベルウィジェット */
    hData->anaryzewindow = GTK_WIDGET( gtk_builder_get_object(builder, "anaryzewindow") );

    hData->startentry = GTK_ENTRY( gtk_builder_get_object(builder, "startentry") );
    hData->endentry = GTK_ENTRY( gtk_builder_get_object(builder, "endentry") );
    hData->radioBtn[0] = GTK_RADIO_BUTTON( gtk_builder_get_object(builder, "Manradio") );
    hData->radioBtn[1] = GTK_RADIO_BUTTON( gtk_builder_get_object(builder, "Radyradio") );
    hData->radioBtn[2] = GTK_RADIO_BUTTON( gtk_builder_get_object(builder, "Noradio") );
    hData->Weatherbox = GTK_COMBO_BOX( gtk_builder_get_object(builder, "Weatherbox") );
    hData->Oldminbox = GTK_COMBO_BOX( gtk_builder_get_object(builder, "Oldminbox") );
    hData->Oldmaxbox = GTK_COMBO_BOX( gtk_builder_get_object(builder, "Oldmaxbox") );
    hData->Showentry = GTK_ENTRY( gtk_builder_get_object(builder, "Showentry") );
    hData->ShowButton = GTK_BUTTON( gtk_builder_get_object(builder, "ShowButton") );
    hData->BackButton = GTK_BUTTON( gtk_builder_get_object(builder, "BackButton") );
    

    /*コンボボックスのモデルを取得*/
    model1 = GTK_LIST_STORE(gtk_combo_box_get_model(hData->Weatherbox));
    model2 = GTK_LIST_STORE(gtk_combo_box_get_model(hData->Oldminbox));
    model3 = GTK_LIST_STORE(gtk_combo_box_get_model(hData->Oldmaxbox));


  /*モデルにデータをセット*/
    for(i=0;i<4;i++){
      gtk_list_store_append(model1,&iter1);
      gtk_list_store_set(model1,&iter1,0,Weather[i],-1);
    }

    for(i=0;i<5;i++){
      gtk_list_store_append(model2,&iter2);
      gtk_list_store_set(model2,&iter2,0,age1[i],-1);
    }

    for(i=0;i<5;i++){
      gtk_list_store_append(model3,&iter3);
      gtk_list_store_set(model3,&iter3,0,age2[i],-1);
    }
    
    /* シグナル、シグナルハンドラ、ユーザデータ登録 */
    gtk_builder_connect_signals(builder,hData);
    /* 残高照会画面表示 */
    gtk_widget_show_all(hData->anaryzewindow);
    /* 残高照会画面表示フラグをセット */
    g_sellWindowFlag = 1;
  
  }
}



G_MODULE_EXPORT void cb_anaryze_next_open(GtkButton *button, gpointer data){
  GtkBuilder        *builder;
  AnaryzeHandleData *AnaryzehData; //残高照会画面の主要Widget保持用
  SellHandleData *SellhData;
  const char *startStr=0, *endStr=0, *ShowStr=0;
  char  radioresult[BUFSIZE];
  char sendBuf[BUFSIZE],recvBuf[BUFSIZE_MAX],labelBuf[BUFSIZE];
  int sendLen,recvLen,recordCount,n;
  char *records[RECORD_MAX], response[BUFSIZE],param1[BUFSIZE],param2[BUFSIZE],param3[BUFSIZE],param4[BUFSIZE];
  char *radio[3] = {"m","f","0"};
  int i;
  GtkTreeModel *model1,*model2,*model3; //GtkListStoreオブジェクト
  GtkTreeIter iter1,iter2,iter3,iter4;
  gchar *WeatherValue="0";
  gint *OldminValue=0;
  gint *OldmaxValue=0;
  GtkListStore *model4;


  SellhData =(SellHandleData *)data;
  AnaryzehData = (AnaryzeHandleData *)malloc(sizeof(AnaryzeHandleData));

  /*抽出開始日に入力されている文字列を取得*/
  startStr = gtk_entry_get_text(SellhData->startentry);
  if(strlen(startStr)<1){
    startStr = "0";
  }
  /*抽出終了日に入力されている文字列を取得*/
  endStr = gtk_entry_get_text(SellhData->endentry);
  if(strlen(endStr)<1){
    endStr = "0";
  }
  /*ラジオボタンの選択されているものを取得*/
  
  for(i=0;i<3;i++){
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(SellhData->radioBtn[i]))){
      sprintf(radioresult, "%s", radio[i]);
      break;
    }
  }
 
  /*コンボボックスの中から現在選択されているアイテムの取得*/
 
  if(gtk_combo_box_get_active_iter(SellhData->Weatherbox,&iter1)){
    model1 = gtk_combo_box_get_model(SellhData->Weatherbox);
    gtk_tree_model_get(model1,&iter1,0,&WeatherValue,-1);
  }
  
  if(strlen(WeatherValue)<1){
    strcpy(WeatherValue,"0");
  }

  if(gtk_combo_box_get_active_iter(SellhData->Oldminbox,&iter2)){
    model2 = gtk_combo_box_get_model(SellhData->Oldminbox);
    gtk_tree_model_get(model2,&iter2,0,&OldminValue,-1);
  }

  if(gtk_combo_box_get_active_iter(SellhData->Oldmaxbox,&iter3)){
    model3 = gtk_combo_box_get_model(SellhData->Oldmaxbox);
    gtk_tree_model_get(model3,&iter3,0,&OldmaxValue,-1);
  }
  
/*表示件数に入力されている文字列を取得*/
  ShowStr = gtk_entry_get_text(SellhData->Showentry);
  if(strlen(ShowStr)<1){
    ShowStr = "0";
  }

/*通信用のソケットディスクリプタが空でないかチェック*/
  if(g_soc>0){
    sendLen = sprintf(sendBuf, "%s %s %s %s %s %d %d %s %s","ABST_DISP",startStr,endStr,radioresult,WeatherValue,OldminValue,OldmaxValue,ShowStr,ENTER);

    send(g_soc,sendBuf,sendLen,0);
    recvLen = recv_data(g_soc,recvBuf,BUFSIZE_MAX);
    recordCount = record_division(recvBuf,records);
    memset(response,0,BUFSIZE);
    memset(param1,0,BUFSIZE);
    memset(param2,0,BUFSIZE);
    memset(param3,0,BUFSIZE);
    memset(param4,0,BUFSIZE);



  if(g_anaryzeWindowFlag == 0){

    builder = gtk_builder_new();
    /* balance.gladeを読み込む */
    gtk_builder_add_from_file(builder, "salesAnaryze2.glade", NULL);
    /* 扱う必要のあるWidgetを構造体hDataのメンバに保持 */
    /* メンバ"balanceWindow"には残高照会画面のウィジェット */
    /* メンバ"idEntry"には口座IDを入力するテキストエントリウィジェット */
    /* メンバ"resultLabel"には結果を表示するラベルウィジェット */
    AnaryzehData->anaryzewindow1 = GTK_WIDGET( gtk_builder_get_object(builder, "anaryzewindow1") );
    AnaryzehData->treeView = GTK_TREE_VIEW( gtk_builder_get_object(builder, "treeview1") );
    AnaryzehData->disp_label = GTK_LABEL(gtk_builder_get_object(builder,"disp_label"));
    /* シグナル、シグナルハンドラ、ユーザデータ登録 */
    gtk_builder_connect_signals(builder,AnaryzehData);
    /* 残高照会画面表示 */
    gtk_widget_show_all(AnaryzehData->anaryzewindow1);
    /* 残高照会画面表示フラグをセット */
    g_anaryzeWindowFlag = 1;
  }



    n = sscanf(records[0], "%s %s %s %s %s",response,param1,param2,param3,param4);
    if(strcmp(response,OK_STAT) !=0){
      showSAErrorMsg(AnaryzehData->disp_label, atoi(param1));
      return;
  }

    sprintf(labelBuf,"表示件数:%d件",atoi(param1));
    gtk_label_set_text(AnaryzehData->disp_label,labelBuf);

    model4 = GTK_LIST_STORE(gtk_tree_view_get_model(AnaryzehData->treeView));
    gtk_list_store_clear(model4);


    for(n=1;n<recordCount;n++){
      sscanf(records[n], "%s %s %s",param1,param2, param3);
      gtk_list_store_append(model4,&iter4);
      gtk_list_store_set(model4,&iter4,0,param1,1,param2,2,param3,-1);

    }
  }




  return;

}


G_MODULE_EXPORT void cb_anaryze_cancel(GtkButton *button, gpointer data){
  SellHandleData *hData; //         Widget   

  hData = (SellHandleData *)data;
  /*                   */
  gtk_widget_hide(hData->anaryzewindow);
  /*         Widget              ) */
  free(hData);
  /*                 */
  g_sellWindowFlag = 0;

}

G_MODULE_EXPORT void cb_anaryze2_cancel(GtkButton *button, gpointer data){
  AnaryzeHandleData *hData; //         Widget   

  hData = (AnaryzeHandleData *)data;
  /*                   */
  gtk_widget_hide(hData->anaryzewindow1);
  /*         Widget              ) */
  free(hData);
  /*                 */
  g_anaryzeWindowFlag = 0;

}

void showSAErrorMsg(GtkLabel *errorLabel, int errorCode){

    switch(errorCode){
        case 200:
            gtk_label_set_text(errorLabel, "ERROR:コマンドリクエストの引数エラー");
            break;
        case 300:
            gtk_label_set_text(errorLabel, "ERROR:データベースエラー(そんな日付はありません！！)");
            break;
        case 5100:
            gtk_label_set_text(errorLabel, "ERROR:ログインエラー");
            break;

        default:
            gtk_label_set_text(errorLabel, "ERROR:致命的なエラーが発生しました");
            break;
    }   
}
