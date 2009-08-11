#include <stdio.h>
#include <string.h>
#include <pcre.h>

#include "jsonUtil.h"

#define OVEC_COUNT 30 /* should be a multiple of 3 */

int basicTest();
int paramTest(const char *fn);
int doMatch(pcre* re, nameValuePair_t paramList[], int alen);
int doPCRE(pcre* re, char *data);
int doMatchBatch(pcre* re, nameValuePair_t paramList[], int alen);

int main(int argc, char *argv[]) {
    basicTest();
    paramTest("./param_list.json");

    return 0;
}

int basicTest() {
    pcre *re;
    const char *error;
    int err_offset;
    int ovector[OVEC_COUNT];
    int rc;

    char *regex = "^From: ([^@]+)@([^\r]+)";
    char *data = "From: zhangjw@fortinet.com\r\n";

    re = pcre_compile(regex, 0, &error, &err_offset, NULL);

    if (!re) {
        printf("PCRE compilation failed at offset %d: %s\n", err_offset, error);
        return 1;
    }

    rc = pcre_exec(re, NULL, data, strlen(data), 0, 0, ovector, OVEC_COUNT);

    if (rc < 0) {
        switch (rc) {
        case PCRE_ERROR_NOMATCH :
            printf("No match found in text\n");
            break;

        default :
            printf("Match eror %d\n", rc);
            break;

        }

        pcre_free(re);
        return 1;
    }

    if (rc < 3) {
        printf("Match did not catch all the groups\n");
    } else {
        /* ovector[0] .. ovector[1] ar the entire matched string */
        char *name_start = data + ovector[2];
        int name_length = ovector[3] - ovector[2];

        char *domain_start = data + ovector[4];
        int domain_length = ovector[5] - ovector[4];

        printf("Mail from: %.*s domain: %.*s\n", name_length, name_start,
                domain_length, domain_start);
    }

    pcre_free(re);

    return 0;
}

int paramTest(const char *fn) {
    nameValuePair_t paramList[MAX_PAIR_ARRAY_LEN];

    loadParamList(fn, paramList, MAX_PAIR_ARRAY_LEN);
    dumpParamList(paramList, MAX_PAIR_ARRAY_LEN);

    pcre *re;
    const char *error;
    int err_offset;

    char *regex = "\\b[a-zA-Z0-9._%-]+@[a-zA-Z0-9._%-]+\\.[a-zA-Z]{2,4}\\b";
    re = pcre_compile(regex, 0, &error, &err_offset, NULL);

    if (!re) {
        printf("PCRE compilation failed at offset %d: %s\n", err_offset, error);
        return 1;
    }

    //doMatch(re, paramList, MAX_PAIR_ARRAY_LEN);
    doMatchBatch(re, paramList, MAX_PAIR_ARRAY_LEN);
    pcre_free(re);

    return 0;
}

int doMatch(pcre* re, nameValuePair_t paramList[], int alen) {
    int i;
    //nameValuePair_t tmpParam;

    for (i = 0; i < alen; i++) {
        doPCRE(re, paramList[i].name);
        doPCRE(re, paramList[i].value);
    }

    return 0;
}

int doMatchBatch(pcre* re, nameValuePair_t paramList[], int alen) {
    char buf[10000];
    bzero(buf, 10000);

    int i;
    //nameValuePair_t tmpParam;

    for (i = 0; i < alen; i++) {
        strcat(buf, paramList[i].name);
        strcat(buf, "\r\n");
        strcat(buf, paramList[i].value);
        strcat(buf, "\r\n");
    }

    printf("check\n %s\n", buf);
    int oc = 3 * alen;
    int *ovector = malloc(oc);
    bzero(ovector, oc);

    int rc;

    rc = pcre_exec(re, NULL, buf, strlen(buf), 0, 0, ovector, oc);

    if (rc < 0) {
        switch (rc) {
        case PCRE_ERROR_NOMATCH :
            printf("No match found in text\n");
            break;

        default :
            printf("Match eror %d\n", rc);
            break;

        }
        free(ovector);
        return 1;
    }

    for (i = 0; i < rc; i++) {
        /* ovector[0] .. ovector[1] are the entire matched string */
        char *str_start = buf + ovector[i];
        int str_length = ovector[i + 1] - ovector[i];

        printf("Found string: %.*s\n", str_length, str_start);
    }

    free(ovector);

    return 0;
}

int doPCRE(pcre* re, char *data) {
    printf("check %s\n", data);

    int ovector[3];
    int rc;

    rc = pcre_exec(re, NULL, data, strlen(data), 0, 0, ovector, 3);

    if (rc < 0) {
        switch (rc) {
        case PCRE_ERROR_NOMATCH :
            printf("No match found in text\n");
            break;

        default :
            printf("Match eror %d\n", rc);
            break;

        }

        return 1;
    }

    /* ovector[0] .. ovector[1] are the entire matched string */
    char *str_start = data + ovector[0];
    int str_length = ovector[1] - ovector[0];

    printf("Found string: %.*s\n", str_length, str_start);

    return 0;
}
