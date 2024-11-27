/*
Grupo:
- Diego Cardoso Nunes (00333984)
- Geancarlo Kozenieski (00264414)
*/
#pragma once

typedef enum {
    DATA_UNDEFINED,
    DATA_INT,
    DATA_FLOAT,
} data_type_t;

data_type_t data_type_infer(data_type_t first, data_type_t second);

char *type_to_str(data_type_t type);