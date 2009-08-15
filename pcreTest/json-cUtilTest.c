/*
 *
 *
 */

#include <stdio.h>

#include "json-cUtil.h"

int main (void) {
    nameValuePair_t paramList[MAX_PAIR_ARRAY_LEN];

    loadParamList("./param_list.json", paramList, MAX_PAIR_ARRAY_LEN);
    dumpParamList(paramList, MAX_PAIR_ARRAY_LEN);

    return 0;
}
