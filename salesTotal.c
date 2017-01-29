/*
 * Copyright (c) 2016
 *    平松 拓海. 
 *
 * salesTotal.c
 * 最終更新日時：2016.12.12 17:10
 *
 */

#include "pos_client.h"
#include "salesTotal.h"

int g_sales_selectWindowFlag = 0; //売上集計選択画面 1:表示 0:非表示
int g_sales_numberWindowFlag = 0; //売上抽出入力画面
int g_sales_number_showWindowFlag =0; //売上抽出結果(ツリービュー)画面
int g_sales_totalWindowFlag = 0; //売上利益算出画面

G_MODULE_EXPORT void cb_sales_select_open(GtkButton *button, gpointer data){
  GtkBuilder        *builder;
  SalesSelectHandleData *hData; //売上集計機能選択画面の主要Widget保持用   

  /*売上集計機能選択画面が未表示の時*/
  if(g_sales_selectWindowFlag == 0){

    /*売上集計機能画面の主要Widget保持用構造体を生成*/
    hData = (SalesSelectHandleData *)malloc(sizeof(SalesSelectHandleData));
    /* salesTotal_select.gladeを読み込む*/
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "salesTotal_select.glade", NULL);
    /*扱う必要のあるWidgetを構造体hDataのメンバに保持*/
    /*メンバ"sales_selectWindow"には売上集計機能選択画面のウィジェット*/
    /*メンバ"sales_numberButton"には売上抽出のウィジェット*/
    /*メンバ"sales_totalButton"には売上利益算出のウィジェット*/
      hData->sales_selectWindow = GTK_WIDGET( gtk_builder_get_object(builder, "sales_selectWindow") );
    hData->sales_numberButton = GTK_BUTTON( gtk_builder_get_object(builder, "sales_numberButton") );
    hData->sales_totalButton = GTK_BUTTON(gtk_builder_get_object(builder, "sales_totalButton"));

  //ウィンドウのタイトル設定
  gtk_window_set_title(GTK_WINDOW(hData->sales_selectWindow),"売上集計の機能を選んでください");

    /*シグナル、シグナルハンドラ、ユーザデータ登録*/
    gtk_builder_connect_signals(builder,hData);
    /*売上集計機能選択画面の表示*/
    gtk_widget_show_all(hData->sales_selectWindow);
    /*売上集計機能選択画面表示フラグをセット*/
    g_sales_selectWindowFlag = 1;
  }
}



G_MODULE_EXPORT void cb_sales_select_cancel(GtkButton *button, gpointer data){
  SalesSelectHandleData *hData; //売上集計機能選択画面の主要Widget保持用

  hData = (SalesSelectHandleData *)data;
  /* 売上集計機能選択画面（ウィンドウ）を非表示 */
  gtk_widget_hide(hData->sales_selectWindow);
  /* 売上集計機能選択画面主要Widget保持構造体を破棄（メモリ開放) */
  free(hData);
  /*売上集計機能選択画面表示フラグをクリア */
  g_sales_selectWindowFlag = 0;

}

/**
 * cb_sales_number
 * 売上抽出画面を表示するだけの関数
 *
 **/
