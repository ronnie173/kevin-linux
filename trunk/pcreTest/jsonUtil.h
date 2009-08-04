#ifndef KZ_JSON_UTIL_H
#define KZ_JSON_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

int loadJSonFile(char *fn, json_t **root);
int loadParamList(char *fn);
int dumpJSonTree(json_t *root, char *buf, char *tmpStr);

#ifdef __cplusplus
}
#endif
#endif
