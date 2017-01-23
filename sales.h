/*
 * Copyright (c) 2016
 *    Futa Hirakoba.  All rights reserved.
 *
 * sales.h
 *        2016.12.12 16:00
 *
 */

/**
 * sales.h
 **/

#ifndef _INCLUDE_SALES_
#define _INCLUDE_SALES_

#include "pos_client.h"

// プロトコルコマンド 

// レスポンスステータス 


// エラーコード 


// 権限 
extern int g_sales1WindowFlag;

const gchar *ageData[]={"10代以下", "20代", "30代","40代","50代","60代以上"};
const gchar *weatherData[]={"晴れ", "雨", "曇り","雪"};

//*** 販売会計画面のWidget with ログイン画面のWidget ***//
typedef struct _salesHandleData{
	char *customerId;
	int pointLogin;
	
	GtkWidget  *salesWindow;           //販売会計ウィンドウ１
	GtkWidget  *breakDialog;			//breakDialogウィンドウ
	GtkWidget  *pointcardWindow;
	GtkWidget  *resultWindow;
	GtkButton  *cancelButton1;          //戻るボタン
	GtkButton  *cancelButton3;			//breakDialogのいいえボタン
	GtkButton  *nextButton;          //次へボタン
	GtkComboBox	*ageComboBox;
	GtkListStore *ageModel;
	GtkTreeIter ageIter;
	GtkComboBox *weatherComboBox;
	GtkListStore *weatherModel;
	GtkTreeIter weatherIter;
	GtkEntry *productidEntry;
	GtkTreeView *productTree;
	GtkListStore *productModel;
	GtkTreeIter productIter;
	GtkEntry *pointidEntry;
	GtkLabel *pointresultLabel;
	GtkButton *pointcardButton;
	GtkSpinButton *valueSpinbutton;

	GtkRadioButton *maleRadiobutton;
	GtkRadioButton *femaleRadiobutton;	
	GtkRadioButton *nenrei10Radiobutton;
	GtkRadioButton *nenrei20Radiobutton;
	GtkRadioButton *nenrei30Radiobutton;
	GtkRadioButton *nenrei40Radiobutton;
	GtkRadioButton *sunnyRadiobutton;
	GtkRadioButton *rainyRadiobutton;
	GtkRadioButton *cloudyRadiobutton;
	GtkRadioButton *snowRadiobutton;
	GtkRadioButton *on_pointRadiobutton;
	GtkRadioButton *off_pointRadiobutton;
}_salesHandleData;
#endif
