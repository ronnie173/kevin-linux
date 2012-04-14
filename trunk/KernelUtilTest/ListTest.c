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

void cleanList(struct list_head *list) {
    struct msg *tmpMsg;
    struct list_head *pos, *npos;
    list_for_each_safe(pos, npos, list) {
        tmpMsg = list_entry(pos, struct msg, list);
        list_del(pos);
        free(tmpMsg);
    }

    return;
}

int main(int argc, char* argv[]) {
    int i;
    struct msg myMsg, *tmpMsg;

    LIST_HEAD(myList);
    INIT_LIST_HEAD(&(myMsg.list));
    snprintf(myMsg.message, 20, "First");

    for (i = 0; i < 10; i++) {
        tmpMsg = (struct msg *)malloc(sizeof(struct msg));
        snprintf(tmpMsg->message, 20, "Kevin %d", i);
        //INIT_LIST_HEAD(&(tmpMsg->list));
        //list_add_tail(&(tmpMsg->list), &myList);
        INIT_LIST_HEAD(&(tmpMsg->list));
        list_add_tail(&(tmpMsg->list), &(myMsg.list));
    }

    printf("=== print the myList ===\n");
    //printList(&myList);

    printf("=== print the myMsg List ===\n");
    printList(&(myMsg.list));


    printf("=== print the list reverse ===\n");
    //printListReverse(&myList);

    //cleanList(&(myList));
    cleanList(&(myMsg.list));

    if (list_empty(&myList)) {
        printf("the myList is empty\n");
    }

    if (list_empty(&(myMsg.list))) {
        printf("the myMsg.list is empty\n");
    }


    return 0;
}
