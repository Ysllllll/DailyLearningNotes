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
#include "table.h"
#include "string_tool.h"
#include "Log.h"
#include "condition.h"
#include "error_code.h"
#include <algorithm>

#define TABLE_FILE_SIZE     (32 * 1024 * 1024)

extern std::string glbDataPath;

Table::Table()
{
  this->fileHandle_ = INVALID_HANDLE_VALUE;
  this->mapHandle_ = NULL;
  this->pBase_ = NULL;
}

Table::~Table()
{
  Close();
}

int Table::Create(const std::string& tabName, const CreateTableParam* pCreateParam, int* pErrPos)
{
  std::string filePath = glbDataPath + "\\" + tabName + ".dat";

  if (pErrPos != nullptr)
    *pErrPos = -1;

  if (INVALID_HANDLE_VALUE != fileHandle_)
    return ER_HANDLE_OPEND;

  if (pCreateParam->pColList_ == nullptr)
    return ER_INVALID_PARAM;

  const std::vector<ColumnItem*>& colVec = pCreateParam->pColList_->GetColumnList();
  if (colVec.size() == 0)
    return ER_TOO_LESS_COLUMN;

  if (colVec.size() > TABLE_MAX_COL)
    return ER_TOO_MANY_COLUMN;

  MetaPage mpage;
  ZeroMemory(&mpage, sizeof(MetaPage));
  strncpy_s(mpage.headStr_, TABLE_HEAD_STR, _TRUNCATE);
  mpage.colCnt_ = static_cast<int>(colVec.size());

  for (size_t idx = 0; idx < colVec.size(); idx++)
  {
    const std::string& colName = colVec[idx]->GetName();
    if (colName.size() == 0 || colName.size() >= COL_NAME_LEN)
    {
      if (pErrPos != nullptr)
        *pErrPos = colVec[idx]->GetSqlPos();

      return ER_INVALID_FIELD_NAME;
    }
    
    strncpy_s(mpage.cols_[idx].colName_, colName.c_str(), _TRUNCATE);
    mpage.cols_[idx].colType_ = colVec[idx]->GetType();
  }

  //�ж��ļ��Ƿ����
  DWORD dwAttribute = GetFileAttributes(filePath.c_str());
  if (INVALID_FILE_ATTRIBUTES != dwAttribute
    && ((FILE_ATTRIBUTE_DIRECTORY & dwAttribute) == 0))
  {
    return ER_FILE_EXISTS;
  }

  //�˴����Դ���һ����ʱ�ļ�����: tab.dat.tmp �����в�����ɺ��޸�Ϊ tab.dat
  HANDLE tmpHandle = CreateFile(filePath.c_str(),
    GENERIC_READ | GENERIC_WRITE,
    0,
    NULL,
    CREATE_NEW,
    FILE_ATTRIBUTE_NORMAL,
    NULL);

  if (INVALID_HANDLE_VALUE == tmpHandle)
  {
    LOG_OUT("CreateFile ʧ��, %d, %s", GetLastError(), filePath.c_str());
    return ER_IO_ERR;
  }

  SetFilePointer(tmpHandle, TABLE_FILE_SIZE, 0, FILE_BEGIN);
  SetEndOfFile(tmpHandle);

  DWORD bytesWritten = 0;
  SetFilePointer(tmpHandle, 0, 0, FILE_BEGIN);
  BOOL writeRet = WriteFile(tmpHandle, (LPCVOID)&mpage, sizeof(mpage), &bytesWritten, NULL);
  DWORD lastErr = GetLastError();
  CloseHandle(tmpHandle);
  if (!writeRet || sizeof(mpage) != bytesWritten)
  {
    LOG_OUT("WriteFile ʧ��, %d, %s", lastErr, filePath.c_str());
    DeleteFile(filePath.c_str());
    return ER_IO_ERR;
  }

  return Open(tabName.c_str());
}

