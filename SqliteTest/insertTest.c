#include "insertTest.h"

int main(int argc, char **argv){
    if (argc < 2) {
        help(argv[0]);
        exit(1);
    }

    int c;

    while ((c = getopt(argc, argv, "d:t:cishn:")) != -1) {
        switch(c) {
            case 'd' :
                dbName = optarg;
            break;

            case 't' :
                tableName = optarg;
            break;

            case 'n' :
                loopCount = atoi(optarg);
            break;

            case 'h' :
                help(argv[0]);
            break;

            case 'c' :
                action = CLEAN_TABLE;
            break;

            case 'i' :
                action = INSERT_TABLE;
            break;

            case 's' :
                action = SEARCH_TABLE;
            break;

            case '?' :
                if ('d' == optopt) {
                    printf("database name is required.\n");
                } else if ('t' == optopt) {
                    printf("table name is required.\n");
                } else if ('n' == optopt) {
                    printf("loop times is required.\n");
                } else {
                    printf("Unknown option '-%c'.\n", optopt);
                }
                exit(1);

            default :
                printf("reach getopt default case\n");
                exit(1);
        }
    }

    int rc;
    rc = sqlite3_open(dbName, &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    /*if (!getTableMax()) {
        printf("getTableMax failed\n");
        goto sql_close;
    }

    if (!testMainLog()) {
        printf("testMainLog failed\n");
        goto sql_close;
    }*/

    switch (action) {
        case INSERT_TABLE :
            if (!strcmp("url_mapping", tableName)) {
                insertUrlTable();
            }
        break;

        case SEARCH_TABLE :
            searchTable();
        break;

        case CLEAN_TABLE :
            cleanTable();
        break;

        default :
            printf("Unknown action [%d]\n", action);
    }

    /*if (!testUrlInsert()) {
        printf("testUrlInsert failed\n");
        goto sql_close;
    }*/

sql_close:    sqlite3_close(db);

    return 0;
}

static void help(char *command) {
    printf("Usage: %s -d dbname -c tablename -h\n", command);
    printf("\t-d <required> specify database name\n");
    printf("\t-t specify table name\n");
    printf("\t-c clean table\n");
    printf("\t-h print this help\n");
    printf("\t-i insert table\n");
    printf("\t-s search table\n");
    printf("\t-n how many times\n");
}

static int cleanTable() {
    printf("clean table [%s]\n", tableName);
    char sqlBuf[256];
    bzero(sqlBuf, 256);
    sprintf(sqlBuf, cleanSql, tableName);
    printf("cleanSql is [%s]\n", sqlBuf);
    int rc;
    rc = sqlite3_exec(db, sqlBuf, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return 1;
    }

    return 0;
}

static int insertUrlTable() {
    int rc;
    char sqlBuf[512];
    char tmpStr[256];
    int urlLen, randNum, i, j;

    printf("insert will loop %d times\n", loopCount);

    /* start clock */
    clock_t start = clock();
    time_t starttime = time(NULL);

    for (j = 0; j < loopCount; j++) {
        bzero(tmpStr, 256);

        urlLen = rand() % URL_PATH_COUNT + 1;
        for (i = 0; i < urlLen; i++) {
            randNum = rand() % URL_PATH_COUNT;
            sprintf(tmpStr, "%s/%s", tmpStr, pathStr[randNum]);
        }
        randNum = rand() % URL_EXT_COUNT;
        sprintf(tmpStr, "%s/%s", tmpStr, extStr[randNum]);
        if (IF_LOG) printf("%s\n", tmpStr);

        bzero(sqlBuf, 256);
        sprintf(sqlBuf, insertSql, "url_mapping", tmpStr);
        if (IF_LOG) printf("insert sql is [%s]\n", sqlBuf);
        rc = sqlite3_exec(db, sqlBuf, 0, 0, &zErrMsg);
        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            goto sth_wrong;
        }
    }

    /* clock stops */
    clock_t end = clock();
    printf("insert spends %.2f seconds\n", ((double)(end - start)) / (double)CLOCKS_PER_SEC);
    time_t endtime = time(NULL);
    printf("insert spends %d seconds\n", (int)(endtime - starttime));

sth_wrong: return 1;
    return 0;
}

static int searchTable() {
    return 0;
}

