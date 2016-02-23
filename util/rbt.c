
#include <stdlib.h>
#include "rbt.h"

#ifdef __cplusplus
#extern "C" {
#endif

RBT_NODE_S* RBT_NewNode(key_type key, data_t data)
{
    RBT_NODE_S *pstNode = (RBT_NODE_S*)RBT_ALLOC(sizeof(RBT_NODE_S));

    if (NULL == pstNode)
    {
        return NULL;
    }
    
    pstNode->key = key;
    pstNode->data = data;

    return pstNode;
}

/*-----------------------------------------------------------
|   node           right
|   / \    ==>     / \
|   a  right     node  y
|       / \           / \
|       b  y         a   b
 -----------------------------------------------------------*/
RBT_NODE_S* RBT_RotateLeft(RBT_NODE_S* pstNode, RBT_NODE_S* pstRoot)
{
    RBT_NODE_S* pstRight = pstNode->pstRight;

    if ((pstNode->pstRight = pstRight->pstLeft))
    {
        pstRight->pstLeft->pstParent = pstNode;
    }
    pstRight->pstLeft = pstNode;

    if ((pstRight->pstParent = pstNode->pstParent))
    {
        if (pstNode == pstNode->pstParent->pstRight)
        {
            pstNode->pstParent->pstRight = pstRight;
        }
        else
        {
            pstNode->pstParent->pstLeft = pstRight;
        }
    }
    else
    {
        pstRoot = pstRight;
    }
    pstNode->pstParent = pstRight;

    return pstRoot;
}

/*-----------------------------------------------------------
|       node           left
|       / \            / \
|    left  y   ==>    a   node
|   / \               / \
|  a   b             b   y
-----------------------------------------------------------*/
RBT_NODE_S* RBT_RotateRight(RBT_NODE_S* pstNode, RBT_NODE_S* pstRoot)
{
    RBT_NODE_S* pstLeft = pstNode->pstLeft;

    if ((pstNode->pstLeft = pstLeft->pstRight))
    {
        pstLeft->pstRight->pstParent = pstNode;
    }
    pstLeft->pstRight = pstNode;

    if ((pstLeft->pstParent = pstNode->pstParent))
    {
        if (pstNode == pstNode->pstParent->pstRight)
        {
            pstNode->pstParent->pstRight = pstLeft;
        }
        else
        {
            pstNode->pstParent->pstLeft = pstLeft;
        }
    }
    else
    {
        pstRoot = pstLeft;
    }
    pstNode->pstParent = pstLeft;

    return pstRoot;
}

RBT_NODE_S* RBT_InsertRebalance(RBT_NODE_S *pstNode, RBT_NODE_S *pstRoot)
{
    RBT_NODE_S *pstParent = NULL; 
    RBT_NODE_S *pstGParent = NULL;
    RBT_NODE_S *pstUncleNode = NULL;
    RBT_NODE_S* pstTmp = NULL;

    while ((pstParent = pstNode->pstParent) && pstParent->enColor == RED)
    {
        pstGParent = pstParent->pstParent;

        if (pstParent == pstGParent->pstLeft)
        {
            pstUncleNode = pstGParent->pstRight;
            if (pstUncleNode && pstUncleNode->enColor == RED)
            {
                pstUncleNode->enColor = BLACK;
                pstParent->enColor = BLACK;
                pstGParent->enColor = RED;
                pstNode = pstGParent;
            }
            else
            {
                if (pstParent->pstRight == pstNode)
                {
                    pstRoot = RBT_RotateLeft(pstParent, pstRoot);
                    pstTmp = pstParent;
                    pstParent = pstNode;
                    pstNode = pstTmp;
                }

                pstParent->enColor = BLACK;
                pstGParent->enColor = RED;
                pstRoot = RBT_RotateRight(pstGParent, pstRoot);
            }
        } 
        else 
        {
            pstUncleNode = pstGParent->pstLeft;
            if (pstUncleNode && pstUncleNode->enColor == RED)
            {
                pstUncleNode->enColor = BLACK;
                pstParent->enColor = BLACK;
                pstGParent->enColor = RED;
                pstNode = pstGParent;
            }
            else
            {
                if (pstParent->pstLeft == pstNode)
                {
                    pstRoot = RBT_RotateRight(pstParent, pstRoot);
                    pstTmp = pstParent;
                    pstParent = pstNode;
                    pstNode = pstTmp;
                }

                pstParent->enColor = BLACK;
                pstGParent->enColor = RED;
                pstRoot = RBT_RotateLeft(pstGParent, pstRoot);
            }
        }
    }

    pstRoot->enColor = BLACK;

    return pstRoot;
}

