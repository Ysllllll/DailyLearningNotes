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
#include "error_code.h"

static const char* errMsg[] = {
  "ERROR 10000 (ER_INIT_ERR) ��ʼ��ʧ��",   
  "ERROR 10001 (ER_INVALID_PARAM) ��Ч�Ĳ���", 
  "ERROR 10002 (ER_FIELD_NOT_FOUND) �ֶβ�����", 
  "ERROR 10003 (ER_SQL_ERR) �����SQL",  
  "ERROR 10004 (ER_TABLE_NOT_FOUND) ������",
  "ERROR 10005 (ER_TABLE_EXISTS) ���Ѵ���",
  "ERROR 10006 (ER_NOMEM) �����ڴ�ʧ��",
  "ERROR 10007 (ER_DATA_TOO_LONG) ����̫��",
  "ERROR 10008 (ER_TABLE_FULL) ������",
  "ERROR 10009 (ER_HANDLE_OPEND) ����Ѵ�",
  "ERROR 10010 (ER_INVALID_FIELD_NAME) ��Ч���ֶ���",
  "ERROR 10011 (ER_FILE_EXISTS) �ļ��Ѵ���",
  "ERROR 10012 (ER_IO_ERR) IOʧ��",
  "ERROR 10013 (ER_TOO_LESS_COLUMN) �ֶ�̫��",
  "ERROR 10014 (ER_TOO_MANY_COLUMN) �ֶ�̫��",
  "ERROR 10015 (ER_TYPE_MISTAKE) ���Ͳ�ƥ��"
};

const char* GetErrMsg(int errCode)
{
  if (errCode == ER_OK)
    return "ERROR 0 (ER_OK) �����ɹ�";

  if (errCode >= ER_MIN_VAL && errCode <= ER_MAX_VAL)
  {
    return errMsg[(errCode - ER_MIN_VAL)];
  }

  return "δ֪�Ĵ�����";
}