G_MODULE_EXPORT void cb_sales_number(GtkButton *button, gpointer data){
  GtkBuilder      *builder;
  SalesNumberHandleData  *hData;  

  /* 売上抽出画面が表示されていない場合 */
  if(g_sales_numberWindowFlag == 0){
    /* 引数(data)を売上抽出画面の主要Widget保持用構造体型にキャスト */
    hData = (SalesNumberHandleData *)malloc(sizeof(SalesNumberHandleData));

    /* salesTotal_number.gladeを読み込む */
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "salesTotal_number.glade", NULL);

    /* 主要Widgetを保持 */
    hData->sales_numberWindow     = GTK_WIDGET( gtk_builder_get_object(builder, "sales_numberWindow") );
    hData->sales_number_typeRadioButton[0] = GTK_RADIO_BUTTON( gtk_builder_get_object(builder, "sales_number_typeRadioButton1") );
    hData->sales_number_typeRadioButton[1] = GTK_RADIO_BUTTON( gtk_builder_get_object(builder, "sales_number_typeRadioButton2") );
    hData->sales_number_typeRadioButton[2] = GTK_RADIO_BUTTON( gtk_builder_get_object(builder, "sales_number_typeRadioButton3") );
    hData->sales_number_typeEntry     = GTK_ENTRY( gtk_builder_get_object(builder, "sales_number_typeEntry") );    
    hData->sales_number_startEntry     = GTK_ENTRY( gtk_builder_get_object(builder, "sales_number_startEntry") );
    hData->sales_number_limitEntry     = GTK_ENTRY( gtk_builder_get_object(builder, "sales_number_limitEntry") );
    hData->sales_numberButton    = GTK_BUTTON( gtk_builder_get_object(builder, "sales_numberButton") );
    hData->sales_number_cancelButton     = GTK_BUTTON( gtk_builder_get_object(builder, "sales_number_cancelButton") );
    hData->sales_number_resultLabel     = GTK_LABEL( gtk_builder_get_object(builder, "sales_number_resultLabel") );

    //ウィンドウのタイトル設定
    gtk_window_set_title(GTK_WINDOW(hData->sales_numberWindow),"売上抽出");
    /* シグナル、シグナルハンドラ、ユーザデータ登録 */
    gtk_builder_connect_signals(builder, hData);
    /* 売上抽出画面表示 */
    gtk_widget_show_all(hData->sales_numberWindow);
    /* 売上抽出画面表示フラグをセット */
    g_sales_numberWindowFlag = 1;

  }
}

G_MODULE_EXPORT void cb_sales_number_exec(GtkButton *button, gpointer data){

  GtkBuilder *builder;
  GtkListStore *model; //ツリーモデル
  GtkTreeIter iter; //行

  SalesNumberHandleData *hData;
  SalesNumberShowHandleData *salesHData;

  const char *numberType,*numberStart,*numberLimit;
  char sendBuf[BUFSIZE],recvBuf[BUFSIZE_MAX],response[BUFSIZE],param1[BUFSIZE];

  char salesProduct[BUFSIZE],salesNumber[BUFSIZE],typeStr[BUFSIZE];
  char *records[RECORD_MAX];
  int sendLen,recvLen,n,recordCount;
  int i;

  //抽出結果画面が未表示の時
  if(g_sales_number_showWindowFlag == 0){
    /* 引数(data)をメイン画面主要Widget保持用構造体型にキャスト */
    salesHData = (SalesNumberShowHandleData *)malloc(sizeof(SalesNumberShowHandleData));

    // salesTotal_number_show.gladeを読み込む
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "salesTotal_number_show.glade",NULL);

    //主要ウィジェットを保持
    salesHData->sales_number_showWindow = GTK_WIDGET(gtk_builder_get_object(builder, "salesTotal_number_showWindow"));
    salesHData->sales_number_showTreeView = GTK_TREE_VIEW(gtk_builder_get_object(builder, "salesTotal_number_showTreeView"));
    salesHData->sales_number_showButton = GTK_BUTTON(gtk_builder_get_object(builder, "salesTotal_number_show_cancelButton"));


    /* シグナル、シグナルハンドラ、ユーザデータ登録 */
    gtk_builder_connect_signals(builder, salesHData);

    hData = (SalesNumberHandleData *)data;

    numberType = gtk_entry_get_text(hData->sales_number_typeEntry);

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hData->sales_number_typeRadioButton[0]))){
      if(strlen(numberType) < 1){
	gtk_label_set_text(hData->sales_number_resultLabel,"ERROR:種類を入力してください");
	return;
      }
    }

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hData->sales_number_typeRadioButton[1])) || gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hData->sales_number_typeRadioButton[2]))){

      if(strlen(numberType) < 1){
	gtk_label_set_text(hData->sales_number_resultLabel,"ERROR:IDを入力してください");
	return;
      }
    }

    numberStart = gtk_entry_get_text(hData->sales_number_startEntry);

    if(strlen(numberStart) < 1){
      gtk_label_set_text(hData->sales_number_resultLabel,"ERROR:抽出開始日を入力してください");
      return;
    }

    numberLimit = gtk_entry_get_text(hData->sales_number_limitEntry);

    if(strlen(numberLimit) < 1){
      gtk_label_set_text(hData->sales_number_resultLabel, "ERROR:抽出終了日を入力してください");
      return;
    }

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hData->sales_number_typeRadioButton[0]))){
      sprintf(typeStr,"%s", "kind");
    }else if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hData->sales_number_typeRadioButton[1]))){
      sprintf(typeStr,"%s", "area");
    }else if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hData->sales_number_typeRadioButton[2]))){
      sprintf(typeStr,"%s", "shop");
    }

    if(g_soc > 0){
    }
    sendLen = sprintf(sendBuf, "%s %s %s %d %d %s",SALE_NUM_INFO,typeStr, numberType,atoi(numberStart),atoi(numberLimit), ENTER);

    send(g_soc, sendBuf, sendLen, 0);
    recvLen = recv_data(g_soc, recvBuf,BUFSIZE_MAX);
    recordCount = record_division(recvBuf, records);
    memset(response, 0, BUFSIZE);
    memset(param1, 0, BUFSIZE);

    /* レスポンスメッセージを解析 */
    n = sscanf(records[0], "%s %s", response, param1);

    if(strcmp(response, OK_STAT) != 0){
      /* エラーメッセージを表示 */
      showErrorMessage(hData->sales_number_resultLabel, atoi(param1));
      return;
    }else if(strcmp(param1,"0") == 0){
      gtk_label_set_text(hData->sales_number_resultLabel, "データが存在しません。");
      return;
    }else{
      //エラーがなかった場合、実行結果ラベルに何も表示しないようにする
      gtk_label_set_text(hData->sales_number_resultLabel, "");
    }
    /* UIからツリービューを取得　*/
    salesHData->sales_number_showTreeView = GTK_TREE_VIEW(gtk_builder_get_object(builder, "salesTotal_number_showTreeView"));
    /* ツリービューのモデルの取得  */
    model = GTK_LIST_STORE(gtk_tree_view_get_model(salesHData->sales_number_showTreeView));
    /* モデルにデータをセット */
    gtk_list_store_clear(model);

    for(i = 1; i < recordCount; i++){

      sscanf(records[i], "%s %s", salesProduct, salesNumber);
      gtk_list_store_append(model,&iter);
      gtk_list_store_set(model,&iter, 0,salesProduct, 1,atoi(salesNumber), -1);

    }

    //ウィンドウのタイトル設定
    gtk_window_set_title(GTK_WINDOW(salesHData->sales_number_showWindow),"売上抽出結果");


    /* 検索結果画面表示 */
    gtk_widget_show_all(salesHData->sales_number_showWindow);
    /* 検索結果画面表示フラグをセット */
    g_sales_number_showWindowFlag = 1;

  }
}

