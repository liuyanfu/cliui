
#ifndef CMD_TABLE_H
#define CMD_TABLE_H

#include "list_inc.h"

#ifdef __cplusplus
#extern "C" {
#endif
/*
* Copyright (c) 2016
* All rights reserved.
*
* �ļ����ƣ�cmd_table.h
* �ļ���ʶ��
* ժ Ҫ���������ڲ�ͷ�ļ�
*
* ��ǰ�汾 0.1
* �� �ߣ������
* ������ڣ�2016��1��31��
*
*/

#define CMD_WORD_MAX 20
#define CMD_WORD_NUM_MAX 50

typedef void (*CMD_PROC_FUNC)(char cmdStr[][CMD_WORD_MAX], int wordNum);

typedef LIST_HEAD(tagCmdListHead, tagCmdItem) CMD_LIST_HEAD_S;

typedef struct tagCmdItem
{
    LIST_ENTRY(tagCmdItem) node;
    CMD_LIST_HEAD_S subItems;
    const char *cmdWord;
    CMD_PROC_FUNC pfFunc;
} CMD_ITEM_S;

CMD_ITEM_S *matchCmd(char (*cmd)[CMD_WORD_MAX], int wordNum);
void getCmdAllFirstItem(char *str, int len);
CMD_LIST_HEAD_S *getBrotherList(char (*cmd)[CMD_WORD_MAX], int wordNum);
CMD_LIST_HEAD_S *getRootList();
#ifdef __cplusplus
}
#endif

#endif

