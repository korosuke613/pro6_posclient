#include<stdio.h>
#include <gtk/gtk.h>
#include "abcAnaryze.h"
#include "pos_client.h"

int g_abcWindowFlag = 0;
int g_abc_dayWindowFlag = 0;
int g_abc_genderWindowFlag = 0;
int g_abc_weatherWindowFlag = 0;
int g_abc_ageWindowFlag = 0;
int g_abc_treeWindowFlag = 0;

void showABCErrorMsg(GtkLabel *erroLabel, int errorCode);


G_MODULE_EXPORT void cb_abc_win_open(GtkButton *button, gpointer data){
  GtkBuilder        *builder;
  Abc0HandleData *hData; 


  if(g_abcWindowFlag == 0){
    hData = (Abc0HandleData *)malloc(sizeof(Abc0HandleData));
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "abcAnaryze0.glade",NULL);

    hData->abc0window = GTK_WIDGET(gtk_builder_get_object(builder,"abc0window"));
    hData->noradio = GTK_RADIO_BUTTON(gtk_builder_get_object(builder,"noradio"));
    hData->genderradio = GTK_RADIO_BUTTON(gtk_builder_get_object(builder,"genderradio"));
    hData->weatherradio = GTK_RADIO_BUTTON(gtk_builder_get_object(builder,"weatherradio"));
    hData->ageradio = GTK_RADIO_BUTTON(gtk_builder_get_object(builder,"ageradio"));
    hData->nextbutton = GTK_BUTTON(gtk_builder_get_object(builder,"nextbutton"));
    hData->backbutton = GTK_BUTTON(gtk_builder_get_object(builder,"backbutton"));


    gtk_builder_connect_signals(builder,hData);
    gtk_widget_show_all(hData->abc0window);
    g_abcWindowFlag = 1;
  }
}


