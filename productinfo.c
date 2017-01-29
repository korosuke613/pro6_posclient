#include "pos_client.h"
#include "productinfo.h"

int g_prodinfoWindowFlag =0;

void productshowErrorMsg(GtkLabel *resultLabel, int errorCode);

/**
 * コールバック関数：「商品情報」ボタンのクリックにより商品情報画面を表示
 * cb_prodinfo_win_open
 **/
G_MODULE_EXPORT void cb_prodinfo_win_open(GtkButton *button, gpointer data){
  GtkBuilder        *builder;
  GtkListStore *model;
  GtkTreeIter     iter;
  ProdinfoHandleData *hData; //商品管理画面の主要Widget保持用
  const gchar *bcateData[BUFSIZE_MAX];
  int recordCount;
  char *records[RECORD_MAX];
  int n,i,r,m,sendLen,recvLen;
  char sendBuf[BUFSIZE],recvBuf[BUFSIZE_MAX];
  char response[BUFSIZE],param1[BUFSIZE],param2[BUFSIZE],param3[BUFSIZE],param4[BUFSIZE];

  /* 商品管理画面が表示されていない場合 */
  if(g_prodinfoWindowFlag == 0){
    /* 商品管理画面の主要Widget保持用構造体を作成 */
    hData = (ProdinfoHandleData *)malloc(sizeof(ProdinfoHandleData));
    /* product_manage_main.gladeを読み込む */
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "product_cate.glade", NULL);
    /* 扱う必要のあるWidgetを構造体hDataのメンバに保持 */
    /* メンバ"prod_mainWindow"には商品管理画面のウィジェット */
    hData->cateWindow = GTK_WIDGET( gtk_builder_get_object(builder, "cateWindow") );
    hData->bigcateLabel = GTK_LABEL( gtk_builder_get_object(builder, "bigcateLabel") );
    hData->bcateCombobox = GTK_COMBO_BOX( gtk_builder_get_object(builder, "bcateCombobox"));
    hData->smallcateLabel = GTK_LABEL( gtk_builder_get_object(builder, "smallcateLabel") );
    hData->scateCombobox = GTK_COMBO_BOX( gtk_builder_get_object(builder, "scateCombobox")); 
    hData->cateLabel = GTK_LABEL( gtk_builder_get_object(builder, "cateLabel") );
    hData->cateCombobox = GTK_COMBO_BOX( gtk_builder_get_object(builder, "cateCombobox"));
    hData->pro_maneTreeview = GTK_TREE_VIEW(gtk_builder_get_object(builder, "pro_maneTreeview"));
    hData->resultLabel = GTK_LABEL( gtk_builder_get_object(builder, "resultLabel"));
   hData->cancellButton = GTK_BUTTON( gtk_builder_get_object(builder, "cancellButton") );


   if(g_soc > 0){
     sendLen = sprintf(sendBuf, "%s %s",CATE_DISP, ENTER);
     send(g_soc, sendBuf, sendLen, 0);
    
     recvLen = recv_data(g_soc, recvBuf,BUFSIZE_MAX);

     recordCount = record_division(recvBuf, records);

     memset(response, 0, BUFSIZE);
     memset(param1, 0, BUFSIZE);

     n = sscanf(records[0], "%s %s", response, param1);

     if(strcmp(response, OK_STAT) != 0){     
       productshowErrorMsg(hData->resultLabel, atoi(param1));
       return;
     }
     
    
    for(i=1; i < recordCount ; i++){
      
      bcateData[i-1] = records[i];   
    }

    model = GTK_LIST_STORE(gtk_combo_box_get_model(hData->bcateCombobox));

    gtk_list_store_clear(model);

    for(r=0; r < atoi(param1); r++){
      // m = sscanf(records[i], "%s",param1);
      gtk_list_store_append(model, &iter);
      gtk_list_store_set(model, &iter, 0, bcateData[r], -1);
    }



     /* 商品管理画面表示 */
     gtk_widget_show_all(hData->cateWindow);

     /* シグナル、シグナルハンドラ、ユーザデータ登録 */
     gtk_builder_connect_signals(builder, hData);

   }
   /* 商品管理画面表示フラグをセット */
   g_prodinfoWindowFlag = 1;
  }



}




