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
#include <time.h>

#define MALE 1
#define FEMALE 0

// プロトコルコマンド 

// レスポンスステータス 


// エラーコード 


// 権限 
extern int g_sales1WindowFlag;

const gchar *ageData[]={"10代以下", "20代", "30代","40代","50代","60代以上"};
const gchar *weatherData[]={"晴れ", "雨", "曇り","雪"};

typedef struct _BuyData{
    int productNumber;
    int purchaseNumber;
    int buyNumber;
}BuyData;

//*** 販売会計画面のWidget with ログイン画面のWidget ***//
typedef struct _salesHandleData{
    char *customerId;
    int useAblePoint;
    int useMoney;
    int getPoint;
    int usePoint;
    int totalMoney;
    int pointLogin;
    char *weatherStr;
    char *ageStr;
    const char *KionStr;
    BuyData nopointData[256];
    int nopointDataMax;
    int selectedGender;

    GtkWidget  *salesWindow;           //販売会計ウィンドウ１
    GtkWidget  *breakDialog;			//breakDialogウィンドウ
    GtkWidget  *pointcardWindow;
    GtkWidget  *resultWindow;
    GtkWidget  *nenreiDialog;
    GtkWidget  *resultdangerDialog;
    GtkWidget  *endDialog;
    GtkButton  *cancelButton1;          //戻るボタン
    GtkButton  *cancelButton3;			//breakDialogのいいえボタン
    GtkButton  *nextButton;          //次へボタン
    GtkButton  *okButton5;
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
    GtkLabel *noticeLabel;
    GtkLabel *totalmoneyLabel;
    GtkLabel *havepointLabel;
    GtkEntry *moneyEntry;
    GtkEntry *usepointEntry;
    GtkTextView *reciptBox;
    GtkEntry *KionEntry;
    GtkRadioButton *maleRadiobutton;
    GtkRadioButton *femaleRadiobutton;	
    GtkLabel *resulterrorLabel;
}_salesHandleData;
#endif