G_MODULE_EXPORT void cb_sales_number_show_cancel(GtkButton *button, gpointer data){
  SalesNumberShowHandleData *hData; //入金画面の主要Widget保持用

  hData = (SalesNumberShowHandleData *)data;
  /* 入金画面（ウィンドウ）を非表示 */
  gtk_widget_hide(hData->sales_number_showWindow);
  /* 入金画面主要Widget保持構造体を破棄（メモリ開放) */
  free(hData);
  /* 入金画面表示フラグをクリア */
  g_sales_number_showWindowFlag = 0;

}

G_MODULE_EXPORT void cb_sales_number_cancel(GtkButton *button, gpointer data){
  SalesNumberHandleData *hData; //入金画面の主要Widget保持用

  hData = (SalesNumberHandleData *)data;
  /* 入金画面（ウィンドウ）を非表示 */
  gtk_widget_hide(hData->sales_numberWindow);
  /* 入金画面主要Widget保持構造体を破棄（メモリ開放) */
  free(hData);
  /* 入金画面表示フラグをクリア */
  g_sales_numberWindowFlag = 0;

}


/**
 * cb_sales_total
 **/
G_MODULE_EXPORT void cb_sales_total(GtkButton *button, gpointer data){
  GtkBuilder      *builder;
  SalesTotalHandleData  *hData;  

  /* 出金画面が表示されていない場合 */
  if(g_sales_totalWindowFlag == 0){
    /* 引数(data)をメイン画面主要Widget保持用構造体型にキャスト */
    hData = (SalesTotalHandleData *)malloc(sizeof(SalesTotalHandleData));

    /* sales_number.gladeを読み込む */
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "salesTotal_total.glade", NULL);

    /* 主要Widgetを保持 */
    hData->sales_totalWindow     = GTK_WIDGET( gtk_builder_get_object(builder, "sales_totalWindow") );
    hData->sales_total_typeEntry     = GTK_ENTRY( gtk_builder_get_object(builder, "sales_total_typeEntry") );    
    hData->sales_total_startEntry     = GTK_ENTRY( gtk_builder_get_object(builder, "sales_total_startEntry") );
    hData->sales_totalButton    = GTK_BUTTON( gtk_builder_get_object(builder, "sales_totalButton") );
    hData->sales_total_retryButton     = GTK_BUTTON( gtk_builder_get_object(builder, "sales_total_retryButton") );
    hData->sales_totalLabel     = GTK_LABEL( gtk_builder_get_object(builder, "sales_totalLabel") );
    hData->sales_total_priceLabel     = GTK_LABEL( gtk_builder_get_object(builder, "sales_total_priceLabel") );
    hData->sales_total_gainLabel     = GTK_LABEL( gtk_builder_get_object(builder, "sales_total_gainLabel") );
    hData->sales_total_cancelButton     = GTK_BUTTON( gtk_builder_get_object(builder, "sales_total_cancelButton") );
    hData->sales_total_resultLabel     = GTK_LABEL( gtk_builder_get_object(builder, "sales_total_resultLabel") );

    hData->sales_total_typeRadioButton[0] = GTK_RADIO_BUTTON( gtk_builder_get_object(builder, "sales_total_typeRadioButton1") );
    hData->sales_total_typeRadioButton[1] = GTK_RADIO_BUTTON( gtk_builder_get_object(builder, "sales_total_typeRadioButton2") );
    hData->sales_total_typeRadioButton[2] = GTK_RADIO_BUTTON( gtk_builder_get_object(builder, "sales_total_typeRadioButton3") );

    hData->sales_total_termRadioButton[0] = GTK_RADIO_BUTTON( gtk_builder_get_object(builder, "sales_total_termRadioButton1") );
    hData->sales_total_termRadioButton[1] = GTK_RADIO_BUTTON( gtk_builder_get_object(builder, "sales_total_termRadioButton2") );
    hData->sales_total_termRadioButton[2] = GTK_RADIO_BUTTON( gtk_builder_get_object(builder, "sales_total_termRadioButton3") );
    hData->sales_total_termRadioButton[3] = GTK_RADIO_BUTTON( gtk_builder_get_object(builder, "sales_total_termRadioButton4") );
    hData->sales_total_termRadioButton[4] = GTK_RADIO_BUTTON( gtk_builder_get_object(builder, "sales_total_termRadioButton5") );

    //ウィンドウのタイトル設定
    gtk_window_set_title(GTK_WINDOW(hData->sales_totalWindow),"売上利益");

    /* シグナル、シグナルハンドラ、ユーザデータ登録 */
    gtk_builder_connect_signals(builder, hData);
    /* 顧客管理画面表示 */
    gtk_widget_show_all(hData->sales_totalWindow);
    /* 顧客管理画面表示フラグをセット */
    g_sales_totalWindowFlag = 1;

  }
}