int Table::Open(const char* pName)
{
  std::string filePath = glbDataPath + "\\" + pName + ".dat";
  if (INVALID_HANDLE_VALUE != fileHandle_)
  {
    return ER_HANDLE_OPEND;
  }

  fileHandle_ = CreateFile(filePath.c_str(),
    GENERIC_READ | GENERIC_WRITE,
    0,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    NULL);

  if (INVALID_HANDLE_VALUE == fileHandle_)
  {
    return ER_IO_ERR;
  }

  mapHandle_ = CreateFileMapping(fileHandle_, NULL, PAGE_READWRITE, 0, TABLE_FILE_SIZE, NULL);
  if (NULL == mapHandle_)
  {
    LOG_OUT("CreateFileMapping ʧ��, %d, %s", GetLastError(), filePath.c_str());
    Close();
    return ER_IO_ERR;
  }

  pBase_ = MapViewOfFile(mapHandle_, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, TABLE_FILE_SIZE);
  if (NULL == pBase_)
  {
    LOG_OUT("MapViewOfFile ʧ��, %d, %s", GetLastError(), filePath.c_str());
    Close();
    return ER_IO_ERR;
  }

  //����Ԫ����
  MetaPage* pMeta = (MetaPage*)pBase_;
  for (int i = 0; i < pMeta->colCnt_; i++)
  {
    std::string colName = pMeta->cols_[i].colName_;
    std::transform(colName.begin(), colName.end(), colName.begin(), ::tolower);
    colPosMap_.insert(std::pair<std::string, int>(colName, i));
  }

  this->tabName_ = pName;
  return ER_OK;
}

int Table::Close()
{
  if (this->pBase_ != NULL)
  {
    FlushViewOfFile(this->pBase_, TABLE_FILE_SIZE);
    UnmapViewOfFile(this->pBase_);
    this->pBase_ = NULL;
  }

  if (this->mapHandle_ != NULL)
  {
    CloseHandle(this->mapHandle_);
    this->mapHandle_ = NULL;
  }
  
  if (this->fileHandle_ != INVALID_HANDLE_VALUE)
  {
    CloseHandle(this->fileHandle_);
    this->fileHandle_ = INVALID_HANDLE_VALUE;
  }

  tabName_ = "";
  colPosMap_.clear();

  return ER_OK;
}

