/*
 * Copyright (c) 2016
 *    Futa Hirakoba.  All rights reserved.
 *
 * sales.c
 * 最終更新日時：2016.12.12 16:00
 *
 */

/**
 * sales.c
 **/

#include "sales.h"

int g_sales1WindowFlag = 0; //販売会計
int g_sales2WindowFlag = 0;
int g_sales3WindowFlag = 0;
int g_sales4WindowFlag = 0;
int g_sales5WindowFlag = 0;
int g_sales6WindowFlag = 0;

_salesHandleData *salesHData;

void showSalesErrorMsg(GtkLabel *errorLabel, int errorCode);
void showSalesOkMsg(GtkLabel *okLabel, int okCode);
void setTextView(GtkTextView *textview, const gchar *text);
int check_stock(int productid);

G_MODULE_EXPORT void cb_sales1_win_open(GtkButton *button, gpointer data){
	GtkBuilder			*builder;
	int i;
	gint value = 1, min = 0, max = 100;
	gint step = 1;
	GtkWidget *tmpSpinbutton;

	/* 残高照会画面が表示されていない場合 */
	if(g_sales1WindowFlag == 0){ 

		/* 残高照会画面の主要Widget保持用構造体を作成 */
		salesHData = (_salesHandleData *)malloc(sizeof(_salesHandleData));
		/* sales1.gladeを読み込む */
		builder = gtk_builder_new();
		gtk_builder_add_from_file(builder, "sales.glade", NULL);
		
		salesHData->nopointDataMax = 0;

		/* salesWindow */
		salesHData->salesWindow = GTK_WIDGET( gtk_builder_get_object(builder, "salesWindow") );
		salesHData->cancelButton1 = GTK_BUTTON( gtk_builder_get_object(builder, "cancelButton1") );
		salesHData->cancelButton3 = GTK_BUTTON( gtk_builder_get_object(builder, "cancelButton3") );
		salesHData->nextButton = GTK_BUTTON( gtk_builder_get_object(builder, "nextButton"));
		salesHData->maleRadiobutton = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "maleRadiobutton"));
		salesHData->femaleRadiobutton = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "femaleRadiobutton"));
		salesHData->valueSpinbutton = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "valueSpinbutton"));
		salesHData->noticeLabel = GTK_LABEL(gtk_builder_get_object(builder, "noticeLabel"));
		salesHData->ageComboBox = GTK_COMBO_BOX(gtk_builder_get_object(builder, "ageComboBox"));
		salesHData->ageModel = GTK_LIST_STORE(gtk_combo_box_get_model(salesHData->ageComboBox));
		salesHData->weatherComboBox = GTK_COMBO_BOX(gtk_builder_get_object(builder, "weatherComboBox"));
		salesHData->weatherModel = GTK_LIST_STORE(gtk_combo_box_get_model(salesHData->weatherComboBox));
		salesHData->productidEntry = GTK_ENTRY( gtk_builder_get_object(builder, "productidEntry") );
		salesHData->KionEntry = GTK_ENTRY( gtk_builder_get_object(builder, "KionEntry"));

		/* breakDialog */
		salesHData->breakDialog = GTK_WIDGET( gtk_builder_get_object(builder, "breakDialog") );

		/* pointcardWindow */
		salesHData->pointcardWindow = GTK_WIDGET( gtk_builder_get_object(builder, "pointcardWindow") );
		salesHData->pointidEntry = GTK_ENTRY( gtk_builder_get_object(builder, "pointidEntry") );
		salesHData->pointresultLabel = GTK_LABEL( gtk_builder_get_object(builder, "pointresultLabel") );
		salesHData->pointcardButton = GTK_BUTTON(gtk_builder_get_object(builder, "pointcardButton"));
		salesHData->pointLogin = 0;
		
		/* resultdangerDialog */
		salesHData->resultdangerDialog = GTK_WIDGET( gtk_builder_get_object(builder, "resultdangerDialog") );

		/* resultWindow */
		salesHData->resultWindow = GTK_WIDGET( gtk_builder_get_object(builder, "resultWindow") );
		salesHData->totalmoneyLabel = GTK_LABEL(gtk_builder_get_object(builder, "totalmoneyLabel"));
		salesHData->havepointLabel = GTK_LABEL(gtk_builder_get_object(builder, "havepointLabel"));
		salesHData->moneyEntry = GTK_ENTRY(gtk_builder_get_object(builder, "moneyEntry"));
		salesHData->usepointEntry = GTK_ENTRY(gtk_builder_get_object(builder, "usepointEntry"));
		salesHData->okButton5 = GTK_BUTTON(gtk_builder_get_object(builder, "okButton5"));

		/* endDialog */
		salesHData->endDialog = GTK_WIDGET( gtk_builder_get_object(builder, "endDialog"));
		salesHData->reciptBox = GTK_TEXT_VIEW( gtk_builder_get_object(builder, "reciptBox"));


		/* salesWindowのツリービュー */
		for(i=0;i<6;i++){
			gtk_list_store_append(salesHData->ageModel,&(salesHData->ageIter));
			gtk_list_store_set(salesHData->ageModel, &(salesHData->ageIter), 0, ageData[i], -1);
		}
		for(i=0;i<4;i++){
			gtk_list_store_append(salesHData->weatherModel,&(salesHData->weatherIter));
			gtk_list_store_set(salesHData->weatherModel, &(salesHData->weatherIter), 0, weatherData[i], -1);
		}

		salesHData->productTree =  GTK_TREE_VIEW(gtk_builder_get_object(builder, "treeview1"));
		salesHData->productModel =  GTK_LIST_STORE(gtk_tree_view_get_model(salesHData->productTree));

		gtk_widget_set_sensitive(GTK_WIDGET(salesHData->nextButton), FALSE);

		gtk_list_store_clear(salesHData->productModel);

		/* シグナル、シグナルハンドラ、ユーザデータ登録 */
		gtk_builder_connect_signals(builder, salesHData);
		/* 残高照会画面表示 */
		gtk_widget_show_all(salesHData->salesWindow);
		/* 残高照会画面表示フラグをセット */
		g_sales1WindowFlag = 1;
	}   
}


