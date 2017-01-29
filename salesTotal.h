
//担当: 平松 パート 二
#include "pos_client.h"

#define SALE_INS_INFO "SALE_INS_INFO" //売上利益算出コマンド
#define SALE_NUM_INFO "SALE_NUM_INFO" //売上抽出コマンド

//externの部分に追加
extern int g_sales_selectWindowFlag; //売上集計選択画面 1:表示 0:非表示
extern int g_sales_numberWindowFlag; //売上個数集計画面 1:表示 0:非表示
extern int g_sales_totalWindowFlag; //売上個数集計画面 1:表示 0:非表示

extern void showErrorMessage(GtkLabel *errorLabel, int errorcode);

//以下構造体


//売上集計コマンド選択画面のwidget
typedef struct _salesSelectHandleData{
  GtkWidget *sales_selectWindow;   //ウィンドウ
  GtkButton  *sales_numberButton;     //売上個数集計コマンドボタン
  GtkButton  *sales_totalButton;     //売上利益コマンドボタン
}SalesSelectHandleData;

//売上個数集計画面のwidget
typedef struct _salesNumberHandleData{
  GtkWidget *sales_numberWindow;   //ウィンドウ
  GtkRadioButton *sales_number_typeRadioButton[3]; //売上抽出カテゴリラジオボタン
  GtkEntry *sales_number_typeEntry; //売上抽出カテゴリのテキストエントリ
  GtkEntry *sales_number_startEntry; //抽出開始日のテキストエントリ
  GtkEntry *sales_number_limitEntry; //抽出終了日のテキストエントリ
  GtkButton  *sales_numberButton;     //売上抽出コマンドボタン
  GtkButton  *sales_number_cancelButton;     //戻るボタン
  GtkLabel *sales_number_resultLabel; //実行結果表示用のテキストラベル
}SalesNumberHandleData;

//売上個数集計結果表示のwidget
typedef struct _salesNumberShowHandleData{
  GtkWidget *sales_number_showWindow; //ウィンドウ
  GtkTreeView *sales_number_showTreeView; //実行結果列挙のツリービュー
  GtkButton *sales_number_showButton; //終了ボタン
}SalesNumberShowHandleData;

//売上利益集計画面のwidget
typedef struct _salesTotalHandleData{
  GtkWidget *sales_totalWindow;   //ウィンドウ
  GtkRadioButton *sales_total_typeRadioButton[3]; //抽出範囲カテゴリのラジオボタン
  GtkEntry  *sales_total_typeEntry; //抽出カテゴリのIDまたは種類のテキストエントリ
  GtkRadioButton *sales_total_termRadioButton[5]; //抽出期間カテゴリのラジオボタン
  GtkEntry *sales_total_startEntry; //抽出開始日のテキストエントリ
  GtkButton *sales_totalButton; //利益算出のボタン
  GtkButton  *sales_total_retryButton; //やり直しのボタン
  GtkLabel *sales_totalLabel; //総売上表示ラベル
  GtkLabel *sales_total_priceLabel; //総仕入れ金額表示ラベル
  GtkLabel *sales_total_gainLabel; //総利益表示ラベル
  GtkButton *sales_total_cancelButton; //売上利益集計終了ボタン
  GtkLabel *sales_total_resultLabel; //実行結果のテキストラベル
}SalesTotalHandleData;