int Table::Insert(const InsertParam* pInsertParam, int* pErrPos)
{
  if (pInsertParam == nullptr)
    return ER_INVALID_PARAM;

  const MetaPage* pMeta = (const MetaPage*)pBase_;
  std::vector<DBVal> rec(pMeta->colCnt_);
  for (size_t idx = 0; idx < rec.size(); idx++)
  {
    DBVAL_SET_NULL(&(rec[idx]));
  }

  const std::vector<ExprItem*>& nameVec = pInsertParam->pNameList_->GetExprList();
  const std::vector<ExprItem*>& valVec = pInsertParam->pValList_->GetExprList();
  
  if (nameVec.empty())
  {
    return ER_SQL_ERR;
  }

  if (nameVec.size() != valVec.size())
  {
    return ER_SQL_ERR;
  }

  int64_t int64Val = 0;
  double doubleVal = 0;

  for (size_t idx = 0; idx < nameVec.size(); idx++)
  {
    const std::string& nameStr = nameVec[idx]->GetTokenStr();
    int fieldPos = GetFieldPos(nameStr.c_str());
    if (fieldPos < 0)
    {
      if (pErrPos != nullptr)
        *pErrPos = nameVec[idx]->GetSqlPos();

      return ER_FIELD_NOT_FOUND;
    }
    int fieldType = pMeta->cols_[fieldPos].colType_;
    char* pEnd = nullptr;

    const std::string& valStr = valVec[idx]->GetTokenStr();
    int valOp = valVec[idx]->GetOp();
    switch (valOp)
    {
    case TK_UINTEGER:
    case TK_INTEGER:
    {
      int64Val = strtoll(valStr.c_str(), &pEnd, 10);
      if (pEnd != (valStr.c_str() + valStr.size()))
      {
        if (pErrPos != nullptr)
          *pErrPos = valVec[idx]->GetSqlPos();

        return ER_SQL_ERR;
      }

      if (valOp == TK_UINTEGER)
        int64Val *= -1;

      if (fieldType == VALUE_TYPE::VAL_INT)
      {
        DBVAL_SET_INT(&(rec[fieldPos]), int64Val);
      }
      else if (fieldType == VALUE_TYPE::VAL_DOUBLE)
      {
        DBVAL_SET_DOUBLE(&(rec[fieldPos]), static_cast<double>(int64Val));
      }
      else
      {
        if (pErrPos != nullptr)
          *pErrPos = valVec[idx]->GetSqlPos();

        return ER_TYPE_MISTAKE;
      }

      break;
    }
    case TK_DOUBLE:
    case TK_UDOUBLE:
    {
      doubleVal = strtod(valStr.c_str(), &pEnd);
      if (pEnd != (valStr.c_str() + valStr.size()))
      {
        if (pErrPos != nullptr)
          *pErrPos = valVec[idx]->GetSqlPos();

        return ER_SQL_ERR;
      }

      if (valOp == TK_UDOUBLE)
        doubleVal *= -1;

      if (fieldType == VALUE_TYPE::VAL_DOUBLE)
      {
        DBVAL_SET_DOUBLE(&(rec[fieldPos]), doubleVal);
      }
      else
      {
        if (pErrPos != nullptr)
          *pErrPos = valVec[idx]->GetSqlPos();

        return ER_TYPE_MISTAKE;
      }

      break;
    }
    case TK_STRING:
    {
      if (fieldType == VALUE_TYPE::VAL_STRING)
      {
        DBVAL_SET_STRING(&(rec[fieldPos]), valStr.c_str(), valStr.size());
      }
      else
      {
        if (pErrPos != nullptr)
          *pErrPos = valVec[idx]->GetSqlPos();

        return ER_TYPE_MISTAKE;
      }
      break;
    }
    default:
      if (pErrPos != nullptr)
        *pErrPos = valVec[idx]->GetSqlPos();

      return ER_SQL_ERR;
    }

  }

  //��rec���л����洢
  //| recLen |  type  |  data         | type | strLen | strData   |
  uint8_t recBuf[MAX_REC_LEN];
  ZeroMemory(recBuf, sizeof(recBuf));
  uint8_t* pTmp = recBuf + 1;
  for (size_t idx = 0; idx < rec.size(); idx++)
  {
    if ((pTmp + 1 + rec[idx].dataLen_) >(recBuf + MAX_REC_LEN))
    {
      return ER_DATA_TOO_LONG;
    }

    switch (rec[idx].valType_)
    {
    case VALUE_TYPE::VAL_NULL:
      *pTmp++ = VALUE_TYPE::VAL_NULL;
      break;
    case VALUE_TYPE::VAL_INT:
      *pTmp++ = VALUE_TYPE::VAL_INT;
      *((int64_t*)pTmp) = rec[idx].val_.intVal_;
      pTmp += sizeof(int64_t);
      break;
    case VALUE_TYPE::VAL_DOUBLE:
      *pTmp++ = VALUE_TYPE::VAL_DOUBLE;
      *((double*)pTmp) = rec[idx].val_.doubleVal_;
      pTmp += sizeof(double);
      break;
    case VALUE_TYPE::VAL_STRING:
      *pTmp++ = VALUE_TYPE::VAL_STRING;
      *pTmp++ = static_cast<uint8_t>(rec[idx].dataLen_);
      strncpy_s((char*)pTmp, (MAX_REC_LEN - (pTmp - recBuf)), rec[idx].val_.strVal_, rec[idx].dataLen_);
      pTmp += rec[idx].dataLen_;
      break;
    }
  }

  uint8_t recLen = static_cast<uint8_t>(pTmp - recBuf);
  recBuf[0] = recLen;

  //��һ��ҳ�����ݴ��ȥ
  for (int idx = 1; idx < (TABLE_FILE_SIZE / (sizeof(DataPage))); idx++)
  {
    DataPage* pPage = &(((DataPage*)pBase_)[idx]);
    if (pPage->pageNo_ != idx)
    {
      //д�����ݵ���ҳ
      pPage->pageNo_ = idx;
      pPage->recCnt_ = 1;
      pPage->freeBytes_ = (DATA_PAGE_DATASIZE - 2 - recLen);
      pPage->chipBytes_ = 0;

      pTmp = pPage->data_ + DATA_PAGE_DATASIZE - recLen;
      memcpy(pTmp, recBuf, recLen);
      ((uint16_t*)(pPage->data_))[0] = static_cast<uint16_t>(pTmp - reinterpret_cast<uint8_t*>(pPage));

      return ER_OK;
    }
    else if ((pPage->freeBytes_ + pPage->chipBytes_) >= (recLen + 2))
    {
      if (pPage->freeBytes_ < recLen)
      {
        //��Ҫ��ҳ�е����ݽ�������
        CleanPage(pPage);
      }

      //�������пռ���ڼ�¼����
      if (pPage->freeBytes_ >= recLen)
      {
        //����д�����ʼλ��
        pTmp = pPage->data_ + pPage->recCnt_ * 2 + pPage->freeBytes_ - recLen;
        memcpy(pTmp, recBuf, recLen);
        ((uint16_t*)(pPage->data_))[pPage->recCnt_] = static_cast<uint16_t>(pTmp - reinterpret_cast<uint8_t*>(pPage));
        pPage->recCnt_++;
        pPage->freeBytes_ -= ((int)recLen + 2);

        return ER_OK;
      }
    }

  }

  return ER_TABLE_FULL;
}

