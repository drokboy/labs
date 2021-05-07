#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct data {
    float x;
    float y;
    char *name;
} data;

typedef struct item {
    data *info;
    int key2;
    char* key1;
    struct keyspace1* key1_back;
} item;

typedef struct keyspace2 {
    int key;
    item* item;
    struct keyspace2 *next;
} keyspace2;

typedef struct keyspace1 {
    char *key;
    int release;
    item *item;
    struct keyspace1 *next;
} keyspace1;

typedef struct table {
    keyspace1 *ks1;
    keyspace2 **ks2;
    int size_ks2;
} table;


int getInt(int *a)
{
int n;
   do{
      n = scanf("%d", a);
      if (n < 0)
         return 0;
      if (n == 0){
         printf("%s\n", "Error! Repeat input");
         scanf("%*c");
      }
   } while (n == 0);
   return 1;
}

int getFloat(float *a)
{
int n;
   do{
      n = scanf("%f", a);
      if (n < 0)
         return 0;
      if (n == 0){
         printf("%s\n", "Error! Repeat input");
         scanf("%*c");
      }
   } while (n == 0);
   return 1;
}

char *get_str() {
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n = 0;
    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
            if (!res) {
                return NULL;
            }
        } else if (n > 0) {
            int chunk_len = strlen(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        } else {
            scanf("%*c");
        }
    } while (n > 0);

    if (len > 0) {
        res[len] = '\0';
    } else {
        res = calloc(1, sizeof(char));
    }

    return res;
}


keyspace1** key1_search(char* key1, table tb){
    int i = 0;
    keyspace1 *ptr1;
    keyspace1** res = NULL;
    if(!(tb.ks1)){
        return NULL;
    }
    ptr1 = tb.ks1;
    while(ptr1){
        if( !(strcmp(ptr1->key, key1)) ) {
            while( ptr1 && !(strcmp(ptr1->key, key1)) ){
                res = realloc(res, (i+1)*sizeof(keyspace1*));
                *(res+i) = ptr1;
                ptr1 = ptr1->next;
                i++;
            }
            res = realloc(res, (i+1)*sizeof(keyspace1*));
            *(res+i) = NULL;
            return res;
        }
        ptr1 = ptr1->next;
    }
    return NULL;
}

int key1_insert(keyspace1* key1, table* tb){
    keyspace1 *ptr1;
    keyspace1** n = key1_search(key1->key, *tb);
    if(n){
        int i = 0;
        while(n[i]){
            i++;
        }
        key1->next = n[i-1]->next;
        n[i-1]->next = key1;
        key1->release = i+1;
        free(n);
        return 1;
    }
    key1->release = 1;
    key1->next = tb->ks1;
    tb->ks1 = key1;
}

void keyspace1_delete_rel(char* key1, table* tb, int release){
    keyspace1 *ptr1 = tb->ks1;
    if(!ptr1){return;}
    keyspace1 *ptr_prev1 = NULL;
    if( !(strcmp(ptr1->key, key1)) && ptr1->release == release ){
        if(ptr1->item->info){
            free(ptr1->item->info->name);
            free(ptr1->item->info);
            ptr1->item->info = NULL;

        }
        else{
            free(ptr1->item->key1);
            free(ptr1->item);
        }
        tb->ks1 = ptr1->next;
        free(ptr1);
        return;
    }
    ptr_prev1 = ptr1;
    ptr1 = ptr1->next;
    while(ptr1){
        if( !(strcmp(ptr1->key, key1)) ){
            while( ptr1 && !(strcmp(ptr1->key, key1)) && ptr1->release != release ){
                ptr_prev1 = ptr1;
                ptr1 = ptr1->next;
            }
            if(!ptr1){return;}
            if(ptr1->item->info){
                free(ptr1->item->info->name);
                free(ptr1->item->info);
                ptr1->item->info = NULL;

            }
            else{
                free(ptr1->item->key1);
                free(ptr1->item);
            }
            ptr_prev1->next = ptr1->next;
            free(ptr1);
            return;
        }
        ptr_prev1 = ptr1;
        ptr1 = ptr1->next;
    }
    return;
}

int hash(int a, table tb){
    return a%tb.size_ks2;
}


