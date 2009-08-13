#include <stdio.h>
#include <string.h>
#include <pcre.h>
#include <time.h>

#include "jsonUtil.h"

#define OVEC_COUNT 30 /* should be a multiple of 3 */
#define ZLOOP 1

int basicTest();
int paramTest(const char *fn);
int doMatch(pcre* re, nameValuePair_t paramList[], int alen);
int doPCRE(pcre* re, char *data, int offset, int *start, int *end);
int doMatchBatch(pcre* re, nameValuePair_t paramList[], int alen);
int mycallout(pcre_callout_block *block);
int printSubStr(const char *data, int start, int end);

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
    re = pcre_compile(regex, PCRE_MULTILINE, &error, &err_offset, NULL);

    if (!re) {
        printf("PCRE compilation failed at offset %d: %s\n", err_offset, error);
        return 1;
    }

    clock_t start = clock();
    int i;
    for (i = 0; i < ZLOOP; i++) {
        doMatch(re, paramList, MAX_PAIR_ARRAY_LEN);
    }
    clock_t end = clock();
    printf("doMatch spends %.f clocks\n", (double)end - start);

    start = clock();
    for (i = 0; i < ZLOOP; i++) {
        doMatchBatch(re, paramList, MAX_PAIR_ARRAY_LEN);
    }
    end = clock();
    printf("doMatchBatch spends %.f clocks\n", (double)end - start);

    pcre_free(re);

    return 0;
}

int doMatch(pcre* re, nameValuePair_t paramList[], int alen) {
    int i, ret;
    //nameValuePair_t tmpParam;

    for (i = 0; i < alen; i++) {
        int start, end;
        ret = doPCRE(re, paramList[i].name, 0, &start, &end);
        if (!ret) printSubStr(paramList[i].name, start, end);
        
        ret = doPCRE(re, paramList[i].value, 0, &start, &end);
        if (!ret) printSubStr(paramList[i].value, start, end);
    }

    return 0;
}

int doMatchBatch(pcre* re, nameValuePair_t paramList[], int alen) {
    char buf[10000];
    bzero(buf, 10000);

    int i, ret, start, end, curPos;
    //nameValuePair_t tmpParam;

    char *pos = buf;
    for (i = 0; i < alen; i++) {
        /*strcat(buf, paramList[i].name);
        strcat(buf, "\r\n");
        strcat(buf, paramList[i].value);
        strcat(buf, "\r\n");*/
        sprintf(pos, "%s\n%s\n", paramList[i].name, paramList[i].value);
        //pos += strlen(buf) -1;
        pos += strlen(paramList[i].name) + strlen(paramList[i].value) + 2;
        //printf("buf len=%d\n", strlen(buf));
        //printf("the buf is\n%s\n", buf);
    }

    //printf("the buf is\n%s\n", buf);
    
    curPos = 0;
    ret = doPCRE(re, buf, curPos, &start, &end);
    while (!ret) {
        printf("start is %d and end is %d\n", start, end);
        printSubStr(buf, start, end);
        ret = doPCRE(re, buf,  curPos + end + 1, &start, &end);
    }
    
    return 0;
}

int doPCRE(pcre* re, char *data, int offset, int *start, int *end) {
    //printf("check\n%s\n", data);
    //printf("data len is %d\n", strlen(data));
    //printf("offset is %d\n", offset);
    
    *start = -1;
    *end = -1;
    
    int ovector[3];
    int rc;

    rc = pcre_exec(re, NULL, data, strlen(data), offset, 0, ovector, 3);

    if (rc < 0) {
        switch (rc) {
        case PCRE_ERROR_NOMATCH :
            //printf("No match found in text\n");
            break;

        default :
            //printf("Match eror %d\n", rc);
            break;

        }

        return 1;
    }

    /* ovector[0] .. ovector[1] are the entire matched string */
    *start = ovector[0];
    *end = ovector[1];
    
    return 0;
}

int mycallout(pcre_callout_block *block) {
    printf("get into call out\n");
    printf("subject is %s\n", block->subject);
    printf("start_match is %d\n", block->start_match);
    printf("current_position is %d\n", block->current_position);
    printf("capture_top is %d\n", block->capture_top);

    printf("start at %d and end at %d\n",
            (block->offset_vector)[0], (block->offset_vector)[1]);
    printf("start at %d and end at %d\n",
            (block->offset_vector)[2], (block->offset_vector)[3]);
    printf("start at %d and end at %d\n",
            (block->offset_vector)[4], (block->offset_vector)[5]);

    return 0;
}

int printSubStr(const char *data, int start, int end) {
    if (data != NULL && start != -1 && end != -1) {
        const char *str_start = data + start;
        int str_length = end - start;

        //printf("Found string: %.*s\n", str_length, str_start);
    }

    return 0;
}