G_MODULE_EXPORT void cb_abc_open(GtkButton *button, gpointer data){
  GtkBuilder        *builder;
  Abc0HandleData *abc0hData; //残高照会画面の主要Widget保持用
  Abc1HandleData *abc1hData;
  Abc2HandleData *abc2hData;
  Abc3HandleData *abc3hData;
  Abc4HandleData *abc4hData;
  Abc5HandleData *abc5hData;
  GtkListStore *model1,*model2,*model3,*model4; //GtkListStoreオブジェクト
  GtkTreeIter iter1,iter2,iter3,iter4;
  const gchar *Weather[] = {"晴れ","雨","曇り","雪"};
  const gchar *age1[] = {"10","20","30","40","50"};
  const gchar *age2[] = {"19","29","39","49","59"};
  const gchar *gender[] = {"男","女"};
  int i;

  abc0hData = (Abc0HandleData *)data;

  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(abc0hData->noradio))){
    if(g_abc_dayWindowFlag == 0){
      abc4hData = (Abc4HandleData *)malloc(sizeof(Abc4HandleData));

      builder = gtk_builder_new();
      gtk_builder_add_from_file(builder,"abcAnaryze4.glade",NULL);

      abc4hData->daywindow = GTK_WIDGET(gtk_builder_get_object(builder,"daywindow"));
      abc4hData->startentry = GTK_ENTRY(gtk_builder_get_object(builder,"startentry"));
      abc4hData->showbutton = GTK_BUTTON(gtk_builder_get_object(builder,"showbutton"));
      abc4hData->backbutton = GTK_BUTTON(gtk_builder_get_object(builder,"backbutton"));

      gtk_builder_connect_signals(builder,abc4hData);
      gtk_widget_show_all(abc4hData->daywindow);
      g_abc_dayWindowFlag = 1;
    }
  }else if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(abc0hData->weatherradio))){
    if(g_abc_weatherWindowFlag == 0){
      abc1hData = (Abc1HandleData *)malloc(sizeof(Abc1HandleData));

      builder = gtk_builder_new();
      gtk_builder_add_from_file(builder,"abcAnaryze1.glade",NULL);

      abc1hData->weatherwindow = GTK_WIDGET(gtk_builder_get_object(builder,"weatherwindow"));
      abc1hData->startentry = GTK_ENTRY(gtk_builder_get_object(builder,"startentry"));
      abc1hData->weatherbox = GTK_COMBO_BOX(gtk_builder_get_object(builder,"weatherbox"));
      abc1hData->showbutton = GTK_BUTTON(gtk_builder_get_object(builder,"showbutton"));
      abc1hData->backbutton = GTK_BUTTON(gtk_builder_get_object(builder,"backbutton"));


      model1 = GTK_LIST_STORE(gtk_combo_box_get_model(abc1hData->weatherbox));
      for(i=0;i<4;i++){
	gtk_list_store_append(model1,&iter1);
	gtk_list_store_set(model1,&iter1,0,Weather[i],-1);
      }
      gtk_combo_box_set_active(GTK_COMBO_BOX(abc1hData->weatherbox),0);
      gtk_builder_connect_signals(builder,abc1hData);
      gtk_widget_show_all(abc1hData->weatherwindow);
      g_abc_weatherWindowFlag = 1;
    }
  }else if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(abc0hData->genderradio))){
      if(g_abc_genderWindowFlag == 0){
	abc2hData = (Abc2HandleData *)malloc(sizeof(Abc2HandleData));

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder,"abcAnaryze2.glade",NULL);

	abc2hData->genderwindow = GTK_WIDGET(gtk_builder_get_object(builder,"genderwindow"));
	abc2hData->startentry = GTK_ENTRY(gtk_builder_get_object(builder,"startentry"));
	abc2hData->genderbox = GTK_COMBO_BOX(gtk_builder_get_object(builder,"genderbox"));
	abc2hData->showbutton = GTK_BUTTON(gtk_builder_get_object(builder,"showbutton"));
	abc2hData->backbutton = GTK_BUTTON(gtk_builder_get_object(builder,"backbutton"));

	model2 = GTK_LIST_STORE(gtk_combo_box_get_model(abc2hData->genderbox));
	for(i=0;i<2;i++){
	gtk_list_store_append(model2,&iter2);
	gtk_list_store_set(model2,&iter2,0,gender[i],-1);
	}
	gtk_combo_box_set_active(GTK_COMBO_BOX(abc2hData->genderbox),0);
	gtk_builder_connect_signals(builder,abc2hData);
	gtk_widget_show_all(abc2hData->genderwindow);
	g_abc_genderWindowFlag = 1;
      }
    }else if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(abc0hData->ageradio))){
	if(g_abc_ageWindowFlag == 0){
	  abc3hData = (Abc3HandleData *)malloc(sizeof(Abc3HandleData));

	  builder = gtk_builder_new();
	  gtk_builder_add_from_file(builder,"abcAnaryze3.glade",NULL);

	  abc3hData->agewindow = GTK_WIDGET(gtk_builder_get_object(builder,"agewindow"));
	  abc3hData->startentry = GTK_ENTRY(gtk_builder_get_object(builder,"startentry"));
	  abc3hData->Age1box = GTK_COMBO_BOX(gtk_builder_get_object(builder,"Age1box"));
	  abc3hData->Age2box = GTK_COMBO_BOX(gtk_builder_get_object(builder,"Age2box"));
	  abc3hData->showbutton = GTK_BUTTON(gtk_builder_get_object(builder,"showbutton"));
	  abc3hData->backbutton = GTK_BUTTON(gtk_builder_get_object(builder,"backbutton"));

	  model3 = GTK_LIST_STORE(gtk_combo_box_get_model(abc3hData->Age1box));
	  for(i=0;i<5;i++){
	    gtk_list_store_append(model3,&iter3);
	    gtk_list_store_set(model3,&iter3,0,age1[i],-1);
	  }

	  model4 = GTK_LIST_STORE(gtk_combo_box_get_model(abc3hData->Age2box));
	  for(i=0;i<5;i++){
	    gtk_list_store_append(model4,&iter4);
	    gtk_list_store_set(model4,&iter4,0,age2[i],-1);
	  }
	  gtk_combo_box_set_active(GTK_COMBO_BOX(abc3hData->Age1box),0);
	  gtk_combo_box_set_active(GTK_COMBO_BOX(abc3hData->Age2box),0);
	  gtk_builder_connect_signals(builder,abc3hData);
	  gtk_widget_show_all(abc3hData->agewindow);
	  g_abc_ageWindowFlag = 1;
	}
      }
    
}


