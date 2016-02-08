/*
* Copyright (c) 2016
* All rights reserved.
*
* 文件名称：cliui.c
* 文件标识：
* 摘 要：cli的提示，TAB件联想功能的实现
*
* 当前版本 0.1
* 作 者：刘彦夫
* 完成日期：2016年1月31日
*
*/

#include <stdio.h>  
#include <termios.h>  
#include <unistd.h>  
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "cliui.h"
#include "cli_history.h"

#define ECHOFLAGS (ECHO | ECHOE | ECHOK | ECHONL |ICANON) 
#define CMD_EOF -1
#define CMD_DONE -2
#define CMD_TAB -3
#define CMD_TAB_FAILED -4
#define CMD_BACK -5
#define CMD_UP -6
#define CMD_DOWN -7


//#define DEBUG_CMD
void debugCmd(char *msg, char cmd[][CMD_WORD_MAX], int wordNum)
{
#ifdef DEBUG_CMD
    int i;
    printf("\n------------------\n");
    printf("%s, cmd len=%d\n", msg, wordNum);
    for (i = 0; i < wordNum; i++)
    {
        printf("%s ", cmd[i]);
    }
    printf("\n------------------\n");
#endif
    return;
}

/*
 * 函数set_disp_mode用于控制是否开启输入回显功能  
 * 如果option为0，则关闭回显，为1则打开回显
 */
int setInputMode(int fd,int option)  
{  
   int err;  
   struct termios term; 
   
   if (tcgetattr(fd,&term) == -1)
   {  
     perror("Cannot get the attribution of the terminal");  
     return 1;  
   }  
   
   if(option)  
   {
        term.c_lflag |= ECHOFLAGS;  
   }
   else  
   {
        term.c_lflag &= ~ECHOFLAGS;
   }
   
   err = tcsetattr(fd, TCSAFLUSH, &term);  
   if(err == -1 && err == EINTR)
   {  
        perror("Cannot set the attribution of the terminal");  
        return 1;  
   }  
   
   return 0;  
}

int isBlank(char *str)
{
    char *chr;
    for (chr = str; *chr != '\0'; chr++)
    {
        if (!isspace(*chr))
        {
            return 0;
        }
    }
    return 1;
}

int completeWord(char cmd[][CMD_WORD_MAX], int wordNum) 
{
    CMD_ITEM_S *i;
    CMD_ITEM_S *last = NULL;
    char *lastWord;
    int wordlen = 0;
    int n = 0;
    CMD_LIST_HEAD_S *lst;
    char candidateWords[CMD_WORD_NUM_MAX * CMD_WORD_MAX] = {0};

    if (0 == wordNum)
    {
        lst = getRootList();
        LIST_FOREACH(i, lst, node)
        {
            /* for safe */
            if (CMD_WORD_NUM_MAX * CMD_WORD_MAX - strlen(candidateWords)
                < CMD_WORD_MAX + 1)
            {
                break;
            }
            strncat(candidateWords, " ", 1);
            strncat(candidateWords, i->cmdWord, CMD_WORD_MAX);
        }
        printf("\n%s\n", candidateWords);
        return CMD_TAB_FAILED;
    }

    lastWord = cmd[wordNum - 1];
    
    /*
      * 补齐自身，先找到父的子item列表，看看能够匹配多少子item
      */
    lst = getBrotherList(cmd, wordNum);    
    if (NULL == lst)
    {
        getCmdAllFirstItem(candidateWords, sizeof(candidateWords) - 1);
        printf("\n%s\n", candidateWords);
        return CMD_TAB_FAILED;
    }
    
    LIST_FOREACH(i, lst, node)
    {
        if (i->cmdWord == strstr(i->cmdWord, lastWord))
        {
            n++;
            last = i;
            /* for safe */
            if (CMD_WORD_NUM_MAX * CMD_WORD_MAX - strlen(candidateWords)
                < CMD_WORD_MAX + 1)
            {
                break;
            }
            strncat(candidateWords, " ", 1);
            strncat(candidateWords, i->cmdWord, CMD_WORD_MAX);
        }
    }

    /*
      * 只有唯一的一个匹配上，那么就用这个代替用户输入
      */
    if (1 == n)
    {
        assert(NULL != last);
        wordlen = strlen(lastWord);
        for (; wordlen > 0; wordlen--)
        {
            printf("\b \b");
        }
        strncpy(lastWord, last->cmdWord, CMD_WORD_MAX);
        printf("%s", lastWord);
        return 0;
    }

    /*
      * 走到这里说明有多个候选，则将所有候选都显示给用户
      */
    printf("\n%s\n", candidateWords);

    return CMD_TAB_FAILED;
}


void mapToCmdFunc(char cmd[][CMD_WORD_MAX], int wordNum) 
{
    CMD_ITEM_S *item = NULL;

    if ('\0' == cmd[0][0])
    {
        return;
    }

    
    item = matchCmd(cmd, wordNum);
    if (NULL != item)
    {
        if (NULL != item->pfFunc)
        {
            item->pfFunc(cmd, wordNum);
            
            /*
                * 存入历史记录中，用户可以用上下键翻阅
                */
            insertCmdHistory(cmd, wordNum);
            dumpHistory();
        }
        else
        {
            /*
                * 未到达最后一有个item就不会处理函数指针，需要提示用户补全参数
                */
            completeWord(cmd, wordNum);
        }
    }
    else
    {
        printf("command not found.\n");
    }
}


