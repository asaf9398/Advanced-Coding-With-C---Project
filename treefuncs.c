#include "treefuncs.h"
#include <string.h>
#include <stdbool.h>
//builds binary search tree from given file
InstrumentTree BuildTreeFromFile(char* instrumentsFile, int* size)
{
    InstrumentTree tr;
    tr.root = NULL;
    TreeNode* res, * father;

    int id = 0;
    unsigned long tempLen;
    char temp[MAX_STR_LEN + 1];

    FILE* fp = fopen(instrumentsFile, "r");
    if (fp == NULL)
    {
        printf("Error , file opening failed!\n");
        exit(1);
    }
    while (!feof(fp))
    {

        fgets(temp, MAX_STR_LEN + 1, fp);
        tempLen = strlen(temp);
        if (temp[tempLen - 1] == '\n')
            temp[tempLen - 1] = '\0';
        res = createNewTreeNode(NULL, NULL, temp, id);
        id++;
        if (tr.root == NULL)
            tr.root = res;
        else
        {
            father = findFather(tr.root, temp);
            if (strcmp(father->instrument, res->instrument) > 0)
                father->left = res;
            else
                father->right = res;
        }
    }
    *size = id;
    fclose(fp);
    return tr;
}
//finds the father of given node in the tree
TreeNode* findFather(TreeNode* root, char* instrument)
{
    TreeNode* res;
    res = root;
    bool found = false;

    while (!found)
    {
        if ((strcmp(res->instrument, instrument) > 0))
        {
            if (res->left == NULL)
                found = true;
            else
                res = res->left;
        }
        else
        {
            if (res->right == NULL)
                found = true;
            else
                res = res->right;
        }
    }
    return res;
}
// creates new treeNode
TreeNode* createNewTreeNode(TreeNode* left, TreeNode* right, char* data, int id)
{
    TreeNode* res;
    res = (TreeNode*)malloc(sizeof(TreeNode));
    checkMemoryAllocation(res);
    res->instrument = (char*)malloc((strlen(data) + 1) * sizeof(char));
    checkMemoryAllocation(res->instrument);
    strcpy(res->instrument, data);
    res->insId = id;
    res->left = left;
    res->right = right;
    return res;
}


//calls recursive func that searches for id of given instrument in the tree
int findInsId(InstrumentTree tree, char* instrument)
{
    return findInsIdHelper(tree.root, instrument);
}
//searches for id of given instrument in the tree, if found it returns its id else NULL
int findInsIdHelper(TreeNode* root, char* instrument)
{
    if (root == NULL)
        return -1;
    else
    {
        if (strcmp(root->instrument, instrument) == 0)
            return root->insId;
        else if (strcmp(root->instrument, instrument) < 0)
            return findInsIdHelper(root->right, instrument);
        else
            return findInsIdHelper(root->left, instrument);
    }
}
//calls recursive func that searches for name of given insid in the tree
char* findInstName(int instId, InstrumentTree tr)
{
    return findInstNameHelper(instId, tr.root);
}
//searches for name of given id in the tree, if found it returns its name else NULL
char* findInstNameHelper(int instId, TreeNode* root)
{
    char* res;

    if (root == NULL)
        return NULL;
    else
    {
        if (root->insId == instId)
            return root->instrument;
        else
        {
            res = findInstNameHelper(instId, root->left);
            if (res != NULL)
                return res;
            else
                return findInstNameHelper(instId, root->right);
        }
    }
}
//free tree
void freeTree(InstrumentTree tr)
{
    freeTreeHelper(tr.root);
    tr.root = NULL;
}
//free tree
void freeTreeHelper(TreeNode* root)
{
    if (root == NULL)
        return;
    else
    {
        freeTreeHelper(root->left);
        freeTreeHelper(root->right);
        free(root->instrument);
        free(root);
    }
}
//checks memory allocation
void checkMemoryAllocation(void* ptr)
{
    if (ptr == NULL)
    {
        printf("memory allocation failed!!");
        exit(1);
    }
}