int Table::ExecQuery(const QueryParam* pQueryParam, Query** ppQuery, int* pErrPos)
{
  Query* pQuery = nullptr;
  int result = BuildQuery(pQueryParam, &pQuery, pErrPos);
  if (result != 0)
    return result;

  MetaPage* pMeta = (MetaPage*)pBase_;
  std::vector<DBVal> recVal(pMeta->colCnt_);
  
  for (int pageIdx = 1; pageIdx < (TABLE_FILE_SIZE / (sizeof(DataPage))); pageIdx++)
  {
    DataPage* pPage = &(((DataPage*)pBase_)[pageIdx]);
  
    if (pPage->pageNo_ == pageIdx && pPage->recCnt_ > 0)
    {
      for (int recIdx = 0; recIdx < pPage->recCnt_; recIdx++)
      {
        const uint8_t* pRec = ((const uint8_t*)pPage) + ((uint16_t*)(pPage->data_))[recIdx];
  
        LoadRecord(pRec, recVal);
        pQuery->AppendData(recVal);
      }
    }
  }

  *ppQuery = pQuery;
  return ER_OK;
}

int Table::ExecDelete(const DeleteParam* pDeleteParam, int* pErrPos)
{
  int retVal = ER_OK;
  Condition delCondition;

  retVal = BuildCondition(pDeleteParam->pCondition_, &delCondition, pErrPos);
  if (retVal != ER_OK)
    return retVal;

  MetaPage* pMeta = (MetaPage*)pBase_;
  std::vector<DBVal> recVal(pMeta->colCnt_);
  for (int pageIdx = 1; pageIdx < (TABLE_FILE_SIZE / (sizeof(DataPage))); pageIdx++)
  {
    DataPage* pPage = &(((DataPage*)pBase_)[pageIdx]);

    if (pPage->pageNo_ == pageIdx && pPage->recCnt_ > 0)
    {
      for (int recIdx = (pPage->recCnt_ - 1); recIdx >= 0; recIdx--)
      {
        uint8_t* pRecOffset = (pPage->data_ + recIdx * 2);
        uint8_t* pRec = ((uint8_t*)pPage) + *((uint16_t*)pRecOffset);
        LoadRecord(pRec, recVal);
        if (delCondition.RunCondition(recVal))
        {
          memmove(pRecOffset, (pRecOffset + 2), ((pPage->recCnt_ - recIdx - 1) * 2));

          pPage->recCnt_--;
          pPage->freeBytes_ += sizeof(uint16_t);
          pPage->chipBytes_ += (*pRec);
        }
      }
    }
  }

  return ER_OK;
}

const std::string& Table::GetTableName() const
{
  return this->tabName_;
}

void Table::GetColumnsInfo(std::vector<ColInfo>& colVec) const
{
  MetaPage* pMeta = (MetaPage*)pBase_;
  for (int i = 0; i < pMeta->colCnt_; i++)
  {
    colVec.push_back(pMeta->cols_[i]);
  }
}

