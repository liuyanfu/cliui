
#ifndef MEM_DBG_RBT_H
#define MEM_DBG_RBT_H

#ifdef __cplusplus
#extern "C" {
#endif

#define RBT_ALLOC malloc
#define RBT_FREE free

typedef void* key_t;
typedef void* data_t;

typedef int (*pfnDataCmp)(void*,void*);

typedef enum tagColor
{
    RED = 0,
    BLACK = 1
} COLOR_E;

typedef struct tagRbtNode
{
    struct tagRbtNode *pstLeft;
    struct tagRbtNode *pstRight;
    struct tagRbtNode *pstParent;
    key_t key;
    data_t data;
    COLOR_E enColor;
} RBT_NODE_S;

RBT_NODE_S* RBT_Insert(key_t key, data_t data, RBT_NODE_S* pstRoot, pfnDataCmp pfn);
RBT_NODE_S* RBT_Search(key_t key, RBT_NODE_S* pstRoot, pfnDataCmp pfn);
RBT_NODE_S* RBT_Erase(key_t key, RBT_NODE_S* pstRoot, pfnDataCmp pfn);
void RBT_WALK(RBT_NODE_S * pstRoot, void (*fn)(void *data));

#ifdef __cplusplus
}
#endif

#endif