G_MODULE_EXPORT void cb_prodinfo_cancel(GtkButton *button, gpointer data){
  ProdinfoHandleData *hData; //         Widget   

  hData = (ProdinfoHandleData *)data;

  gtk_widget_hide(hData->cateWindow);
  free(hData);

  g_prodinfoWindowFlag = 0;

}




G_MODULE_EXPORT void cb_scate_exec(GtkComboBox *combobox, gpointer data){
  GtkTreeModel  *model;
  GtkListStore *model2;
  GtkTreeIter     iter;
  ProdinfoHandleData *hData; //商品管理画面の主要Widget保持用
  gchar          *bcatevalue;
  int b,recordCount,n,m,i,r,sendLen,recvLen;
  char *records[RECORD_MAX];
  char sendBuf[BUFSIZE],cateBuf[BUFSIZE],recvBuf[BUFSIZE_MAX];
  char response[BUFSIZE],param1[BUFSIZE],param2[BUFSIZE],param3[BUFSIZE],param4[BUFSIZE];

  hData = (ProdinfoHandleData *)data;

  if(gtk_combo_box_get_active_iter(hData->bcateCombobox, &iter) ){
    model = gtk_combo_box_get_model(hData->bcateCombobox);
    gtk_tree_model_get(model, &iter, 0, &bcatevalue, -1);
    b = atoi(bcatevalue);
  }


  if(g_soc > 0){
    sendLen = sprintf(sendBuf, "%s %d %s",CATE_DETAIL_DISP, b, ENTER);
    send(g_soc, sendBuf, sendLen, 0);

    recvLen = recv_data(g_soc, recvBuf,BUFSIZE_MAX);

    recordCount = record_division(recvBuf, records);

    memset(response, 0, BUFSIZE);
    memset(param1, 0, BUFSIZE);
    memset(param2, 0, BUFSIZE);
    memset(param3, 0, BUFSIZE);

    n = sscanf(records[0], "%s %s", response, param1);

    if(strcmp(response, OK_STAT) != 0){
      productshowErrorMsg(hData->resultLabel, atoi(param1));
      return;
    }

    model2 = GTK_LIST_STORE(gtk_combo_box_get_model(hData->scateCombobox));

    gtk_list_store_clear(model2);

    for(i = 1; i < recordCount; i++){
      m = sscanf(records[i], "%s %s %s", param1,param2,param3);
      sprintf(cateBuf,"%s %s",param1,param3);
      gtk_list_store_append(model2, &iter);
      gtk_list_store_set(model2, &iter, 0, cateBuf, -1);
    }


 
  }



}

