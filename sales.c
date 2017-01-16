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

_salesHandleData *salesHData;

void showSalesErrorMsg(GtkLabel *erroLabel, int errorCode);

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

        /* 扱う必要のあるWidgetを構造体hDataのメンバに保持 */
        /* メンバ"balanceWindow"には残高照会画面のウィジェット */
        /* メンバ"idEntry"には口座IDを入力するテキストエントリウィジェット */
        /* メンバ"resultLabel"には結果を表示するラベルウィジェット */
        salesHData->salesWindow = GTK_WIDGET( gtk_builder_get_object(builder, "salesWindow") );
        salesHData->cancelButton1 = GTK_BUTTON( gtk_builder_get_object(builder, "cancelButton1") );
        salesHData->breakDialog = GTK_WIDGET( gtk_builder_get_object(builder, "breakDialog") );
        salesHData->cancelButton3 = GTK_BUTTON( gtk_builder_get_object(builder, "cancelButton3") );
        salesHData->pointcardWindow = GTK_WIDGET( gtk_builder_get_object(builder, "pointcardWindow") );

        salesHData->ageComboBox = GTK_COMBO_BOX(gtk_builder_get_object(builder, "ageComboBox"));
        salesHData->weatherComboBox = GTK_COMBO_BOX(gtk_builder_get_object(builder, "weatherComboBox"));
        salesHData->ageModel = GTK_LIST_STORE(gtk_combo_box_get_model(salesHData->ageComboBox));
        salesHData->weatherModel = GTK_LIST_STORE(gtk_combo_box_get_model(salesHData->weatherComboBox));

        salesHData->productidEntry = GTK_ENTRY( gtk_builder_get_object(builder, "productidEntry") );
        salesHData->pointidEntry = GTK_ENTRY( gtk_builder_get_object(builder, "pointidEntry") );
        salesHData->pointresultLabel = GTK_LABEL( gtk_builder_get_object(builder, "pointresultLabel") );
        salesHData->pointcardButton = GTK_BUTTON(gtk_builder_get_object(builder, "pointcardButton"));
        salesHData->pointLogin = 0;

        salesHData->maleRadiobutton = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "maleRadiobutton"));
        salesHData->femaleRadiobutton = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "femaleRadiobutton"));

        salesHData->valueSpinbutton = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "valueSpinbutton"));

        //		salesHData->valueSpinbutton = GTK_SPIN_BUTTON(tmpSpinbutton);
        //		adjustment = gtk_adjustment_new(50.0, 0.0, 100.0, 1.0, 5.0, 0.0);
        //		gtk_spin_button_configure(salesHData->valueSpinbutton, adjustment, 1.0, 2);

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
    int i, productPrice;
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
            /* レスポンスメッセージを解析 */
            sscanf(records[0], "%s %s", response, param[0]);

            for(i=1;i<recordCount;i++){
                sscanf(records[i], "%s %s %s %s %s %s %s", param[0], param[1], param[2], param[3], param[4], param[5], param[6]);
                if(atoi(param[1]) == atoi(productidStr)){
                    strcpy(productName, param[2]);
                    productPrice = (int)((double)atoi(param[3])/0.7);
                    break;
                }
            }
        }else{
            sscanf(records[0], "%s %s %s %s %s %s", param[0], param[1], param[2], param[3], param[4], param[5]);
            strcpy(productName, param[2]);
            productPrice = atoi(param[4]);
        }


        gtk_list_store_append(salesHData->productModel,&(salesHData->productIter));
        gtk_list_store_set(salesHData->productModel, &(salesHData->productIter),
                0, atoi(productidStr), 
                1, atoi(purchaseidStr), 
                2, productName, 
                3, productPrice, 
                4, 1, 
                5, productPrice,
                6, atoi(param[1]),
                -1
                );


    }

}

/**
 * コールバック関数：「戻る」ボタンのクリックによりウィンドウを非表示にする
 * cb_sales1_win_cancel
 **/
G_MODULE_EXPORT void cb_sales1_win_cancel(GtkButton *button, gpointer data){

    /* 残高照会画面（ウィンドウ）を非表示 */
    gtk_widget_hide(salesHData->salesWindow);
    gtk_widget_hide(salesHData->breakDialog);
    gtk_widget_hide(salesHData->pointcardWindow);
    /* 残高照会画面主要Widget保持構造体を破棄（メモリ開放) */
    free(salesHData);
    /* 残高照会画面表示フラグをクリア */
    g_sales1WindowFlag = 0;
    g_sales2WindowFlag = 0;
    g_sales3WindowFlag = 0;
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
        /* 残高照会画面表示フラグをセット */
        g_sales2WindowFlag = 1;
    }   
}


G_MODULE_EXPORT void cb_sales2_win_cancel(GtkButton *button, gpointer data){

    /* 残高照会画面（ウィンドウ）を非表示 */
    gtk_widget_hide(salesHData->breakDialog);
    gtk_widget_set_sensitive( GTK_WIDGET(salesHData->salesWindow), TRUE );
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
       }
    }
}

G_MODULE_EXPORT void cb_sales1_tree_correct(GtkButton *button, gpointer data){
    printf("OK\n");
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

