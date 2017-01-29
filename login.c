/**
 * login.c
 **/

#include "pos_client.h"

void showErrorMsg(GtkLabel *erroLabel, int errorCode);
void set_capability_enable(MainHandleData *hData, char *area, char *store, int storeId, int permissionId);

/**
 * コールバック関数
 * cb_login_win_open
 **/
G_MODULE_EXPORT void cb_login_win_open(GtkButton *button, gpointer data){
	GtkBuilder      *builder;
	MainHandleData  *hData;      //メイン画面の主要Widget保持用
	LoginHandleData *loginHData; //ログイン画面の主要Widget保持用

	/* 引数(data)をメイン画面主要Widget保持用構造体型にキャスト */
	hData = (MainHandleData *)data;

	/* ログイン画面の主要Widget保持用構造体を作成 */
	loginHData = (LoginHandleData *)malloc(sizeof(LoginHandleData));

	/* login.gladeを読み込む */
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "login.glade", NULL);
	/* 主要Widgetを保持 */
	loginHData->loginWindow     = GTK_WIDGET( gtk_builder_get_object(builder, "loginWindow") );
	loginHData->storeIdEntry    = GTK_ENTRY( gtk_builder_get_object(builder, "storeIdEntry") );
	loginHData->passwordEntry   = GTK_ENTRY( gtk_builder_get_object(builder, "passwordEntry") );
	loginHData->errorLabel      = GTK_LABEL( gtk_builder_get_object(builder, "errorLabel") );

	/* ログイン画面主要Widget保持用構造体のメンバーとしてメイン画面の主要Widget保持用構造体をセット */
	loginHData->mainHandleData = hData;

	/* シグナル、シグナルハンドラ、ユーザデータ登録 */
	gtk_builder_connect_signals(builder, loginHData);

	/* ログイン画面表示 */
	gtk_widget_show_all(loginHData->loginWindow);
}

/**
 * コールバック関数：「キャンセル」ボタンのクリックによりウィンドウを非表示にする
 * cb_login_cancel
 **/
G_MODULE_EXPORT void cb_login_cancel(GtkButton *button, gpointer data){
	LoginHandleData *loginHData; //ログイン画面の主要Widget保持用

	/* ログイン画面主要Widget保持用構造体を取得 */
	loginHData =(LoginHandleData *)data;

	/* ログイン画面（ウィンドウ）を非表示 */
	gtk_widget_hide(loginHData->loginWindow);
	/* ログイン画面主要Widget保持構造体を破棄（メモリ開放) */
	free(loginHData);

}

/**
 * コールバック関数：「ログイン」ボタンのクリックによるログイン処理
 * cb_login_exec
 **/
G_MODULE_EXPORT void cb_login_exec(GtkButton *button, gpointer data){
	MainHandleData *hData;       //メイン画面の主要Widget保持用
	LoginHandleData *loginHData; //ログイン画面の主要Widget保持用
	const char *userStr, *passwordStr; 
	char sendBuf[BUFSIZE], recvBuf[BUFSIZE_MAX];
	int sendLen, recvLen, recordCount, n;
	char *records[RECORD_MAX], response[BUFSIZE], param1[BUFSIZE], param2[BUFSIZE], param3[BUFSIZE], param4[BUFSIZE];


	loginHData = (LoginHandleData *)data;
	hData = loginHData->mainHandleData;

	/* ユーザIDに入力されている文字列を取得 */
	userStr = gtk_entry_get_text(loginHData->storeIdEntry);
	if(strlen(userStr)<1){
		gtk_label_set_text(loginHData->errorLabel, "ERROR:ユーザ名を入力してください");
		return;
	}
	/* パスワードに入力されている文字列を取得 */
	passwordStr = gtk_entry_get_text(loginHData->passwordEntry);
	if(strlen(passwordStr)<1){
		gtk_label_set_text(loginHData->errorLabel, "ERROR:パスワードを入力してください");
		return;
	}

	/*通信用のソケットディスクリプタが空でないかチェック*/
	if(g_soc>0){
		sendLen = sprintf(sendBuf, "%s %s %s %s",LOGIN,userStr,passwordStr,ENTER);
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



		if(strcmp(response, OK_STAT) != 0){
			/* エラーメッセージを表示 */
			showErrorMsg(loginHData->errorLabel, atoi(param1));
			return;
		}
		
		set_capability_enable(hData, param4, param3, atoi(param1), atoi(param2));
		
		/* ログイン画面（ウィンドウ）を非表示 */
		gtk_widget_hide(loginHData->loginWindow);
		/* ログイン画面主要Widget保持構造体を破棄（メモリ開放) */
		free(loginHData);
	
	}


}
/**
 * 権限によるメイン画面機能の使用許可設定
 * set_capability_enable 
 *  [PARAMETER]
 *   MainHandleData *hData :  メイン画面主要Widget保持用構造体のポインタ
 *   char *area            :  地域名
 *   char *store           :  店舗名
 *   int storeId           :  店舗ID
 *   int permissionId      :  権限ID
 *  [RETURN]
 *    NONE
 **/