G_MODULE_EXPORT void cb_prodcate_exec(GtkComboBox *combobox, gpointer data){
  ProdinfoHandleData *hData;
  GtkTreeModel  *model;
  GtkListStore *model2;
  GtkTreeIter     iter;
  gchar          *scatevalue;
  int s,recordCount,n,m,i,r,sendLen,recvLen;
  char *records[RECORD_MAX];
  char sendBuf[BUFSIZE],cateBuf[BUFSIZE],recvBuf[BUFSIZE_MAX];
  char response[BUFSIZE],param1[BUFSIZE],param2[BUFSIZE],param3[BUFSIZE],param4[BUFSIZE];

  hData = (ProdinfoHandleData *)data;

  if(gtk_combo_box_get_active_iter(hData->scateCombobox, &iter) ){

    model = gtk_combo_box_get_model(hData->scateCombobox);

    gtk_tree_model_get(model, &iter, 0, &scatevalue, -1);

    s = atoi(scatevalue);
   
  }

   
  if(g_soc > 0){
    sendLen = sprintf(sendBuf, "%s %d %s",CATE_INFO_DISP, s, ENTER);
  
    send(g_soc, sendBuf, sendLen, 0);
    
    recvLen = recv_data(g_soc, recvBuf,BUFSIZE_MAX);

    recordCount = record_division(recvBuf, records);

    memset(response, 0, BUFSIZE);
    memset(param1, 0, BUFSIZE);
    memset(param2, 0, BUFSIZE);
   


    n = sscanf(records[0], "%s %s", response, param1);

    if(strcmp(response, OK_STAT) != 0){
     
      productshowErrorMsg(hData->resultLabel, atoi(param1));
      return;
    }

    model2 = GTK_LIST_STORE(gtk_combo_box_get_model(hData->cateCombobox));
    
    gtk_list_store_clear(model2);

    for(i = 1; i < recordCount; i++){
      m = sscanf(records[i], "%s %s", param1,param2);
      sprintf(cateBuf,"%s %s",param1,param2);
      gtk_list_store_append(model2, &iter);
      gtk_list_store_set(model2, &iter, 0, cateBuf, -1);
    }


  }





}



G_MODULE_EXPORT void cb_prodinfo_exec(GtkComboBox *combobox, gpointer data){
  ProdinfoHandleData *hData;
  GtkTreeModel  *model;
  GtkListStore *model2;
  GtkTreeIter     iter;
  gchar          *productvalue;
  int recordCount;
  char *records[RECORD_MAX];
  int n,m,i,r,p,sendLen,recvLen;
  char sendBuf[BUFSIZE],cateBuf[BUFSIZE],recvBuf[BUFSIZE_MAX];
  char response[BUFSIZE],param1[BUFSIZE],param2[BUFSIZE],param3[BUFSIZE],param4[BUFSIZE];

  hData = (ProdinfoHandleData *)data;

  if(gtk_combo_box_get_active_iter(hData->cateCombobox, &iter) ){

    model = gtk_combo_box_get_model(hData->cateCombobox);

    gtk_tree_model_get(model, &iter, 0, &productvalue, -1);

    p = atoi(productvalue);
   
  }

if(g_soc > 0){
    sendLen = sprintf(sendBuf, "%s %d %s",PROD_INFO_DISP, p, ENTER);
  
    send(g_soc, sendBuf, sendLen, 0);
    
    recvLen = recv_data(g_soc, recvBuf,BUFSIZE_MAX);

    recordCount = record_division(recvBuf, records);

    memset(response, 0, BUFSIZE);
    memset(param1, 0, BUFSIZE);
    memset(param2, 0, BUFSIZE);
    memset(param3, 0, BUFSIZE);
    memset(param4, 0, BUFSIZE);


    n = sscanf(records[0], "%s %s %s %s %s", response, param1, param2, param3, param4);

    if(strcmp(response, OK_STAT) != 0){     
      productshowErrorMsg(hData->resultLabel, atoi(param1));
      return;
    }




    model2 = GTK_LIST_STORE(gtk_tree_view_get_model(hData->pro_maneTreeview));

    gtk_list_store_clear(model2);

    for(i = 0; i < recordCount; i++){
   n = sscanf(records[i], "%s %s %s %s %s", response, param1, param2, param3, param4);
   gtk_list_store_append(GTK_LIST_STORE(model2), &iter);
   gtk_list_store_set(GTK_LIST_STORE(model2), &iter, 0, atoi(param1), 1, param2, 2, param4, -1);	
   gtk_label_set_text(hData->resultLabel,records[i]);
    }



 }





}

void productshowErrorMsg(GtkLabel *resultLabel, int errorCode){

  switch(errorCode){
  default:
    gtk_label_set_text(resultLabel, "カテゴリを選択してください");
    break;
  }
}

