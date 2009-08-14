#include <stdio.h>
#include <string.h>
#include <pcre.h>
#include <time.h>

#include "jsonUtil.h"

#define OVEC_COUNT 30 /* should be a multiple of 3 */
#define ZLOOP 1000000
#define PRINT_FLAG 0

int basicTest();
int paramTest(const char *fn);
int doMatch(pcre* re, nameValuePair_t paramList[], int alen);
int doPCRE(pcre* re, char *data, int len, int offset, int *start, int *end);
int doMatchBatch(pcre* re, char *buf, int len);
int mycallout(pcre_callout_block *block);
int printSubStr(const char *data, int start, int end);
int pairArrayToBuf(nameValuePair_t paramList[], int alen, char *buf);
pcre* compileRE(const char *regex);
int perfTest(const char *fn);
int perfTest2(const char *fn);
int perfTest3(const char *fn);
int batchTest(const char *fn);

int main(int argc, char *argv[]) {
    //basicTest();
    //paramTest("./param_list.json");
    //perfTest("./param_list.json");
    //perfTest2("./param_list.json");
    perfTest3("./param_list.json");
    //batchTest("./short.json");

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

    char *regex = "\\b[a-zA-Z0-9._%-]+@[a-zA-Z0-9._%-]+\\.[a-zA-Z]{2,4}\\b";
    pcre *re = compileRE(regex);

    if (!re) {
        return 1;
    }

    int i;

    /* array match */
    clock_t start = clock();
    for (i = 0; i < ZLOOP; i++) {
        doMatch(re, paramList, MAX_PAIR_ARRAY_LEN);
    }
    clock_t end = clock();
    printf("doMatch spends %.f clocks\n", (double)end - start);

    start = clock();
    for (i = 0; i < ZLOOP; i++) {
        doMatch(re, paramList, MAX_PAIR_ARRAY_LEN / 2);
    }
    end = clock();
    printf("half doMatch spends %.f clocks\n", (double)end - start);

    pcre_free(re);

    /* put array into buffer */
    start = clock();
    char buf[10000];
    for (i = 0; i < ZLOOP; i++) {
        bzero(buf, 10000);
        pairArrayToBuf(paramList, MAX_PAIR_ARRAY_LEN, buf);
    }
    end = clock();
    printf("arrayToBuf spends %.f clocks\n", (double)end - start);

    /* batch match */
    int len = strlen(buf);
    printf("buf len is %d\n", len);
    printf("buf is \n%s\n", buf);
    re = compileRE(regex);

    if (!re) {
        return 1;
    }

    start = clock();
    for (i = 0; i < ZLOOP; i++) {
        doMatchBatch(re, buf, len);
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
        ret = doPCRE(re, paramList[i].name, paramList[i].nameLen, 0, &start, &end);
        if (!ret) printSubStr(paramList[i].name, start, end);

        ret = doPCRE(re, paramList[i].value, paramList[i].valueLen, 0, &start, &end);
        if (!ret) printSubStr(paramList[i].value, start, end);
    }

    return 0;
}

int doMatchBatch(pcre* re, char *buf, int len) {
    int ret, start, end, curPos;

    /*curPos = 0;
    ret = doPCRE(re, buf, len, curPos, &start, &end);
    while (!ret) {
        if (PRINT_FLAG) printf("start is %d and end is %d\n", start, end);
        if (PRINT_FLAG) printSubStr(buf, start, end);
        ret = doPCRE(re, buf, len, curPos + end + 1, &start, &end);
    }*/

    char *newPos = buf;
    ret = doPCRE(re, newPos, len, 0, &start, &end);
    //while (!ret) {
    if (!ret) {
        if (PRINT_FLAG) printf("start is %d and end is %d\n", start, end);
        if (PRINT_FLAG) printSubStr(newPos, start, end);
        newPos += end + 1;
        //len = strlen(newPos);
        len -= end + 1;
        //if (PRINT_FLAG) printf("new buf len is %d\n", len);
        //if (PRINT_FLAG) printf("new buf is\n%s\n", newPos);
        ret = doPCRE(re, newPos, len, 0, &start, &end);
    }

    if (!ret) {
        if (PRINT_FLAG) printf("start is %d and end is %d\n", start, end);
        if (PRINT_FLAG) printSubStr(newPos, start, end);
        newPos += end + 1;
        len -= end + 1;
        ret = doPCRE(re, newPos, len, 0, &start, &end);
    }

    if (!ret) {
        if (PRINT_FLAG) printf("start is %d and end is %d\n", start, end);
        if (PRINT_FLAG) printSubStr(newPos, start, end);
    }

    return 0;
}

