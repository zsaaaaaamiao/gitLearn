#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "uthash.h"

#define RET_MAX 300

 struct ListNode {
    int val;
    struct ListNode *next;
 };

//array -> ListNode 链表
struct ListNode* MakeList(int *arrayVal, int numsSize)
{
    struct ListNode *node = (struct ListNode*) malloc(sizeof(struct ListNode));
    struct ListNode *head = node;
    node->val = arrayVal[0];
    for(int i = 1; i < numsSize; ++i){
        node->next = (struct ListNode*) malloc(sizeof(struct ListNode));
        node->next->val = arrayVal[i];
        node = node->next;
    }
    node->next = NULL;
    return head;
}

//数组反转
void ArrayToggle(char *array)
{
    int arrLen = strlen(array);
    for(int i = 0; i < arrLen /2; i++){
        char cTmp = array[i];
        array[i] = array[arrLen -1 - i];
        array[arrLen -1 - i] = cTmp;
    }
}


struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

//HASH
typedef struct {
    struct TreeNode* node;
    int cnt; 
    UT_hash_handle hh;
}HASHNODE;



#define RED     0
#define GREEN   1
#define BLUE    2
#define INTNULL -10000
#define MAX_LEN 1000

HASHNODE *g_hashF, *g_hashG = NULL;

//uthash 使用
void HashaddTree(struct TreeNode* node)
{
    if (!node) {
        return;
    }
    HashaddTree(node->left);
    HashaddTree(node->right);
    HASHNODE *tmp = NULL;
    HASH_FIND_PTR(g_hashG,&node->left,tmp);         //HASH_FIND_PTR 第二个参数是&key
    int gleft = tmp == NULL ? 0 : tmp->cnt;
    
    HASH_FIND_PTR(g_hashG,&node->right,tmp);
    int gright = tmp == NULL ? 0 : tmp->cnt;

    HASH_FIND_PTR(g_hashF,&node->left,tmp);
    int fleft = tmp == NULL ? 0 : tmp->cnt;
    
    HASH_FIND_PTR(g_hashF,&node->right,tmp);
    int fright = tmp == NULL ? 0 : tmp->cnt;

    int f = node->val + gleft + gright;
    HASHNODE *fadd = (HASHNODE *)malloc(sizeof(HASHNODE));
    fadd->node = node;
    fadd->cnt = f;
    HASH_ADD_PTR(g_hashF,node,fadd);
    int aleft = fleft > gleft ? fleft : gleft;
    int aright = fright > gright ? fright : gright;
    int g = aleft + aright;
    HASHNODE *gadd = (HASHNODE *)malloc(sizeof(HASHNODE));
    gadd->node = node;
    gadd->cnt = g;
    HASH_ADD_PTR(g_hashG,node,gadd);

}

//array -> tree
struct TreeNode* ArrtoTree(int *arr, int len)
{
    struct TreeNode** mark = (struct TreeNode**)malloc(sizeof(struct TreeNode*) * len);
    *mark = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    struct TreeNode* ret = *mark;
    mark[0]->val = arr[0];
    mark[0]->left = NULL;
    mark[0]->right = NULL;
    for (int i = 1; i < len; i++) {
        if (arr[i] == INTNULL) { 
            continue;
        }
        mark[i] = (struct TreeNode*)malloc(sizeof(struct TreeNode));
        mark[i]->val = arr[i];
        mark[i]->left = NULL;
        mark[i]->right = NULL;
        int j = (i - 1) / 2; 
        if ( (i - 1) % 2 ==  0) {
            mark[j]->left = mark[i];
        }else {
            mark[j]->right = mark[i];
        }
    }
    free(mark);
    return ret;
}

void dfs(char (*board)[4], int row, int col, int i, int j, int (*add)[2], int *len, int *flag)
{
    if (i < 0 || i > row -1 || j < 0 || j > col - 1) {
        *flag = 1;
        return;
    }
    if (board[i][j] == 'X' || board[i][j] == 'Q') {
        return;
    }
    board[i][j] = 'Q';
    add[*len][0] = i;
    add[*len][0] = j;
    (*len)++;
    dfs(board, row, col, i - 1, j, add, len, flag);
    dfs(board, row, col, i + 1, j, add, len, flag);
    dfs(board, row, col, i, j - 1, add, len, flag);
    dfs(board, row, col, i, j + 1, add, len, flag);
}

void Replace(char (*board)[4], int len, int flag, int (*add)[2])
{                  
    if (flag == 0) {
        return;
    }
    for (int k = 0; k < len; k++) {
        board[add[k][0]][add[k][1]] = 'X';
    }
}


void solve(char (*board)[4], int boardSize, int boardColSize){
    int row = boardSize;
    int col = boardColSize;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            int flag = 0;
            int len = 0;
            int  (*add)[2] = (int (*)[2])calloc(row * col * 2, sizeof(int));
            dfs(board, row, col, i, j, add, &len, &flag);
            Replace(board,len,flag,add);
            free(add);
        }
    }
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (board[i][j] == 'Q') {
                board[i][j] = 'O';
            }
        }
    }
    return;
}

int main(void)
{
    int arr[] = {6,5,4,3,2,3,2};
    /*
    int arr[] = {13,9,20,INTNULL,INTNULL,15,7};
    struct TreeNode* root = ArrtoTree(arr, 7);xxxx
    */
    char board[4][4] = { {'X','X','X','X'}, {'X','O','O','X'}, {'X','X','O','X'}, {'X','O','X','X'}};
    solve(board,4,4);
    return 0;
}