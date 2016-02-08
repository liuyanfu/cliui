
#include "mem_inc.h"
#include "stdlib.h"

#ifdef __cplusplus
#extern "C" {
#endif

MEM_DBG_TREE_S g_stMemTree;

static int MemElemCmp(void *pstLhs, void *pstRhs);

void MEM_DBG_Init()
{
	g_stMemTree.pstTreeRoot = NULL;
	g_stMemTree.uiTreeNodeCount = 0;
	g_stMemTree.pCmpFn = (int (*)(void*,void*))&MemElemCmp;
}

MEM_DBG_ELEM_S * MEM_DBG_CreateElem
(
    MEM_DBG_TYPE_E enType,
    void *pAddr, 
    unsigned int uiSize, 
    const char *pcszFileName, 
    unsigned int uiLine
)
{
    MEM_DBG_ELEM_S *pstElem = NULL;

    pstElem = (MEM_DBG_ELEM_S *)malloc(sizeof(MEM_DBG_ELEM_S));
    MEM_DBG_ASSERT(pstElem);

    pstElem->enType = enType;
    pstElem->pAddr = pAddr;
    pstElem->uiSize = uiSize;
    pstElem->pcszFileName = pcszFileName;
    pstElem->uiLine = uiLine;
    time(&pstElem->uiTime);

    return pstElem;
}

void MEM_DBG_DeleteElem(MEM_DBG_ELEM_S *pstElem)
{
    if (pstElem)
    {
    	free(pstElem);
    }
}

void MEM_DBG_Insert(MEM_DBG_ELEM_S *pstElem)
{
    MEM_DBG_ASSERT(
        NULL == RBT_Search(pstElem->pAddr, g_stMemTree.pstTreeRoot, g_stMemTree.pCmpFn));

    g_stMemTree.pstTreeRoot = RBT_Insert(pstElem->pAddr, 
    	   pstElem, 
    	   g_stMemTree.pstTreeRoot,
    	   g_stMemTree.pCmpFn);

    g_stMemTree.uiTreeNodeCount++;
}

MEM_DBG_ELEM_S *MEM_DBG_Find(void *pAddr)
{
    return RBT_Search(pAddr, g_stMemTree.pstTreeRoot, g_stMemTree.pCmpFn)->data;
}

MEM_DBG_ELEM_S *MEM_DBG_Remove(void *pAddr)
{
    RBT_NODE_S* pstNode = NULL;
    MEM_DBG_ELEM_S *pstRet = NULL;

    pstNode = RBT_Search(pAddr, g_stMemTree.pstTreeRoot, g_stMemTree.pCmpFn);
    MEM_DBG_ASSERT(pstNode);
    pstRet = pstNode->data;
    g_stMemTree.pstTreeRoot = RBT_Erase(
    	pAddr, 
    	g_stMemTree.pstTreeRoot, 
    	g_stMemTree.pCmpFn);

    g_stMemTree.uiTreeNodeCount--;
    return pstRet;
}

/*
static void MEM_DBG_DoEachDetail(RBT_NODE_S *pstNode, void (*pFn)(MEM_DBG_ELEM_S *) )
{
    if (NULL == pstNode)
    	return;

    if (pstNode->pstLeft)
    	MEM_DBG_DoEachDetail(pstNode->pstLeft, pFn);

    pFn(pstNode->data);

    if (pstNode->pstRight)
    	MEM_DBG_DoEachDetail(pstNode->pstRight, pFn);
}
*/

void MEM_DBG_Foreach(void (*pFn)(MEM_DBG_ELEM_S *) )
{
    //MEM_DBG_DoEachDetail(g_stMemTree.pstTreeRoot, pFn);
    RBT_WALK(g_stMemTree.pstTreeRoot, (void (*)(void*))pFn);
}

static int MemElemCmp(void *pstLhs, void *pstRhs)
{
    if (pstLhs > pstRhs)
    {
    	return 1;
    }
    else if (pstLhs < pstRhs)
    {
    	return -1;
    }
    else
    {
    	return 0;
    }
}
#ifdef __cplusplus
}
#endif


