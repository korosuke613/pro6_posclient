/**
 * pos_main.c
 **/

#include "pos_client.h"

int g_soc = -1;       //クライアントのソケットディスクリプタ
int g_permissionId = 0; //権限ID デフォルト:0 権限なし
int g_storeId = 0;

/** 
 * コールバック関数 
 * cb_pos_quit
 **/
G_MODULE_EXPORT void cb_pos_quit(GtkButton *button, gpointer data){
	if(g_soc > 0){
		/* サーバとの接続を切断 */
		close(g_soc);
	}
	gtk_main_quit();
}




int main(int argc, char *argv[]){
	GtkBuilder *builder;        //GtkBuilderオブジェクト
	MainHandleData *hData;      //メイン画面の主要Widget保持用
	char       labelBuf[BUFSIZE];
	int        port;
    const char *userStr, *passwordStr;
    char sendBuf[BUFSIZE], recvBuf[BUFSIZE_MAX];
    int sendLen, recvLen, recordCount, n;
    char *records[RECORD_MAX], response[BUFSIZE], param1[BUFSIZE], param2[BUFSIZE], param3[BUFSIZE], param4[BUFSIZE];

	if(argc != 3){
		printf("Usage: %s server-ipaddr port\n", argv[0]);
		exit(1);
	}

	port = atoi(argv[2]);

	/* サーバに接続要求 */
	if((g_soc = setup_connect(argv[1], port)) < 0){
		exit(1);
	}

	/* GTK+の初期化  */
	gtk_init(&argc, &argv);

	/* メイン画面の主要Widget保持用構造体を作成 */
	hData = (MainHandleData *)malloc(sizeof(MainHandleData));

	/* GtkBuilderオブジェクトの生成 */ 
	builder = gtk_builder_new();
	/* Gladeで作成したファイルからUIを読み込む */
	gtk_builder_add_from_file(builder, "pos_main.glade", NULL);

	/* 主要Widgetを保持 */
	hData->mainWindow           = GTK_WIDGET( gtk_builder_get_object(builder, "mainWindow") );
	hData->areaLabel            = GTK_LABEL( gtk_builder_get_object(builder, "areaLabel") );
	hData->storeLabel           = GTK_LABEL( gtk_builder_get_object(builder, "storeLabel") );
	hData->connectLabel         = GTK_LABEL( gtk_builder_get_object(builder, "connectLabel") );
	hData->salesButton          = GTK_BUTTON( gtk_builder_get_object(builder, "salesButton") );
	hData->stockButton          = GTK_BUTTON( gtk_builder_get_object(builder, "stockButton") );
	hData->purchaseButton       = GTK_BUTTON( gtk_builder_get_object(builder, "purchaseButton") );
	hData->pointButton          = GTK_BUTTON( gtk_builder_get_object(builder, "pointButton") );
	hData->productButton        = GTK_BUTTON( gtk_builder_get_object(builder, "productButton") );
	hData->customerButton       = GTK_BUTTON( gtk_builder_get_object(builder, "customerButton") );
	hData->salesTotalButton     = GTK_BUTTON( gtk_builder_get_object(builder, "salesTotalButton") );
	hData->salesAnalysisButton  = GTK_BUTTON( gtk_builder_get_object(builder, "salesAnalysisButton") );
	hData->abcAnalysisButton    = GTK_BUTTON( gtk_builder_get_object(builder, "abcAnalysisButton") );

	/* コネクション先のサーバ情報を表示 */
	sprintf(labelBuf, "Server: %s Port: %d", argv[1], port);
	gtk_label_set_text(hData->connectLabel, labelBuf);

	/* ログインなしを表示（地域、店舗)\ */
	gtk_label_set_text(hData->areaLabel, "【地域】 :");
	gtk_label_set_text(hData->storeLabel, "【店舗】 :");
	/* 各機能ボタンを無効化 */

	gtk_widget_set_sensitive( GTK_WIDGET(hData->salesButton), FALSE );
	gtk_widget_set_sensitive( GTK_WIDGET(hData->stockButton), FALSE );  
	gtk_widget_set_sensitive( GTK_WIDGET(hData->purchaseButton), FALSE ); 
	gtk_widget_set_sensitive( GTK_WIDGET(hData->pointButton), FALSE );

	gtk_widget_set_sensitive( GTK_WIDGET(hData->productButton), FALSE );  
	gtk_widget_set_sensitive( GTK_WIDGET(hData->customerButton), FALSE );  
	gtk_widget_set_sensitive( GTK_WIDGET(hData->salesTotalButton), FALSE );  
	gtk_widget_set_sensitive( GTK_WIDGET(hData->salesAnalysisButton), FALSE );  
	gtk_widget_set_sensitive( GTK_WIDGET(hData->abcAnalysisButton), FALSE );  

	/* UIで設定されているシグナルに対するコールバック関数を登録 */
	gtk_builder_connect_signals(builder, hData);

	/* メイン・ウィンドウ上のすべてウィジェットを表示 */
	gtk_widget_show_all(hData->mainWindow);



    /*通信用のソケットディスクリプタが空でないかチェック*/
    if(g_soc>0){
        sendLen = sprintf(sendBuf, "%s %s %s %s",LOGIN,"101","password",ENTER);
        send(g_soc, sendBuf, sendLen, 0);
        recvLen=recv_data(g_soc, recvBuf, BUFSIZE_MAX);
        recordCount=record_division(recvBuf, records);
        memset(response,0,BUFSIZE);
        memset(param1,0,BUFSIZE);
        memset(param2,0,BUFSIZE);
        memset(param3,0,BUFSIZE);
        memset(param4,0,BUFSIZE);
        
        /* レスポンスメッセージを解析 */
        n = sscanf(records[0], "%s %s %s %s %s", response, param1, param2, param3, param4);
        

        set_capability_enable(hData, param4, param3, atoi(param1), atoi(param2));
    }



	/* メインループ */
	gtk_main();

	return 0;
}


