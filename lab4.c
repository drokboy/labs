#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

const char *MSGS[] = {"0.Quit", "1. Add element", "2.Delete element", "3. Find element", "4.Show the tree", "5.Delete tree", "6. Timing"};
const int MSGS_SIZE = sizeof(MSGS) / sizeof(MSGS[0]);


int main() {
	int c;
	Tree tree;
	tree.root = NULL;
	do {
		c = dialog(MSGS, MSGS_SIZE);
		switch(c) {
			case 0:
				break;
			case 1:
				D_add(&tree);
				break;
			case 2:
				break;
			case 3:
				D_find(&tree);
				break;
			case 4:
				printTree(tree.root);
				break;
			case 5:
				break;
			case 6:
				break;
		}
	} while (c != 0);
	delete_tree(tree.root);
	return 0;
}