G_MODULE_EXPORT void cb_sales_total_exec(GtkButton *button, gpointer data){

  GtkBuilder *builder;
  SalesTotalHandleData *hData;

  const char *typeStr,*startDate;
  char sendBuf[BUFSIZE],recvBuf[BUFSIZE_MAX],response[BUFSIZE],param1[BUFSIZE],param2[BUFSIZE],param3[BUFSIZE];
  char *records[BUFSIZE];

  char totalType[BUFSIZE],totalTerm[BUFSIZE];
  int sendLen,recvLen,n,recordCount;
 
  hData = (SalesTotalHandleData *)data;

  /* salesTotal_total.gladeを読み込む */
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "salesTotal_total.glade", NULL);

  typeStr = gtk_entry_get_text(hData->sales_total_typeEntry);

  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hData->sales_total_typeRadioButton[0]))){

    if(strlen(typeStr) < 1){
      gtk_label_set_text(hData->sales_total_resultLabel,"ERROR:種類を入力してください");
      return;
    }
  }

  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hData->sales_total_typeRadioButton[1])) || gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hData->sales_total_typeRadioButton[2]))){

    if(strlen(typeStr) < 1){
      gtk_label_set_text(hData->sales_total_resultLabel,"ERROR:IDを入力してください");
      return;
    }
  }

  startDate = gtk_entry_get_text(hData->sales_total_startEntry);

  if(strlen(startDate) < 1){
    gtk_label_set_text(hData->sales_total_resultLabel,"ERROR:抽出開始日を入力してください");
    return;
  }

  if(g_soc > 0){

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hData->sales_total_typeRadioButton[0]))){
      sprintf(totalType,"%s", "kind");
    }else if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hData->sales_total_typeRadioButton[1]))){
      sprintf(totalType,"%s", "area");
    }else if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hData->sales_total_typeRadioButton[2]))){
      sprintf(totalType,"%s", "shop");
    }

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hData->sales_total_termRadioButton[0]))){
      sprintf(totalTerm,"%s", "day");
    }else if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hData->sales_total_termRadioButton[1]))){
      sprintf(totalTerm,"%s", "week");
    }else if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hData->sales_total_termRadioButton[2]))){
      sprintf(totalTerm,"%s", "month");
    }else if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hData->sales_total_termRadioButton[3]))){
      sprintf(totalTerm,"%s", "quarter");
    }else if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hData->sales_total_termRadioButton[4]))){
      sprintf(totalTerm,"%s", "year");
    }
  }
  sendLen = sprintf(sendBuf, "%s %s %s %d %s %s",SALE_INS_INFO, totalType, typeStr, atoi(startDate), totalTerm, ENTER);

  send(g_soc, sendBuf, sendLen, 0);
  recvLen = recv_data(g_soc, recvBuf, BUFSIZE_MAX);
  recordCount = record_division(recvBuf, records);
  memset(response, 0, BUFSIZE);
  memset(param1, 0, BUFSIZE);
  memset(param2, 0, BUFSIZE);
  memset(param3, 0, BUFSIZE);

  //レスポンスメッセージを解析
  n = sscanf(records[0], "%s %s %s %s", response, param1, param2, param3);

  if(strcmp(response, OK_STAT) != 0){
    /* エラーメッセージを表示 */
    showErrorMsg(hData->sales_total_resultLabel, atoi(param1));
    return;
  }

  //エラーがなかった場合、実行結果ラベルに何も表示しないようにする
  gtk_label_set_text(hData->sales_total_resultLabel, "");

  gtk_label_set_text(hData->sales_totalLabel, param1);
  gtk_label_set_text(hData->sales_total_priceLabel, param2);
  gtk_label_set_text(hData->sales_total_gainLabel, param3);

  /* シグナル、シグナルハンドラ、ユーザデータ登録 */
  gtk_builder_connect_signals(builder, hData);

}

