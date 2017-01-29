/**
 * pos_client.h
 **/

#ifndef _INCLUDE_POS_
#define _INCLUDE_POS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>

#include <gtk/gtk.h>

#define BUFSIZE 1024
#define BUFSIZE_MAX 4096
#define RECORD_MAX 1000
#define ENTER "\n"
#define DATA_END ".\n" //レスポンスデータの最終デリミタ

// プロトコルコマンド 
/*ログイン*/
#define LOGIN "LOGIN" //ログイン

// レスポンスステータス 
#define OK_STAT "+OK"

// エラーコード 


// 権限 

extern int g_soc;             //通信用ソケットディスクリプタ
extern int g_permissionId;      //権限ID デフォルト:0 権限なし
extern int g_storeId;

//*** メイン画面の主要Widget保持用構造体 ***//
typedef struct _mainHandleData{
  GtkWidget  *mainWindow;           //メインウィンドウ
  GtkLabel   *areaLabel;            //地域ラベル
  GtkLabel   *storeLabel;           //店舗ラベル
  GtkLabel   *connectLabel;         //サーバ接続表示ラベル
  GtkButton  *salesButton;          //販売会計ボタン
  GtkButton  *stockButton;          //在庫管理ボタン
  GtkButton  *purchaseButton;       //仕入れボタン
  GtkButton  *pointButton;          //ポイント発行発行ボタン
  GtkButton  *productButton;        //商品管理ボタン
  GtkButton  *customerButton;       //顧客管理ボタン
  GtkButton  *salesTotalButton;     //売上集計ボタン
  GtkButton  *salesAnalysisButton;  //販売分析ボタン
  GtkButton  *abcAnalysisButton;    //ABC分析ボタン
}MainHandleData;

//*** ログイン画面のWidget ***//
typedef struct _loginHandleData{
  GtkWidget *loginWindow;         //ウィンドウ
  GtkEntry  *storeIdEntry;        //店舗ID
  GtkEntry  *passwordEntry;       //パスワード
  GtkLabel  *errorLabel;          //エラー表示用ラベル
  MainHandleData *mainHandleData; //メイン画面の主要Widget保持用構造体
}LoginHandleData;


#endif