int Table::BuildQuery(const QueryParam* pQueryParam, Query** ppQuery, int* pErrPos)
{
  Query* pQuery = nullptr;
  MetaPage* pMeta = (MetaPage*)pBase_;
  int groupFieldPos = -1;

  const std::vector<ExprItem*>& targetVec = pQueryParam->pTargetList_->GetExprList();

  //�ж��Ƿ��Ǿۺϲ�ѯ
  bool isGroup = pQueryParam->pGroup_ != nullptr;
  if (!isGroup)
  {
    //��Ӧ�� pQueryParam->pGroup_ == nullptr ������
    //���ܵ�SQLΪ: SELECT count(*) FROM <tabname>
    for (auto targetIt = targetVec.begin(); targetIt != targetVec.end(); targetIt++)
    {
      //�����ֹ�ID��*������ֶ����Ǿۺϲ�ѯ
      int targetType = (*targetIt)->GetOp();
      if (targetType != TK_STRING && targetType != TK_ID && targetType != TK_STAR)
      {
        isGroup = true;
        break;
      }
    }
  }

  if (!isGroup)
  {
    pQuery = new RawQuery();
  }
  else
  {
    if (pQueryParam->pGroup_ != nullptr)
    {
      //���������ݷ�Ϊһ����в�ѯ
      const std::string& groupName = pQueryParam->pGroup_->GetGroupField();
      groupFieldPos = GetFieldPos(groupName.c_str());
      if (groupFieldPos < 0)
      {
        //�����ֶβ�����
        if (pErrPos != nullptr)
          *pErrPos = pQueryParam->pGroup_->GetSqlPos();

        return ER_FIELD_NOT_FOUND;
      }

      if (pMeta->cols_[groupFieldPos].colType_ != VALUE_TYPE::VAL_INT
        && pMeta->cols_[groupFieldPos].colType_ != VALUE_TYPE::VAL_STRING)
      {
        if (pErrPos != nullptr)
          *pErrPos = pQueryParam->pGroup_->GetSqlPos();

        return ER_SQL_ERR;
      }
    }
    pQuery = new GroupQuery(groupFieldPos);
  }

  int retVal = ER_OK;

  do {
    //��������
    Condition* pCondition = new Condition();
    retVal = BuildCondition(pQueryParam->pCondition_, pCondition, pErrPos);
    if (retVal != ER_OK)
    {
      delete pCondition;
      break;
    }
    pQuery->SetCondition(pCondition);

    if (pQueryParam->pLimit_ != nullptr)
    {
      pQuery->SetQueryOffset(pQueryParam->pLimit_->GetOffset());
      pQuery->SetQueryCnt(pQueryParam->pLimit_->GetQueryCnt());
    }

    //������ѯ���
    for (auto targetIt = targetVec.begin(); targetIt != targetVec.end(); targetIt++)
    {
      retVal = BuildTarget(pQuery, isGroup, groupFieldPos, (*targetIt));
      if (retVal != ER_OK)
      {

        break;
      }
    }

    //��ʼ�������
    //Ϊ�˽�� select count(*) from tab ����ֻ��һ������
    pQuery->InitResult();
  } while (false);

  if (retVal != ER_OK)
  {
    delete pQuery;
  }
  else
  {
    *ppQuery = pQuery;
  }

  return retVal;
}

