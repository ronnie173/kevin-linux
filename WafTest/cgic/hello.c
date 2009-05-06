#include <stdio.h>
#include "cgic.h"
#include <string.h>
#include <stdlib.h>

int cgiMain() {
	/* Send the content type, letting the browser know this is HTML */
	cgiHeaderContentType("text/html");

    fprintf(cgiOut, "<html><body><h1>hello world</h1></body></html>");

	return 0;
}

