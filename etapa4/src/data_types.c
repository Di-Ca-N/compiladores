#include "data_types.h"

char *type_to_str(data_type_t type) {
    switch (type) {
        case DATA_INT:
            return "int";

        case DATA_FLOAT:
            return "float";
        
        default:
            return "UNKNOWN";
    }
}

data_type_t data_type_infer(data_type_t first, data_type_t second){
    if(first == DATA_FLOAT || second == DATA_FLOAT) return DATA_FLOAT;
    return DATA_INT;
}