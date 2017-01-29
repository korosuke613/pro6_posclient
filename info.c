/**
 * login.c
 **/

#include "pos_client.h"


/**
 * コールバック関数
 * cb_login_win_open
 **/
GtkWidget		*infoWindow;

G_MODULE_EXPORT void cb_info_win_open(GtkButton *button, gpointer data){
	GtkBuilder      *builder;

	/* login.gladeを読み込む */
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "pos_main.glade", NULL);
	/* 主要Widgetを保持 */
	infoWindow     = GTK_WIDGET( gtk_builder_get_object(builder, "infoWindow") );

	/* ログイン画面表示 */
	gtk_widget_show_all(infoWindow);
}

/**
 * コールバック関数：「キャンセル」ボタンのクリックによりウィンドウを非表示にする
 * cb_login_cancel
 **/
G_MODULE_EXPORT void cb_info_cancel(GtkButton *button, gpointer data){
	/* ログイン画面（ウィンドウ）を非表示 */
	gtk_widget_hide(infoWindow);
}

