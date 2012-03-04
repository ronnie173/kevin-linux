#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

struct msg {
    struct list_head list;
    char message[20];
};

void printList(struct list_head *theList) {
    struct msg *tmp;
    struct list_head *pos;
    list_for_each(pos, theList) {
        tmp = list_entry(pos, struct msg, list);
        printf("the msg is [%s]\n", tmp->message);
    }
}

void printListReverse(struct list_head *theList) {
    struct msg *tmp;
    list_for_each_entry_reverse(tmp, theList, list) {
        printf("the msg is [%s]\n", tmp->message);
    }
}

int main(int argc, char* argv[]) {
    int i;
    struct msg *myMsg;
    struct list_head *pos, *npos;

    LIST_HEAD(myList);

    for (i = 0; i < 10; i++) {
        myMsg = (struct msg *)malloc(sizeof(struct msg));
        snprintf(myMsg->message, 20, "Kevin %d", i);
        //INIT_LIST_HEAD(&(myMsg->list));
        list_add_tail(&(myMsg->list), &myList);
    }

    printf("=== print the list ===\n");
    printList(&myList);

    printf("=== print the list reverse ===\n");
    printListReverse(&myList);

    list_for_each_safe(pos, npos, &myList) {
        myMsg = list_entry(pos, struct msg, list);
        list_del(pos);
        free(myMsg);
    }

    if (list_empty(&myList)) {
        printf("the list is empty\n");
    }

    return 0;
}