void set_capability_enable(MainHandleData *hData, char *area, char *store, int storeId, int permissionId){
	char labelBuf1[BUFSIZE], labelBuf2[BUFSIZE];

	sprintf(labelBuf1,"【地域】：%s", area);
	gtk_label_set_text(hData->areaLabel, labelBuf1);

	sprintf(labelBuf2,"【店舗】：%s  %d", store, storeId);
	gtk_label_set_text(hData->storeLabel, labelBuf2);
	
	g_storeId = storeId;

	switch(permissionId){
		case 10: //店舗
			gtk_widget_set_sensitive( GTK_WIDGET(hData->salesButton), TRUE );  
			gtk_widget_set_sensitive( GTK_WIDGET(hData->stockButton), TRUE );  
			gtk_widget_set_sensitive( GTK_WIDGET(hData->purchaseButton), TRUE );  
			gtk_widget_set_sensitive( GTK_WIDGET(hData->pointButton), TRUE );  
			gtk_widget_set_sensitive( GTK_WIDGET(hData->productButton), FALSE );  
			gtk_widget_set_sensitive( GTK_WIDGET(hData->customerButton), FALSE );  
			gtk_widget_set_sensitive( GTK_WIDGET(hData->salesTotalButton), FALSE );  
			gtk_widget_set_sensitive( GTK_WIDGET(hData->salesAnalysisButton), FALSE );  
			gtk_widget_set_sensitive( GTK_WIDGET(hData->abcAnalysisButton), FALSE );  
			break;  
		case 1: //本部
			gtk_widget_set_sensitive( GTK_WIDGET(hData->salesButton), FALSE );  
			gtk_widget_set_sensitive( GTK_WIDGET(hData->stockButton), FALSE );  
			gtk_widget_set_sensitive( GTK_WIDGET(hData->purchaseButton), FALSE );  
			gtk_widget_set_sensitive( GTK_WIDGET(hData->pointButton), FALSE );  
			gtk_widget_set_sensitive( GTK_WIDGET(hData->productButton), TRUE );  
			gtk_widget_set_sensitive( GTK_WIDGET(hData->customerButton), TRUE );  
			gtk_widget_set_sensitive( GTK_WIDGET(hData->salesTotalButton), TRUE );  
			gtk_widget_set_sensitive( GTK_WIDGET(hData->salesAnalysisButton), TRUE );  
			gtk_widget_set_sensitive( GTK_WIDGET(hData->abcAnalysisButton), TRUE );  
			break;  
	}

	g_permissionId = permissionId;  //権限IDをグローバル変数に設定

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
void showErrorMsg(GtkLabel *errorLabel, int errorCode){

	switch(errorCode){
		case 200:
			gtk_label_set_text(errorLabel, "ERROR:コマンドリクエストの引数エラー");
			break;
		case 300:
			gtk_label_set_text(errorLabel, "ERROR:データベースエラー");
			break;
		case 5100:
			gtk_label_set_text(errorLabel, "ERROR:ログインエラー");
			break;

		default:
			gtk_label_set_text(errorLabel, "ERROR:致命的なエラーが発生しました");
			break;
	}
}
