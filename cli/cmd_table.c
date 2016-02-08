/*
* Copyright (c) 2016
* All rights reserved.
*
* 文件名称：cmd_table.c
* 文件标识：
* 摘 要：命令行注册，以及模式匹配实现
*
* 当前版本 0.1
* 作 者：刘彦夫
* 完成日期：2016年1月31日
*
*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "cmd_table.h"

static CMD_LIST_HEAD_S g_cmdTableRoot  = LIST_HEAD_INITIALIZER(0);

void showAll()
{
    CMD_ITEM_S *i, *j, *k;
    LIST_FOREACH(i, &g_cmdTableRoot, node)
    {
        printf("%s:", i->cmdWord);
        LIST_FOREACH(j, &i->subItems, node)
        {
            printf("%s :", j->cmdWord);
            LIST_FOREACH(k, &j->subItems, node)
            {
                printf("%s ", k->cmdWord);
                
            }            
        }
        printf("\n");
    } 
    printf("\n----------------\n");
}

void getCmdAllFirstItem(char *str, int len)
{
    CMD_ITEM_S *i;
    int pos = 0;
    
    LIST_FOREACH(i, &g_cmdTableRoot, node)
    {
        pos += snprintf(str + pos, len - pos, "\n%s", i->cmdWord);
    } 
}

CMD_ITEM_S *createCmdItem(const char *cmdWord, CMD_PROC_FUNC func)
{
    CMD_ITEM_S *item = (CMD_ITEM_S *)malloc(sizeof(CMD_ITEM_S));
    memset(item, 0, sizeof(CMD_ITEM_S));
    item->cmdWord = cmdWord;
    item->pfFunc = func;
    return item;
}

CMD_ITEM_S *insertCmdItem(const char *cmdWord, CMD_PROC_FUNC func, CMD_ITEM_S *preItem)
{
    CMD_ITEM_S *item = NULL;

    item = createCmdItem(cmdWord, func);
    assert(item);
    if (NULL == preItem) /* 说明是第一个节点 */
    {
        LIST_INSERT_HEAD(&g_cmdTableRoot, item, node);
    }
    else
    {
        LIST_INSERT_HEAD(&preItem->subItems, item, node);
    }
    
#ifdef TEST
    showAll();
#endif

    return item;
}

CMD_ITEM_S *getSubCmdItem(CMD_LIST_HEAD_S *listHead, const char *cmdWord)
{
    CMD_ITEM_S *item;
    
    LIST_FOREACH(item, listHead, node)
    {
        if (0 == strcmp(cmdWord, item->cmdWord))
        {
            return item;
        }
    }

    return NULL;
}


void registerCmd(char (*cmd)[CMD_WORD_MAX], CMD_PROC_FUNC func, int wordNum)
{
    int i;
    CMD_ITEM_S *curItem = NULL;
    CMD_ITEM_S *parentItem = NULL;
    CMD_LIST_HEAD_S *lst = NULL;
    CMD_PROC_FUNC pfn = NULL;
    
    for (i = 0; i < wordNum; i++)
    {
        /*
           * 循环的第一次一定会先在全局表上搜索，后续则在子item上搜索
           */
        lst = (parentItem) ? (&parentItem->subItems) : &g_cmdTableRoot;
        
        curItem = getSubCmdItem(lst, cmd[i]);
        if (curItem)
        {
            parentItem = curItem;
            continue;
        }

        pfn = (i == wordNum - 1) ? func : NULL; /* 仅在最后一个item上注册处理函数 */
        parentItem = insertCmdItem(cmd[i], pfn, parentItem);
    }
}

int isValueType(const char *cmdWord)
{
    if (cmdWord[0] == '<' && cmdWord[strlen(cmdWord) - 1] == '>')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
 * 模糊匹配
 */
CMD_ITEM_S *smartMatchCmdItem(CMD_LIST_HEAD_S  *lst, const char *cmdWord, int isFirstMatch)
{
    CMD_ITEM_S *item;
    CMD_ITEM_S *last;
    int n = 0;

    LIST_FOREACH(item, lst, node)
    {
        if (isValueType(item->cmdWord))
        {
            return item;
        }
        
        if (item->cmdWord == strstr(item->cmdWord, cmdWord))
        {
            if (isFirstMatch)
            {
                return item;
            }

            last = item;
            n++;
        }
    }

    /*
      * 如果就匹配上一个，就认为匹配到了，0个或>1个，都认为匹配失败
      */
    if (1 == n)
    {
        return last;
    }

    return NULL;
}

CMD_ITEM_S *matchCmd(char (*cmd)[CMD_WORD_MAX], int wordNum)
{
    int i;
    CMD_ITEM_S *curItem = NULL;
    CMD_ITEM_S *parentItem = NULL;
    CMD_LIST_HEAD_S *lst = NULL;
    
    for (i = 0; i < wordNum; i++)
    {
        /*
           * 循环的第一次一定会先在全局表上搜索，后续则在子item上搜索
           */
        lst = (parentItem) ? (&parentItem->subItems) : &g_cmdTableRoot;
        
        curItem = smartMatchCmdItem(lst, cmd[i], 0);
        if (NULL == curItem)
        {
            return NULL;
        }
        parentItem = curItem;
    }

    return curItem;
}

CMD_LIST_HEAD_S *getRootList() { return &g_cmdTableRoot; } 

CMD_LIST_HEAD_S *getBrotherList(char (*cmd)[CMD_WORD_MAX], int wordNum)
{
    int i;
    CMD_ITEM_S *curItem = NULL;
    CMD_ITEM_S *parentItem = NULL;
    CMD_LIST_HEAD_S *lst = NULL;

    for (i = 0; i < wordNum - 1; i++)
    {
        /*
           * 循环的第一次一定会先在全局表上搜索，后续则在子item上搜索
           */
        lst = (curItem) ? (&curItem->subItems) : &g_cmdTableRoot;
        curItem = smartMatchCmdItem(lst, cmd[i], 0);
        if (NULL == curItem)
        {
            return NULL;
        }
        parentItem = curItem;
    }

    return parentItem ? &parentItem->subItems : &g_cmdTableRoot;
}