G_MODULE_EXPORT void cb_sales1_send(GtkButton *button, gpointer data){
	const char *tmp;
	char sendBuf[BUFSIZE], recvBuf[BUFSIZE_MAX];
	int sendLen, recvLen, recordCount=0;
	char *records[RECORD_MAX], response[BUFSIZE], param[9][BUFSIZE];
	int i, productPrice, no;
	char productName[BUFSIZE], productidStr[BUFSIZE], purchaseidStr[BUFSIZE];

	tmp = gtk_entry_get_text(salesHData->productidEntry);
	strncpy(productidStr, tmp, 5);
	productidStr[5]='\0';

	strcpy(purchaseidStr, tmp+5);

	/*通信用のソケットディスクリプタが空でないかチェック*/
	if(g_soc>0){
		if(salesHData->pointLogin == 0){
			sendLen = sprintf(sendBuf, "%s %s %s","PURCHASE_DT_DISP",purchaseidStr,ENTER);
		}else{
			sendLen = sprintf(sendBuf, "%s %s %s %s %s", "SALE", productidStr, purchaseidStr, "1", ENTER);
			printf("%s", sendBuf);
		}
		send(g_soc, sendBuf, sendLen, 0);
		recvLen=recv_data(g_soc, recvBuf, BUFSIZE_MAX);
		recordCount=record_division(recvBuf, records);
		memset(response,0,BUFSIZE);
		for(i=0;i<9;i++){
			memset(param[i],0,BUFSIZE);
		}

		if(salesHData->pointLogin == 0){
			
			if(!check_stock(atoi(productidStr))){
				gtk_label_set_text(salesHData->noticeLabel, "ERROR:在庫切れ");
				return;
			}
			/* レスポンスメッセージを解析 */
			sscanf(records[0], "%s %s", response, param[0]);

			for(i=1;i<recordCount;i++){
				sscanf(records[i], "%s %s %s %s %s %s %s", param[0], param[1], param[2], param[3], param[4], param[5], param[6]);
				if(atoi(param[1]) == atoi(productidStr)){					
					if(check_rimit_day(param[6])){
						gtk_label_set_text(salesHData->noticeLabel, "ERROR:賞味期限切れ");
						return;
					}
					salesHData->nopointDataMax++;
					strcpy(productName, param[2]);
					productPrice = (int)((double)atoi(param[3])/0.648);
					salesHData->nopointData[salesHData->nopointDataMax].productNumber = atoi(productidStr);
					salesHData->nopointData[salesHData->nopointDataMax].purchaseNumber = atoi(purchaseidStr);
					salesHData->nopointData[salesHData->nopointDataMax].buyNumber = 1;
					no = salesHData->nopointDataMax;
					break;
				}
			}
		
		}else{
			sscanf(records[0], "%s %s %s %s %s %s", param[0], param[1], param[2], param[3], param[4], param[5]);
			strcpy(productName, param[2]);
			productPrice = atoi(param[4]);
			if(!strcmp(param[0], "-ERR")){
				showSalesErrorMsg(salesHData->noticeLabel, atoi(param[1]));
				return;
			}
			showSalesOkMsg(salesHData->noticeLabel, 0);
			no = atoi(param[1]);
		}


		gtk_list_store_append(salesHData->productModel,&(salesHData->productIter));
		gtk_list_store_set(salesHData->productModel, &(salesHData->productIter),
				0, atoi(productidStr), 
				1, atoi(purchaseidStr), 
				2, productName, 
				3, productPrice, 
				4, 1, 
				5, productPrice,
				6, no,
				-1
				);
		gtk_widget_set_sensitive(GTK_WIDGET(salesHData->nextButton), TRUE);


	}

}