int Table::BuildCondition(const ExprItem* pCondiExpr, Condition* pCondition, int* pErrPos)
{
  int retVal = 0;
  MetaPage* pMeta = (MetaPage*)pBase_;
  if (pCondiExpr == nullptr)
    return ER_OK;

  const ExprItem* pLeftExpr = pCondiExpr->GetLeftExpr();
  const ExprItem* pRightExpr = pCondiExpr->GetRightExpr();

  DBVal rightVal;
  DBVAL_SET_NULL(&rightVal);

  int op = pCondiExpr->GetOp();
  if (op == TK_AND)
  {
    if (pLeftExpr == nullptr || pRightExpr == nullptr)
    {
      if (pErrPos != nullptr)
        *pErrPos = pCondiExpr->GetSqlPos();

      return ER_SQL_ERR;
    }

    retVal = BuildCondition(pLeftExpr, pCondition, pErrPos);
    if (retVal != ER_OK)
      return retVal;

    retVal = BuildCondition(pRightExpr, pCondition, pErrPos);
    if (retVal != ER_OK)
      return retVal;
  }
  else
  {
    //�������ʽ��߱������ֶ�
    if (pLeftExpr == nullptr)
    {
      if (pErrPos != nullptr)
        *pErrPos = pCondiExpr->GetSqlPos();

      return ER_SQL_ERR;
    }

    if (pLeftExpr->GetOp() != TK_ID)
    {
      if (pErrPos != nullptr)
        *pErrPos = pCondiExpr->GetSqlPos();

      return ER_SQL_ERR;
    }

    const std::string& fieldName = pLeftExpr->GetTokenStr();
    int fieldPos = GetFieldPos(fieldName.c_str());
    if (fieldPos < 0)
    {
      if (pErrPos != nullptr)
        *pErrPos = pCondiExpr->GetSqlPos();

      return ER_FIELD_NOT_FOUND;
    }

    if (pRightExpr == nullptr)
    {
      //һ���������������� �磺 <field> is not null  ,   <field> is null
      if (op == TK_ISNOTNULL)
      {
        pCondition->AddCondition(new IsNotNullCondition(fieldPos));
      }
      else if (op == TK_ISNULL)
      {
        pCondition->AddCondition(new IsNullCondition(fieldPos));
      }
      else
      {
        if (pErrPos != nullptr)
          *pErrPos = pCondiExpr->GetSqlPos();

        return ER_SQL_ERR;
      }
    }
    else
    {
      //��������������������: num > 5 �ȵ�
      const std::string& valueStr = pRightExpr->GetTokenStr();
      int32_t rightOp = pRightExpr->GetOp();
      char* pEnd = nullptr;

      switch (rightOp)
      {
      case TK_INTEGER:
      case TK_UINTEGER:
      {
        rightVal.val_.intVal_ = strtoll(valueStr.c_str(), &pEnd, 10);
        if (pEnd != (valueStr.c_str() + valueStr.size()))
        {
          if (pErrPos != nullptr)
            *pErrPos = pRightExpr->GetSqlPos();

          return ER_SQL_ERR;
        }

        if (rightOp == TK_UINTEGER)
          rightVal.val_.intVal_ *= -1;

        rightVal.valType_ = VALUE_TYPE::VAL_INT;
        rightVal.dataLen_ = sizeof(int64_t);
        break;
      }
      case TK_DOUBLE:
      case TK_UDOUBLE:
      {
        rightVal.val_.doubleVal_ = strtod(valueStr.c_str(), &pEnd);
        if (pEnd != (valueStr.c_str() + valueStr.size()))
        {
          if (pErrPos != nullptr)
            *pErrPos = pRightExpr->GetSqlPos();

          return ER_SQL_ERR;
        }

        if (rightOp == TK_UDOUBLE)
          rightVal.val_.doubleVal_ *= -1;

        rightVal.valType_ = VALUE_TYPE::VAL_DOUBLE;
        rightVal.dataLen_ = sizeof(double);
        break;
      }
      case TK_STRING:
      {
        DBVAL_SET_STRING(&rightVal, valueStr.c_str(), valueStr.size());
        break;
      }
      default:
        if (pErrPos != nullptr)
          *pErrPos = pRightExpr->GetSqlPos();

        return ER_SQL_ERR;
      }

      //������ʽ���ߵ����Ͳ�ƥ��
      if (pMeta->cols_[fieldPos].colType_ == VALUE_TYPE::VAL_DOUBLE
        && rightVal.valType_ == VALUE_TYPE::VAL_INT)
      {
        //int64 ����ת double, ���� double_type >= 5
        rightVal.val_.doubleVal_ = static_cast<double>(rightVal.val_.intVal_);
        rightVal.valType_ = VALUE_TYPE::VAL_DOUBLE;
        rightVal.dataLen_ = sizeof(double);
      }

      if (pMeta->cols_[fieldPos].colType_ != rightVal.valType_)
      {
        if (pErrPos != nullptr)
          *pErrPos = pLeftExpr->GetSqlPos();

        return ER_SQL_ERR;
      }

      ConditionItem* pCondItem = nullptr;
      switch (rightVal.valType_)
      {
      case VALUE_TYPE::VAL_INT:
        pCondItem = BuildIntCondition(fieldPos, op, rightVal.val_.intVal_);
        break;
      case VALUE_TYPE::VAL_DOUBLE:
        pCondItem = BuildDoubleCondition(fieldPos, op, rightVal.val_.doubleVal_);
        break;
      case VALUE_TYPE::VAL_STRING:
        pCondItem = BuildStringCondition(fieldPos, op, rightVal.val_.strVal_);
        break;
      }
      if (pCondItem == nullptr)
      {
        if (pErrPos != nullptr)
          *pErrPos = pLeftExpr->GetSqlPos();

        return ER_SQL_ERR;
      }

      pCondition->AddCondition(pCondItem);
    }
  }

  return ER_OK;
}