G_MODULE_EXPORT void cb_abc_day_open(GtkButton *button, gpointer data){
  GtkBuilder *builder;
  Abc4HandleData *abc4hData;
  Abc5HandleData *abc5hData;
  const char *startStr;
  char sendBuf[BUFSIZE],recvBuf[BUFSIZE_MAX],labelBuf[BUFSIZE];
  int sendLen,recvLen,recordCount,n;
  char *records[RECORD_MAX], response[BUFSIZE],param1[BUFSIZE],param2[BUFSIZE],param3[BUFSIZE],param4[BUFSIZE],param5[BUFSIZE],param6[BUFSIZE],param7[BUFSIZE];
  GtkListStore *model;
  GtkTreeIter iter;

  abc4hData =(Abc4HandleData *)data;
  abc5hData = (Abc5HandleData *)malloc(sizeof(Abc5HandleData));

  /*抽出開始日に入力されている文字列を取得*/
  startStr = gtk_entry_get_text(abc4hData->startentry);


  /*通信用のソケットディスクリプタが空でないかチェック*/
  if(g_soc>0){
    sendLen = sprintf(sendBuf, "%s %s %s",ABC,startStr,ENTER);

    send(g_soc,sendBuf,sendLen,0);
    recvLen = recv_data(g_soc,recvBuf,BUFSIZE_MAX);
    recordCount = record_division(recvBuf,records);
    memset(response,0,BUFSIZE);
    memset(param1,0,BUFSIZE);
    memset(param2,0,BUFSIZE);
    memset(param3,0,BUFSIZE);
    memset(param4,0,BUFSIZE);
    memset(param5,0,BUFSIZE);
    memset(param7,0,BUFSIZE);
    
    if(g_abc_treeWindowFlag == 0){

      builder = gtk_builder_new();
      gtk_builder_add_from_file(builder,"abcAnaryze5.glade",NULL);

      abc5hData->abcwindow1 = GTK_WIDGET(gtk_builder_get_object(builder,"abcwindow1"));
      abc5hData->treeView = GTK_TREE_VIEW(gtk_builder_get_object(builder,"treeview1"));
      abc5hData->disp_Label = GTK_LABEL(gtk_builder_get_object(builder,"disp_label"));

      gtk_builder_connect_signals(builder,abc5hData);
      gtk_widget_show_all(abc5hData->abcwindow1);
      g_abc_treeWindowFlag = 1;
    }

    n = sscanf(records[0],"%s %s %s %s %s %s %s %s",response,param1,param2,param3,param4,param5,param6,param7);

   if(strcmp(response,OK_STAT) !=0){
      showABCErrorMsg(abc5hData->disp_Label, atoi(param1));
      return;
  }


    sprintf(labelBuf,"表示件数%d件",atoi(param1));
    gtk_label_set_text(abc5hData->disp_Label,labelBuf);

    model = GTK_LIST_STORE(gtk_tree_view_get_model(abc5hData->treeView));
    gtk_list_store_clear(model);

    for(n=1;n<recordCount;n++){
      sscanf(records[n],"%s %s %s %s %s %s %s",param1,param2,param3,param4,param5,param6,param7);
      gtk_list_store_append(model,&iter);
      gtk_list_store_set(model,&iter,0,param1,1,param2,2,param3,3,param4,4,param5,5,param6,6,param7,-1);
    }
  }

  return;
}


