#include <stdio.h>
#include <stdlib.h>

#define CUR 0
#define DAD 1
#define BRO 2

typedef struct Node Node;

struct Node
{
    int id;
    Node *son;
    Node *bro;
    int value;
};

void ShowHelp() {
    printf("\n");
    printf("-----------------\n");
    printf("List of commands:\n");
    printf("a arg1 arg2: add node, arg1 - dad's id, arg2 - value\n");
    printf("c arg1 arg2: change value, arg1 - id, arg2 - new value\n");
    printf("d arg1: delete node, arg1 - id\n");
    printf("f: power of tree\n");
    printf("p: print tree\n");
    printf("q: quit\n");
    printf("\n");
    printf("NOTE:\n");
    printf("node №1:2 means id = 1, value = 2\n");
    printf("-----------------\n");
    printf("\n");
}

void RecursiveNodeSearch(int id, Node* root, Node** res) {
    if ((root->son != NULL) && (root->son->id == id)) {
        res[CUR] = root->son;
        res[DAD] = root;
    } else if ((root->bro != NULL) && (root->bro->id == id)) {
        res[CUR] = root->bro;
        res[BRO] = root;
    } else {
        if (root->son != NULL) RecursiveNodeSearch(id, root->son, res);
        if (root->bro != NULL) RecursiveNodeSearch(id, root->bro, res);
    }
}

void NodeSearch(int id, Node* root, Node** res) {
    res[CUR] = NULL;
    res[DAD] = NULL;
    res[BRO] = NULL;
    if (root->id == id) {
        res[0] = root;
    } else {
        RecursiveNodeSearch(id, root, res);
    }
}

void AddNode(int dad_id, int value, int *idCounter, Node* root) {
    Node *res[3];
    NodeSearch(dad_id, root, res);
    if (res[CUR] != NULL) {
        Node *dad = res[CUR];
        Node *newNode = (Node*)malloc(sizeof(Node));
        newNode->value = value;
        newNode->id = *idCounter;
        *idCounter = *idCounter + 1;
        newNode->bro = NULL;
        newNode->son = NULL;
        if (dad != NULL) {
            if (dad->son == NULL) {
                dad->son = newNode;
            } else {
                Node *oldBro = dad->son;
                while (oldBro->bro != NULL) {
                    oldBro = oldBro->bro;
                }
                oldBro->bro = newNode;
            }
        }
    }
}

void ChangeValue(int id, int value, Node *root) {
    Node *res[3];
    NodeSearch(id, root, res);
    res[CUR]->value = value;
}

void DeleteNodeR(Node *node) {
    if (node->son != NULL) {
        DeleteNodeR(node->son);
    }
    if (node->bro != NULL) {
        DeleteNodeR(node->bro);
    }
    free(node);
}

void DeleteNode(int id, Node *root) {
    Node *res[3];
    NodeSearch(id, root, res);
    if (res[DAD] != NULL) {
        res[DAD]->son = res[CUR]->bro;
    } else if (res[BRO] != NULL) {
        res[BRO]->bro = res[CUR]->bro;
    }
    if (res[CUR]->son != NULL) {
        DeleteNodeR(res[CUR]->son);
    }
    free(res[CUR]);
}

void PrintTree(Node *root, int depth) {
    for(int i = 0; i < depth; i++) {
        printf("   ");
    }
    printf("№%d:%d\n", root->id, root->value);
    if (root->son != NULL) {
        PrintTree(root->son, depth + 1);
    }
    if (root->bro != NULL) {
        PrintTree(root->bro, depth);
    }
}

void PrintTreeDec(Node *root) {
    printf("------------\n");
    PrintTree(root, 0);
    printf("------------\n");
}

int Power(Node* root) {
    if (root == NULL) {
        return 0;
    }
    Node* son = root->son;
    int maxPower = 0;
    int curPower = 0;
    while (son != NULL) {
        curPower++;
        son = son->bro;
    }
    if (curPower > maxPower) {
        maxPower = curPower;
    }
    son = root->son;
    while (son != NULL) {
        int sonCountM = Power(son);
        if (sonCountM > maxPower) {
            maxPower = sonCountM;
        }
        son = son->bro;
    }
    
    return maxPower;
}

int main() {
    int idCounter = 1;
    Node *root =  (Node*)malloc(sizeof(Node));
    root->bro = NULL;
    root->son = NULL;
    root->id = 0;
    root->value = 0;
    char command;
    int arg1, arg2;
    PrintTreeDec(root);
    printf("Enter 'h' to show help note\n");
    do {
        scanf("%c", &command);
        if (command == 'h') {
            ShowHelp();
        } else if (command == 'a') {
            scanf("%d %d", &arg1, &arg2);
            AddNode(arg1, arg2, &idCounter, root);
            PrintTreeDec(root);
        } else if (command == 'c') {
            scanf("%d %d", &arg1, &arg2);
            ChangeValue(arg1, arg2, root);
            PrintTreeDec(root);
        } else if (command == 'd') {
            scanf("%d", &arg1);
            DeleteNode(arg1, root);
            PrintTreeDec(root);
        } else if (command == 'p') {
            PrintTreeDec(root);
        } else if (command == 'f') {
            int power = Power(root);
            printf("------------\n");
            printf("Power of tree: %d\n", power);
            printf("------------\n");
        }
    } while (command != 'q');
    return 0;
}