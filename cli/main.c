/*
* Copyright (c) 2016
* All rights reserved.
*
* �ļ����ƣ�cliui.c
* �ļ���ʶ��
* ժ Ҫ��clib��ʵ���ͼ򵥲����ļ������ļ���Ubuntu 14.04.3 LTS + gcc 4.8.4������ͨ��
*
* ��ǰ�汾 0.1
* �� �ߣ������
* ������ڣ�2016��1��31��
*
*/
#include <stdio.h>
#include "cliui.h"
#include "../util/girl.h"

void showlun(const char cmdStr[][CMD_WORD_MAX], int wordNum)
{
    printf("vlun show all\n");
}

void showlunde(const char cmdStr[][CMD_WORD_MAX], int wordNum)
{
    printf("vlun show detail\n");
}

void showvdisk(const char cmdStr[][CMD_WORD_MAX], int wordNum)
{
    printf("vdisk show all\n");
}

void showvdiskdetail(const char cmdStr[][CMD_WORD_MAX], int wordNum)
{
    printf("vdisk show detail\n");
}

void showvdiskdetail1(char cmdStr[][CMD_WORD_MAX], int wordNum)
{
    printf("vdisk show detail %s\n", cmdStr[3]);
}


void dofavor(const char cmdStr[][CMD_WORD_MAX], int wordNum)
{
    printf("%s", __girl__global);
}

char cmds[][4][CMD_WORD_MAX] = {
    {"vlun", "show", "all"},
    {"vlun", "show", "detail"},
    {"vdisk", "show", "all"},
    {"vdisk", "show", "detail"},
    {"do", "me", "favor"}
};

void *funcs[] = {showlun, showlunde, showvdisk, showvdiskdetail, dofavor};

int main()
{
    int i;
    CMD_ITEM_S *item;
    char cmd1[][CMD_WORD_MAX] = {"vdisk", "show", "detail", "<1-10>"};
    for (i = 0; i < sizeof(cmds)/sizeof(cmds[0]); i++)
    {
        registerCmd(cmds[i], funcs[i], 3);
    }

    for (i = 0; i < sizeof(cmds)/sizeof(cmds[0]) - 1; i++)
    {
        item = matchCmd(cmds[i], 3); 
        item->pfFunc(NULL, 0);
    }

    registerCmd(cmd1, showvdiskdetail1, 4);


    (void)initCli();
    
    processCli("[test]");
    
    DeinitCli();    
    return 0;
}

