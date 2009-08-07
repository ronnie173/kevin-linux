#ifndef KZ_JSON_UTIL_H
#define KZ_JSON_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NAME_LEN 100
#define MAX_VALUE_LEN 100
#define MAX_PAIR_ARRAY_LEN 3

typedef struct nameValuePair {
    char name[MAX_NAME_LEN];
    char value[MAX_VALUE_LEN];
} nameValuePair_t;

int loadJSonFile(char *fn, json_t **root);
int loadParamList(char *fn, nameValuePair_t list[], int alen);
int dumpJSonTree(json_t *root, char *buf, char *tmpStr);
int dumpParamList(nameValuePair_t list[], int len);

#ifdef __cplusplus
}
#endif
#endif
