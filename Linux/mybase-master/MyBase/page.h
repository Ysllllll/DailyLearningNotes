#pragma once
/*
** Copyright (c) 2019 ZhangQuan
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public
** License as published by the Free Software Foundation; either
** version 3 of the License, or (at your option) any later version.
**
** Author contact information:
**   zhangqhn@foxmail.com
*/

#define TABLE_HEAD_STR_LEN 16
#define TABLE_HEAD_STR   "MYBASE 1"

#define COL_NAME_LEN   60
#define TABLE_MAX_COL  15

#define DATA_PAGE_DATASIZE     (4080)
#define DATA_PAGE_HEADSIZE     (4096 - DATA_PAGE_DATASIZE)

#define MAX_REC_LEN    255

//ColInfo 256
typedef struct _ColInfo
{
  char colName_[COL_NAME_LEN];     // ����
  int  colType_;                   // ������
  char  padding_[192];             // ��䵽256�ֽ�                   
}ColInfo;


typedef struct _MetaPage
{
  char headStr_[TABLE_HEAD_STR_LEN];  // �ַ�����MYBASE 1
  int colCnt_;                        // ������
  char padding_[236];                 // ��䵽256�ֽ�                    

  ColInfo cols_[TABLE_MAX_COL];
}MetaPage;


typedef struct _DataPage
{
  int pageNo_;     // ҳ�ţ�����ҳ��1��ʼ
  int recCnt_;     // ��¼����
  int freeBytes_;  // �����ֽ�����
  int chipBytes_;  // ��Ƭ�ֽ�����
  uint8_t data_[DATA_PAGE_DATASIZE];
}DataPage;