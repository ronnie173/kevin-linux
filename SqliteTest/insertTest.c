#include "insertTest.h"

int main(int argc, char **argv){
    if (argc < 2) {
        help(argv[0]);
        exit(1);
    }

    srandom((unsigned int)time(NULL));

    int c;

    while ((c = getopt(argc, argv, "d:t:cishn:bm")) != -1) {
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

            case 'm' :
                action = MAX_VALUE;
            break;

            case 'b' :
                batchOrNot = BATCH;
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

    //getTableMax();

    //testMainLog();

    switch (action) {
        case INSERT_TABLE :
            if (!strcmp("url_mapping", tableName)) {
            	if (batchOrNot) {
            		insertUrlTableBatch();
            	} else {
            		insertUrlTable();
            	}
            }
        break;

        case SEARCH_TABLE :
            if (!strcmp("url_mapping", tableName)) {
                searchUrlTableByUrl();
            }
        break;

        case CLEAN_TABLE :
            cleanTable();
        break;

        case MAX_VALUE :
            getTableMax();
        break;

        default :
            printf("Unknown action [%d]\n", action);
    }

    //testUrlInsert();

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
    printf("\t-b batch processing\n");
}

static int cleanTable() {
    printf("clean table [%s]\n", tableName);
    char sqlBuf[256], *zErrMsg;
    bzero(sqlBuf, 256);
    sqlite3 *mydb;
    int rc;

    rc = sqlite3_open(dbName, &mydb);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(mydb));
        sqlite3_close(mydb);
        return 1;
    }

    sprintf(sqlBuf, cleanSql, tableName);
    printf("cleanSql is [%s]\n", sqlBuf);
    rc = sqlite3_exec(mydb, sqlBuf, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        sqlite3_close(mydb);
        return 1;
    }

    sqlite3_close(mydb);
    return 0;
}

static int insertUrlTable() {
    int rc, j;
    char sqlBuf[512], tmpStr[256], *zErrMsg;

    sqlite3 *mydb;

    rc = sqlite3_open(dbName, &mydb);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(mydb));
        sqlite3_close(mydb);
        return 1;
    }

    printf("insert will loop %d times\n", loopCount);

    /* start clock */
    clock_t start = clock();
    time_t starttime = time(NULL);

    for (j = 0; j < loopCount; j++) {
        bzero(tmpStr, 256);
        randomURL(tmpStr);
        if (IF_LOG) printf("%s\n", tmpStr);

        bzero(sqlBuf, 512);
        sprintf(sqlBuf, insertSql, "url_mapping", tmpStr);
        //if (IF_LOG) printf("insert sql is [%s]\n", sqlBuf);
        printf("%s\n", sqlBuf);
        rc = sqlite3_exec(mydb, sqlBuf, 0, 0, &zErrMsg);
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
    
sth_wrong: sqlite3_close(mydb);
		return 1;
}

static int insertUrlTableBatch() {
	printf("get into insertUrlTableBatch()\n");
	
	int rc, j, ret = 0;
    char *bigBuf, *zErrMsg;
    char sqlBuf[300], tmpStr[256];
    char *tmpFn = "./sql.txt";
    char *urlInsertSql = "insert into url_mapping values(NULL, '%s');";
    
    if (loopCount <= 0) {
    	printf("loopCount is wrong [%d]\n", loopCount);
    	return 0;
    }
    
    printf("insert will loop %d times\n", loopCount);

    FILE *tmpFile = fopen(tmpFn, "w+");
    fprintf(tmpFile, "%s\n", "begin transaction;");
    
    for (j = 0; j < loopCount; j++) {
        bzero(tmpStr, 256);
        bzero(sqlBuf, 300);
        randomURL(tmpStr);
        if (IF_LOG) printf("%s\n", tmpStr);

        sprintf(sqlBuf, urlInsertSql, tmpStr);
        //if (IF_LOG) printf("insert sql is [%s]\n", sqlBuf);
        fprintf(tmpFile, "%s\n", sqlBuf);
    }
    fprintf(tmpFile, "%s\n", "commit;");
    fclose(tmpFile);
    
    struct stat st;
    stat(tmpFn, &st);
    int size = (int)st.st_size;
    printf("file size is %d\n", size);
    bigBuf = malloc(size);
    bzero(bigBuf, size);

    
    int fd, len;
    fd = open(tmpFn, O_RDONLY);
    len = (int)read(fd, bigBuf, (size_t)size);
    close(fd);
    printf("read %d bytes\n", len);
    if (len != size) {
    	printf("read file error\n");
    	goto sth_wrong;
    }

    sqlite3 *mydb;

    rc = sqlite3_open(dbName, &mydb);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(mydb));
        sqlite3_close(mydb);
        return 1;
    }

    /* start clock */
    clock_t start = clock();
    time_t starttime = time(NULL);

    rc = sqlite3_exec(mydb, bigBuf, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        ret = 1;
        goto sth_wrong;
    }
    /* clock stops */
    clock_t end = clock();
    printf("insert spends %.2f seconds\n", ((double)(end - start)) / (double)CLOCKS_PER_SEC);
    time_t endtime = time(NULL);
    printf("insert spends %d seconds\n", (int)(endtime - starttime));
    