int check_rimit_day(char *date){
	time_t now;
	struct tm *ltm;
	char day[3], year[5], month[3];
	
	time(&now);
	ltm = localtime(&now);

	strncpy(year, date, 4);
	year[4]='\0';

	strncpy(month, date+5, 2);
	month[2]='\0';
	
	strcpy(day, date+8);

	printf("y=%s m=%s d=%s\n", year, month, day);
	printf("y=%d m=%d d=%d\n", atoi(year), atoi(month), atoi(day));

	if(ltm->tm_year+1900 > atoi(year))return 1;
	else if(ltm->tm_year+1900 < atoi(year))return 0;
	else{
		if(ltm->tm_mon+1 > atoi(month))return 1;
		else if(ltm->tm_mon+1 < atoi(month))return 0;
		else{
			if(ltm->tm_mday > atoi(day))return 1;
			else if(ltm->tm_mday <= atoi(day))return 0;
		}
	}
	
	return 0;

}

int check_stock(int productid){
	char sendBuf[BUFSIZE], recvBuf[BUFSIZE_MAX];
	int sendLen, recvLen, recordCount=0;
	char *records[RECORD_MAX], response[BUFSIZE], param[9][BUFSIZE];
	int i;

	sendLen = sprintf(sendBuf, "%s %d %s","STOCK_DISP",g_storeId,ENTER);
	send(g_soc, sendBuf, sendLen, 0);
	recvLen=recv_data(g_soc, recvBuf, BUFSIZE_MAX);
	recordCount=record_division(recvBuf, records);
	memset(response,0,BUFSIZE);
	for(i=0;i<9;i++){
		memset(param[i],0,BUFSIZE);
	}
	/* レスポンスメッセージを解析 */
	sscanf(records[0], "%s %s", response, param[0]);

	for(i=1;i<recordCount;i++){
		sscanf(records[i], "%s %s %s", param[0], param[1], param[2]);
		if(atoi(param[0]) == productid){
			if(atoi(param[2]) > 0)return 1;
			else return 0;
		}
	}
	return 0;
}
/**
 * コールバック関数：「戻る」ボタンのクリックによりウィンドウを非表示にする
 * cb_sales1_win_cancel
 **/
