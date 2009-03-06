/*
Two ways for cookie expiration: "Expires" and "Max-Age".
No major browsers such as IE, Firefox support "Max-Age" so we only need to consider "Expires" for now.

The expiration date is specified in the "Wdy, DD-Mon-YYYY HH:MM:SS GMT" format (Greenwich Mean Time (GMT) format)

When FortiWeb sees "Set-Cookie" in HTTP response header

1. Convert "expires" datetime GMT string into a struct tm
2. Convert a struct tm into a time_t which is a "long int"
    In "/usr/include/bits/types.h"
    #define __SLONGWORD_TYPE long int

    In "/usr/include/bits/typesizes.h"
    #define __TIME_T_TYPE __SLONGWORD_TYPE

    In "/user/include/bits/types.h"
    #define __STD_TYPE __extension__ typedef
    __STD_TYPE __TIME_T_TYPE __time_t;

    In "/usr/include/time.h"
    typedef __time_t time_t;
3. Convert a time_t into an 8 bytes HEX string
4. Store the time HEX string into FortiWeb cookie

When FortiWeb sees "Cookie:" in HTTP request header
1. Extract the time HEX string from FortiWeb cookie
2. Convert the HEX string into a time_t
3. Compare the time_t with current system time

 */
#include <stdio.h>
#include <time.h>
#include <string.h>
 
int getMonth(const char *strMonth) {
    int i;
    static char *months[] = { 
        "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    for (i = 0; i < 12; ++i) {
        if (!strncmp(strMonth, months[i], 3)) return i;
    }
    return -1;
}
 
struct tm strToTm(const char *strDate) {
    struct tm tmDate = { 0 };
    int year = 0;
    char strMonth[4] = { 0 };
 
    sscanf(strDate, "%*[^,], %d %3s %d %d:%d:%d %*s", &(tmDate.tm_mday), 
        strMonth, &year, &(tmDate.tm_hour), &(tmDate.tm_min), &(tmDate.tm_sec));
    tmDate.tm_mon = getMonth(strMonth);
    tmDate.tm_year = year - 1900;
    tmDate.tm_isdst = -1;
    return tmDate;
}
 
int main(void) {
    char expires[] = "Sat, 13 Nov 2008 23:29:00 GMT";
    struct tm tmDate1, tmDate2;
    char strDate[64] = { 0 };
    char strTimeT[9] = { 0 };
    time_t tmpTime1, tmpTime2, now;
    double diff = 0;
    
    printf("Original cookie expires string is [%s]\n", expires);
    
    // Convert GMT time string to a struct tm"
    tmDate1 = strToTm(expires);
    strftime(strDate, 64, "%d/%m/%Y %I:%M:%S %p", &tmDate1);
    printf("struct tm tmDate1 string  is [%s]\n", strDate);
 
    // Convert struct tm to time_t
    tmpTime1 = mktime(&tmDate1);
    if (-1 == tmpTime1) {
        printf("Error getting time_t from struct tm.\n");
        return(0);
    }
    printf("time_t tmpTime1 str is [%s]\n", ctime(&tmpTime1));
    
    // Convert time_t to HEX string
    sprintf(strTimeT, "%08lX", tmpTime1);
    printf("time_t tmpTime1 HEX string is [%s]\n", strTimeT);
    //printf("%08lX\n", 1L);
    
    // Convert HEX string to time_t
    sscanf(strTimeT, "%081X", (unsigned int *)&tmpTime2);
    printf("Convert time_t tmpTime2 HEX string is [%08lX]\n", tmpTime2);
    printf("time_t tmpTime2 ctime str is [%s]\n", ctime(&tmpTime2));
    
    // Convert time_t to struct tm
    localtime_r(&tmpTime2, &tmDate2);
    strftime(strDate, 64, "%d/%m/%Y %I:%M:%S %p", &tmDate2);
    printf("struct tm tmDate2 string is [%s]\n", strDate);
    
    // get current system time
    now = time(NULL);
    localtime_r(&now, &tmDate2);
    strftime(strDate, 64, "%d/%m/%Y %I:%M:%S %p", &tmDate2);
    printf("struct tm tmDate2 string is [%s]\n", strDate);
    printf("time_t now ctime str is [%s]\n", ctime(&now));

    // compare the current system time with the expires time, diff = time1 - time2
    diff = difftime(now, tmpTime2);
    printf("time diff is [%f]\n", diff);
    
    if (diff < 0) {
        printf("the time is good.\n");
    } else {
        printf("the time is expired.\n");
    }
    
    return 0;
}