sth_wrong: sqlite3_close(mydb);
	free(bigBuf);
	return ret;
}

static int searchUrlTableByUrl(char *url) {
    int rc, j, id;
    sqlite3 *mydb;
    char *sql = "select id from url_mapping where name = '%s'";
    char sqlBuf[512], tmpBuf[256];

    if (loopCount <= 0) {
        printf("loopCount is wrong [%d]\n", loopCount);
        return 0;
    }

    rc = sqlite3_open(dbName, &mydb);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(mydb));
        sqlite3_close(mydb);
        return 1;
    }

    /* start clock */
    clock_t start = clock();
    time_t starttime = time(NULL);

    for (j = 0; j < loopCount; j++) {
        bzero(tmpBuf, 256);
        randomURL(tmpBuf);
        bzero(sqlBuf, 512);
        sprintf(sqlBuf, sql, tmpBuf);
        if (IF_LOG) printf("search sql is [%s]\n", sqlBuf);

        id = searchFirstID(mydb, sqlBuf);
        if (id > 0) {
            printf("found id %d\n", id);
            printf("search sql is [%s]\n", sqlBuf);
        }
    }

    /* clock stops */
    clock_t end = clock();
    printf("search by url spends %.2f seconds\n", ((double)(end - start)) / (double)CLOCKS_PER_SEC);
    time_t endtime = time(NULL);
    printf("search by url spends %d seconds\n", (int)(endtime - starttime));

    sqlite3_close(mydb);

    return 0;
}

static int searchFirstID(sqlite3 *mydb, char *sql) {
    int rc, id = -1;
    sqlite3_stmt *stmt;
    const char *tail;

    rc = sqlite3_prepare(mydb, sql, (int)strlen(sql), &stmt, &tail);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(mydb));
        return -1;
    }

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        if (!strcmp("id", sqlite3_column_name(stmt, 0))) {
            id = (int)sqlite3_column_int64(stmt, 0);
        } else {
            printf("column name is NOT id\n");
        }
    } else {
         if (IF_LOG) printf("can not find id\n");
    }

    sqlite3_finalize(stmt);

    return id;
}

static int testMainLog() {
    int rc;
    sqlite3 *mydb;
    char insertSql[512], *zErrMsg;

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

    rc = sqlite3_open(dbName, &mydb);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(mydb));
        sqlite3_close(mydb);
        return 1;
    }

    /* populate main_log table */
    /* id int8 primary key asc, timestamp datetime not null,
    src_ip int4 not null, src_port int2 not null,
    dst_ip int4 not null, dst_port int2 not null,
    bandwidth int2 not null, 
      
    continent_id int1 not null, country_id int2 not null, state_id int4,
    city_id int4, host_id int4 not null, url_id int4 not null,
    attack_id int2 not null */
    sprintf(insertSql, "insert into main_log values(%lu, %u, %u, %hu,"
            " %u, %hu, %hu, %u, %u, %u, %u, %u, %u, %u)", 1uL, (uint)time(0),
            996941108u, 80, 3232235777u, 8080, 100, 1, 1, 1, 1, 1, 1, 1);
    
    printf("insertSql is [%s]\n", insertSql);
    rc = sqlite3_exec(mydb, insertSql, 0, 0, &zErrMsg);
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
        rc = sqlite3_exec(mydb, selectMainLogSql, selectCallback, "mainLogSearch", &zErrMsg);
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