RBT_NODE_S* RBT_EraseRebalance(RBT_NODE_S *pstNode, RBT_NODE_S *pstParent, RBT_NODE_S *pstRoot)
{
    RBT_NODE_S *pstOther = NULL;
    RBT_NODE_S *pstOldLeft = NULL;
    RBT_NODE_S *pstOldRight = NULL;

    while ((!pstNode || pstNode->enColor == BLACK) && pstNode != pstRoot)
    {
        if (pstParent->pstLeft == pstNode)
        {
            pstOther = pstParent->pstRight;
            if (pstOther->enColor == RED)
            {
                pstOther->enColor = BLACK;
                pstParent->enColor = RED;
                pstRoot = RBT_RotateLeft(pstParent, pstRoot);
                pstOther = pstParent->pstRight;
            }
            if ((!pstOther->pstLeft || pstOther->pstLeft->enColor == BLACK) &&
                (!pstOther->pstRight || pstOther->pstRight->enColor == BLACK))
            {
                pstOther->enColor = RED;
                pstNode = pstParent;
                pstParent = pstNode->pstParent;
            }
            else
            {
                if (!pstOther->pstRight || pstOther->pstRight->enColor == BLACK)
                {
                    if ((pstOldLeft = pstOther->pstLeft))
                    {
                        pstOldLeft->enColor = BLACK;
                    }
                    pstOther->enColor = RED;
                    pstRoot = RBT_RotateRight(pstOther, pstRoot);
                    pstOther = pstParent->pstRight;
                }
                pstOther->enColor = pstParent->enColor;
                pstParent->enColor = BLACK;
                if (pstOther->pstRight)
                {
                    pstOther->pstRight->enColor = BLACK;
                }
                pstRoot = RBT_RotateLeft(pstParent, pstRoot);
                pstNode = pstRoot;
                break;
            }
        }
        else
        {
            pstOther = pstParent->pstLeft;
            if (pstOther->enColor == RED)
            {
                pstOther->enColor = BLACK;
                pstParent->enColor = RED;
                pstRoot = RBT_RotateRight(pstParent, pstRoot);
                pstOther = pstParent->pstLeft;
            }
            if ((!pstOther->pstLeft || pstOther->pstLeft->enColor == BLACK) &&
                (!pstOther->pstRight || pstOther->pstRight->enColor == BLACK))
            {
                pstOther->enColor = RED;
                pstNode = pstParent;
                pstParent = pstNode->pstParent;
            }
            else
            {
                if (!pstOther->pstLeft || pstOther->pstLeft->enColor == BLACK)
                {
                    if ((pstOldRight = pstOther->pstRight))
                    {
                        pstOldRight->enColor = BLACK;
                    }
                    pstOther->enColor = RED;
                    pstRoot = RBT_RotateLeft(pstOther, pstRoot);
                    pstOther = pstParent->pstLeft;
                }
                pstOther->enColor = pstParent->enColor;
                pstParent->enColor = BLACK;
                if (pstOther->pstLeft)
                {
                    pstOther->pstLeft->enColor = BLACK;
                }
                pstRoot = RBT_RotateRight(pstParent, pstRoot);
                pstNode = pstRoot;
                break;
            }
        }
    }

    if (pstNode)
    {
        pstNode->enColor = BLACK;
    } 

    return pstRoot;
}

RBT_NODE_S* RBT_SearchAux(key_type key, RBT_NODE_S* pstRoot, RBT_NODE_S** save, pfnDataCmp pfn)
{
    RBT_NODE_S *pstNode = pstRoot, *pstParent = NULL;
    int iRet = 0;

    while (pstNode)
    {
        pstParent = pstNode;
        iRet = pfn(pstNode->key, key);
        if (0 < iRet)
        {
            pstNode = pstNode->pstLeft;
        }
        else if (0 > iRet)
        {
            pstNode = pstNode->pstRight;
        }
        else
        {
            return pstNode;
        }
    }

    if (save)
    {
        *save = pstParent;
    }

    return NULL;
}

RBT_NODE_S* RBT_Insert(key_type key, data_t data, RBT_NODE_S* pstRoot, pfnDataCmp pfn)
{
    RBT_NODE_S *pstParent = NULL;
    RBT_NODE_S *pstNode = NULL;

    pstParent = NULL;
    if ((pstNode = RBT_SearchAux(key, pstRoot, &pstParent, pfn)))
    {
        return pstRoot;
    }

    pstNode = RBT_NewNode(key, data);
    pstNode->pstParent = pstParent; 
    pstNode->pstLeft = pstNode->pstRight = NULL;
    pstNode->enColor = RED;

    if (pstParent)
    {
        if (pfn(pstParent->key, key) > 0) //pstParent->key > key)
        {
            pstParent->pstLeft = pstNode;
        }
        else
        {
            pstParent->pstRight = pstNode;
        }
    }
    else
    {
        pstRoot = pstNode;
    }

    return RBT_InsertRebalance(pstNode, pstRoot);
}