int getOneWord(char word[CMD_WORD_MAX])
{
    int ch;
    int i = strlen(word);
    
    while ((ch = getchar()) != EOF)
    {
        if (i >= CMD_WORD_MAX - 1)
        {
            printf("too many characters input.\n");
            for (; i >= 0; i--) 
            { 
                printf("\b \b");
                word[i] = 0;
            }
            
            i = 0;
        }

        if (ch == '\t')
        {
            return CMD_TAB;
        }

        if (ch == '\r' || ch == '\n')
        {
            putchar(ch);
            return CMD_DONE;
        }

        if (ch == EOF)
        {
            return CMD_EOF;
        }
    
        if (ch == ' ')
        {
            putchar(ch);
            while ((ch = getchar()) != EOF)
            {               
                if (ch == ' ')
                {
                    //putchar(ch);
                    continue;
                }
                ungetc(ch, stdin);
                break;
            }
            
            return 0;
        }

        if (ch == '\b') 
        {
            if (i > 0)
            {
                printf("\b \b");
                word[--i] = '\0';
            }
            else /* 退到上一个word */
            {
                printf("\b \b");
                return CMD_BACK;
            }
            continue;
        }

        /*
           * 控制字符27
           */
#ifndef WIN32
#define KB_CTRL 27
#define KB_DIRECTION 91
#define KB_UP 65
#define KB_DOWN 66
#define KB_LEFT 68
#define KB_RIGHT 67
        if (KB_CTRL == ch)
        {
            /*
                * 方向键
                */
            if (KB_DIRECTION == getchar())
            {
                switch (getchar())
                {
                    case KB_UP:
                        return CMD_UP;

                    case KB_DOWN:
                        return CMD_DOWN;
                        
                    case KB_LEFT:
                    case KB_RIGHT:
                    default:
                    {
                        continue;
                    }
                }
            }
        }
#endif
        word[i++] = ch;
        putchar(ch);
    }

    return 0;
}

void processCli(const char *prompt)
{
    int i = 0;
    char cmd[CMD_WORD_NUM_MAX][CMD_WORD_MAX]; 
    int ret = 0;
    char (*pcmd)[CMD_WORD_MAX];
    int wordNum;

    printf("%s", prompt);
    for (;;)
    {
        memset(cmd, 0, sizeof(cmd));
        i = 0;
        for (;;)
        {
            ret = getOneWord(cmd[i]);            
            if (CMD_DONE == ret)
            {   
                i++;
                mapToCmdFunc(cmd, i);
                debugCmd("CMD_DONE", cmd, i);
                break;
            }
            else if (CMD_EOF == ret)
            {
                return;
            }
            else if (CMD_TAB == ret)
            {
                int j;
                int r;
                r = completeWord(cmd, i + 1);
                if (CMD_TAB_FAILED == r)
                {
                    printf("%s", prompt);
                    for (j = 0; j < i + 1; j++)
                    {
                        printf("%s", cmd[j]);
                        if (j != i)
                        {
                            printf(" ");
                        }
                    }
                    debugCmd("CMD_TAB_FAILED", cmd, i);
                    continue;
                } 
                
                continue;
            }            
            else if (CMD_BACK == ret)
            {
                if (i > 0) i--;
                debugCmd("CMD_BACK",cmd, i);
                continue;
            }
            else if (CMD_UP == ret)
            {
                int j;
                wordNum = getPrevCmd(&pcmd);
                COPY_CMD(cmd, pcmd, wordNum);
                i = wordNum - 1;
                printf("\n%s", prompt);
                for (j = 0; j < wordNum; j++)
                {
                    printf("%s", cmd[j]);
                    if (j != wordNum - 1)
                    {
                        printf(" ");
                    }
                }
                debugCmd("CMD_UP",cmd, i);
                dumpHistory();
                continue;
            }
            else if (CMD_DOWN == ret)
            {
                int j;
                wordNum = getNextCmd(&pcmd);
                COPY_CMD(cmd, pcmd, wordNum);
                i = wordNum - 1;  
                printf("\n%s", prompt);                
                for (j = 0; j < wordNum; j++)
                {
                    printf("%s", cmd[j]);
                    if (j != wordNum - 1)
                    {
                        printf(" ");
                    }
                }
                debugCmd("CMD_DOWN",cmd, i);
                dumpHistory();
                continue;
            }
            else
            {
                debugCmd("???",cmd, i);
                i++;
            }
        }
        
        printf("%s", prompt);
    }
    
}

int initCli()
{
    if (setInputMode(STDIN_FILENO, 0)) 
    {
        return -1;
    }
    return 0;
}

void DeinitCli()
{
    setInputMode(STDIN_FILENO, 1);
}




