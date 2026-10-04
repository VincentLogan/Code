#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    bool is_leaf;
    int left;
    int center;
    int right;
    int num;
    struct node *left_node;
    struct node *center_node;
    struct node *right_node;
    int center_key;
    int right_key;
} node;
typedef node *ptr_node;

typedef struct list_node {
    ptr_node node;
    struct list_node *next;
} list_node;
typedef list_node *ptr_list_node;

ptr_node initialize_node();
ptr_node insert(ptr_node root, int key);
int get_key(ptr_node root);
bool search(ptr_node root, int key);
void print_node(ptr_node);
void lever_order_traversal(ptr_list_node head);

int main() {
    int number_of_tree;
    int key;
    scanf("%d", &number_of_tree);
    ptr_node root = initialize_node();
    for (int i = 0; i < number_of_tree; i++) {
        scanf("%d", &key);
        if (!search(root, key)) {
            ptr_node temp = insert(root, key);
            if (temp != NULL) {
                ptr_node new_root = initialize_node();
                new_root->is_leaf = false;
                new_root->left_node = root;
                new_root->center_node = temp;
                new_root->center_key = get_key(new_root->center_node);
                root = new_root;
            }
        }
        else {
            printf("Key %d is duplicated\n", key);
        }
    }
    ptr_list_node new_list = (ptr_list_node)malloc(sizeof(list_node));
    new_list->node = root;
    new_list->next = NULL;
    lever_order_traversal(new_list);
    return 0;
}

ptr_node initialize_node() {
    ptr_node new_node = (ptr_node)malloc(sizeof(node));
    new_node->is_leaf = true;
    new_node->num = 0;
    new_node->center = -1;
    new_node->left = -1;
    new_node->right = -1;
    new_node->left_node = NULL;
    new_node->center_node = NULL;
    new_node->right_node = NULL;
    new_node->right_key = -1;
    new_node->center_key = -1;
    return new_node;
}

ptr_node insert(ptr_node root, int key) {
    if (root->is_leaf && root->num < 3) {
        root->num++;
        if (key < root->left || root->left == -1) {
            root->right = root->center;
            root->center = root->left;
            root->left = key;
        }
        else {
            if (root->center == -1) {
                root->center = key;
            }
            else if (key < root->center) {
                root->right = root->center;
                root->center = key;
            }
            else {
                root->right = key;
            }
        }
        return NULL;
    }
    else if (root->is_leaf && root->num == 3) {
        ptr_node new_leaf = initialize_node();
        root->num = 2;
        new_leaf->num = 2;
        if (key < root->left) {
            new_leaf->left = root->center;
            new_leaf->center = root->right;
            root->right = -1;
            root->center = root->left;
            root->left = key;
        }
        else if (key < root->center) {
            new_leaf->left = root->center;
            new_leaf->center = root->right;
            root->right = -1;
            root->center = key;
        }
        else if (key < root->right) {
            new_leaf->left = key;
            new_leaf->center = root->right;
            root->right = -1;
        }
        else {
            new_leaf->left = root->right;
            new_leaf->center = key;
            root->right = -1;
        }
        return new_leaf;
    }
    else {
        ptr_node new_node = NULL;
        if (key < root->center_key) {
            new_node = insert(root->left_node, key);
        }
        else if (key < root->right_key || root->right_node == NULL) {
            new_node = insert(root->center_node, key);
        }
        else {
            new_node = insert(root->right_node, key);
        }
        if (new_node != NULL) {
            if (root->right_node == NULL) {
                if (key < root->center_key) {
                    root->right_node = root->center_node;
                    root->center_node = new_node;
                }
                else {
                    root->right_node = new_node;
                }
                root->center_key = get_key(root->center_node);
                root->right_key = get_key(root->right_node);
                return NULL;
            }
            else {
                ptr_node new_nonleaf = initialize_node();
                new_nonleaf->is_leaf = false;
                if (key < root->center_key) {
                    new_nonleaf->left_node = root->center_node;
                    new_nonleaf->center_node = root->right_node;
                    new_nonleaf->center_key = root->right_key;
                    root->right_node = NULL;
                    root->right_key = -1;
                    root->center_node = new_node;
                    root->center_key = get_key(root->center_node);
                }
                else if (key < root->right_key) {
                    new_nonleaf->left_node = new_node;
                    new_nonleaf->center_node = root->right_node;
                    new_nonleaf->center_key = get_key(new_nonleaf->center_node);
                    root->right_node = NULL;
                    root->right_key = -1;
                    root->center_key = get_key(root->center_node);
                }
                else {
                    new_nonleaf->left_node = root->right_node;
                    new_nonleaf->center_node = new_node;
                    new_nonleaf->center_key = get_key(new_nonleaf->center_node);
                    root->right_node = NULL;
                    root->right_key = -1;
                }
                return new_nonleaf;
            }
        }
        return NULL;
    }
}

int get_key(ptr_node root) {
    if (!root->is_leaf) {
        int key = get_key(root->left_node);
        return key;
    }
    return root->left;
}

bool search(ptr_node root, int key) {
    while (!root->is_leaf) {
        if (key < root->center_key) {
            root = root->left_node;
        }
        else if (key < root->right_key || root->right_node == NULL) {
            root = root->center_node;
        }
        else {
            root = root->right_node;
        }
    }
    if (key == root->left || key == root->center || key == root->right) {
        return true;
    }
    else {
        return false;
    }
}

void print_node(ptr_node root) {
    if (!root->is_leaf) {
        printf("[%d", root->center_key);
        if (root->right_key != -1) {
            printf(",%d", root->right_key);
        }
        printf("]");
    }
    else {
        printf("[%d", root->left);
        if (root->center != -1) {
            printf(",%d", root->center);
        }
        if (root->right != -1) {
            printf(",%d", root->right);
        }
        printf("]");
    }
}

void lever_order_traversal(ptr_list_node head) {
    if (!head->node->is_leaf) {
        ptr_list_node list_head = NULL;
        ptr_list_node list_tail = NULL;
        ptr_list_node temp = head;
        while (head != NULL) {
            temp = head;
            print_node(head->node);

            ptr_list_node new_list_node = (ptr_list_node)malloc(sizeof(list_node));
            new_list_node->node = head->node->left_node;
            new_list_node->next = NULL;
            if (list_tail == NULL) {
                list_head = new_list_node;
                list_tail = new_list_node;
            }
            else {
                list_tail->next = new_list_node;
                list_tail = new_list_node;
            }

            ptr_list_node new_list_node2 = (ptr_list_node)malloc(sizeof(list_node));
            new_list_node2->node = head->node->center_node;
            new_list_node2->next = NULL;
            list_tail->next = new_list_node2;
            list_tail = new_list_node2;

            if (head->node->right_node != NULL) {
                ptr_list_node new_list_node3 = (ptr_list_node)malloc(sizeof(list_node));
                new_list_node3->node = head->node->right_node;
                new_list_node3->next = NULL;
                list_tail->next = new_list_node3;
                list_tail = new_list_node3;
            }

            head = head->next;
            free(temp);
        }
        printf("\n");
        lever_order_traversal(list_head);
    }
    else {
        while (head != NULL) {
            ptr_list_node temp = head;
            print_node(head->node);
            head = head->next;
            free(temp);
        }
        printf("\n");
    }
}
