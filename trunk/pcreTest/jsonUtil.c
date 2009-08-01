#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <strings.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "json.h"
//#include "json_helper.h"
#include "jsonUtil.h"

int main (void) {
    loadJSonFile("./param_list.json");

    return 0;
}

int loadJSonFile(char* fn) {
    int err;

    printf("%s", "====== Tree Test 1 Starts =======\n");

    setlocale (LC_ALL, "");
    //char *document = "{\"entry\":{\"name\":\"Andew\",\"phone\":\"555 123 456\"}}";

    int fd = open(fn, O_RDONLY);
    if (-1 == fd) {
        printf("open file failed\n");
        return 1;
    }

    struct stat fs;
    if (fstat(fd, &fs)) {
        printf("failed to get file stat\n");
        close(fd);
        return 1;
    }

    size_t fileLen = (size_t)fs.st_size;
    char *document = malloc(fileLen);
    bzero(document, fileLen);

    size_t count = read(fd, document, fileLen);
    if (count != fileLen) {
        printf("failed to read file\n");
        free(document);
        close(fd);

        return 1;
    }

    json_t *root = NULL;
    //json_t *root = json_new_object();
    if (NULL == root) {
        printf("%s\n", "new json object failed");
    }
    printf("Parsing the document...\n");
    err = json_parse_document(&root, document);
    if (err != JSON_OK) {
        printf("%s, err=%d\n", "parse error", err);
        goto cleanup;
    }
    printf("Printing the document tree...\n");
    json_tree_to_string(root, &document);
    printf("%s\n", document);

    // clean up
cleanup:
    json_free_value(&root);
    free(document);
    close(fd);
    printf("%s", "====== Tree Test 1 Ends =======\n");

    return 0;
}
