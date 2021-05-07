#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

void swap_keys(char** a, char ** b) {
	char *c = *a;
	*a = *b;
	*b = c;
}

void swap_info(Info *a, Info *b) {
	Info c = *a;
	c.next = a->next;
	*a = *b;
	a->next = b->next;
	*b = c;
}

void sort_2(char **a, char **b, Info *c, Info *d) {
	if (*a > *b) {
		swap_keys(a, b);
		swap_info(c, d);
	}
}

void sort_3(char **a, char **b, char **c, Info *d, Info *e, Info *f){
	sort_2(a, b, d, e);
	sort_2(a, c, d, f);
	sort_2(b, c, e, f);
}

void sort_keys(Node *node) {
	if (!node) return;
	int size = node->size;
	if (size == 1) return;
	if (size == 2) sort_2(&(node->key[0]), &(node->key[1]), node->info[0], node->info[1]);
	if (size == 3) sort_3(&(node->key[0]), &(node->key[1]), &(node->key[2]), 
			        node->info[0], node->info[1], node->info[2]);
}

void insert_to_node(Node *node, char *k, Info *info) {
	node->key[node->size] = k;
	node->info[node->size] = info;
	node->size++;
	sort_keys(node);
}

/*void remove_from_node(Node *node, char *k, Info *info, Node *s) {
	node->size = s;
	node->key[node->size] = k;
	node->info[node->size] = info;
	if (s >= 1 && k[0]
*/

int is_leaf(Node *node) {
	if (!node) return -1;
	if (node->first || node->second || node->third) return 0;
	else return 1;
}

int find(Node *ptr, char *k) { // Проверка на нахождение ключа в вершине
	for (int i = 0; i < ptr->size; ++i) 
		if (ptr->key[i] == k) return 1;
	return -1;
}


void become_node(Node *node, char *k, Info *info, Node *a, Node *b) {
	node->key[0] = k;
	node->info[0] = info;
	node->first = a;
	node->second = b;
	node->third = NULL;
	node->fourth = NULL;
	node->parent = NULL;
	node->size = 1;
}

Node *new_node(char *k, Info *info) {
	Node *ptr = (Node*) calloc (1, sizeof(Node));
	ptr->first = NULL;
	ptr->second = NULL;
	ptr->third = NULL;
	ptr->fourth = NULL;
	ptr->parent = NULL;
	ptr->key[0] = k;
	info->next = NULL;
	ptr->info[0] = info;
	ptr->size = 1;
	return ptr;
}

int getInt(int *a) {
	int n;
	do {
		n = scanf("%d", a);
		if (n < 0) return 0;
		if (n == 0) {
			printf("%s\n", "Error! Repeat input");
			scanf("%*c");
		}
	} while (n == 0);
	return 1;
}

int dialog(const char* msgs[], int N) {
	char* errmsg = "";
	int rc;
	int i, n;
	do {
		puts(errmsg);
		errmsg = "You are wrong. Repeat, please!";
		for (i = 0; i < N; ++i) {
			puts(msgs[i]);
		}

		puts("Make your choise: --> ");
		n = getInt(&rc);
		if (n == 0) {
			rc = 0;
		}

	} while (rc < 0 || rc >= N);
	return rc;
}

char *getstr() {
    char buf[81] = {0};
    char *res = NULL;
    char *s = NULL;
    int len = 0;
    int n;
    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
            if (!res) {
                return NULL;
            }
        } else if (n > 0) {
            int chunk_len = strlen(buf);
            int str_len = len + chunk_len;
            s = res;
            res = (char *) realloc(res, sizeof(char) * (str_len + 1));
            if (!res) {
               free(s);
               return NULL;
            }
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        } else {
            scanf("%*c");
        }
    } while (n > 0);

    if (len > 0) {
        res[len] = '\0';
    } else {
        res = (char *) calloc(1, sizeof(char));
        if (!res) {
           return NULL;
        }
    }
    return res;
}


Node *split(Node *item) {
	if (item->size < 3) return item;
	Node *ptr1 = new_node(item->key[0], item->info[0]);
	ptr1->first = item->first;
	ptr1->second = item->second;
	ptr1->parent = item->parent;

	Node *ptr2 = new_node(item->key[2], item->info[2]);
	ptr2->first = item->first;
	ptr2->second = item->second;
	ptr2->parent = item->parent;

	if (ptr1->first) ptr1->first->parent = ptr1;
	if (ptr1->second) ptr1->second->parent = ptr1;

	if (ptr2->first) ptr2->first->parent = ptr2;
	if (ptr2->second) ptr2->second->parent = ptr2;

	if (item->parent) {
		insert_to_node(item->parent, item->key[1], item->info[1]);

		if (item->parent->first == item) item->parent->first = NULL;
		else if (item->parent->second == item) item->parent->second = NULL;
		else if (item->parent->third == item) item->parent->third = NULL;

		if (item->parent->first == NULL) {
			item->parent->fourth = item->parent->third;
			item->parent->third = item->parent->second;
			item->parent->second = ptr2;
			item->parent->first = ptr1;
		} else if (item->parent->second == NULL) {
			item->parent->fourth = item->parent->third;
			item->parent->third = ptr2;
			item->parent->third = ptr1;
		} else if (item->parent->third == NULL) {
			item->parent->fourth = ptr2;
			item->parent->third = ptr1;
		}

		Node *tmp = item->parent;
		free(item);
		return tmp;
	} else {
		ptr1->parent = item;
		ptr2->parent = item;
		become_node(item, item->key[1], item->info[1], ptr1, ptr2);
		return item;
	}
}

