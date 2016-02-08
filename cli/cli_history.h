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

#ifndef CMD_HIS_H
#define CMD_HIS_H
#include <string.h>
#include <assert.h>
#include "cmd_table.h"

#ifdef __cplusplus
#extern "C" {
#endif 

void dumpHistory();
void insertCmdHistory(char cmd[][CMD_WORD_MAX], int wordNum);
int getPrevCmd(char (** cmd)[CMD_WORD_MAX]);
int getNextCmd(char (** cmd)[CMD_WORD_MAX]);

#define COPY_CMD(dst, src, wordNum) \
do { \
    int i; \
    for (i = 0; i < wordNum; i++) \
    { \
        strncpy(dst[i], src[i], CMD_WORD_MAX); \
    } \
} while (0)

#ifdef __cplusplus
}
#endif

#endif


