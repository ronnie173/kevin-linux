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
#include <getopt.h>

#define SERVER "webserver/1.0"
#define PROTOCOL "HTTP/1.0"
#define RFC1123FMT "%a, %d %b %Y %H:%M:%S GMT"
#define PORT 8123
#define MAX_PATH 1024

/* Delay n seconds before send out http response body */
int bodyDelaySec = 0;

/* The port number web server will listen, default is 8123. */
int port_num = PORT;

/* Whether to display verbose messages. */
int verbose = 0;

/* Home dir path. default is current dir. */
char *home_dir = ".";

/* the full path of the request file. */
char full_path[MAX_PATH];

/*ssize_t writeline(int sockd, const void *vptr, size_t n) {
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
}*/

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
    
    if (bodyDelaySec > 0) sleep(bodyDelaySec);
    
    fprintf(f, "<HTML><HEAD><TITLE>%d %s</TITLE></HEAD>\r\n", status, title);
    fprintf(f, "<BODY><H4>%d %s</H4>\r\n", status, title);
    fprintf(f, "%s\r\n", text);
    fprintf(f, "</BODY></HTML>\r\n");
}

void send_file(FILE *f, char *path, struct stat *statbuf) {
    printf("get into send_file\n");
    
    char data[4096];
    int n;

    FILE *file = fopen(full_path, "r");
    if (!file) {
        printf("can not open the file\n");
        send_error(f, 403, "Forbidden", NULL, "Access denied.");
    } else {
        int length = S_ISREG(statbuf->st_mode) ? statbuf->st_size : -1;
        send_headers(f, 200, "OK", NULL, get_mime_type(path), length,
                statbuf->st_mtime);

        if (bodyDelaySec > 0) sleep(bodyDelaySec);

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
    char *path;
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

    bzero(full_path, MAX_PATH);
    strcat(full_path, home_dir);
    strcat(full_path, path);
    printf("the full path is [%s]\n", full_path);
    
    if (strcmp(method, "GET") != 0) {
        printf("method [%s] is NOT supported\n", method);
        return -1;
    } else if (stat(full_path, &statbuf) < 0) {
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

    send_file(f, full_path, &statbuf);

    return 0;
}

/* 
 * Prints usage information for this program to STREAM (typically
 * stdout or stderr), and exit the program with EXIT_CODE.  Does not
 * return.
 */
void print_usage(FILE* stream, int exit_code) {
    fprintf(stream, "Usage:  simple_http options [ inputfile ... ]\n");
    fprintf(stream,
            "  -h  --help             Display this usage information.\n"
            "  -b  --bodydelay        Delay n sec before send out body.\n"
            "  -p  --port             The listen port number.\n"
            "  -d  --homedir          The home directory.\n"
            "  -v  --verbose          Print verbose messages.\n");
    exit(exit_code);
}

int main(int argc, char *argv[]) {
    int next_option;

    /* A string listing valid short options letters.  */
    const char* const short_options = "hb:p:d:v";
    /* An array describing valid long options.  */
    const struct option long_options[] = { 
            { "help", no_argument, NULL, 'h' }, 
            { "bodydelay", required_argument, NULL, 'b' }, 
            { "port", required_argument, NULL, 'p' }, 
            { "homedir", required_argument, NULL, 'd' }, 
            { "verbose", no_argument, NULL, 'v' }, 
            { NULL, 0, NULL, 0 } /* Required at end of array.  */
    };

    do {
        next_option = getopt_long(argc, argv, short_options, long_options, NULL);
        switch (next_option) {
        case 'h': /* -h or --help */
            /* User has requested usage information.  Print it to standard
             output, and exit with exit code zero (normal termination).  */
            print_usage(stdout, 0);

        case 'b': /* -b or --bodydelay */
            bodyDelaySec = atoi(optarg);
            break;
       
        case 'p': /* -p or --port */
            port_num = atoi(optarg);
            break;

        case 'd': /* -d or --homedir */
            home_dir = strdup(optarg);
            break;

        case 'v': /* -v or --verbose */
            verbose = 1;
            break;

        case '?': /* The user specified an invalid option.  */
            /* Print usage information to standard error, and exit with exit
             code one (indicating abnormal termination).  */
            print_usage(stderr, 1);

        case -1: /* Done with options.  */
            break;

        default: /* Something else: unexpected.  */
            abort();
        }
    } while (next_option != -1);

    if (bodyDelaySec > 0) {
        printf("Delay %d seconds before sending out http body\n", bodyDelaySec);
    }
    
    printf("Home directory is [%s]\n", home_dir);
    
    int sock, ret, optval = 1;
    struct sockaddr_in sin;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return -1;
    }
    
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, 
            (const void *)&optval, sizeof(int)) < 0) {
        printf("Failed to set socket option\n");
        return -2;
    }
    
    bzero((char *)&sin, sizeof(sin));
    
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(port_num);
    
    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        printf("Failed to bind\n");
        return -3;
    }

    listen(sock, 5);
    printf("HTTP server listening on port %d at %s\n", port_num, inet_ntoa(
            sin.sin_addr));

    while (1) {
        int s;
        FILE *f;

        s = accept(sock, NULL, NULL);
        if (s < 0)
            break;

        f = fdopen(s, "r");
        ret = process_req(f);
        //fclose(f);
        
        f = fdopen(s, "w");
        if (ret) {
            switch (ret) {
            case -1 :
                send_error(f, 501, "Not supported", NULL, "Method is not supported.");
            break;
            
            case -2 :
                send_error(f, 404, "Not Found", NULL, "File not found.");
            break;
            
            case -3 :
                send_error(f, 501, "Not supported", NULL, "DIR listing NOT supported.");
            break;
            
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
