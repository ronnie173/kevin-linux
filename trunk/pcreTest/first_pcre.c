#include <stdio.h>
#include <string.h>
#include <pcre.h>

#include "jsonUtil.h"

#define OVEC_COUNT 30 /* should be a multiple of 3 */

int basicTest();
int paramTest(const char *fn);

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
    
    return 0;
}
