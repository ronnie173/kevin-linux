/* 
 *
 *
 */

#include <stdio.h>

#include "json.h"
#include "json_helper.h"
#include "jsonUtil.h"

int main (void) {
    nameValuePair_t paramList[MAX_PAIR_ARRAY_LEN];
    
    loadParamList("./param_list.json", paramList, MAX_PAIR_ARRAY_LEN);
    dumpParamList(paramList, MAX_PAIR_ARRAY_LEN);

    return 0;
}