G_MODULE_EXPORT void cb_abc_gender_open(GtkButton *button, gpointer data){
  GtkBuilder *builder;
  Abc2HandleData *abc2hData;
  Abc5HandleData *abc5hData;
  const char *startStr;
  char sendBuf[BUFSIZE],recvBuf[BUFSIZE_MAX],labelBuf[BUFSIZE];
  int sendLen,recvLen,recordCount,n;
  char *records[RECORD_MAX], response[BUFSIZE],param1[BUFSIZE],param2[BUFSIZE],param3[BUFSIZE],param4[BUFSIZE],param5[BUFSIZE],param6[BUFSIZE],param7[BUFSIZE];
  GtkTreeModel *model1;
  GtkListStore *model2;
  GtkTreeIter iter1,iter2;
  gchar *genderValue =NULL;

  abc2hData =(Abc2HandleData *)data;
  abc5hData = (Abc5HandleData *)malloc(sizeof(Abc5HandleData));

  /*抽出開始日に入力されている文字列を取得*/
  startStr = gtk_entry_get_text(abc2hData->startentry);

 /*コンボボックスの中から現在選択されているアイテムの取得*/
 
  if(gtk_combo_box_get_active_iter(abc2hData->genderbox,&iter1)){
    model1 = gtk_combo_box_get_model(abc2hData->genderbox);
    gtk_tree_model_get(model1,&iter1,0,&genderValue,-1);
  }

  if(strcmp(genderValue,"男")==0){
    strcpy(genderValue,"m");
  }else if(strcmp(genderValue,"女")==0){
    strcpy(genderValue,"f");
  }


  /*通信用のソケットディスクリプタが空でないかチェック*/
  if(g_soc>0){
    sendLen = sprintf(sendBuf, "%s %s %s %s %s",ABC,startStr,"gender",genderValue,ENTER);

    send(g_soc,sendBuf,sendLen,0);
    recvLen = recv_data(g_soc,recvBuf,BUFSIZE_MAX);
    recordCount = record_division(recvBuf,records);
    memset(response,0,BUFSIZE);
    memset(param1,0,BUFSIZE);
    memset(param2,0,BUFSIZE);
    memset(param3,0,BUFSIZE);
    memset(param4,0,BUFSIZE);
    memset(param5,0,BUFSIZE);
    memset(param7,0,BUFSIZE);
    
    if(g_abc_treeWindowFlag == 0){

      builder = gtk_builder_new();
      gtk_builder_add_from_file(builder,"abcAnaryze5.glade",NULL);

      abc5hData->abcwindow1 = GTK_WIDGET(gtk_builder_get_object(builder,"abcwindow1"));
      abc5hData->treeView = GTK_TREE_VIEW(gtk_builder_get_object(builder,"treeview1"));
      abc5hData->disp_Label = GTK_LABEL(gtk_builder_get_object(builder,"disp_label"));

      gtk_builder_connect_signals(builder,abc5hData);
      gtk_widget_show_all(abc5hData->abcwindow1);
      g_abc_treeWindowFlag = 1;
    }

    n = sscanf(records[0],"%s %s %s %s %s %s %s %s",response,param1,param2,param3,param4,param5,param6,param7);

    if(strcmp(response,OK_STAT) !=0){
      showABCErrorMsg(abc5hData->disp_Label, atoi(param1));
      return;
    }

    sprintf(labelBuf,"表示件数%d件",atoi(param1));
    gtk_label_set_text(abc5hData->disp_Label,labelBuf);

    model2 = GTK_LIST_STORE(gtk_tree_view_get_model(abc5hData->treeView));
    gtk_list_store_clear(model2);

    for(n=1;n<recordCount;n++){
      sscanf(records[n],"%s %s %s %s %s %s %s",param1,param2,param3,param4,param5,param6,param7);
      gtk_list_store_append(model2,&iter2);
      gtk_list_store_set(model2,&iter2,0,param1,1,param2,2,param3,3,param4,4,param5,5,param6,6,param7,-1);
    }
  }

  return;
}



G_MODULE_EXPORT void cb_abc_weather_open(GtkButton *button, gpointer data){
  GtkBuilder *builder;
  Abc1HandleData *abc1hData;
  Abc5HandleData *abc5hData;
  const char *startStr;
  char sendBuf[BUFSIZE],recvBuf[BUFSIZE_MAX],labelBuf[BUFSIZE];
  int sendLen,recvLen,recordCount,n;
  char *records[RECORD_MAX], response[BUFSIZE],param1[BUFSIZE],param2[BUFSIZE],param3[BUFSIZE],param4[BUFSIZE],param5[BUFSIZE],param6[BUFSIZE],param7[BUFSIZE];
  GtkTreeModel *model1;
  GtkListStore *model2;
  GtkTreeIter iter1,iter2;
  gchar *weatherValue =NULL;



  abc1hData =(Abc1HandleData *)data;
  abc5hData = (Abc5HandleData *)malloc(sizeof(Abc5HandleData));

  /*抽出開始日に入力されている文字列を取得*/
  startStr = gtk_entry_get_text(abc1hData->startentry);


  /*コンボボックスの中から現在選択されているアイテムの取得*/
 
  if(gtk_combo_box_get_active_iter(abc1hData->weatherbox,&iter1)){
    model1 = gtk_combo_box_get_model(abc1hData->weatherbox);
    gtk_tree_model_get(model1,&iter1,0,&weatherValue,-1);
  }




  /*通信用のソケットディスクリプタが空でないかチェック*/
  if(g_soc>0){
    sendLen = sprintf(sendBuf, "%s %s %s %s %s",ABC,startStr,"weather",weatherValue,ENTER);

    send(g_soc,sendBuf,sendLen,0);
    recvLen = recv_data(g_soc,recvBuf,BUFSIZE_MAX);
    recordCount = record_division(recvBuf,records);
    memset(response,0,BUFSIZE);
    memset(param1,0,BUFSIZE);
    memset(param2,0,BUFSIZE);
    memset(param3,0,BUFSIZE);
    memset(param4,0,BUFSIZE);
    memset(param5,0,BUFSIZE);
    memset(param7,0,BUFSIZE);
    
    if(g_abc_treeWindowFlag == 0){

      builder = gtk_builder_new();
      gtk_builder_add_from_file(builder,"abcAnaryze5.glade",NULL);

      abc5hData->abcwindow1 = GTK_WIDGET(gtk_builder_get_object(builder,"abcwindow1"));
      abc5hData->treeView = GTK_TREE_VIEW(gtk_builder_get_object(builder,"treeview1"));
      abc5hData->disp_Label = GTK_LABEL(gtk_builder_get_object(builder,"disp_label"));

      gtk_builder_connect_signals(builder,abc5hData);
      gtk_widget_show_all(abc5hData->abcwindow1);
      g_abc_treeWindowFlag = 1;
    }

    n = sscanf(records[0],"%s %s %s %s %s %s %s %s",response,param1,param2,param3,param4,param5,param6,param7);

    if(strcmp(response,OK_STAT) !=0){
      showABCErrorMsg(abc5hData->disp_Label, atoi(param1));
      return;
    }

    sprintf(labelBuf,"表示件数%d件",atoi(param1));
    gtk_label_set_text(abc5hData->disp_Label,labelBuf);

    model2 = GTK_LIST_STORE(gtk_tree_view_get_model(abc5hData->treeView));
    gtk_list_store_clear(model2);

    for(n=1;n<recordCount;n++){
      sscanf(records[n],"%s %s %s %s %s %s %s",param1,param2,param3,param4,param5,param6,param7);
      gtk_list_store_append(model2,&iter2);
      gtk_list_store_set(model2,&iter2,0,param1,1,param2,2,param3,3,param4,4,param5,5,param6,6,param7,-1);
    }
  }

  return;
}




