/*
 * Copyright (c) 2016
 *    平松 拓海.
 *
 * customer.c
 * 最終更新日時：2016.12.12 17:10
 *
 */
#include "pos_client.h"
#include "customer.h"

int g_customer_manageWindowFlag = 0; //顧客管理画面表示フラグ　1:表示 0:非表示
int g_customer_showWindowFlag = 0; //検索結果表示用ツリービューフラグ　1:表示 0:非表示

//以下コールバック関数

G_MODULE_EXPORT void cb_customer_manage_open(GtkButton *button, gpointer data){
  GtkBuilder        *builder;
  CustomerHandleData *hData; //顧客管理画面の主要Widget保持用

  /* 顧客管理画面画面が表示されていない場合 */
  if(g_customer_manageWindowFlag == 0){
    /* 顧客管理画面の主要Widget保持用構造体を作成 */
    hData = (CustomerHandleData *)malloc(sizeof(CustomerHandleData));
    /* customer.gladeを読み込む */
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "customer.glade", NULL);
    /* 扱う必要のあるWidgetを構造体hDataのメンバに保持 */
    /* メンバ"customerWindow"には顧客管理画面のウィジェット */
    /* メンバ"customerEntry"には検索する範囲の先頭の顧客IDを入力するテキストエントリウィジェット */
    /* メンバ"limitEntry"には検索件数を入力するテキストエントリウィジェット */
    /* メンバ"resultLabel"には結果を表示するラベルウィジェット */
    hData->customerWindow = GTK_WIDGET( gtk_builder_get_object(builder, "customerWindow") );
    hData->startEntry = GTK_ENTRY( gtk_builder_get_object(builder, "startEntry") );
    hData->numberSpinButton = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "numberSpinButton"));
    hData->resultLabel = GTK_LABEL(gtk_builder_get_object(builder, "resultLabel"));
    gtk_window_set_title(GTK_WINDOW(hData->customerWindow),"顧客管理");
    
    /* シグナル、シグナルハンドラ、ユーザデータ登録 */
    gtk_builder_connect_signals(builder,hData);
    /* 顧客管理画面表示 */
    gtk_widget_show_all(hData->customerWindow);
    /* 顧客管理画面表示フラグをセット */
    g_customer_manageWindowFlag = 1;
  }
}


G_MODULE_EXPORT void cb_customer_cancel(GtkButton *button, gpointer data){
  CustomerHandleData *hData; //顧客管理画面の主要Widget保持用

  hData = (CustomerHandleData *)data;
  /* 顧客管理画面（ウィンドウ）を非表示 */
  gtk_widget_hide(hData->customerWindow);
  /* 入金画面主要Widget保持構造体を破棄（メモリ開放) */
  free(hData);
  /* 入金画面表示フラグをクリア */
  g_customer_manageWindowFlag = 0;

}


/**
 * cb_customer_show
 **/
