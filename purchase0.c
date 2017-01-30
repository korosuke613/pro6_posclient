/*
 * Copyright (c) 2016
 *
 *    Yuto Sumiyoshi.  All rights reserved.
 *
 * purchase.c
 * 最終更新日時：2016.12.12
 *
 */

#include "pos_client.h"
#include "purchase0.h"

int g_add0WindowFlag = 0;




G_MODULE_EXPORT void cb_add_win_open(GtkButton *button, gpointer data){
  GtkBuilder      *builder;
  addHandleData *addhData; //ログイン画面の主要Widget保持用
  if(g_add0WindowFlag ==0){
  /* ログイン画面の主要Widget保持用構造体を作成 */
  addhData = (addHandleData *)malloc(sizeof(addHandleData));

  /* login.gladeを読み込む */
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "purchase0.glade", NULL);
  /* 主要Widgetを保持 */
  addhData->pur0Window     = GTK_WIDGET( gtk_builder_get_object(builder, "pur0Window") );
 addhData->purinfoButton    = GTK_BUTTON( gtk_builder_get_object(builder, "purinfoButton"));
 addhData->proinfoButton    = GTK_BUTTON( gtk_builder_get_object(builder, "proinfoButton"));
 addhData->purButton    = GTK_BUTTON( gtk_builder_get_object(builder, "purButton"));
 addhData->backButton    = GTK_BUTTON( gtk_builder_get_object(builder, "backButton"));
  /* シグナル、シグナルハンドラ、ユーザデータ登録 */
  gtk_builder_connect_signals(builder, addhData);
  /* ログイン画面表示 */
  gtk_widget_show_all(addhData->pur0Window);

  g_add0WindowFlag = 1; 
 }
}


G_MODULE_EXPORT void cb_add0_cancel(GtkButton *button, gpointer data){
  addHandleData *hData; //       Widget   

  hData = (addHandleData *)data;
  /*                 */
  gtk_widget_hide(hData->pur0Window);
  /*       Widget              ) */
  free(hData);
  /*               */
  g_add0WindowFlag = 0;

}


