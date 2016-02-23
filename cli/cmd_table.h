
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
* 文件名称：cmd_table.h
* 文件标识：
* 摘 要：命令行内部头文件
*
* 当前版本 0.1
* 作 者：刘彦夫
* 完成日期：2016年1月31日
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

