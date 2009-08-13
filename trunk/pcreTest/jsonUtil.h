#ifndef KZ_JSON_UTIL_H
#define KZ_JSON_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "json.h"

#define MAX_NAME_LEN 100
#define MAX_VALUE_LEN 100
#define MAX_PAIR_ARRAY_LEN 30

typedef struct nameValuePair {
    char name[MAX_NAME_LEN];
    char value[MAX_VALUE_LEN];
} nameValuePair_t;

int loadJSonFile(const char *fn, json_t **root);
int loadParamList(const char *fn, nameValuePair_t list[], int alen);
int dumpJSonTree(json_t *root, char *buf, char *tmpStr);
int dumpParamList(nameValuePair_t list[], int len);

#ifdef __cplusplus
}
#endif
#endif
