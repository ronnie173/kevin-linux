/* study json_render_tree
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <strings.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "json.h"
#include "json_helper.h"
#include "jsonUtil.h"

/*
int main (void) {
    nameValuePair_t paramList[MAX_PAIR_ARRAY_LEN];
    
    loadParamList("./param_list.json", paramList, MAX_PAIR_ARRAY_LEN);
    dumpParamList(paramList, MAX_PAIR_ARRAY_LEN);

    return 0;
}*/

int loadParamList(const char *fn, nameValuePair_t list[], int alen) {
    json_t *root = NULL;
    int ret = loadJSonFile(fn, &root);
    if (ret) {
        printf("failed to load param list\n");
        return 1;
    }

    char *buf = NULL;
    //char *buf = malloc(1000);
    //bzero(buf, 1000);
    printf("Printing the document tree...\n");
    json_tree_to_string(root, &buf);
    printf("%s\n", buf);
    free(buf);

    printf("root type is %d and text is [%s]\n", root->type, root->text);
    json_render_tree(root);

    json_t *node = root;
    char tmpStr[100];
    bzero(tmpStr, 100);
    buf = malloc(1000);
    memset(buf, 0, 1000);

    dumpJSonTree(node, buf, tmpStr);
    printf("TREE:\n%s\n", buf);

    free(buf);
    
    /* try to parse param_list.json */
    /* check ParamList tag */
    node = node->child;
    if (JSON_STRING != node->type || strcmp("ParamList", node->text)) {
        printf("the paramlist file is wrong\n");
        printf("root type is %d and text is [%s]\n", node->type, node->text);
    } else {
        /* try to get name/value pairs */
        json_t *valNode = NULL;
        int count = 0;
        for (node = node->child->child; 
                NULL != node && count < alen; 
                node = node->next) {
            /* get name */
            printf("node type is %d and text is [%s]\n", node->type, node->text);
            strncpy(list[count].name, node->text, MAX_NAME_LEN); 
            /* get name */
            valNode = node->child;
            printf("node type is %d and text is [%s]\n", valNode->type, valNode->text);
            strncpy(list[count].value, valNode->text, MAX_VALUE_LEN); 
            count++;
        }
    }

    json_free_value(&root);

    return 0;
}

int loadJSonFile(const char* fn, json_t **root) {
    int err;

    printf("%s\n", "====== get into loadJSonFile =======\n");

    setlocale (LC_ALL, "");
    //char *document = "{\"entry\":{\"name\":\"Andew\",\"phone\":\"555 123 456\"}}";

    int fd = open(fn, O_RDONLY);
    if (-1 == fd) {
        printf("open file failed\n");
        return 1;
    }

    struct stat fs;
    if (fstat(fd, &fs)) {
        printf("failed to get file stat\n");
        close(fd);
        return 1;
    }

    size_t fileLen = (size_t)fs.st_size;
    char *document = malloc(fileLen);
    bzero(document, fileLen);

    size_t count = read(fd, document, fileLen);
    if (count != fileLen) {
        printf("failed to read file\n");
        free(document);
        close(fd);

        return 1;
    }

    printf("Parsing the document...\n");
    err = json_parse_document(root, document);
    //printf("root type is %d and test is %s\n", (*root)->type, (*root)->text);
    free(document);
    close(fd);
    //printf("root type is %d and test is %s\n", (*root)->type, (*root)->text);

    if (err != JSON_OK) {
        printf("%s, err=%d\n", "parse error", err);
        json_free_value(root);
        return 1;
    }

    return 0;
}

int dumpJSonTree(json_t *root, char *buf, char *tmpStr) {
    json_t *node = root;
    bzero(tmpStr, 100);

    if (NULL == node) return 0;
    sprintf(tmpStr, "node type is %d and text is [%s]\n", node->type, node->text);
    strncat(buf, tmpStr, 100);

    if (NULL != node->child) {
        for (node = node->child; NULL != node; node = node->next) {
            dumpJSonTree(node, buf, tmpStr);
        }
    }

    return 0;
}

int dumpParamList(nameValuePair_t list[], int len) {
    int i;
    for (i = 0; i < len; i++) {
        printf("name is [%s] and value is [%s]\n", list[i].name, list[i].value);
    }
    
    return 0;
}