int Table::GetFieldPos(const char* pName)
{
  std::string name = pName;
  std::transform(name.begin(), name.end(), name.begin(), ::tolower);
  auto colIt = colPosMap_.find(name);
  if (colIt != colPosMap_.end())
    return colIt->second;

  return -1;
}

ConditionItem* Table::BuildIntCondition(int fieldPos, int op, int64_t val)
{
  switch (op)
  {
  case TK_LT:
    return new IntCondition<TK_LT>(fieldPos, val);
  case TK_LE:
    return new IntCondition<TK_LE>(fieldPos, val);
  case TK_GT:
    return new IntCondition<TK_GT>(fieldPos, val);
  case TK_GE:
    return new IntCondition<TK_GE>(fieldPos, val);
  case TK_EQ:
    return new IntCondition<TK_EQ>(fieldPos, val);
  case TK_NE:
    return new IntCondition<TK_NE>(fieldPos, val);
  }

  return nullptr;
}

ConditionItem* Table::BuildDoubleCondition(int fieldPos, int op, double val)
{
  switch (op)
  {
  case TK_LT:
    return new DoubleCondition<TK_LT>(fieldPos, val);
  case TK_LE:
    return new DoubleCondition<TK_LE>(fieldPos, val);
  case TK_GT:
    return new DoubleCondition<TK_GT>(fieldPos, val);
  case TK_GE:
    return new DoubleCondition<TK_GE>(fieldPos, val);
  case TK_EQ:
    return new DoubleCondition<TK_EQ>(fieldPos, val);
  case TK_NE:
    return new DoubleCondition<TK_NE>(fieldPos, val);
  }
  return nullptr;
}

ConditionItem* Table::BuildStringCondition(int fieldPos, int op, const std::string& val)
{
  switch (op)
  {
  case TK_EQ:
    return new StringCondition<TK_EQ>(fieldPos, val);
  case TK_NE:
    return new StringCondition<TK_NE>(fieldPos, val);
  case TK_LIKE:
    return new StringCondition<TK_LIKE>(fieldPos, val);
  }

  return nullptr;
}

int Table::BuildTarget(Query* pQuery, bool isGroup, int groupFieldPos, const ExprItem* pTarget)
{
  const MetaPage* pMeta = (const MetaPage*)pBase_;
  int op = pTarget->GetOp();

  if (TK_ID == op)
  {
    const std::string& fieldName = pTarget->GetTokenStr();
    int fieldPos = GetFieldPos(fieldName.c_str());
    if (fieldPos < 0)
      return ER_FIELD_NOT_FOUND;

    if (isGroup && groupFieldPos != fieldPos)
      return ER_SQL_ERR;

    pQuery->AddTarget(fieldName.c_str(), new RawResultField(fieldPos));

    return ER_OK;
  }
  else if (TK_STAR == op)
  {
    if (isGroup)
      return ER_SQL_ERR;

    for (int idx = 0; idx < pMeta->colCnt_; idx++)
    {
      pQuery->AddTarget(pMeta->cols_[idx].colName_, new RawResultField(idx));
    }

    return ER_OK;
  }
  else if (TK_FUNC == op)
  {
    std::string funcName = pTarget->GetTokenStr();
    std::transform(funcName.begin(), funcName.end(), funcName.begin(), ::tolower);

    const ExprList* pArgs = pTarget->GetArgList();
    if (pArgs == nullptr)
      return ER_SQL_ERR;

    const std::vector<ExprItem*>& argVec = pArgs->GetExprList();
    if (argVec.size() != 1) //Ŀǰ���еľۺϺ�����ֻ��һ������
      return ER_SQL_ERR;

    int argOp = argVec[0]->GetOp();

    if (funcName.compare("count") == 0 && argOp == TK_STAR)
    {
      // count(*) 
      pQuery->AddTarget("count(*)", new CountField(-1));
      return ER_OK;
    }
    
    if (argOp != TK_ID)
      return ER_SQL_ERR;

    const std::string& argName = argVec[0]->GetTokenStr();
    int argPos = GetFieldPos(argName.c_str());
    if (argPos < 0)
      return ER_FIELD_NOT_FOUND;

    int argType = pMeta->cols_[argPos].colType_;

    std::string targetName = funcName + "(" + argName + ")";

    if (funcName.compare("count") == 0)
    {
      pQuery->AddTarget(targetName.c_str(), new CountField(argPos));
    }
    else if (funcName.compare("avg") == 0)
    {
      switch (argType)
      {
      case VALUE_TYPE::VAL_INT:
        pQuery->AddTarget(targetName.c_str(), new AvgIntField(argPos));
        return ER_OK;
      case VALUE_TYPE::VAL_DOUBLE:
        pQuery->AddTarget(targetName.c_str(), new AvgDoubleField(argPos));
        return ER_OK;
      }
    }
    else if (funcName.compare("max") == 0)
    {
      switch (argType)
      {
      case VALUE_TYPE::VAL_INT:
        pQuery->AddTarget(targetName.c_str(), new MaxField<VALUE_TYPE::VAL_INT>(argPos));
        return ER_OK;
      case VALUE_TYPE::VAL_DOUBLE:
        pQuery->AddTarget(targetName.c_str(), new MaxField<VALUE_TYPE::VAL_DOUBLE>(argPos));
        return ER_OK;
      }
    }
    else if (funcName.compare("min") == 0)
    {
      switch (argType)
      {
      case VALUE_TYPE::VAL_INT:
        pQuery->AddTarget(targetName.c_str(), new MinField<VALUE_TYPE::VAL_INT>(argPos));
        return ER_OK;
      case VALUE_TYPE::VAL_DOUBLE:
        pQuery->AddTarget(targetName.c_str(), new MinField<VALUE_TYPE::VAL_DOUBLE>(argPos));
        return ER_OK;
      }
    }
    else if (funcName.compare("sum") == 0)
    {
      switch (argType)
      {
      case VALUE_TYPE::VAL_INT:
        pQuery->AddTarget(targetName.c_str(), new SumIntField(argPos));
        return ER_OK;
      case VALUE_TYPE::VAL_DOUBLE:
        pQuery->AddTarget(targetName.c_str(), new SumDoubleField(argPos));
        return ER_OK;
      }
    }
  }

  return ER_SQL_ERR;
}