G_MODULE_EXPORT void cb_sales1_win_cancel(GtkButton *button, gpointer data){
	char sendBuf[BUFSIZE], recvBuf[BUFSIZE_MAX];
	int sendLen, recvLen, recordCount=0;
	char *records[RECORD_MAX], response[BUFSIZE], param[9][BUFSIZE];
	int i;

		sendLen = sprintf(sendBuf, "%s%s","CANCEL",ENTER);
		send(g_soc, sendBuf, sendLen, 0);
		recvLen=recv_data(g_soc, recvBuf, BUFSIZE_MAX);
		recordCount=record_division(recvBuf, records);
		memset(response,0,BUFSIZE);
		for(i=0;i<9;i++){
			memset(param[i],0,BUFSIZE);
		}
		/* レスポンスメッセージを解析 */
		sscanf(records[0], "%s", response);


	/* 残高照会画面（ウィンドウ）を非表示 */
	gtk_widget_hide(salesHData->salesWindow);
	gtk_widget_hide(salesHData->breakDialog);
	gtk_widget_hide(salesHData->pointcardWindow);
	gtk_widget_hide(salesHData->resultWindow);
	gtk_widget_hide(salesHData->resultdangerDialog);
	gtk_widget_hide(salesHData->endDialog);

	/* 残高照会画面主要Widget保持構造体を破棄（メモリ開放) */
	free(salesHData);
	/* 残高照会画面表示フラグをクリア */
	g_sales1WindowFlag = 0;
	g_sales2WindowFlag = 0;
	g_sales3WindowFlag = 0;
	g_sales4WindowFlag = 0;
	g_sales5WindowFlag = 0;
	g_sales6WindowFlag = 0;
}

/**
 * コールバック関数
 * cb_balance_exec
 **/



G_MODULE_EXPORT void cb_sales2_win_open(GtkButton *button, gpointer data){

	/* 残高照会画面が表示されていない場合 */
	if(g_sales2WindowFlag == 0){ 
		gtk_widget_show_all(salesHData->breakDialog);
		gtk_widget_set_sensitive( GTK_WIDGET(salesHData->salesWindow), FALSE );
		if(g_sales5WindowFlag == 1)gtk_widget_set_sensitive( GTK_WIDGET(salesHData->resultWindow), FALSE );
		/* 残高照会画面表示フラグをセット */
		g_sales2WindowFlag = 1;
	}   
}


G_MODULE_EXPORT void cb_sales2_win_cancel(GtkButton *button, gpointer data){
	/* 残高照会画面（ウィンドウ）を非表示 */
	gtk_widget_hide(salesHData->breakDialog);
	if(g_sales5WindowFlag == 0)gtk_widget_set_sensitive( GTK_WIDGET(salesHData->salesWindow), TRUE );
	if(g_sales5WindowFlag == 1)gtk_widget_set_sensitive( GTK_WIDGET(salesHData->resultWindow), TRUE );
	g_sales2WindowFlag = 0;
}


G_MODULE_EXPORT void cb_sales3_win_open(GtkButton *button, gpointer data){

	if(salesHData->pointLogin == 0){
		/* 残高照会画面が表示されていない場合 */
		if(g_sales3WindowFlag == 0){ 
			gtk_widget_show_all(salesHData->pointcardWindow);
			gtk_widget_set_sensitive( GTK_WIDGET(salesHData->salesWindow), FALSE );
			/* 残高照会画面表示フラグをセット */
			g_sales3WindowFlag = 1;
		}   
	}
}


G_MODULE_EXPORT void cb_sales3_win_cancel(GtkButton *button, gpointer data){

	/* 残高照会画面（ウィンドウ）を非表示 */
	gtk_widget_hide(salesHData->pointcardWindow);
	gtk_widget_set_sensitive( GTK_WIDGET(salesHData->salesWindow), TRUE );
	g_sales3WindowFlag = 0;
}

