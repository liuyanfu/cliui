/*
* Copyright (c) 2016
* All rights reserved.
*
* 文件名称：cliui.h
* 文件标识：
* 摘 要：命令行对外接口文件
*
* 当前版本 0.1
* 作 者：刘彦夫
* 完成日期：2016年1月31日
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

