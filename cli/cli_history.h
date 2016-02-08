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