G_MODULE_EXPORT void cb_sales3_exec(GtkButton *button, gpointer data){
	char sendBuf[BUFSIZE], recvBuf[BUFSIZE_MAX];
	int sendLen, recvLen, recordCount=0;
	char *records[RECORD_MAX], response[BUFSIZE], param[9][BUFSIZE];
	int i, productPrice;
	const char *pointidStr;
	char labelBuf[BUFSIZE];

	pointidStr = gtk_entry_get_text(salesHData->pointidEntry);
	/*通信用のソケットディスクリプタが空でないかチェック*/
	if(g_soc>0){
		sendLen = sprintf(sendBuf, "%s %d %s %s","SELV_SALE", g_storeId, pointidStr,ENTER);
		send(g_soc, sendBuf, sendLen, 0);
		recvLen=recv_data(g_soc, recvBuf, BUFSIZE_MAX);
		recordCount=record_division(recvBuf, records);
		memset(response,0,BUFSIZE);
		for(i=0;i<9;i++){
			memset(param[i],0,BUFSIZE);
		}
		/* レスポンスメッセージを解析 */
		sscanf(records[0], "%s %s %s", response, param[0], param[1]);

		if(strcmp(response, OK_STAT) != 0){
			/* エラーメッセージを表示 */
			showSalesErrorMsg(salesHData->pointresultLabel, atoi(param[1]));
			return;
		}
		showSalesOkMsg(salesHData->noticeLabel, 0);
		salesHData->pointLogin = 1;
		gtk_widget_set_sensitive(GTK_WIDGET(salesHData->pointcardButton), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(salesHData->ageComboBox), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(salesHData->maleRadiobutton), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(salesHData->femaleRadiobutton), FALSE);

		sprintf(labelBuf,"会員番号：%s", param[1]);
		gtk_button_set_label(salesHData->pointcardButton, labelBuf);
		gtk_widget_hide(salesHData->pointcardWindow);
		gtk_widget_set_sensitive( GTK_WIDGET(salesHData->salesWindow), TRUE );
		g_sales3WindowFlag = 0;
	}
}

G_MODULE_EXPORT void cb_sales1_tree_delete(GtkButton *button, gpointer data){
	char sendBuf[BUFSIZE], recvBuf[BUFSIZE_MAX];
	int sendLen, recvLen, recordCount=0;
	char *records[RECORD_MAX], response[BUFSIZE], param[9][BUFSIZE];
	int i;
	//テスト用リスト削除コード
	GtkListStore *store;
	GtkTreeSelection *selection;
	GtkTreeIter iter;
	gboolean success;
	GtkTreeModel *model;
	gint selectNum;
	gchar *productName;

	selection = gtk_tree_view_get_selection(salesHData->productTree);
	if(!selection)return;

	store = GTK_LIST_STORE(gtk_tree_view_get_model(salesHData->productTree));
	model = GTK_TREE_MODEL(gtk_tree_view_get_model(salesHData->productTree));
	success = gtk_tree_selection_get_selected(selection, NULL, &iter);
	if(success){
		gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 6, &selectNum, -1);
		gtk_list_store_remove(store, &iter);

		if(salesHData->pointLogin == 0){
			salesHData->nopointData[selectNum].buyNumber = 0;
			return;
		}

		if(g_soc>0){
			sendLen = sprintf(sendBuf, "%s %d %s %s","CORRECT", selectNum, "0",ENTER);
			send(g_soc, sendBuf, sendLen, 0);
			recvLen=recv_data(g_soc, recvBuf, BUFSIZE_MAX);
			recordCount=record_division(recvBuf, records);
			memset(response,0,BUFSIZE);
			for(i=0;i<9;i++){
				memset(param[i],0,BUFSIZE);
			}
			/* レスポンスメッセージを解析 */
			sscanf(records[0], "%s %s %s %s %s %s %s", response, param[0], param[1], param[2], param[3], param[4], param[5]);

			if(strcmp(response, OK_STAT) != 0){
				/* エラーメッセージを表示 */
				showSalesErrorMsg(salesHData->pointresultLabel, atoi(param[1]));
				return;
			}
			showSalesOkMsg(salesHData->noticeLabel, 0);
		}
	}
}