Node *insert(Node *ptr, char *k, Info *info) {
	if (!ptr) return new_node(k, info);
	info->next = NULL;
	Info *el;
	if (k == ptr->key[0]) {
		el = ptr->info[0];
		while (el && el->next) {
			el = el->next;
		}
		el->next = info;
	} else if (ptr->size == 2 && k == ptr->key[1]) {
		el = ptr->info[1];
		while (el && el->next) {
			el = el->next;
		}
		el->next = info;
	} else if (is_leaf(ptr) == 1) {
		insert_to_node(ptr, k, info);
	} else if (k < ptr->key[0]) {
		insert(ptr->first, k, info);
	} else if ((ptr->size == 1 && k > ptr->key[0] || (ptr->size == 2 && k > ptr->key[0] && k < ptr->key[1]))) {
		insert(ptr->second, k, info);
	} else if (ptr->size == 2 && k > ptr->key[1]) insert(ptr->third, k, info);
	return split(ptr);
}

/*Node *search(Node *ptr, char *k) {
    if (!ptr) return NULL;

    if (find(ptr, k)) return ptr;
    else if (k < ptr->key[0]) return search(ptr->first, k);
    else if ((ptr->size == 2) && (k < ptr->key[1]) || (ptr->size == 1)) return search(ptr->second, k);
    else if (ptr->size == 2) return search(ptr->third, k);
}
*/


Info *search(Node *ptr, char *k) {
	if (!ptr) return NULL;
	if (k < ptr->key[0]) return search(ptr->first, k);
	else if ((ptr->size == 2) && (k < ptr->key[1]) || (ptr->size == 1)) return search(ptr->second, k);
	else if (ptr->size == 2) return search(ptr->third, k);
}




	
int D_add(Tree *tree) {
	int init;
	char *key;
	int n;
	int k;
	char *str; 
	printf("Enter the key: \n");
	scanf("%*c");
	key = getstr();
	if (!key) return -1;
	printf("Enter the information: \n");
	init = getInt(&n);
	if (init == -1) return -1;
	init = getInt(&k);
	if (init == -1) return -1;
	str = getstr();
	if (!str) return -1;
	Info *info = (Info *) calloc(1, sizeof(Info));
	info->n = n;
	info->k = k;
	info->str = str;
	info->next = NULL;
	Node *node = insert(tree->root, key, info);
	tree->root = node;
	return 0;
}

int D_find(Tree *tree) {
	int init;
	char *key;
	Info *info;
	printf("Enter the key: \n");
	key = getstr();
	scanf("%*c");
	info = search(tree->root, key);
		while(info) {
			printf("Info: %d %d %s\n", key, info->n, info->k, info->str);
	               	info = info->next;
	 		scanf("%*c");		
		}
			
	return 0;
}


void printTree(Node *node) {
	Node *ptr = node;
	if (!ptr) return;
	printTree(ptr->first);
	Info *info;
	info = ptr->info[0];
	while (info) {
		printf("Key: %s\n",ptr->key[0]);
		printf("Info: %d, %d, %s\n", info->n, info->k, info->str);
		info = info->next;
	}

	printTree(ptr->second);
	if (ptr->size > 1) {
		info = ptr->info[1];
		while (info) {
			printf("Key: %s\n", ptr->key[1]);
			printf("Info: %d, %d, %s\n", info->n, info->k, info->str);
			info = info->next;
		}
		printTree(ptr->third);
	}
}

Node *search_min(Node *ptr) {
	if (!ptr) return ptr;
	if (!(ptr->first)) return ptr;
	else return search_min(ptr->first);



Node *fix(Node *leaf) {
	if (leaf->size == 0 && leaf->parent == NULL) {
		free(leaf);
		return NULL;
	}

	if (leaf->size != 0) {
		if (leaf->parent) return fix(leaf->parent);
		else return leaf;
	}

	Node *parent = leaf->parent;
	if (parent->first->size == 2 || parent->second->size == 2 || parent->size == 2) leaf = redistribite(leaf);
	else leaf = merge(leaf);
	return fix(leaf);
}


Node *redistribure(Node *leaf) {
	Node *parent = leaf->parent;
	Node *first = parent->first;
	Node *second = parent->third;
	Node *third = parent->third;

	if ((parent->size == 2) && (first->size < 2) && (second->size < 2) && (third->size < 2)) {
		if (first == leaf) {
			parent->first = parent->second;
			parent->second = parent->third;
			parent->third = NULL;
			//Node *ptr = insert_to_node(parent->key[0], );
			parent->first->third = parent->first->second;
			parent->first->second = parent->first->first;


void delete_tree(Node *node) {
	Node *ptr = node;
	if (!ptr) return;
	delete_tree(ptr->first);
	Info *info, *prev;
	info = ptr->info[0];
	while (info) {
		free(info->str);
		prev = info;
		info = info->next;
		free(prev);
	}
	delete_tree(ptr->second);
	if (ptr->size > 1) {
		info = ptr->info[1];
		while (info) {
			free(info->str);
			prev = info;
			info = info->next;
			free(prev);
		}
	}
	delete_tree(ptr->third);
	free(ptr);
	node = NULL;
}


