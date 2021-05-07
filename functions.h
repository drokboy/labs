#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Info {
	int n;
	int k;
	char *str;
	struct Info *next;
} Info;

typedef struct Node {
	int size;
	char *key[3];
	Info *info[3];
	struct Node *first;
	struct Node *second;
	struct Node *third;
	struct Node *fourth;
	struct Node *parent;
} Node;

typedef struct Tree {
	Node *root;
} Tree;


int getInt(int *a);
int dialog(const char *msgs[], int n);
void swap_keys(char **a, char **b);
void swap_info(Info *a, Info *b);
void sort_2(char **a, char **b, Info *c, Info *d);
void sort_3(char **a, char **b, char **c, Info *d, Info *e, Info *f);
void sort_keys(Node *node);
int is_leaf(Node *node);
Node *new_node(char *k, Info *info);
void insert_to_node(Node *node, char *k, Info *info);
void become_node(Node *node, char *k, Info *info, Node *a, Node *b);
Node *split(Node *item);
Node *insert(Node *ptr, char *k, Info *info);
void printTree(Node *node);
void delete_tree(Node *node);
int D_add(Tree *tree);
int find(Node *ptr, char *k);
int D_find(Tree *tree);
Info *search(Node* ptr, char *k);
Node *search_min(Node *ptr);
#endif