G_MODULE_EXPORT void cb_sales1_tree_correct(GtkButton *button, gpointer data){
	char sendBuf[BUFSIZE], recvBuf[BUFSIZE_MAX];
	int sendLen, recvLen, recordCount=0;
	char *records[RECORD_MAX], response[BUFSIZE], param[9][BUFSIZE];
	int i;
	//テスト用リスト削除コード
	GtkListStore *store;
	GtkTreeSelection *selection;
	GtkTreeIter iter;
	gboolean success;
	GtkTreeModel *model;
	gint selectNum;
	const gchar *productName;
	int productid, purchaseid; 
	int productPrice, num;

	selection = gtk_tree_view_get_selection(salesHData->productTree);
	if(!selection)return;

	store = GTK_LIST_STORE(gtk_tree_view_get_model(salesHData->productTree));
	model = GTK_TREE_MODEL(gtk_tree_view_get_model(salesHData->productTree));
	success = gtk_tree_selection_get_selected(selection, NULL, &iter);
	if(success){
		gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, 
				&productid, 1, &purchaseid, 2, &productName, 3,  
				&productPrice, 6, &selectNum, -1);

		//スピンボタンの値を取得
		num = (int)gtk_spin_button_get_value(salesHData->valueSpinbutton);
		if(num < 1){
			return;
		}

			gtk_list_store_set(salesHData->productModel, &iter,
					0, productid, 
					1, purchaseid, 
					2, productName, 
					3, productPrice, 
					4, num,
					5, num*productPrice,
					6, selectNum,
					-1
					);         
		if(salesHData->pointLogin == 0){
			salesHData->nopointData[selectNum].buyNumber = num;
			return;
		}

		if(g_soc>0){
			sendLen = sprintf(sendBuf, "%s %d %d %s","CORRECT", selectNum, num, ENTER);
			send(g_soc, sendBuf, sendLen, 0);
			recvLen=recv_data(g_soc, recvBuf, BUFSIZE_MAX);
			recordCount=record_division(recvBuf, records);
			memset(response,0,BUFSIZE);
			for(i=0;i<9;i++){
				memset(param[i],0,BUFSIZE);
			}
			/* レスポンスメッセージを解析 */
			sscanf(records[0], "%s %s %s %s %s %s %s", 
					response, param[0], param[1], param[2], param[3], param[4], param[5]);
			if(strcmp(response, OK_STAT) != 0){
				/* エラーメッセージを表示 */
				showSalesErrorMsg(salesHData->noticeLabel, atoi(param[0]));
				return;
			}
			//gtk_list_store_append(salesHData->productModel, &iter);

			showSalesOkMsg(salesHData->noticeLabel, 0);
		}
	}
}

G_MODULE_EXPORT void cb_sales4_win_open(GtkButton *button, gpointer data){
	char sendBuf[BUFSIZE], recvBuf[BUFSIZE_MAX];
	int sendLen, recvLen, recordCount=0;
	char *records[RECORD_MAX], response[BUFSIZE], param[9][BUFSIZE];
	int i;


	if(salesHData->pointLogin == 0){
		sendLen = sprintf(sendBuf, "%s %d %s %d %s %s","SELV_SALE", g_storeId, "0", salesHData->selectedGender, salesHData->ageStr, ENTER);
		send(g_soc, sendBuf, sendLen, 0);
		recvLen=recv_data(g_soc, recvBuf, BUFSIZE_MAX);
		recordCount=record_division(recvBuf, records);
		for(i=1;i<=salesHData->nopointDataMax;i++){
			if(salesHData->nopointData[i].buyNumber == 0)continue;
			sendLen = sprintf(sendBuf, "%s %d %d %d %s", 
					"SALE", 
					salesHData->nopointData[i].productNumber, 
					salesHData->nopointData[i].purchaseNumber,
					salesHData->nopointData[i].buyNumber, 
					ENTER);
			printf("%s", sendBuf);
			send(g_soc, sendBuf, sendLen, 0);
			recvLen=recv_data(g_soc, recvBuf, BUFSIZE_MAX);
			recordCount=record_division(recvBuf, records);
			sscanf(records[0], "%s", response);
			printf("%s\n", response);
			if(strcmp(response, OK_STAT) != 0){
				gtk_label_set_text(salesHData->totalmoneyLabel, "エラー");
				gtk_widget_set_sensitive( GTK_WIDGET(salesHData->okButton5), FALSE );
		gtk_widget_show_all(salesHData->resultWindow);
		gtk_widget_set_sensitive( GTK_WIDGET(salesHData->salesWindow), FALSE );
		gtk_widget_hide(salesHData->resultdangerDialog);
		/* 残高照会画面表示フラグをセット */
		g_sales4WindowFlag = 1;
			}	
		}
	}
	/* 残高照会画面が表示されていない場合 */
	if(g_sales4WindowFlag == 0){ 
		sendLen = sprintf(sendBuf, "%s%s","RESULT",ENTER);
		send(g_soc, sendBuf, sendLen, 0);
		recvLen=recv_data(g_soc, recvBuf, BUFSIZE_MAX);
		recordCount=record_division(recvBuf, records);
		memset(response,0,BUFSIZE);
		for(i=0;i<9;i++){
			memset(param[i],0,BUFSIZE);
		}
		/* レスポンスメッセージを解析 */
		sscanf(records[0], "%s %s %s %s", response, param[0], param[1], param[2]);

		salesHData->totalMoney = atoi(param[0]);
		gtk_label_set_text(salesHData->totalmoneyLabel, param[0]);

		if(atoi(param[0]) < atoi(param[1]))salesHData->useAblePoint = atoi(param[0]);
		else salesHData->useAblePoint = atoi(param[1]);

		salesHData->getPoint = atoi(param[2]);

		sprintf(param[8], "%s%d%s", "利用ポイント(最大", salesHData->useAblePoint, ")");
		gtk_label_set_text(salesHData->havepointLabel, param[8]);
		
		gtk_widget_show_all(salesHData->resultWindow);
		gtk_widget_set_sensitive( GTK_WIDGET(salesHData->salesWindow), FALSE );
		gtk_widget_hide(salesHData->resultdangerDialog);
		/* 残高照会画面表示フラグをセット */
		g_sales4WindowFlag = 1;
	}   
}