RBT_NODE_S* RBT_Search(key_type key, RBT_NODE_S* pstRoot, pfnDataCmp pfn)
{
    return RBT_SearchAux(key, pstRoot, NULL, pfn);
}

RBT_NODE_S* RBT_Erase(key_type key, RBT_NODE_S *pstRoot, pfnDataCmp pfn)
{
    RBT_NODE_S *pstChild = NULL;
    RBT_NODE_S *pstParent = NULL;
    RBT_NODE_S *pstOld = NULL; 
    RBT_NODE_S *pstLeft = NULL;
    RBT_NODE_S *pstNode = NULL;
    COLOR_E enColor = RED;

    if (!(pstNode = RBT_SearchAux(key, pstRoot, NULL, pfn)))
    {
        return pstRoot;
    }

    pstOld = pstNode;

    if (pstNode->pstLeft && pstNode->pstRight)
    {
        pstNode = pstNode->pstRight;
        while ((pstLeft = pstNode->pstLeft) != NULL)
        {
            pstNode = pstLeft;
        }
        pstChild = pstNode->pstRight;
        pstParent = pstNode->pstParent;
        enColor = pstNode->enColor;

        if (pstChild)
        {
            pstChild->pstParent = pstParent;
        }
        if (pstParent)
        {
            if (pstParent->pstLeft == pstNode)
            {
                pstParent->pstLeft = pstChild;
            }
            else
            {
                pstParent->pstRight = pstChild;
            }
        }
        else
        {
            pstRoot = pstChild;
        }

        if (pstNode->pstParent == pstOld)
        {
            pstParent = pstNode;
        }

        pstNode->pstParent = pstOld->pstParent;
        pstNode->enColor = pstOld->enColor;
        pstNode->pstRight = pstOld->pstRight;
        pstNode->pstLeft = pstOld->pstLeft;

        if (pstOld->pstParent)
        {
            if (pstOld->pstParent->pstLeft == pstOld)
            {
                pstOld->pstParent->pstLeft = pstNode;
            }
            else
            {
                pstOld->pstParent->pstRight = pstNode;
            }
        } 
        else
        {
            pstRoot = pstNode;
        }

        pstOld->pstLeft->pstParent = pstNode;
        if (pstOld->pstRight)
        {
            pstOld->pstRight->pstParent = pstNode;
        }
    }
    else
    {
        if (!pstNode->pstLeft)
        {
            pstChild = pstNode->pstRight;
        }
        else if (!pstNode->pstRight)
        {
            pstChild = pstNode->pstLeft;
        }
        pstParent = pstNode->pstParent;
        enColor = pstNode->enColor;

        if (pstChild)
        {
            pstChild->pstParent = pstParent;
        }
        if (pstParent)
        {
            if (pstParent->pstLeft == pstNode)
            {
                pstParent->pstLeft = pstChild;
            }
            else
            {
                pstParent->pstRight = pstChild;
            }
        }
        else
        {
            pstRoot = pstChild;
        }
    }

    RBT_FREE(pstOld);

    if (enColor == BLACK)
    {
        pstRoot = RBT_EraseRebalance(pstChild, pstParent, pstRoot);
    }

    return pstRoot;
}

void RBT_WALK(RBT_NODE_S * pstRoot, void (*fn)(void *data))
{
	RBT_NODE_S *pstNode , *prev = 0;
	// get first
	for (pstNode = pstRoot; pstNode; pstNode = pstNode->pstLeft)
		prev = pstNode;
	if (!prev)
		return;

	pstNode = prev;
        fn(pstNode->data);    

	while(pstNode)
	{
		// get next
		if (pstNode->pstRight)
		{
			for (pstNode = pstNode->pstRight; pstNode->pstLeft; pstNode = pstNode->pstLeft)
				;
		}
		else
		{
			for (;;)
			{
				prev = pstNode;
				pstNode = pstNode->pstParent;
				if (NULL == pstNode)
				{
					return;
				}
				if (pstNode->pstLeft == prev)
					break;
			}
		}
		fn(pstNode->data);
	}
}

#ifdef __cplusplus
}
#endif

