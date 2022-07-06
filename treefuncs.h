
#ifndef FINALPROJECT_TREEFUNCS_H
#define FINALPROJECT_TREEFUNCS_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_STR_LEN 150
typedef struct treeNode {
    char* instrument;
    unsigned short insId;
    struct treeNode* left;
    struct treeNode* right;
} TreeNode;

typedef struct tree {
    TreeNode* root;
}InstrumentTree;

InstrumentTree BuildTreeFromFile(char* instrumentsFile, int* size);
TreeNode* findFather(TreeNode* root, char* instrument);
TreeNode* createNewTreeNode(TreeNode* left, TreeNode* right, char* data, int id);
void printTreeInorder(InstrumentTree tr);
void printTreeInorderHelper(TreeNode* root);
void freeTree(InstrumentTree tr);
void freeTreeHelper(TreeNode* root);
void checkMemoryAllocation(void* ptr);
int findInsId(InstrumentTree tree, char* instrument);
int findInsIdHelper(TreeNode* root, char* instrument);
char* findInstName(int instId, InstrumentTree tr);
char* findInstNameHelper(int instId, TreeNode* root);
#endif //FINALPROJECT_TREEFUNCS_H
