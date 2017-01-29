#include "pos_client.h"
#include "point.h"
int g_pointWindowFlag=0;
void pointshowErrorMsg(GtkLabel *resultLabel, int errorCode);

/**
 * コールバック関数：「ポイントカード発行」ボタンのクリックによりポイントカード発行画面を表示
 * cb_point_win_open
 **/
G_MODULE_EXPORT void cb_point_win_open(GtkButton *button, gpointer data){
  GtkBuilder        *builder;
  pointHandleData *hData; //ポイントカード画面の主要Widget保持用

  /* ポイントカード画面が表示されていない場合 */
  if(g_pointWindowFlag == 0){
    /* ポイントカード画面の主要Widget保持用構造体を作成 */
    hData = (pointHandleData *)malloc(sizeof(pointHandleData));
    /* point.gladeを読み込む */
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "point.glade", NULL);
    /* 扱う必要のあるWidgetを構造体hDataのメンバに保持 */
    /* メンバ"pointWindow"にはポイントカード画面のウィジェット */
    hData->pointWindow = GTK_WIDGET( gtk_builder_get_object(builder, "pointWindow") );
    hData->cardEntry    = GTK_ENTRY( gtk_builder_get_object(builder, "cardEntry") );
    hData->nameEntry    = GTK_ENTRY( gtk_builder_get_object(builder, "nameEntry") );
    hData->menradioButton  = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "menradioButton") );
    hData->womenradioButton  = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "womenradioButton") );
    hData->dayEntry    = GTK_ENTRY( gtk_builder_get_object(builder, "dayEntry") );
    hData->addEntry    = GTK_ENTRY( gtk_builder_get_object(builder, "addEntry") );
    hData->tellEntry    = GTK_ENTRY( gtk_builder_get_object(builder, "tellEntry") );
    hData->mailEntry    = GTK_ENTRY( gtk_builder_get_object(builder, "mailEntry") );
    hData->resultLabel      = GTK_LABEL( gtk_builder_get_object(builder, "resultLabel") );
    /* シグナル、シグナルハンドラ、ユーザデータ登録 */
    gtk_builder_connect_signals(builder, hData);
    /* ポイントカード画面表示 */
    gtk_widget_show_all(hData->pointWindow);
    /* ポイントカード画面表示フラグをセット */
    g_pointWindowFlag = 1;
  }
}


G_MODULE_EXPORT void cb_point_cancel(GtkButton *button, gpointer data){
  pointHandleData *hData; //         Widget   

  hData = (pointHandleData *)data;
  /*                   */
  gtk_widget_hide(hData->pointWindow);
  /*         Widget              ) */
  free(hData);
  /*                 */
  g_pointWindowFlag = 0;

}


G_MODULE_EXPORT void cb_point_exec(GtkButton *button, gpointer data){
  pointHandleData *pointhData;       //メイン画面の主要Widget保持用
  const char *cardStr,*nameStr,*dayStr,*addStr,*tellStr,*mailStr;
  int gender,recordCount,n,sendLen,recvLen;
  char *records[RECORD_MAX];
  char sendBuf[BUFSIZE],recvBuf[BUFSIZE_MAX];
  char response[BUFSIZE],param1[BUFSIZE];



  pointhData = (pointHandleData *)data;

  /*1,カード番号「cardEntry」に入力された値を取り出し適当な変数に格納する*/
  cardStr = gtk_entry_get_text(pointhData->cardEntry);
  if(strlen(cardStr) < 1){
    gtk_label_set_text(pointhData->resultLabel,"ERROR:カード番号を入力してください");
    return;
  }

  /*2,氏名「nameEntry」に入力された値を取り出し適当な変数に格納する*/
  nameStr = gtk_entry_get_text(pointhData->nameEntry);
  if(strlen(nameStr) < 1){
    gtk_label_set_text(pointhData->resultLabel,"ERROR:氏名を入力してください");
    return;
  }



  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pointhData->menradioButton))){
    gender=1;
  } else if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pointhData->womenradioButton))){
    gender=0;
  }




  /*3,生年月日「dayEntry」に入力された値を取り出し適当な変数に格納する*/
  dayStr = gtk_entry_get_text(pointhData->dayEntry);
  if(strlen(dayStr) < 1){
    gtk_label_set_text(pointhData->resultLabel,"ERROR:生年月日を入力してください");
    return;
  }



  /*4,住所「addEntry」に入力された値を取り出し適当な変数に格納する*/
  addStr = gtk_entry_get_text(pointhData->addEntry);
  if(strlen(addStr) < 1){
    gtk_label_set_text(pointhData->resultLabel,"ERROR:住所を入力してください");
    return;
  }


  /*5,TEL「tellEntry」に入力された値を取り出し適当な変数に格納する*/
  tellStr = gtk_entry_get_text(pointhData->tellEntry);
  if(strlen(tellStr) < 1){
    gtk_label_set_text(pointhData->resultLabel,"ERROR:電話番号(TEL)を入力してください");
    return;
  }

  /*6,Email「mailEntry」に入力された値を取り出し適当な変数に格納する*/
  mailStr = gtk_entry_get_text(pointhData->mailEntry);
  if(strlen(mailStr) < 1){
    gtk_label_set_text(pointhData->resultLabel,"ERROR:Emailを入力してください");
    return;
  }

  if(g_soc > 0){
    sendLen = sprintf(sendBuf, "%s %s %s %d %s %s %s %s %s",POINT_CREATE,cardStr,nameStr,gender,dayStr,addStr,tellStr,mailStr, ENTER );

    send(g_soc, sendBuf, sendLen, 0);


    recvLen = recv_data(g_soc, recvBuf,BUFSIZE_MAX);

    recordCount = record_division(recvBuf,records);

    memset(response, 0, BUFSIZE);
    memset(param1, 0, BUFSIZE);


    n = sscanf(records[0], "%s %s", response, param1);

    if(strcmp(response, OK_STAT) != 0){
      /* エラーメッセージを表示 */
      pointshowErrorMsg(pointhData->resultLabel, atoi(param1));
      return;
    }

    gtk_label_set_text(pointhData->resultLabel, records[0]);
  

  }

  return;

}


void pointshowErrorMsg(GtkLabel *resultLabel, int errorCode){

  switch(errorCode){
  default:
    gtk_label_set_text(resultLabel, "このカード番号はすでに使われています");
    break;

  }

}
