#include "pos_client.h"
#include "productadd.h"

int g_prodaddWindowFlag =0;
void addshowErrorMsg(GtkLabel *resultLabel, int errorCode);

/**
 * コールバック関数：「商品情報」ボタンのクリックにより商品情報画面を表示
 * cb_prodadd_win_open
 **/
G_MODULE_EXPORT void cb_prodadd_win_open(GtkButton *button, gpointer data){
  GtkBuilder        *builder;
  ProdaddHandleData *hData; //商品管理画面の主要Widget保持用

  /* 商品管理画面が表示されていない場合 */
  if(g_prodaddWindowFlag == 0){
    /* 商品管理画面の主要Widget保持用構造体を作成 */
    hData = (ProdaddHandleData *)malloc(sizeof(ProdaddHandleData));
    /* product_manage_main.gladeを読み込む */
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "product_add.glade", NULL);
    /* 扱う必要のあるWidgetを構造体hDataのメンバに保持 */
    /* メンバ"prod_mainWindow"には商品管理画面のウィジェット */
    hData->addWindow = GTK_WIDGET( gtk_builder_get_object(builder, "addWindow") );
    hData->prod_idLabel = GTK_LABEL( gtk_builder_get_object(builder, "prod_idLabel") );
    hData->prod_idEntry    = GTK_ENTRY( gtk_builder_get_object(builder, "prod_idEntry") );
    hData->prod_nameLabel = GTK_LABEL( gtk_builder_get_object(builder, "prod_nameLabel") );
    hData->prod_nameEntry    = GTK_ENTRY( gtk_builder_get_object(builder, "prod_nameEntry") );
    hData->prod_smallcateLabel = GTK_LABEL( gtk_builder_get_object(builder, "prod_smallcateLabel") );
    hData->prod_smallcateEntry    = GTK_ENTRY( gtk_builder_get_object(builder, "prod_smallcateEntry") );
    hData->prod_priceLabel = GTK_LABEL( gtk_builder_get_object(builder, "prod_priceLabel") );
    hData->prod_priceEntry    = GTK_ENTRY( gtk_builder_get_object(builder, "prod_priceEntry") );
    hData->resultLabel = GTK_LABEL( gtk_builder_get_object(builder, "resultLabel"));
   hData->okButton = GTK_BUTTON( gtk_builder_get_object(builder, "okButton") );
   hData->cancellButton = GTK_BUTTON( gtk_builder_get_object(builder, "cancellButton") );


    /* シグナル、シグナルハンドラ、ユーザデータ登録 */
    gtk_builder_connect_signals(builder, hData);
    /* 商品管理画面表示 */
    gtk_widget_show_all(hData->addWindow);
    /* 商品管理画面表示フラグをセット */
    g_prodaddWindowFlag = 1;
  }

}




G_MODULE_EXPORT void cb_prodadd_cancel(GtkButton *button, gpointer data){
  ProdaddHandleData *hData; //         Widget   

  hData = (ProdaddHandleData *)data;
  /*                   */
  gtk_widget_hide(hData->addWindow);
  /*         Widget              ) */
  free(hData);
  /*                 */
  g_prodaddWindowFlag = 0;

}





G_MODULE_EXPORT void cb_prodadd_exec(GtkButton *button, gpointer data){
  ProdaddHandleData *hData;       //メイン画面の主要Widget保持用
  const char *idStr,*nameStr,*scateStr,*priceStr;
  int recordCount,n,sendLen,recvLen;
  char *records[RECORD_MAX];
  char sendBuf[BUFSIZE],recvBuf[BUFSIZE_MAX];
  char response[BUFSIZE],param1[BUFSIZE],param2[BUFSIZE],param3[BUFSIZE],param4[BUFSIZE];



  hData = (ProdaddHandleData *)data;

  idStr = gtk_entry_get_text(hData->prod_idEntry);
  if(strlen(idStr) < 1){
    gtk_label_set_text(hData->resultLabel, "ERROR:商品IDを入力してください");
    return;
  }
  
  nameStr = gtk_entry_get_text(hData->prod_nameEntry);
  if(strlen(nameStr) < 1){
    gtk_label_set_text(hData->resultLabel, "ERROR:商品名を入力してください");
    return;
  }
  
  scateStr = gtk_entry_get_text(hData->prod_smallcateEntry);
  if(strlen(scateStr) < 1){
    gtk_label_set_text(hData->resultLabel, "ERROR:小カテゴリを入力してください");
    return;
  }

  priceStr = gtk_entry_get_text(hData->prod_priceEntry);
  if(strlen(priceStr) < 1){
    gtk_label_set_text(hData->resultLabel, "ERROR:商品価格を入力してください");
    return;
  }



  if(g_soc > 0){
    sendLen = sprintf(sendBuf, "%s %d %s %d %d %s",PROD_INFO_APPEND,atoi(idStr),nameStr,atoi(scateStr),atoi(priceStr), ENTER);

    send(g_soc, sendBuf, sendLen, 0);


    recvLen = recv_data(g_soc, recvBuf,BUFSIZE_MAX);

    recordCount = record_division(recvBuf,records);

    memset(response, 0, BUFSIZE);
    memset(param1, 0, BUFSIZE);
    memset(param2, 0, BUFSIZE);
    memset(param3, 0, BUFSIZE);
    memset(param4, 0, BUFSIZE);


    n = sscanf(records[0], "%s %d %s %d %d", response, param1, param2, param3, param4);

      if(strcmp(response, OK_STAT) != 0){
      /* エラーメッセージを表示 */
      addshowErrorMsg(hData->resultLabel, atoi(param1));
      return;
    }

    gtk_label_set_text(hData->resultLabel, records[0]);
  

  }

  return;

}


void addshowErrorMsg(GtkLabel *resultLabel, int errorCode){

  switch(errorCode){
  default:
    gtk_label_set_text(resultLabel, "このIDにはすでに登録されています");
    break;

  }

}

