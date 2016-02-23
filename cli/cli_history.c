/*
* Copyright (c) 2016
* All rights reserved.
*
* 文件名称：cli_history.c
* 文件标识：
* 摘 要：cli历史命令上下翻滚功能
*
* 当前版本 0.1
* 作 者：刘彦夫
* 完成日期：2016年2月23日
*
*/
#include <stdio.h>
#include "cli_history.h"
#define CMD_HISTORY_NUM 10


typedef struct tagCmdLog 
{
    char log[CMD_HISTORY_NUM][CMD_WORD_NUM_MAX][CMD_WORD_MAX];
    int logCmdSize[CMD_HISTORY_NUM];
    unsigned int beginIndex;
    unsigned int lastIndex;
    unsigned int curIndex;
} CMD_LOG_S;

static CMD_LOG_S g_cmdLog = {
    {{""}},
    {0},
    -1,
    -1,
    -1
};

void dumpHistory()
{
//#define DEBUG_CMD
#ifdef DEBUG_CMD
    int i, j;
    printf("\n==============\nbeg:%d,cur:%d,last:%d========\n", 
        g_cmdLog.beginIndex,
        g_cmdLog.curIndex,
        g_cmdLog.lastIndex);

    for (i = 0; i < CMD_HISTORY_NUM; i++)
    {
        for (j = 0; j < g_cmdLog.logCmdSize[i]; j++)
        {
            printf("%s ", g_cmdLog.log[i][j]);
        }
        putchar('\n');
    }
#endif
}

void insertCmdHistory(char cmd[][CMD_WORD_MAX], int wordNum)
{
    unsigned int nextIndex;
    char (*pcmd)[CMD_WORD_MAX];
    int isSame = 0;
    int i;

    /*
      * 一定是初始化的第一次插入
      */
    if (g_cmdLog.curIndex == -1)
    {
        assert(g_cmdLog.beginIndex == -1);
        pcmd = g_cmdLog.log[0];
        COPY_CMD(pcmd, cmd, wordNum);
        g_cmdLog.logCmdSize[0] = wordNum;
        g_cmdLog.lastIndex = 0;
        g_cmdLog.curIndex = 0;
        g_cmdLog.beginIndex = 0;
        return;
    }

    /*
      * 如果就是上一次重复的命令，则不记录
      */
    isSame = 1;
    for (i = 0; i < wordNum; i++)
    {
        if (0 != strcmp(cmd[i], g_cmdLog.log[g_cmdLog.lastIndex][i]))
        {
            isSame = 0;
            break;
        }
    }

    /*
      * 如果和最后一个log命令相等，则插入位置不变
      */
    if (isSame)
    {
        nextIndex = g_cmdLog.lastIndex;
    }
    else
    {
         nextIndex = (++g_cmdLog.lastIndex) % CMD_HISTORY_NUM;
    }

    pcmd = g_cmdLog.log[nextIndex];   
    COPY_CMD(pcmd, cmd, wordNum);
    g_cmdLog.logCmdSize[nextIndex] = wordNum;
    g_cmdLog.lastIndex = nextIndex;
    g_cmdLog.curIndex = nextIndex;

    /*
      * 反转覆盖第一个，需要把第一个下标往下移动
      */
    if (!isSame && nextIndex == g_cmdLog.beginIndex)
    {
        ++g_cmdLog.beginIndex;
        g_cmdLog.beginIndex = (g_cmdLog.beginIndex) % CMD_HISTORY_NUM;
    }
}

int getPrevCmd(char (** cmd)[CMD_WORD_MAX])
{
    if (g_cmdLog.curIndex == -1)
    {
        return 0;
    }
    
    if (g_cmdLog.beginIndex == g_cmdLog.curIndex)
    {
        *cmd = g_cmdLog.log[g_cmdLog.beginIndex];
        return g_cmdLog.logCmdSize[g_cmdLog.beginIndex];
    }
    else
    {
        unsigned int idx = g_cmdLog.curIndex % CMD_HISTORY_NUM;
        *cmd = g_cmdLog.log[idx];
        g_cmdLog.curIndex = idx - 1;
        return g_cmdLog.logCmdSize[idx];
    }
}

int getNextCmd(char (** cmd)[CMD_WORD_MAX])
{
    if (g_cmdLog.curIndex == -1)
    {
        return 0;
    }
    
    if (g_cmdLog.lastIndex == g_cmdLog.curIndex)
    {
        *cmd = g_cmdLog.log[g_cmdLog.lastIndex];
        return g_cmdLog.logCmdSize[g_cmdLog.lastIndex];
    }
    else
    {
        unsigned int idx = (++g_cmdLog.curIndex) % CMD_HISTORY_NUM;
        g_cmdLog.curIndex = idx;
        *cmd = g_cmdLog.log[idx];
        return g_cmdLog.logCmdSize[idx];
    }
}

