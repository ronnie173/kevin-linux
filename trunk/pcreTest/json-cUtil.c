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

#include "json-cUtil.h"

/*
int main (void) {
    nameValuePair_t paramList[MAX_PAIR_ARRAY_LEN];

    loadParamList("./param_list.json", paramList, MAX_PAIR_ARRAY_LEN);
    dumpParamList(paramList, MAX_PAIR_ARRAY_LEN);

    return 0;
}*/

int loadParamList(char *fn, nameValuePair_t list[], int alen) {
    int ret = 0;
    json_object *root = json_object_from_file(fn);
    if (NULL != root) {
        /*const char *str = json_object_to_json_string(root);
        if (NULL != str) {
            printf("file is\n%s\n", str);
        } else {
            ret = 1;
            printf("failed to get json string\n");
        }
        dumpJsonObj(root);*/

        json_object *node = root;
        if (!json_object_is_type(node, json_type_object)) {
            printf("the paramlist file is wrong\n");
        } else {
            node = json_object_object_get(node, "ParamList");
            if (NULL == node) {
                printf("can NOT find ParamList\n");
            } else {
                /* try to get name/value pairs */
                char *key; struct json_object *val;
                struct lh_entry *entry = NULL;
                int count = 0;
                for (entry = json_object_get_object(node)->head;
                entry && count < alen; entry = entry->next) {
                    key = (char*)entry->k;
                    val = (struct json_object*)entry->v;
                    /* get name */
                    //printf("key is [%s]\n", key);
                    strncpy(list[count].name, key, MAX_NAME_LEN);
                    list[count].nameLen = strlen(list[count].name);
                    /* get value */
                    //printf("value is [%s]\n", json_object_to_json_string(val));
                    strncpy(list[count].value, json_object_get_string(val), MAX_VALUE_LEN);
                    list[count].valueLen = strlen(list[count].value);
                    count++;
                }
            }
        }
        json_object_put(root);
    } else {
        printf("failed to get json obj from file\n");
        ret = 1;
    }

    return ret;
}

int dumpJsonObj(struct json_object *root) {
    json_type type = json_object_get_type(root);

    if (type == json_type_null) {
        return 0;
    } else if (type == json_type_object) {
        char *key; struct json_object *val;
        struct lh_entry *entry = NULL;
        for (entry = json_object_get_object(root)->head; entry; entry = entry->next) {
            key = (char*)entry->k;
            val = (struct json_object*)entry->v;
            printf("Object=%s: \n", key);
            dumpJsonObj(val);
        }
    } else if (type == json_type_string) {
        printf("string=%s\n", json_object_to_json_string(root));
    } else {
        printf("unsupported type\n");
    }

    return 0;
}

int dumpParamList(nameValuePair_t list[], int len) {
    int i;
    for (i = 0; i < len; i++) {
        printf("name is [%s] (%d) and value is [%s] (%d)\n", list[i].name,
                list[i].nameLen, list[i].value, list[i].valueLen);
    }

    return 0;
}

int getFileLen(const char* fn) {
    int ret = -1;

    struct stat buf;

    if (!stat(fn, &buf)) {
        ret = (int)buf.st_size;
    }

    return ret;
}

int loadFile(const char* fn, char *buf, int fileLen) {
    int fd = open(fn, O_RDONLY);
    if (-1 == fd) {
        printf("open file failed\n");
        return 1;
    }


    size_t count = read(fd, buf, fileLen);
    if (count != fileLen) {
        printf("failed to read file\n");
        close(fd);

        return 1;
    }

    close(fd);

    return 0;
}
