//
// webserver.c
//
// Simple HTTP server sample for sanos
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define SERVER "webserver/1.0"
#define PROTOCOL "HTTP/1.0"
#define RFC1123FMT "%a, %d %b %Y %H:%M:%S GMT"
#define PORT 8111

char *path;

ssize_t writeline(int sockd, const void *vptr, size_t n) {
    size_t      nleft;
    ssize_t     nwritten;
    const char *buffer;

    buffer = vptr;
    nleft  = n;

    while ( nleft > 0 ) {
        if ( (nwritten = write(sockd, buffer, nleft)) <= 0 ) {
            if ( errno == EINTR ) {
                nwritten = 0;
            } else {
                printf("Error in writeline()\n");
            }
        }
        nleft  -= nwritten;
        buffer += nwritten;
    }

    return n;
}

char *get_mime_type(char *name) {
    char *ext = strrchr(name, '.');
    if (!ext)
        return NULL;
    if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0)
        return "text/html";
    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0)
        return "image/jpeg";
    if (strcmp(ext, ".gif") == 0)
        return "image/gif";
    if (strcmp(ext, ".png") == 0)
        return "image/png";
    if (strcmp(ext, ".css") == 0)
        return "text/css";
    if (strcmp(ext, ".au") == 0)
        return "audio/basic";
    if (strcmp(ext, ".wav") == 0)
        return "audio/wav";
    if (strcmp(ext, ".avi") == 0)
        return "video/x-msvideo";
    if (strcmp(ext, ".mpeg") == 0 || strcmp(ext, ".mpg") == 0)
        return "video/mpeg";
    if (strcmp(ext, ".mp3") == 0)
        return "audio/mpeg";
    return NULL;
}

void send_headers(FILE *f, int status, char *title, char *extra, 
        char *mime, int length, time_t date) {
    printf("get into send_headers\n");
    
    time_t now;
    char timebuf[128];

    fprintf(f, "%s %d %s\r\n", PROTOCOL, status, title);
    fprintf(f, "Server: %s\r\n", SERVER);
    now = time(NULL);
    strftime(timebuf, sizeof(timebuf), RFC1123FMT, gmtime(&now));
    fprintf(f, "Date: %s\r\n", timebuf);
    if (extra)
        fprintf(f, "%s\r\n", extra);
    if (mime)
        fprintf(f, "Content-Type: %s\r\n", mime);
    if (length >= 0)
        fprintf(f, "Content-Length: %d\r\n", length);
    if (date != -1) {
        strftime(timebuf, sizeof(timebuf), RFC1123FMT, gmtime(&date));
        fprintf(f, "Last-Modified: %s\r\n", timebuf);
    }
    fprintf(f, "Connection: close\r\n");
    fprintf(f, "\r\n");
}

void send_error(FILE *f, int status, char *title, char *extra, char *text) {
    send_headers(f, status, title, extra, "text/html", -1, -1);
    fprintf(f, "<HTML><HEAD><TITLE>%d %s</TITLE></HEAD>\r\n", status, title);
    fprintf(f, "<BODY><H4>%d %s</H4>\r\n", status, title);
    fprintf(f, "%s\r\n", text);
    fprintf(f, "</BODY></HTML>\r\n");
}

void send_file(FILE *f, char *path, struct stat *statbuf) {
    printf("get into send_file\n");
    
    char data[4096];
    int n;

    FILE *file = fopen(path, "r");
    if (!file) {
        printf("can not open the file\n");
        send_error(f, 403, "Forbidden", NULL, "Access denied.");
    } else {
        int length = S_ISREG(statbuf->st_mode) ? statbuf->st_size : -1;
        send_headers(f, 200, "OK", NULL, get_mime_type(path), length,
                statbuf->st_mtime);

        while ((n = fread(data, 1, sizeof(data), file)) > 0)
            fwrite(data, 1, n, f);
        fclose(file);
    }
}

/*
 * -1, method is not supported
 * -2, file not found
 * -3, dir not found
 */