item* key2_search(int key2, table tb){
    keyspace2 *ptr = *(tb.ks2 + hash(key2, tb));
    item* it = NULL;
    while(ptr){
        if(ptr->key == key2){
            it = ptr->item;
            return it;
        }
        ptr = ptr->next;
    }
    return it;
}


int key2_insert(keyspace2* key2, table* tb){
    item* n = key2_search(key2->key, *tb);
    if(n){return 0;}
    keyspace2 *ptr = *(tb->ks2 + hash(key2->key, *tb));
    if(ptr){
        *(tb->ks2 + hash(key2->key, *tb)) = key2;
        key2->next = ptr;
    }
    else{
        *(tb->ks2 + hash(key2->key, *tb)) = key2;
        key2->next = NULL;
    }
    return 1;
}

void key2_delete(int key2, table* tb){
    keyspace2 *ptr = *(tb->ks2 + hash(key2, *tb));
    keyspace2 *ptr_prev;
    if(ptr->key == key2){
        if(ptr->item->info){
            free(ptr->item->info->name);
            free(ptr->item->info);
            ptr->item->info = NULL;

        }
        else{
            free(ptr->item->key1);
            free(ptr->item);
        }
        *(tb->ks2 + hash(key2, *tb)) = ptr->next;
        free(ptr);
        return;
    }
    ptr_prev = ptr;
    ptr = ptr->next;
    while(ptr){
        if(ptr->key == key2){
            if(ptr->item->info){
                free(ptr->item->info->name);
                free(ptr->item->info);
                ptr->item->info = NULL;

            }
            else{
                free(ptr->item->key1);
                free(ptr->item);
            }
            ptr_prev->next = ptr->next;
            free(ptr);
            return;
        }
        ptr_prev = ptr;
        ptr = ptr->next;
    }
    return;
}

void table_delete(table* tb){
    keyspace2** ks2 = tb->ks2;
    keyspace2* ptr2;
    keyspace2* ptr_prev2;
    int i = 0, key2;
    for(i = 0;i < tb->size_ks2; i++){
        ptr2 = *(ks2+i);
        ptr_prev2 = ptr2;
        while(ptr2){
            ptr2 = ptr2->next;
            key2 = ptr_prev2->key;
            keyspace1_delete_rel(ptr_prev2->item->key1, tb, ptr_prev2->item->key1_back->release);
            key2_delete(key2, tb);
            ptr_prev2 = ptr2;
        }
        i++;
    }
    free(tb->ks2);
    free(tb);
}


int D_add(table* tb){
    int n, m;
    keyspace1* key1 = NULL;
    keyspace2* key2 = NULL;
    do{
        if(key1){
            free(key1->item->info->name);
            free(key1->item->info);
            free(key1->key);
            free(key1);
            free(key2);
        }
        key1 = calloc(1, sizeof(keyspace1));
        key2 = calloc(1, sizeof(keyspace2));
        key1->item = calloc(1, sizeof(item));
        key1->item->info = calloc(1, sizeof(data));
        printf("Please enter the first key\n");
        key1->key = get_str();
        if(!(key1->key)){
            free(key1->item->info);
            free(key1->item);
            free(key1);
            free(key2);
            return -1;
        }
        printf("Please enter the second key\n");
        n = getInt(&(key2->key));
        if(!n){
            free(key1->key);
            free(key1->item->info);
            free(key1->item);
            free(key1);
            free(key2);
            return -1;
        }
        printf("Please enter name\n");
        scanf("%*c");
        key1->item->info->name = get_str();
        if(!(key1->item->info->name)){
            free(key1->key);
            free(key1->item->info);
            free(key1->item);
            free(key1);
            free(key2);
            return -1;
        }
        printf("Please enter x\n");
        n = getFloat(&(key1->item->info->x));
        if(!n){
            free(key1->key);
            free(key1->item->info->name);
            free(key1->item->info);
            free(key1->item);
            free(key1);
            free(key2);
            return -1;
        }
        printf("Please enter y\n");
        n = getFloat(&(key1->item->info->y));
        if(!n){
            free(key1->key);
            free(key1->item->info->name);
            free(key1->item->info);
            free(key1->item);
            free(key1);
            free(key2);
            return -1;
        }
        key1->item->key1 = key1->key;
        key1->item->key1_back = key1;
        key1->item->key2 = key2->key;printf("O\n");
        key1_insert(key1, tb);
        m = key2_insert(key2, tb);
        if (m == 0){printf("key2 is not free\n");scanf("%*c");}
        key2->item = key1->item;
    }while(m != 1);
    return n;
}

