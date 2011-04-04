#include "insertTest.h"

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

static int selectCallback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    unsigned int ipInt;
    time_t ts;
    char *colName, *colVal;
    
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
    return 0;
}

int main(int argc, char **argv){
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *continentCountSql = "select count(id) from continent_mapping";
    char *countryCountSql = "select count(id) from country_mapping";
    char *stateCountSql = "select count(id) from state_mapping";
    char *cityCountSql = "select count(id) from city_mapping";
    char *hostCountSql = "select count(id) from host_mapping";
    char *urlCountSql = "select count(id) from url_mapping";
    char *attackCountSql = "select count(id) from attack_mapping";
    char *cleanSql = "delete from main_log";
    char *selectMainLogSql = "select ml.id, ml.timestamp, ml.src_ip, "
    		"ml.src_port, ml.dst_ip, ml.dst_port, ml.bandwidth, con.name, "
    		"cou.name, sta.name, cit.name, host.name, url.name, att.name "
    		"from main_log ml, continent_mapping con, country_mapping cou, "
    		"state_mapping sta, city_mapping cit, host_mapping host, "
    		"url_mapping url, attack_mapping att where "
    		"ml.id = 1 and "
    		"ml.continent_id = con.id and ml.country_id = cou.id and "
    		"ml.state_id = sta.id and ml.city_id = cit.id and "
    		"ml.host_id = host.id and ml.url_id = url.id and "
    		"ml.attack_id = att.id";
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
    
    
    if( argc != 2 ){
        fprintf(stderr, "Usage: %s DATABASE\n", argv[0]);
        exit(1);
    }
    
    rc = sqlite3_open(argv[1], &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    
    printf("sql is [%s]\n", continentCountSql);
    rc = sqlite3_exec(db, continentCountSql, countCallback, "continent", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sql_close;
    }

    printf("sql is [%s]\n", countryCountSql);
    rc = sqlite3_exec(db, countryCountSql, countCallback, "country", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sql_close;
    }
    
    printf("sql is [%s]\n", stateCountSql);
    rc = sqlite3_exec(db, stateCountSql, countCallback, "state", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sql_close;
    }
    
    printf("sql is [%s]\n", cityCountSql);
    rc = sqlite3_exec(db, cityCountSql, countCallback, "city", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sql_close;
    }
    
    printf("sql is [%s]\n", hostCountSql);
    rc = sqlite3_exec(db, hostCountSql, countCallback, "host", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sql_close;
    }

    printf("sql is [%s]\n", urlCountSql);
    rc = sqlite3_exec(db, urlCountSql, countCallback, "url", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sql_close;
    }

    printf("sql is [%s]\n", attackCountSql);
    rc = sqlite3_exec(db, attackCountSql, countCallback, "attack", &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sql_close;
    }
    
    /* clean main log table */
    printf("clean main_log table\n");
    rc = sqlite3_exec(db, cleanSql, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sql_close;
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
    		" %u, %hu, %hu, %u, %u, %u, %u, %u, %u, %u)", 1, time(0),
    		996941108, 80, 3232235777, 8080, 100, 1, 1, 1, 1, 1, 1, 1);
    
    printf("insertSql is [%s]\n", insertSql);
    rc = sqlite3_exec(db, insertSql, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        goto sql_close;
    }
    
    /* select * from main_log */
    clock_t start = clock();
    int loop;
    for (loop = 0; loop < Z_LOOP; loop++) {
    	rc = sqlite3_exec(db, selectMainLogSql, selectCallback, 0, &zErrMsg);
    	if( rc != SQLITE_OK ){
    		fprintf(stderr, "SQL error: %s\n", zErrMsg);
    		sqlite3_free(zErrMsg);
    		goto sql_close;
    	}
    }
    clock_t end = clock();
    printf("select spends %.2f seconds\n", ((double)(end - start)) / (double)CLOCKS_PER_SEC);
    
sql_close:    sqlite3_close(db);

    char tmpStr[256];
    bzero(tmpStr, 256);

    int urlLen, randNum, i, j;
    for (j = 0; j < URL_INSERT_LOOP; j++) {
        urlLen = rand() % URL_PATH_COUNT + 1;
        for (i = 0; i < urlLen; i++) {
            randNum = rand() % URL_PATH_COUNT;
            sprintf(tmpStr, "%s/%s", tmpStr, pathStr[randNum]);
        }
        randNum = rand() % URL_EXT_COUNT;
        sprintf(tmpStr, "%s/%s", tmpStr, extStr[randNum]);

        printf("%s\n", tmpStr);
        bzero(tmpStr, 256);
    }

    return 0;
}