void Table::LoadRecord(const uint8_t* pRec, std::vector<DBVal>& recVal)
{
  //��һ���ֽ��Ǽ�¼����
  pRec++;

  for (size_t colIdx = 0; colIdx < recVal.size(); colIdx++)
  {
    int type = *pRec;
    int strLen = 0;
    pRec++;
    switch (type)
    {
    case VALUE_TYPE::VAL_NULL:
      DBVAL_SET_NULL(&(recVal[colIdx]));
      break;
    case VALUE_TYPE::VAL_INT:
      DBVAL_SET_INT(&(recVal[colIdx]), *((int64_t*)pRec));
      pRec += sizeof(int64_t);
      break;
    case VALUE_TYPE::VAL_DOUBLE:
      DBVAL_SET_DOUBLE(&(recVal[colIdx]), *((double*)pRec));
      pRec += sizeof(double);
      break;
    case VALUE_TYPE::VAL_STRING:
      strLen = *((uint8_t*)pRec);
      pRec++;
      DBVAL_SET_STRING(&(recVal[colIdx]), (const char*)pRec, strLen);
      pRec += strLen;
      break;
    }
  }
}

void Table::CleanPage(DataPage* pPage)
{
  DataPage tmpPage;
  ZeroMemory(&tmpPage, sizeof(DataPage));

  uint8_t* pTmp = tmpPage.data_ + DATA_PAGE_DATASIZE;
  for (int recIdx = 0; recIdx < pPage->recCnt_; recIdx++)
  {
    uint8_t* pRecOffset = (pPage->data_ + recIdx * 2);
    uint8_t* pRec = pPage->data_ + *((uint16_t*)pRecOffset);

    pTmp -= *(pRec);
    memcpy(pTmp, pRec, *(pRec));
    ((uint16_t*)tmpPage.data_)[recIdx] = static_cast<uint16_t>(pTmp - tmpPage.data_);
  }

  tmpPage.pageNo_ = pPage->pageNo_;
  tmpPage.recCnt_ = pPage->recCnt_;
  tmpPage.chipBytes_ = 0;
  tmpPage.freeBytes_ = static_cast<int>(pTmp - tmpPage.data_ - (pPage->recCnt_ * 2));
  
  memcpy(pPage, &tmpPage, sizeof(DataPage));
}