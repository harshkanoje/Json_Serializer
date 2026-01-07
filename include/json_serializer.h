#ifndef JSON_SERIALIZER_H
#define JSON_SERIALIZER_H

#include "data_models.h"
#include <stddef.h>

/**
 * Robust error handling statuses as required by Section 5 [4].
 */
typedef enum {
    SERIALIZER_OK = 0,
    SERIALIZER_BUFFER_TOO_SMALL,
    SERIALIZER_INVALID_INPUT,
    SERIALIZER_ERROR
} SerializerStatus;

/**
 * Public API for JSON Serialization [4, 5].
 * 
 * @param input_data Pointer to populated GatewayMetadata.
 * @param output_buffer Pre-allocated buffer to store the JSON string.
 * @param buffer_size Size of the output_buffer to prevent overflows.
 * @return SerializerStatus indicating success or specific error.
 */
SerializerStatus serialize_to_json(const GatewayMetadata *input_data, 
                                   char *output_buffer, 
                                   size_t buffer_size);

#endif // JSON_SERIALIZER_H