G_MODULE_EXPORT void cb_abc_age_open(GtkButton *button, gpointer data){
  GtkBuilder *builder;
  Abc3HandleData *abc3hData;
  Abc5HandleData *abc5hData;
  const char *startStr;
  char sendBuf[BUFSIZE],recvBuf[BUFSIZE_MAX],labelBuf[BUFSIZE];
  int sendLen,recvLen,recordCount,n;
  char *records[RECORD_MAX], response[BUFSIZE],param1[BUFSIZE],param2[BUFSIZE],param3[BUFSIZE],param4[BUFSIZE],param5[BUFSIZE],param6[BUFSIZE],param7[BUFSIZE];
  GtkTreeModel *model1,*model2;
  GtkListStore *model3;
  GtkTreeIter iter1,iter2,iter3;
  gchar *age1Value =NULL,*age2Value =NULL;



  abc3hData =(Abc3HandleData *)data;
  abc5hData = (Abc5HandleData *)malloc(sizeof(Abc5HandleData));

  /*抽出開始日に入力されている文字列を取得*/
  startStr = gtk_entry_get_text(abc3hData->startentry);


  /*コンボボックスの中から現在選択されているアイテムの取得*/
 
  if(gtk_combo_box_get_active_iter(abc3hData->Age1box,&iter1)){
    model1 = gtk_combo_box_get_model(abc3hData->Age1box);
    gtk_tree_model_get(model1,&iter1,0,&age1Value,-1);
  }


  if(gtk_combo_box_get_active_iter(abc3hData->Age2box,&iter2)){
    model2 = gtk_combo_box_get_model(abc3hData->Age2box);
    gtk_tree_model_get(model2,&iter2,0,&age2Value,-1);
  }




  /*通信用のソケットディスクリプタが空でないかチェック*/
  if(g_soc>0){
    
    sendLen = sprintf(sendBuf, "%s %s %s %s %s %s",ABC,startStr,"age",age1Value,age2Value,ENTER);
    
    send(g_soc,sendBuf,sendLen,0);
    recvLen = recv_data(g_soc,recvBuf,BUFSIZE_MAX);
    recordCount = record_division(recvBuf,records);
    memset(response,0,BUFSIZE);
    memset(param1,0,BUFSIZE);
    memset(param2,0,BUFSIZE);
    memset(param3,0,BUFSIZE);
    memset(param4,0,BUFSIZE);
    memset(param5,0,BUFSIZE);
    memset(param7,0,BUFSIZE);
    
    if(g_abc_treeWindowFlag == 0){

      builder = gtk_builder_new();
      gtk_builder_add_from_file(builder,"abcAnaryze5.glade",NULL);

      abc5hData->abcwindow1 = GTK_WIDGET(gtk_builder_get_object(builder,"abcwindow1"));
      abc5hData->treeView = GTK_TREE_VIEW(gtk_builder_get_object(builder,"treeview1"));
      abc5hData->disp_Label = GTK_LABEL(gtk_builder_get_object(builder,"disp_label"));

      gtk_builder_connect_signals(builder,abc5hData);
      gtk_widget_show_all(abc5hData->abcwindow1);
      g_abc_treeWindowFlag = 1;
    }

    n = sscanf(records[0],"%s %s %s %s %s %s %s %s",response,param1,param2,param3,param4,param5,param6,param7);

    if(strcmp(response,OK_STAT) !=0){
      showABCErrorMsg(abc5hData->disp_Label, atoi(param1));
      return;
    }

    sprintf(labelBuf,"表示件数%d件",atoi(param1));
    gtk_label_set_text(abc5hData->disp_Label,labelBuf);

    model3 = GTK_LIST_STORE(gtk_tree_view_get_model(abc5hData->treeView));
    gtk_list_store_clear(model3);

    for(n=1;n<recordCount;n++){
      sscanf(records[n],"%s %s %s %s %s %s %s",param1,param2,param3,param4,param5,param6,param7);
      gtk_list_store_append(model3,&iter3);
      gtk_list_store_set(model3,&iter3,0,param1,1,param2,2,param3,3,param4,4,param5,5,param6,6,param7,-1);
    }
  }

  return;
}













