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