G_MODULE_EXPORT void cb_sales5_win_open(GtkButton *button, gpointer data){

	GtkTreeModel	*model;
	GtkTreeIter		iter;

	model = gtk_combo_box_get_model(salesHData->weatherComboBox);
	if(!gtk_combo_box_get_active_iter(salesHData->weatherComboBox, &iter)){
			gtk_label_set_text(salesHData->noticeLabel, "ERROR:天気未入力");
		return;
	}
	if(gtk_entry_get_text_length(salesHData->KionEntry) == 0){
			gtk_label_set_text(salesHData->noticeLabel, "ERROR:気温未入力");
		return;
	}
	gtk_tree_model_get(model, &iter, 0, &(salesHData->weatherStr), -1); 

	model = gtk_combo_box_get_model(salesHData->ageComboBox);
	if(salesHData->pointLogin == 0){
		if(!gtk_combo_box_get_active_iter(salesHData->ageComboBox, &iter)){
			gtk_label_set_text(salesHData->noticeLabel, "ERROR:年齢未入力");
			return;
		}
		gtk_tree_model_get(model, &iter, 0, &(salesHData->ageStr), -1); 
		salesHData->ageStr[2]='\0';
		printf("age=%s\n", salesHData->ageStr);
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(salesHData->maleRadiobutton))){
			salesHData->selectedGender = MALE;
			printf("male\n");
		}else{
			printf("female\n");
			salesHData->selectedGender = FEMALE;
		}
	}
	salesHData->KionStr = gtk_entry_get_text(salesHData->KionEntry);


	/* 残高照会画面が表示されていない場合 */
	if(g_sales5WindowFlag == 0){ 
		gtk_widget_set_sensitive( GTK_WIDGET(salesHData->salesWindow), FALSE );
		gtk_widget_show_all(salesHData->resultdangerDialog);
		/* 残高照会画面表示フラグをセット */
		g_sales5WindowFlag = 1;


	}   
}


G_MODULE_EXPORT void cb_sales5_win_cancel(GtkButton *button, gpointer data){
	/* 残高照会画面（ウィンドウ）を非表示 */
	gtk_widget_hide(salesHData->resultdangerDialog);
	gtk_widget_set_sensitive( GTK_WIDGET(salesHData->salesWindow), TRUE );
	g_sales5WindowFlag = 0;
}