G_MODULE_EXPORT void cb_abc0_cancel(GtkButton *button, gpointer data){
  Abc0HandleData *hData; //         Widget   

  hData = (Abc0HandleData *)data;
  /*                   */
  gtk_widget_hide(hData->abc0window);
  /*         Widget              ) */
  free(hData);
  /*                 */
  g_abcWindowFlag = 0;

}

G_MODULE_EXPORT void cb_abc1_cancel(GtkButton *button, gpointer data){
  Abc1HandleData *hData; //         Widget   

  hData = (Abc1HandleData *)data;
  /*                   */
  gtk_widget_hide(hData->weatherwindow);
  /*         Widget              ) */
  free(hData);
  /*                 */
  g_abc_weatherWindowFlag = 0;

}


G_MODULE_EXPORT void cb_abc2_cancel(GtkButton *button, gpointer data){
  Abc2HandleData *hData; //         Widget   

  hData = (Abc2HandleData *)data;
  /*                   */
  gtk_widget_hide(hData->genderwindow);
  /*         Widget              ) */
  free(hData);
  /*                 */
  g_abc_genderWindowFlag = 0;

}

G_MODULE_EXPORT void cb_abc3_cancel(GtkButton *button, gpointer data){
  Abc3HandleData *hData; //         Widget   

  hData = (Abc3HandleData *)data;
  /*                   */
  gtk_widget_hide(hData->agewindow);
  /*         Widget              ) */
  free(hData);
  /*                 */
  g_abc_ageWindowFlag = 0;

}


G_MODULE_EXPORT void cb_abc4_cancel(GtkButton *button, gpointer data){
  Abc4HandleData *hData; //         Widget   

  hData = (Abc4HandleData *)data;
  /*                   */
  gtk_widget_hide(hData->daywindow);
  /*         Widget              ) */
  free(hData);
  /*                 */
  g_abc_dayWindowFlag = 0;

}

G_MODULE_EXPORT void cb_abc5_cancel(GtkButton *button, gpointer data){
  Abc5HandleData *hData; //         Widget   

  hData = (Abc5HandleData *)data;
  /*                   */
  gtk_widget_hide(hData->abcwindow1);
  /*         Widget              ) */
  free(hData);
  /*                 */
  g_abc_treeWindowFlag = 0;

}

void showABCErrorMsg(GtkLabel *errorLabel, int errorCode){

    switch(errorCode){
        case 200:
            gtk_label_set_text(errorLabel, "ERROR:抽出開始日を入力してください。");
            break;
        case 300:
            gtk_label_set_text(errorLabel, "ERROR:データベースエラー（そんな日付はありません！！）");
            break;
        case 5100:
            gtk_label_set_text(errorLabel, "ERROR:ログインエラー");
            break;

        default:
            gtk_label_set_text(errorLabel, "ERROR:致命的なエラーが発生しました");
            break;
    }   
}