G_MODULE_EXPORT void cb_customer_show_exec(GtkButton *button, gpointer data){

  GtkBuilder      	*builder;
  GtkListStore 		*model; //ツリーモデル
  GtkTreeIter 		iter; //行

  CustomerHandleData  	*hData;  
  CustomerShowHandleData*customerHData;

  const char *startNumber;
  char sendBuf[BUFSIZE],recvBuf[BUFSIZE_MAX],response[BUFSIZE],param1[BUFSIZE];

  char customerId[BUFSIZE],customerName[BUFSIZE],customerGender[BUFSIZE],customerBirth[BUFSIZE],customerAddress[BUFSIZE],customerPhone[BUFSIZE],customerEmail[BUFSIZE],customerCard[BUFSIZE];

  char *records[RECORD_MAX];
  int limitNumber;
  int sendLen,recvLen,n,recordCount;
  int i;

  /* 検索結果画面が表示されていない場合 */
  if(g_customer_showWindowFlag == 0){
    /* 引数(data)をメイン画面主要Widget保持用構造体型にキャスト */
    customerHData = (CustomerShowHandleData *)malloc(sizeof(CustomerShowHandleData));

    /* customer_show.gladeを読み込む */
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "customerShow.glade", NULL);

    /* 主要Widgetを保持 */
    customerHData->customerShowWindow = GTK_WIDGET( gtk_builder_get_object(builder, "customerShowWindow") );
    customerHData->customerShowTreeView = GTK_TREE_VIEW( gtk_builder_get_object(builder,"customerShowTreeView"));
    customerHData->customerShowFinishButton  = GTK_BUTTON( gtk_builder_get_object(builder, "customerShowFinishButton") );

    /* シグナル、シグナルハンドラ、ユーザデータ登録 */
    gtk_builder_connect_signals(builder, customerHData);
  }

  //ウィンドウのタイトル設定
  gtk_window_set_title(GTK_WINDOW(customerHData->customerShowWindow),"顧客情報一覧");

  hData = (CustomerHandleData *)data;

  startNumber = gtk_entry_get_text(hData->startEntry);

  if(strlen(startNumber) < 1){
    gtk_label_set_text(hData->resultLabel, "ERROR:何人目の顧客から検索するか入力してください");
    return;
  }

  limitNumber = gtk_spin_button_get_value(hData->numberSpinButton) + atoi(startNumber) -1;


  if(g_soc > 0){
    sendLen = sprintf(sendBuf, "%s %d %d %s",USER_LIST_DISP, atoi(startNumber),limitNumber,ENTER);

    send(g_soc, sendBuf, sendLen, 0);
    recvLen = recv_data(g_soc, recvBuf,BUFSIZE_MAX);
    recordCount = record_division(recvBuf, records);
    memset(response, 0, BUFSIZE);
    memset(param1, 0, BUFSIZE);

    /* レスポンスメッセージを解析 */
    n = sscanf(records[0], "%s %s", response, param1);

    if(strcmp(response, OK_STAT) != 0){
      /* エラーメッセージを表示 */
      showErrorMsg(hData->resultLabel, atoi(param1));
      return;
    }

    if(atoi(startNumber) <= 0 || limitNumber-atoi(startNumber) <= 0 && atoi(startNumber) != limitNumber){
    gtk_label_set_text(hData->resultLabel, "ERROR:不正な入力です");
    return;
    }

    //エラーがなかった場合、実行結果ラベルに何も表示しないようにする
    gtk_label_set_text(hData->resultLabel, "");

    /* UIからツリービューを取得　*/
    customerHData->customerShowTreeView = GTK_TREE_VIEW(gtk_builder_get_object(builder, "customerShowTreeView"));
    /* ツリービューのモデルの取得  */
    model = GTK_LIST_STORE(gtk_tree_view_get_model(customerHData->customerShowTreeView));
    /* モデルにデータをセット */
    gtk_list_store_clear(model);

    //ツリービューにデータを代入
    for(i = 1; i < recordCount; i++){

      sscanf(records[i], "%s %s %s %s %s %s %s %s", customerId, customerName, customerGender, customerBirth, customerAddress, customerPhone, customerEmail, customerCard);
      gtk_list_store_append(model,&iter);

      //tならツリービューの性別に男をそうでないなら女を表示する
      if(strcmp(customerGender,"t") == 0){

	gtk_list_store_set(model,&iter, 0,atoi(customerId), 1,customerName, 2,"男", 3,customerBirth, 4,customerAddress, 5,customerPhone, 6,customerEmail, 7,atoi(customerCard), -1);

      } else { 
	gtk_list_store_set(model,&iter, 0,atoi(customerId), 1,customerName, 2,"女", 3,customerBirth, 4,customerAddress, 5,customerPhone, 6,customerEmail, 7,atoi(customerCard), -1);
      }

    }

    /* 検索結果画面表示 */
    gtk_widget_show_all(customerHData->customerShowWindow);
    /* 検索結果画面表示フラグをセット */
    g_customer_showWindowFlag = 1;

  }
}

G_MODULE_EXPORT void cb_customer_show_cancel(GtkButton *button, gpointer data){
  CustomerShowHandleData *hData; //検索結果画面の主要Widget保持用

  hData = (CustomerShowHandleData *)data;
  /* 顧客管理画面（ウィンドウ）を非表示 */
  gtk_widget_hide(hData->customerShowWindow);
  /* 検索結果画面主要Widget保持構造体を破棄（メモリ開放) */
  free(hData);
  /* 検索結果画面表示フラグをクリア */
  g_customer_showWindowFlag = 0;

}