G_MODULE_EXPORT void cb_sales_total_cancel(GtkButton *button, gpointer data){
  SalesTotalHandleData *hData; //入金画面の主要Widget保持用

  hData = (SalesTotalHandleData *)data;
  /* 入金画面（ウィンドウ）を非表示 */
  gtk_widget_hide(hData->sales_totalWindow);
  /* 入金画面主要Widget保持構造体を破棄（メモリ開放) */
  free(hData);
  /* 入金画面表示フラグをクリア */
  g_sales_totalWindowFlag = 0;

}

/**
 * ログインエラーメッセージ表示
 * showErrorMsg 
 *  [PARAMETER]
 *   GtkLabel *errorLabel   :  ログイン画面のエラー表示用GtkLabelのポインタ
 *   int errorCode          :  エラーレスポンスコード
 *  [RETURN]
 *    NONE
 **/
void showErrorMessage(GtkLabel *errorLabel, int errorCode){
  
  switch(errorCode){
  case 200:
    gtk_label_set_text(errorLabel, "ERROR:コマンドリクエストの引数エラー");
    break;
  case 300:
    gtk_label_set_text(errorLabel, "ERROR:データベースエラー");
    break;
  case 400:
    gtk_label_set_text(errorLabel, "ERROR:種類またはIDが見つかりません");
    break;
  case 5000:
    gtk_label_set_text(errorLabel, "ERROR:権限がありません");
    break;
  case 5100:
    gtk_label_set_text(errorLabel,"ERROR:ログインエラー");
    break;
  default:
    gtk_label_set_text(errorLabel, "ERROR:致命的なエラーが発生しました");
  }
}
