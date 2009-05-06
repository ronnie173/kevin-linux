/* Change this if the SERVER_NAME environment variable does not report
	the true name of your web server. */
#if 1
#define SERVER_NAME cgiServerName
#endif

#define SAVED_ENVIRONMENT "/tmp/cgicsave.env"

#include <stdio.h>
#include "cgic.h"
#include <string.h>
#include <stdlib.h>

void HandleSubmit();

int cgiMain() {
    /* Send the content type, letting the browser know this is HTML */
    cgiHeaderContentType("text/html");

    fprintf(cgiOut, "<html><body><h1>hello world</h1></body></html>");

    /* Top of the page */
    fprintf(cgiOut, "<HTML><HEAD>\n");
    fprintf(cgiOut, "<TITLE>cgic test</TITLE></HEAD>\n");
    fprintf(cgiOut, "<BODY><H1>cgic test</H1>\n");
    /* If a submit button has already been clicked, act on the 
        submission of the form. */
    if (cgiFormSubmitClicked("OK") == cgiFormSuccess) {
        HandleSubmit();
        fprintf(cgiOut, "<hr>\n");
    }

    /* Finish up the page */
    fprintf(cgiOut, "</BODY></HTML>\n");

    return 0;
}

void HandleSubmit() {
    char tmpStr[255];
    size_t strLen = 0;

    cgiFormStringNoNewlines("username", tmpStr, 255);
    strLen = strlen(tmpStr);
    fprintf(cgiOut, "<p>username len is %d</p>", strLen);

    cgiFormStringNoNewlines("password", tmpStr, 255);
    strLen = strlen(tmpStr);
    fprintf(cgiOut, "<p>password len is %d</p>", strLen);

    cgiFormStringNoNewlines("address", tmpStr, 255);
    strLen = strlen(tmpStr);
    fprintf(cgiOut, "<p>address len is %d</p>", strLen);

    cgiFormStringNoNewlines("city", tmpStr, 255);
    strLen = strlen(tmpStr);
    fprintf(cgiOut, "<p>city len is %d</p>", strLen);

    cgiFormStringNoNewlines("state", tmpStr, 255);
    strLen = strlen(tmpStr);
    fprintf(cgiOut, "<p>state len is %d</p>", strLen);

    cgiFormStringNoNewlines("zipcode", tmpStr, 255);
    strLen = strlen(tmpStr);
    fprintf(cgiOut, "<p>zipcode len is %d</p>", strLen);

    cgiFormStringNoNewlines("email", tmpStr, 255);
    strLen = strlen(tmpStr);
    fprintf(cgiOut, "<p>email len is %d</p>", strLen);

    cgiFormStringNoNewlines("phone", tmpStr, 255);
    strLen = strlen(tmpStr);
    fprintf(cgiOut, "<p>phone len is %d</p>", strLen);
}


