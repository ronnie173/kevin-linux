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
#define URL_INSERT_LOOP 10
//#define URL_INSERT_LOOP 1 * 1000 * 100
#define URL_PATH_COUNT 20
#define URL_EXT_COUNT 5

char *pathStr[URL_PATH_COUNT] = {
	"test", "hello", "kevin", "fortinet", "eclipse",
	"help", "windows", "file", "edit", "source",
	"refactor", "navigate", "search", "run", "project",
	"fatal", "error", "warning", "home", "oracle"
};

char *extStr[URL_EXT_COUNT] = {
    ".asp", ".php", ".jsp", ".do", ".action"
};

int continentCount = 0;
int countryCount = 0;
int stateCount = 0;
int cityCount = 0;
int hostCount = 0;
int urlCount = 0;
int attackCount = 0;

char ipBuf[64];
char timeBuf[128];

#ifdef __cplusplus
}
#endif

#endif