G_MODULE_EXPORT void cb_sales6_win_open(GtkButton *button, gpointer data){
	char sendBuf[BUFSIZE], recvBuf[BUFSIZE_MAX];
	int sendLen, recvLen, recordCount=0;
	char *records[RECORD_MAX], response[BUFSIZE], param[16][BUFSIZE], reciptText[BUFSIZE], resultTime[BUFSIZE];
	int i;
	const char *moneyStr, *usepointStr;

	time_t now;
	struct tm *ltm;

	/* 残高照会画面が表示されていない場合 */
	if(g_sales6WindowFlag == 0){ 

		moneyStr = gtk_entry_get_text(salesHData->moneyEntry);
		usepointStr = gtk_entry_get_text(salesHData->usepointEntry);
		salesHData->useMoney = atoi(moneyStr);
		salesHData->usePoint = atoi(usepointStr);

		if(salesHData->useAblePoint < atoi(usepointStr)){
			//showErrorMsg
			return;
		}

		if((salesHData->useMoney + salesHData->usePoint) < salesHData->totalMoney){
			//ここにエラー処理追加
			return;
		}

		sendLen = sprintf(sendBuf, "%s %d %s %s%s", 
				"END", salesHData->usePoint, salesHData->weatherStr, salesHData->KionStr, ENTER);
		send(g_soc, sendBuf, sendLen, 0);
		recvLen=recv_data(g_soc, recvBuf, BUFSIZE_MAX);
		recordCount=record_division(recvBuf, records);
		memset(response,0,BUFSIZE);
		for(i=0;i<9;i++){
			memset(param[i],0,BUFSIZE);
		}
		/* レスポンスメッセージを解析 */
		sscanf(records[0], "%s %s %s", response, param[0], param[1]);

		time(&now);
		ltm = localtime(&now);

		sprintf(resultTime, "%d/%d/%d %d:%d", ltm->tm_year+1900, ltm->tm_mon+1, ltm->tm_mday, ltm->tm_hour, ltm->tm_min);
		sprintf(param[3], "%s%d", "小計：￥", salesHData->totalMoney);
		sprintf(param[5], "%s%d", "使用ポイント：", salesHData->usePoint);
		sprintf(param[9], "%s%s", "合計：￥", param[0]);
		sprintf(param[4], "%s%d", "お預り：￥", salesHData->useMoney);
		sprintf(param[6], "%s%d", "お釣り：￥", salesHData->useMoney + salesHData->usePoint - salesHData->totalMoney);
		sprintf(param[7], "%s%d", "発行ポイント：", salesHData->getPoint);
		sprintf(param[8], "%s%s", "現在保有ポイント：", param[1]);

		sprintf(reciptText, 
				"%s\n\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s", 
				resultTime,
				param[3],	//小計
				param[5],	//使用ポイント
				param[9],	//合計
				"------------------------",
				param[4],	//お預り
				param[6],	//お釣り
				"------------------------",
				param[7],	//発行ポイント
				param[8]	//現在保有ポイント
			   );

		setTextView(salesHData->reciptBox, reciptText);


		gtk_widget_show_all(salesHData->endDialog);
		gtk_widget_set_sensitive( GTK_WIDGET(salesHData->resultWindow), FALSE );
		/* 残高照会画面表示フラグをセット */
		g_sales6WindowFlag = 1;


	}   
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
void showSalesErrorMsg(GtkLabel *errorLabel, int errorCode){

	switch(errorCode){
		case 200:
			gtk_label_set_text(errorLabel, "ERROR:在庫不足");
			break;
		case 300:
			gtk_label_set_text(errorLabel, "ERROR:データベースエラー");
			break;
		case 1200:
			gtk_label_set_text(errorLabel, "ERROR:賞味期限切れ");
			break;
		case 5100:
			gtk_label_set_text(errorLabel, "ERROR:ログインエラー");
			break;

		default:
			gtk_label_set_text(errorLabel, "ERROR:致命的なエラーが発生しました");
			break;
	}   
}

void showSalesOkMsg(GtkLabel *okLabel, int okCode){
	switch(okCode){
		default:
			gtk_label_set_text(okLabel, "商品をスキャンしてください");
			break;
	}
}

void setTextView(GtkTextView *textview, const gchar *text)
{
	GtkTextBuffer *buffer;

	buffer = gtk_text_view_get_buffer(textview);
	gtk_text_buffer_set_text(buffer, text, -1);
}
