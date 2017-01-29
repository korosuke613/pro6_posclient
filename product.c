#include "pos_client.h"
#include "product.h"

int g_prodmanaWindowFlag =0;
/**
 * コールバック関数：「商品管理」ボタンのクリックにより商品情報画面を表示
 * cb_pro_manage_win_open
 **/
G_MODULE_EXPORT void cb_pro_manage_win_open(GtkButton *button, gpointer data){
  GtkBuilder        *builder;
  prodmanaHandleData *hData; //商品管理画面の主要Widget保持用

  /* 商品管理画面が表示されていない場合 */
  if(g_prodmanaWindowFlag == 0){
    /* 商品管理画面の主要Widget保持用構造体を作成 */
    hData = (prodmanaHandleData *)malloc(sizeof(prodmanaHandleData));
    /* product_manage_main.gladeを読み込む */
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "product_main.glade", NULL);
    /* 扱う必要のあるWidgetを構造体hDataのメンバに保持 */
    /* メンバ"prod_mainWindow"には商品管理画面のウィジェット */
    hData->prod_mainWindow = GTK_WIDGET( gtk_builder_get_object(builder, "prod_mainWindow") );

    /* シグナル、シグナルハンドラ、ユーザデータ登録 */
    gtk_builder_connect_signals(builder, hData);
    /* 商品管理画面表示 */
    gtk_widget_show_all(hData->prod_mainWindow);
    /* 商品管理画面表示フラグをセット */
    g_prodmanaWindowFlag = 1;
  }

}


G_MODULE_EXPORT void cb_promane_cancel(GtkButton *button, gpointer data){
  prodmanaHandleData *hData; //         Widget   

  hData = (prodmanaHandleData *)data;
  /*                   */
  gtk_widget_hide(hData->prod_mainWindow);
  /*         Widget              ) */
  free(hData);
  /*                 */
  g_prodmanaWindowFlag = 0;

}