int D_search_key2(table* tb){
    item* it;
    int key2;
    printf("Please enter the second key\n");
    int n = getInt(&key2);
    if(!n){
        return -1;
    }
    it = key2_search(key2, *tb);
    if (!it){
        printf("2nd key doesn't exist\n");
        return 0;
    }
    printf("name = %s\nx = %f\ny = %f\n", it->info->name, it->info->x, it->info->y);
    return 1;
}

int D_delete_key1_all(table* tb){
    int n, i = 0, k = 0;
    keyspace1* ks1;
    keyspace1** space1;
    printf("Please enter the first key\n");
    char* key1 = get_str();
    if(!key1){
        return -1;
    }
    space1 = key1_search(key1, *tb);
    if (!space1){
        free(key1);
        printf("1st key doesn't exist\n");
        return 0;
    }
    keyspace1* ptr1 = tb->ks1;
    keyspace1* ptr_prev1 = ptr1;
    while(ptr1 != *space1){
        ptr_prev1 = ptr1;
        ptr1 = ptr1->next;
        k = 1;
    }
    while(*(space1+i)){
        int key2 = (*(space1+i))->item->key2;
        ks1 = (*(space1+i))->next;
        keyspace1_delete_rel(key1, tb, i+1);//
        key2_delete(key2, tb);
        i++;
    }
    if(k == 0){
        tb->ks1 = ks1;
    }
    else{
        ptr_prev1->next = ks1;
    }
    free(space1);
    free(key1);
    return 1;
}

int D_delete_key1_one(table* tb){
    int n, i = 0;
    keyspace1** space1;
    printf("Please enter the first key\n");
    char* key1 = get_str();
    if(!key1){
        return 0;
    }
    space1 = key1_search(key1, *tb);
    if (!space1){
        free(key1);
        printf("1st key doesn't exist\n");
        return 0;
    }
    printf("Please enter release\n");
    n = getInt(&i);
    if(n == 0){
        free(space1);
        free(key1);
        return 0;
    }
    keyspace1* ptr1 = tb->ks1;
    keyspace1* ptr_prev1 = ptr1;
    while(ptr1 != *space1){
        ptr_prev1 = ptr1;
        ptr1 = ptr1->next;
    }
    if(tb->ks1 == *(space1+i-1)){
            tb->ks1 = (*(space1+i-1))->next;
    }
    else{
        ptr_prev1->next = (*(space1+i-1))->next;
    }
    int key2 = (*(space1+i-1))->item->key2;//printf("P\n");
    keyspace1_delete_rel(key1, tb, i);
    key2_delete(key2, tb);

    free(key1);
    free(space1);
    return 1;
}

int D_search_key1_all(table* tb){
    int i = 0;
    char* key1;
    printf("Please enter key1\n");
    key1 = get_str();
    if(!key1){return 0;}
    keyspace1** space1 = key1_search(key1, *tb);
    while(*(space1+i)){
        printf("name = %s\nx = %f\ny = %f\nkey1 = %s, key2 = %d\n", space1[i]->item->info->name, space1[i]->item->info->x, space1[i]->item->info->y,
               space1[i]->key, space1[i]->item->key2);
        i++;
    }
    free(space1);
    free(key1);
    return 1;
}

int D_search(table* tb){
    int i, key2;
    char* key1;
    printf("Please enter key2\n");
    i = getInt(&key2);
    if(!i){return 0;}
    printf("Please enter key1\n");
    scanf("%*c");
    key1 = get_str();
    if(!key1){return 0;}
    keyspace1** space1 = key1_search(key1, *tb);
    if(space1) {
        i = 0;
        while (space1[i]) {
            if (space1[i]->item->key2 == key2) {
                printf("name = %s\nx = %f\ny = %f\nkey1 = %s, key2 = %d\n", space1[i]->item->info->name,
                       space1[i]->item->info->x, space1[i]->item->info->y,
                       space1[i]->key, space1[i]->item->key2);
                free(space1);
                free(key1);
                return 1;
            }
            i++;
        }
        free(space1);
    }
    free(key1);
    printf("Such element does not exist\n");
    return 1;
}

