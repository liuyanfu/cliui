/*
* Copyright (c) 2016
* All rights reserved.
*
* �ļ����ƣ�cliui.h
* �ļ���ʶ��
* ժ Ҫ�������ж���ӿ��ļ�
*
* ��ǰ�汾 0.1
* �� �ߣ������
* ������ڣ�2016��1��31��
*
*/

#include "cmd_table.h"

#ifdef __cplusplus
#extern "C" {
#endif

int initCli();

void DeinitCli();

void registerCmd(char (*cmd)[CMD_WORD_MAX], CMD_PROC_FUNC func, int wordNum);
void processCli(const char *prompt);

#ifdef __cplusplus
}
#endif