int doPCRE(pcre* re, char *data, int len, int offset, int *start, int *end) {
    //printf("check\n%s\n", data);
    //printf("data len is %d\n", len);
    //printf("offset is %d\n", offset);

    *start = -1;
    *end = -1;

    int ovector[3] = { 0, 0, 0 };
    int rc;

    rc = pcre_exec(re, NULL, data, len, offset, 0, ovector, 3);

    if (rc < 0) {
        switch (rc) {
        case PCRE_ERROR_NOMATCH :
            if (PRINT_FLAG) printf("No match found in text\n");
            break;

        default :
            if (PRINT_FLAG) printf("Match eror %d\n", rc);
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

        if (PRINT_FLAG) printf("Found string: %.*s\n", str_length, str_start);
    }

    return 0;
}

int pairArrayToBuf(nameValuePair_t paramList[], int alen, char *buf) {
    int i;

    char *pos = buf;
    for (i = 0; i < alen; i++) {
        /*strcat(buf, paramList[i].name);
        strcat(buf, "\r\n");
        strcat(buf, paramList[i].value);
        strcat(buf, "\r\n");*/
        sprintf(pos, "%s\n%s\n", paramList[i].name, paramList[i].value);
        //pos += strlen(buf) -1;
        pos += paramList[i].nameLen + paramList[i].valueLen + 2;
        //printf("buf len=%d\n", strlen(buf));
        //printf("the buf is\n%s\n", buf);
    }

    return 0;
}

pcre* compileRE(const char *regex) {
    const char *error;
    int err_offset;

    //pcre *re = pcre_compile(regex, PCRE_MULTILINE, &error, &err_offset, NULL);
    pcre *re = pcre_compile(regex, 0, &error, &err_offset, NULL);

    if (!re) {
        printf("PCRE compilation failed at offset %d: %s\n", err_offset, error);
    }

    return re;
}

int perfTest(const char *fn) {
    nameValuePair_t paramList[MAX_PAIR_ARRAY_LEN];

    loadParamList(fn, paramList, MAX_PAIR_ARRAY_LEN);
    //dumpParamList(paramList, MAX_PAIR_ARRAY_LEN);

    char *regex = "\\b[a-zA-Z0-9._%-]+@[a-zA-Z0-9._%-]+\\.[a-zA-Z]{2,4}\\b";

    /* full length test */
    pcre *re = compileRE(regex);

    if (!re) {
        return 1;
    }

    int i, len;
    char buf[10000];
    bzero(buf, 10000);
    pairArrayToBuf(paramList, MAX_PAIR_ARRAY_LEN, buf);

    len = strlen(buf);
    printf("buf len is %d\n", len);
    printf("buf is \n%s\n", buf);

    clock_t start = clock();
    for (i = 0; i < ZLOOP; i++) {
        doMatchBatch(re, buf, len);
    }
    clock_t end = clock();
    printf("full length spends %.f clocks\n", (double)end - start);
    pcre_free(re);

    /* half length test */
    re = compileRE(regex);
    if (!re) {
        return 1;
    }

    bzero(buf, 10000);
    pairArrayToBuf(paramList, MAX_PAIR_ARRAY_LEN / 2, buf);
    len = strlen(buf);
    printf("buf len is %d\n", len);
    printf("buf is \n%s\n", buf);

    start = clock();
    for (i = 0; i < ZLOOP; i++) {
        doMatchBatch(re, buf, len);
    }
    end = clock();
    printf("half spends %.f clocks\n", (double)end - start);
    pcre_free(re);

    /* short length test */
    re = compileRE(regex);
    if (!re) {
        return 1;
    }

    bzero(buf, 10000);
    pairArrayToBuf(paramList, 11, buf);
    len = strlen(buf);
    printf("buf len is %d\n", len);
    printf("buf len is %d\n", strlen(buf));
    printf("buf is \n%s\n", buf);

    start = clock();
    for (i = 0; i < ZLOOP; i++) {
        doMatchBatch(re, buf, len);
    }
    end = clock();
    printf("short spends %.f clocks\n", (double)end - start);
    pcre_free(re);

    return 0;
}

int perfTest2(const char *fn) {
    nameValuePair_t paramList[MAX_PAIR_ARRAY_LEN];

    loadParamList(fn, paramList, MAX_PAIR_ARRAY_LEN);
    //dumpParamList(paramList, MAX_PAIR_ARRAY_LEN);

    char *regex = "\\b[a-zA-Z0-9._%-]+@[a-zA-Z0-9._%-]+\\.[a-zA-Z]{2,4}\\b";

    /* full length test */
    pcre *re = compileRE(regex);

    if (!re) {
        return 1;
    }

    int i, len, start, end;
    char buf[10000];
    bzero(buf, 10000);
    pairArrayToBuf(paramList, MAX_PAIR_ARRAY_LEN, buf);

    len = strlen(buf);
    printf("buf len is %d\n", len);
    printf("buf is \n%s\n", buf);

    clock_t startTime = clock();
    for (i = 0; i < ZLOOP; i++) {
        doPCRE(re, buf, len, 0, &start, &end);
        if (PRINT_FLAG) printSubStr(buf, start, end);
    }
    clock_t endTime = clock();
    printf("full length spends %.f clocks\n", (double)endTime - startTime);
    pcre_free(re);

    /* half length test */
    re = compileRE(regex);
    if (!re) {
        return 1;
    }

    bzero(buf, 10000);
    pairArrayToBuf(paramList, MAX_PAIR_ARRAY_LEN / 2, buf);
    len = strlen(buf);
    printf("buf len is %d\n", len);
    printf("buf is \n%s\n", buf);

    startTime = clock();
    for (i = 0; i < ZLOOP; i++) {
        doPCRE(re, buf, len, 0, &start, &end);
        if (PRINT_FLAG) printSubStr(buf, start, end);
    }
    endTime = clock();
    printf("half spends %.f clocks\n", (double)endTime - startTime);
    pcre_free(re);

    /* short length test */
    re = compileRE(regex);
    if (!re) {
        return 1;
    }

    bzero(buf, 10000);
    pairArrayToBuf(paramList, 5, buf);
    len = strlen(buf);
    printf("buf len is %d\n", len);
    printf("buf len is %d\n", strlen(buf));
    printf("buf is \n%s\n", buf);

    startTime = clock();
    for (i = 0; i < ZLOOP; i++) {
        doPCRE(re, buf, len, 0, &start, &end);
        if (PRINT_FLAG) printSubStr(buf, start, end);
    }
    endTime = clock();
    printf("short spends %.f clocks\n", (double)endTime - startTime);
    pcre_free(re);

    return 0;
}

int perfTest3(const char *fn) {
    nameValuePair_t paramList[MAX_PAIR_ARRAY_LEN];

    loadParamList(fn, paramList, MAX_PAIR_ARRAY_LEN);
    //dumpParamList(paramList, MAX_PAIR_ARRAY_LEN);

    char *regex = "\\b[a-zA-Z0-9._%-]+@[a-zA-Z0-9._%-]+\\.[a-zA-Z]{2,4}\\b";

    /* full length test */
    pcre *re = compileRE(regex);

    if (!re) {
        return 1;
    }

    int i;

    clock_t startTime = clock();
    for (i = 0; i < ZLOOP; i++) {
        doMatch(re, paramList, MAX_PAIR_ARRAY_LEN);
    }
    clock_t endTime = clock();
    printf("full length spends %.f clocks\n", (double)endTime - startTime);
    pcre_free(re);

    /* half length test */
    re = compileRE(regex);
    if (!re) {
        return 1;
    }

    startTime = clock();
    for (i = 0; i < ZLOOP; i++) {
        doMatch(re, paramList, MAX_PAIR_ARRAY_LEN / 2);
    }
    endTime = clock();
    printf("half spends %.f clocks\n", (double)endTime - startTime);
    pcre_free(re);

    /* short length test */
    re = compileRE(regex);
    if (!re) {
        return 1;
    }

    startTime = clock();
    for (i = 0; i < ZLOOP; i++) {
        doMatch(re, paramList, MAX_PAIR_ARRAY_LEN / 5);
    }
    endTime = clock();
    printf("short spends %.f clocks\n", (double)endTime - startTime);
    pcre_free(re);

    return 0;
}

int batchTest(const char *fn) {
    nameValuePair_t paramList[MAX_PAIR_ARRAY_LEN];

    loadParamList(fn, paramList, MAX_PAIR_ARRAY_LEN);
    //dumpParamList(paramList, MAX_PAIR_ARRAY_LEN);

    char *regex = "\\b[a-zA-Z0-9._%-]+@[a-zA-Z0-9._%-]+\\.[a-zA-Z]{2,4}\\b";

    /* full length test */
    pcre *re = compileRE(regex);

    if (!re) {
        return 1;
    }

    int len;
    char buf[10000];
    bzero(buf, 10000);
    pairArrayToBuf(paramList, MAX_PAIR_ARRAY_LEN, buf);

    len = strlen(buf);
    printf("buf len is %d\n", len);
    printf("buf is \n%s\n", buf);

    doMatchBatch(re, buf, len);

    pcre_free(re);

    return 0;
}