int D_delete(table* tb){
    int i, key2;
    char* key1;
    printf("Please enter key2\n");
    i = getInt(&key2);
    if(!i){return 0;}
    printf("Please enter key1\n");
    scanf("%*c");
    key1 = get_str();
    if(!key1){return 0;}
    keyspace1** space1 = key1_search(key1, *tb);
    if(space1) {
        i = 0;
        while (space1[i]) {
            if (space1[i]->item->key2 == key2) {
                keyspace1_delete_rel(key1, tb, space1[i]->release);
                key2_delete(key2, tb);
                free(space1);
                free(key1);
                return 1;
            }
            i++;
        }
        free(space1);
    }
    free(key1);
    printf("Such element does not exist\n");
    return 1;
}


int D_output(table* tb){
    keyspace1* ptr = tb->ks1;
    while(ptr){//printf("Y\n");
        printf("name = %s\nx = %f\ny = %f\nkey1 = %s, key2 = %d\n", ptr->item->info->name, ptr->item->info->x, ptr->item->info->y,
            ptr->key, ptr->item->key2);
        ptr = ptr->next;
    }
    return 1;
}

const char *MSGS[] = {"0. Exit", "1. New element input", "2. Search with all keys", "3. Remove an element with all keys", "4. Search all releases of 1st key",
"5. Search release of 1st key", "6. Search with 2nd key", "7. Delete all elements of first key", "8. Delete release of 1st key", "9. Output"};
const int MSGS_SIZE = sizeof(MSGS) / sizeof(MSGS[0]);


int dialog(const char *msgs[], int n) {
    char *error = "";
    int choice;
    do {
	puts(error);
	error = "You're wrong. Try again!";
	for (int i = 0; i < n; ++i) {
	    puts(msgs[i]);
	}
	puts("Make your choice: ");
	choice = getchar() - '0';
	while (getchar() != '\n') {}
    } while (choice < 0 || choice >= n);
    return choice;
}

table* table_new(){
    table* tb = calloc(sizeof(table), 1);
    printf("Please enter size of key space 2\n");
    scanf("%d", &(tb->size_ks2));
    scanf("%*c");
    tb->ks2 = calloc(sizeof(keyspace2*), tb->size_ks2);
    for(int i = 0; i < tb->size_ks2; i++){
        *(tb->ks2 + i) = NULL;
    }
    tb->ks1 = NULL;
    return tb;
}

int main() {
    int c = 0, n;
    table* tb = table_new();
    do {
	c = dialog(MSGS, MSGS_SIZE);
	switch(c) {
	case 0:
	    break;
	case 1:
	    printf("***\nOption 1\n***\n");
	    n = D_add(tb);
	    if(n == -1){
            return 0;
        }
	    printf("x = %f\n y = %f\n name = %s", tb->ks1->item->info->x, tb->ks1->item->info->y, tb->ks1->item->info->name);
	    scanf("%*c");
	    break;
	case 2:
	    printf("***\nOption 2\n***\n");
	    D_search(tb);
	    //scanf("%*c");
	    break;
    case 3:
	    printf("***\nOption 3\n***\n");
	    D_delete(tb);
	    break;
    case 4:
	    printf("***\nOption 4\n***\n");
	    D_search_key1_all(tb);
	    break;
    case 6:
	    printf("***\nOption 5\n***\n");
	    D_search_key2(tb);
	    scanf("%*c");
	    break;
    case 7:
	    printf("***\nOption 6\n***\n");
        D_delete_key1_all(tb);
	    break;
    case 8:
	    printf("***\nOption 6\n***\n");
        D_delete_key1_one(tb);
        scanf("%*c");
	    break;
    case 9:
	    printf("***\nOption 8\n***\n");
	    D_output(tb);
	    break;
    }
    } while (c != 0);
    table_delete(tb);
    return 0;
}

