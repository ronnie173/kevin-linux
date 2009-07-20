#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "json.h"
#include "json_helper.h"
 
#define BUFFER_SIZE 1024

int streamParse(void);
int basicTest();
json_t *new_entry(char *name, char *phone);
int entryTest(void);
int treeTest1(void);

int main (void) {
    //basicTest();
    //entryTest();
    treeTest1();
    streamParse();
    
    return EXIT_SUCCESS;
}

int basicTest() {
    printf("%s", "====== basic JSon Test Starts =======\n");
    
    char *text;
    json_t *root, *entry, *label, *value;
    setlocale (LC_ALL, "");

    // creates the root node
    root = json_new_object();

    // create an entry node
    entry = json_new_object();

    // insert the first label-value pair
    label = json_new_string("name");
    value = json_new_string("Andew");
    json_insert_child(label, value);
    json_insert_child(entry, label);

    // insert the second label-value pair
    label = json_new_string("phone");
    value = json_new_string("555 123 456");
    json_insert_child(label, value);
    json_insert_child(entry, label);

    // inserts that object as a value in a label-value pair
    label = json_new_string("entry");
    json_insert_child(label, entry);

    // inserts that label-value pair into the root object
    json_insert_child(root, label);

    // print the result
    json_tree_to_string(root, &text);
    printf("%s\n",text);

    // clean up
    free(text);
    json_free_value(&root);

    printf("%s", "====== basic JSon Test Ends =======\n");

    return EXIT_SUCCESS;
}

json_t *new_entry(char *name, char *phone) {
    json_t *entry, *label, *value;

    // create an entry node
    entry = json_new_object();
    
    // insert the first label-value pair
    label = json_new_string("name");
    value = json_new_string("Andew");
    json_insert_child(label, value);
    json_insert_child(entry, label);
    
    // insert the second label-value pair
    label = json_new_string("phone");
    value = json_new_string("555 123 456");
    json_insert_child(label, value);
    json_insert_child(entry, label);

    // inserts that object as a value in a label-value pair
    label = json_new_string("entry");
    json_insert_child(label, entry);

    return label;
}

int entryTest(void) {
    printf("%s", "====== Entry Test Starts =======\n");

    setlocale (LC_ALL, "");

    json_t *root, *subtree;

    // creates the root node
    root = json_new_object();

    // creates the desired MJSON document subtree
    subtree = new_entry("Andrew", "555 123 456");

    // inserts the subtree into the root object
    json_insert_child(root, subtree);

    // print the result
    char *text;
    json_tree_to_string(root, &text);
    printf("%s\n", text);

    // clean up
    free(text);
    json_free_value(&root);

    printf("%s", "====== Entry Test Ends =======\n");

    return EXIT_SUCCESS;
}

int treeTest1(void) {
    int err;
    
    printf("%s", "====== Tree Test 1 Starts =======\n");

    setlocale (LC_ALL, "");
    char *document = "{\"entry\":{\"name\":\"Andew\",\"phone\":\"555 123 456\"}}";

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
    
    printf("%s", "====== Tree Test 1 Ends =======\n");

    return EXIT_SUCCESS;
}

int streamParse(void) {
    char buffer[BUFFER_SIZE];
    char *temp = NULL;
    unsigned int error = JSON_INCOMPLETE_DOCUMENT;
 
    struct json_parsing_info state;
 
    json_jpi_init(&state);
 
    while ((error == JSON_WAITING_FOR_EOF) || (error == JSON_INCOMPLETE_DOCUMENT)) {
        if(fgets (buffer, BUFFER_SIZE, stdin) != NULL) {
            switch(error = json_parse_fragment( &state, buffer)) {
                case JSON_OK:
                    printf("complete\n");
                    json_tree_to_string(state.cursor, &temp);
                    printf("%s\n",temp);
                    break;
 
                case JSON_WAITING_FOR_EOF:
                case JSON_INCOMPLETE_DOCUMENT:
                    break;
 
                default:
                    printf("Some error occurred: %d\n", error);
            }
        } else {
            if(error == JSON_WAITING_FOR_EOF)
                error = JSON_OK;
            else
                error = JSON_UNKNOWN_PROBLEM;
        }
    }

    if(error == JSON_OK) {
        json_render_tree(state.cursor);
    } else {
        printf("Document wasn't valid.\n");
    }
    /* perform cleanup */
    json_free_value(&state.cursor);
    
    return 0;
}
