#include <stdio.h>
#include <stdlib.h>

typedef struct avlnode {
    struct avlnode *father;
    struct avlnode *left;
    struct avlnode *right;
    int val;
    int highth_of_left;
    int highth_of_right;
    int hight;
    int BF;

} AVLnode;
typedef AVLnode *ptrAVLnode;

ptrAVLnode AVLroot = NULL;
ptrAVLnode creatnode(int val);
ptrAVLnode insert(ptrAVLnode root, int val);
void LLrotation(ptrAVLnode root);
void LRrotation(ptrAVLnode root);
void RRrotation(ptrAVLnode root);
void RLrotation(ptrAVLnode root);
void updateHight(ptrAVLnode root);

int main() {
    int num_of_node;
    int val_of_node;
    scanf("%d", &num_of_node);
    if (num_of_node == 0) {
        return 0;
    }
    scanf("%d", &val_of_node);
    AVLroot = creatnode(val_of_node);
    for (int i = 0; i < num_of_node - 1; i++) {
        scanf("%d", &val_of_node);
        insert(AVLroot, val_of_node);
    }
    printf("%d\n", AVLroot->val);
    return 0;
}

ptrAVLnode creatnode(int val) {
    ptrAVLnode newnode = (ptrAVLnode)malloc(sizeof(AVLnode));
    newnode->val = val;
    newnode->highth_of_left = 0;
    newnode->highth_of_right = 0;
    newnode->BF = 0;
    newnode->hight = 1;
    newnode->father = NULL;
    newnode->left = NULL;
    newnode->right = NULL;
    return newnode;
}

ptrAVLnode insert(ptrAVLnode root, int val) {
    ptrAVLnode newnode;
    if (val > root->val && root->right == NULL) {
        newnode = creatnode(val);
        newnode->father = root;
        root->right = newnode;
        updateHight(root);
        return newnode;
    }
    else if (val < root->val && root->left == NULL) {
        newnode = creatnode(val);
        newnode->father = root;
        root->left = newnode;
        updateHight(root);
        return newnode;
    }
    else if (val > root->val && root->right != NULL) {
        newnode = insert(root->right, val);
    }
    else if (val < root->val && root->left != NULL) {
        newnode = insert(root->left, val);
    }
    else {
        return NULL;
    }
    updateHight(root);
    if (root->BF < -1 || root->BF > 1) {
        if (root->BF == 2 && val > root->left->val) {
            LRrotation(root);
        }
        else if (root->BF == 2 && val < root->left->val) {
            LLrotation(root);
        }
        else if (root->BF == -2 && val > root->right->val) {
            RRrotation(root);
        }
        else {
            RLrotation(root);
        }
    }
    return newnode;
}

void updateHight(ptrAVLnode root) {
    if (root->left == NULL) {
        root->highth_of_left = 0;
    }
    else {
        root->highth_of_left = root->left->hight;
    }
    if (root->right == NULL) {
        root->highth_of_right = 0;
    }
    else {
        root->highth_of_right = root->right->hight;
    }
    root->BF = root->highth_of_left - root->highth_of_right;
    if (root->highth_of_left > root->highth_of_right) {
        root->hight = root->highth_of_left + 1;
    }
    else {
        root->hight = root->highth_of_right + 1;
    }
}

void LLrotation(ptrAVLnode root) {
    ptrAVLnode temp = root->left;

    if (temp->right != NULL) {
        root->left = temp->right;
        temp->right->father = root;
    }
    else {
        root->left = NULL;
    }

    if (root->father != NULL) {
        temp->father = root->father;
        if (root->father->val < temp->val) {
            root->father->right = temp;
        }
        else {
            root->father->left = temp;
        }
    }
    else {
        temp->father = NULL;
        AVLroot = temp;
    }

    temp->right = root;
    root->father = temp;
    updateHight(root);
    updateHight(temp);
}

void RRrotation(ptrAVLnode root) {
    ptrAVLnode temp = root->right;

    if (temp->left != NULL) {
        root->right = temp->left;
        temp->left->father = root;
    }
    else {
        root->right = NULL;
    }

    if (root->father != NULL) {
        temp->father = root->father;
        if (root->father->val < temp->val) {
            root->father->right = temp;
        }
        else {
            root->father->left = temp;
        }
    }
    else {
        temp->father = NULL;
        AVLroot = temp;
    }

    temp->left = root;
    root->father = temp;
    updateHight(root);
    updateHight(temp);
}

void LRrotation(ptrAVLnode root) {
    RRrotation(root->left);
    LLrotation(root);
}

void RLrotation(ptrAVLnode root) {
    LLrotation(root->right);
    RRrotation(root);
}