static int testMainLog() {
    int rc;
    char insertSql[512];

    sqlite3_uint64 id; /* int8 primary key asc */
    time_t timestamp; /* datetime not null */
    uint src_ip; /* int4 not null */
    unsigned short int src_port; /* int2 not null */
    uint dst_ip; /* int4 not null */
    unsigned short int dst_port; /* int2 not null */
    unsigned short int bandwidth; /* int2 not null */

    /* don't use foreign key for now */
    unsigned char continent_id; /* int1 not null */
    unsigned short int country_id; /* int2 not null */
    uint state_id; /* int4 */
    uint city_id; /* int4 */
    uint host_id; /* int4 not null */
    uint url_id; /* int4 not null */
    unsigned short int attack_id; /* int2 not null */

    /* populate main_log table */
    /* id int8 primary key asc, timestamp datetime not null,
    src_ip int4 not null, src_port int2 not null,
    dst_ip int4 not null, dst_port int2 not null,
    bandwidth int2 not null, 
      
    continent_id int1 not null, country_id int2 not null, state_id int4,
    city_id int4, host_id int4 not null, url_id int4 not null,
    attack_id int2 not null */
    sprintf(insertSql, "insert into main_log values(%lu, %u, %u, %hu,"
            " %u, %hu, %hu, %u, %u, %u, %u, %u, %u, %u)", 1, time(0),
            996941108, 80, 3232235777, 8080, 100, 1, 1, 1, 1, 1, 1, 1);
    
    printf("insertSql is [%s]\n", insertSql);
    rc = sqlite3_exec(db, insertSql, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sth_wrong;
    }
    
    /* select * from main_log */
    clock_t start = clock();
    time_t starttime = time(NULL);
    int loop;
    for (loop = 0; loop < Z_LOOP; loop++) {
        rc = sqlite3_exec(db, selectMainLogSql, selectCallback, "mainLogSearch", &zErrMsg);
        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            goto sth_wrong;
        }
    }
    clock_t end = clock();
    printf("select spends %.2f seconds\n", ((double)(end - start)) / (double)CLOCKS_PER_SEC);
    time_t endtime = time(NULL);
    printf("select spends %d seconds\n", (int)(endtime -starttime));

sth_wrong: return 1;
    return 0;
}

static int testUrlInsert() {
    int rc;
    char sqlBuf[512];

    char tmpStr[256];
    bzero(tmpStr, 256);

    int urlLen, randNum, i, j;

    /* start clock */
    clock_t start = clock();
    time_t starttime = time(NULL);

    for (j = 0; j < loopCount; j++) {
        bzero(tmpStr, 256);

        urlLen = rand() % URL_PATH_COUNT + 1;
        for (i = 0; i < urlLen; i++) {
            randNum = rand() % URL_PATH_COUNT;
            sprintf(tmpStr, "%s/%s", tmpStr, pathStr[randNum]);
        }
        randNum = rand() % URL_EXT_COUNT;
        sprintf(tmpStr, "%s/%s", tmpStr, extStr[randNum]);
        if (IF_LOG) printf("%s\n", tmpStr);

        /* check if url exists already */
        bzero(sqlBuf, 256);
        selectID = 0;
        sprintf(sqlBuf, selectIDSql, "url_mapping", "name", tmpStr);
        if (IF_LOG) printf("select sql is [%s]\n", sqlBuf);
        rc = sqlite3_exec(db, sqlBuf, selectCallback, "urlSearch", &zErrMsg);
        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            goto sth_wrong;
        }

        if (selectID > 0) {
            printf("url exists with id [%u] - [%s]\n", selectID, tmpStr);
        } else {
            bzero(sqlBuf, 256);
            sprintf(sqlBuf, insertSql, "url_mapping", tmpStr);
            if (IF_LOG) printf("insert sql is [%s]\n", sqlBuf);
            rc = sqlite3_exec(db, sqlBuf, 0, 0, &zErrMsg);
            if( rc != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
                goto sth_wrong;
            }
        }
    }

    /* clock stops */
    clock_t end = clock();
    printf("insert spends %.2f seconds\n", ((double)(end - start)) / (double)CLOCKS_PER_SEC);
    time_t endtime = time(NULL);
    printf("insert spends %d seconds\n", (int)(endtime - starttime));

sth_wrong: return 1;
    return 0;
}