int process_req(FILE *f) {
    char buf[4096];
    char *method;
    char *protocol;
    struct stat statbuf;
    //char pathbuf[4096];
    //int len;

    if (!fgets(buf, sizeof(buf), f))
        return -1;
    printf("URL: %s", buf);

    method = strtok(buf, " ");
    path = strtok(NULL, " ");
    protocol = strtok(NULL, "\r");

    printf("method=[%s]\n", method);
    printf("path=[%s]\n", path);
    printf("protocol=[%s]\n", protocol);

    if (!method || !path || !protocol) {
        printf("failed to parse method, path and protocol\n");
        return -1;
    }

    if (strcmp(method, "GET") != 0) {
        printf("method [%s] is NOT supported\n", method);
        return -1;
    } else if (stat(path, &statbuf) < 0) {
        printf("File %s is NOT found\n", path);
        return -2;
    } else if (S_ISDIR(statbuf.st_mode)) {
        printf("dir listing is NOT supported\n");
        return -3;
        /*len = strlen(path);
        if (len == 0 || path[len - 1] != '/') {
            snprintf(pathbuf, sizeof(pathbuf), "Location: %s/", path);
            send_error(f, 302, "Found", pathbuf,
                    "Directories must end with a slash.");
        } else {
            snprintf(pathbuf, sizeof(pathbuf), "%sindex.html", path);
            if (stat(pathbuf, &statbuf) >= 0)
                send_file(f, pathbuf, &statbuf);
            else {
                DIR *dir;
                struct dirent *de;

                send_headers(f, 200, "OK", NULL, "text/html", -1,
                        statbuf.st_mtime);
                fprintf(
                        f,
                        "<HTML><HEAD><TITLE>Index of %s</TITLE></HEAD>\r\n<BODY>",
                        path);
                fprintf(f, "<H4>Index of %s</H4>\r\n<PRE>\n", path);
                fprintf(f, "Name Last Modified Size\r\n");
                fprintf(f, "<HR>\r\n");
                if (len > 1)
                    fprintf(f, "<A HREF=\"..\">..</A>\r\n");

                dir = opendir(path);
                while ((de = readdir(dir)) != NULL) {
                    char timebuf[32];
                    struct tm *tm;

                    strcpy(pathbuf, path);
                    strcat(pathbuf, de->d_name);

                    stat(pathbuf, &statbuf);
                    tm = gmtime(&statbuf.st_mtime);
                    strftime(timebuf, sizeof(timebuf), "%d-%b-%Y %H:%M:%S", tm);

                    fprintf(f, "<A HREF=\"%s%s\">", de->d_name, S_ISDIR(
                            statbuf.st_mode) ? "/" : "");
                    fprintf(f, "%s%s", de->d_name,
                            S_ISDIR(statbuf.st_mode) ? "/</A>" : "</A> ");
                    if (strlen(de->d_name) < 32)
                        fprintf(f, "%*s", 32 - strlen(de->d_name), "");

                    if (S_ISDIR(statbuf.st_mode))
                        fprintf(f, "%s\r\n", timebuf);
                    else
                        fprintf(f, "%s %10d\r\n", timebuf, (int)statbuf.st_size);
                }
                closedir(dir);

                fprintf(
                        f,
                        "</PRE>\r\n<HR>\r\n<ADDRESS>%s</ADDRESS>\r\n</BODY></HTML>\r\n",
                        SERVER);
            }
        }*/
    }
    
    return 0;
}

int process_res(FILE *f) {
    struct stat statbuf;

    send_file(f, path, &statbuf);

    return 0;
}

int main(int argc, char *argv[]) {
    int sock, ret;
    struct sockaddr_in sin;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(PORT);
    bind(sock, (struct sockaddr *) &sin, sizeof(sin));

    listen(sock, 5);
    printf("HTTP server listening on port %d at %s\n", PORT, inet_ntoa(
            sin.sin_addr));

    while (1) {
        int s;
        FILE *f;

        s = accept(sock, NULL, NULL);
        if (s < 0)
            break;

        f = fdopen(s, "r");
        ret = process_req(f);
        fclose(f);
        
        f = fdopen(s, "w");
        if (ret) {
            switch (ret) {
            case -1 :
                send_error(f, 501, "Not supported", NULL, "Method is not supported.");
            case -2 :
                send_error(f, 404, "Not Found", NULL, "File not found.");
            case -3 :
                send_error(f, 501, "Not supported", NULL, "DIR listing NOT supported.");
            default :
                printf("unknown error\n");
            }
        } else {
           process_res(f); 
        }
                
        fclose(f);
        close(s);
    }

    close(sock);
    return 0;
}
