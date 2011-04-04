#ifndef insertTest_h
#define insertTest_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <strings.h>

#include <sqlite3.h>

#define Z_LOOP 1
//#define Z_LOOP 1 * 1000 * 100
#define URL_INSERT_LOOP 1 * 1000
//#define URL_INSERT_LOOP 1 * 1000 * 100
#define URL_PATH_COUNT 20
#define URL_EXT_COUNT 5
#define IF_LOG 0

char *pathStr[URL_PATH_COUNT] = {
	"test", "hello", "kevin", "fortinet", "eclipse",
	"help", "windows", "file", "edit", "source",
	"refactor", "navigate", "search", "run", "project",
	"fatal", "error", "warning", "home", "oracle"
};

char *extStr[URL_EXT_COUNT] = {
    "check.asp", "input.php", "out.jsp", "delete.do", "update.action"
};

int continentCount = 0;
int countryCount = 0;
int stateCount = 0;
int cityCount = 0;
int hostCount = 0;
int urlCount = 0;
int attackCount = 0;
unsigned int selectID = 0;

char ipBuf[64];
char timeBuf[128];

sqlite3 *db;
char *zErrMsg = 0;

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
char *selectIDSql = "select id from %s where %s = '%s'";
char *insertSql = "insert into %s values(NULL, '%s')";
char *cleanUrlSql = "delete from url_mapping";

static int getTableMax();
static int testMainLog();
static int testUrlInsert();
static int convertInt2IP(unsigned int ipAddress, char *buf);
static int countCallback(void *table, int argc, char **argv, char **azColName);
static int selectCallback(void *NotUsed, int argc, char **argv, char **azColName);

#ifdef __cplusplus
}
#endif

#endif