static int convertInt2IP(unsigned int ipAddress, char *buf) {
    int i;
    unsigned char octet[4] = {0,0,0,0};
    for (i = 0; i < 4; i++) {
        octet[i] = (ipAddress >> (i*8)) & 0xFF;
    }
    sprintf(buf, "%d.%d.%d.%d", (int)octet[3], (int)octet[2], (int)octet[1], (int)octet[0]);

    return 0;
}

static int countCallback(void *table, int argc, char **argv, char **azColName) {
    char *tableName = (char*)table;

    if (!strncmp("continent", tableName, 10)) {
        continentCount = atoi(argv[0]);
        printf("continent count is %d\n", continentCount);
    } else if (!strncmp("country", tableName, 10)) {
            countryCount = atoi(argv[0]);
            printf("country count is %d\n", countryCount);
    } else if (!strncmp("state", tableName, 10)) {
            stateCount = atoi(argv[0]);
            printf("state count is %d\n", stateCount);
    } else if (!strncmp("city", tableName, 10)) {
            cityCount = atoi(argv[0]);
            printf("city count is %d\n", cityCount);
    } else if (!strncmp("host", tableName, 10)) {
            hostCount = atoi(argv[0]);
            printf("host count is %d\n", hostCount);
    } else if (!strncmp("url", tableName, 10)) {
            urlCount = atoi(argv[0]);
            printf("url count is %d\n", urlCount);
    } else if (!strncmp("attack", tableName, 10)) {
            attackCount = atoi(argv[0]);
            printf("attack count is %d\n", attackCount);
    } else {
        printf("unknown table name [%s]\n", tableName);
    }
    return 0;
}

static int selectCallback(void *action, int argc, char **argv, char **azColName) {
    int i;
    unsigned int ipInt;
    time_t ts;
    char *colName, *colVal;

    if (NULL == action) {
        printf("action is null");
        return 1;
    }

    char *actionStr = (char *)action;

    if (!strcmp("mainLogSearch", actionStr)) {
        for (i = 0; i < argc; i++) {
            colName = azColName[i];
            colVal = argv[i] ? argv[i] : "NULL";

            if (!strncmp("src_ip", colName, 10) || !strncmp("dst_ip", colName, 10)) {
                //printf("colVal=[%s]\n", colVal);
                ipInt = (unsigned int)atoi(colVal);
                convertInt2IP(ipInt, ipBuf);
                colVal = ipBuf;
            } else if (!strncmp("timestamp", colName, 10)) {
                //printf("colVal=[%s]\n", colVal);
                ts = (time_t)atoi(colVal);
                //printf("timestamp=[%u]\n", ts);
                ctime_r(&ts, timeBuf);
                //printf("timeBuf=[%s]\n", timeBuf);
                colVal = timeBuf;
            }
            //printf("%s = %s\n", colName, colVal);
        }
        //printf("\n");
    } else if (!strcmp("urlSearch", actionStr)) {
        if (argc > 0) {
            selectID = atoi(argv[0]);
        } else {
            selectID = 0;
        }
    }

    return 0;
}

static int getTableMax() {
    int rc;

    printf("sql is [%s]\n", continentCountSql);
    rc = sqlite3_exec(db, continentCountSql, countCallback, "continent", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sth_wrong;
    }

    printf("sql is [%s]\n", countryCountSql);
    rc = sqlite3_exec(db, countryCountSql, countCallback, "country", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sth_wrong;
    }

    printf("sql is [%s]\n", stateCountSql);
    rc = sqlite3_exec(db, stateCountSql, countCallback, "state", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sth_wrong;
    }

    printf("sql is [%s]\n", cityCountSql);
    rc = sqlite3_exec(db, cityCountSql, countCallback, "city", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sth_wrong;
    }

    printf("sql is [%s]\n", hostCountSql);
    rc = sqlite3_exec(db, hostCountSql, countCallback, "host", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sth_wrong;
    }

    printf("sql is [%s]\n", urlCountSql);
    rc = sqlite3_exec(db, urlCountSql, countCallback, "url", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sth_wrong;
    }

    printf("sql is [%s]\n", attackCountSql);
    rc = sqlite3_exec(db, attackCountSql, countCallback, "attack", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sth_wrong;
    }

sth_wrong: return 1;

    return 0;
}
