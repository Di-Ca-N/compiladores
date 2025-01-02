/*
Grupo:
- Diego Cardoso Nunes (00333984)
- Geancarlo Kozenieski (00264414)
*/

#include "data_types.h"

char *type_to_str(data_type_t type) {
    switch (type) {
        case DATA_UNDEFINED:
            return "undefined";

        case DATA_INT:
            return "int";

        case DATA_FLOAT:
            return "float";
        
        default:
            return "UNKNOWN";
    }
}

data_type_t data_type_infer(data_type_t first, data_type_t second){
    if (first == DATA_UNDEFINED || second == DATA_UNDEFINED) return DATA_UNDEFINED;
    if (first == DATA_FLOAT || second == DATA_FLOAT) return DATA_FLOAT;
    return DATA_INT;
}

int get_data_type_size(data_type_t type) { // For now all our data types have 4 bytes.
    return 4; 
}