sth_wrong: sqlite3_close(mydb);
    return 0;
}

static int testUrlInsert() {
    int rc;
    char sqlBuf[512], tmpStr[256], *zErrMsg;
    sqlite3 *mydb;

    bzero(tmpStr, 256);

    int j;

    if (loopCount <= 0) {
        printf("loopCount is wrong [%d]\n", loopCount);
        return 0;
    }

    rc = sqlite3_open(dbName, &mydb);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(mydb));
        sqlite3_close(mydb);
        return 1;
    }

    /* start clock */
    clock_t start = clock();
    time_t starttime = time(NULL);

    for (j = 0; j < loopCount; j++) {
        bzero(tmpStr, 256);
        randomURL(tmpStr);

        /* check if url exists already */
        bzero(sqlBuf, 256);
        unsigned int selectID = 0;

        sprintf(sqlBuf, selectIDSql, "url_mapping", "name", tmpStr);
        if (IF_LOG) printf("select sql is [%s]\n", sqlBuf);

        selectID = searchFirstID(mydb, sqlBuf);
        if (selectID > 0) {
            printf("url exists with id [%u] - [%s]\n", selectID, tmpStr);
        } else {
            bzero(sqlBuf, 256);
            sprintf(sqlBuf, insertSql, "url_mapping", tmpStr);
            if (IF_LOG) printf("insert sql is [%s]\n", sqlBuf);
            rc = sqlite3_exec(mydb, sqlBuf, 0, 0, &zErrMsg);
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

sth_wrong:     sqlite3_close(mydb);
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
    }

    return 0;
}

static int getTableMax() {
    int rc;
    char *zErrMsg;
    sqlite3 *mydb;
    char *continentCountSql = "select count(id) from continent_mapping";
    char *countryCountSql = "select count(id) from country_mapping";
    char *stateCountSql = "select count(id) from state_mapping";
    char *cityCountSql = "select count(id) from city_mapping";
    char *hostCountSql = "select count(id) from host_mapping";
    char *urlCountSql = "select count(id) from url_mapping";
    char *attackCountSql = "select count(id) from attack_mapping";

    rc = sqlite3_open(dbName, &mydb);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(mydb));
        sqlite3_close(mydb);
        return 1;
    }

    printf("sql is [%s]\n", continentCountSql);
    rc = sqlite3_exec(mydb, continentCountSql, countCallback, "continent", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sth_wrong;
    }

    printf("sql is [%s]\n", countryCountSql);
    rc = sqlite3_exec(mydb, countryCountSql, countCallback, "country", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sth_wrong;
    }

    printf("sql is [%s]\n", stateCountSql);
    rc = sqlite3_exec(mydb, stateCountSql, countCallback, "state", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sth_wrong;
    }

    printf("sql is [%s]\n", cityCountSql);
    rc = sqlite3_exec(mydb, cityCountSql, countCallback, "city", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sth_wrong;
    }

    printf("sql is [%s]\n", hostCountSql);
    rc = sqlite3_exec(mydb, hostCountSql, countCallback, "host", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sth_wrong;
    }

    printf("sql is [%s]\n", urlCountSql);
    rc = sqlite3_exec(mydb, urlCountSql, countCallback, "url", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sth_wrong;
    }

    printf("sql is [%s]\n", attackCountSql);
    rc = sqlite3_exec(mydb, attackCountSql, countCallback, "attack", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sth_wrong;
    }

sth_wrong: sqlite3_close(mydb);

    return 0;
}

static int randomURL(char *buf) {
    int i, urlLen, randNum;

    urlLen = rand() % URL_PATH_COUNT + 1;
    for (i = 0; i < urlLen; i++) {
        randNum = rand() % URL_PATH_COUNT;
        sprintf(buf, "%s/%s", buf, pathStr[randNum]);
    }
    randNum = rand() % URL_EXT_COUNT;
    sprintf(buf, "%s/%s", buf, extStr[randNum]);
    if (IF_LOG) printf("%s\n", buf);

    return 0;
}
