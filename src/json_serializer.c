#include "json_serializer.h"
#include <stdio.h>
#include <string.h>


 // SAFE_APPEND macro to handle snprintf buffer management.

#define SAFE_APPEND(...) { \
    int ret = snprintf(ptr, rem, __VA_ARGS__); \
    if (ret < 0 || (size_t)ret >= rem) return SERIALIZER_BUFFER_TOO_SMALL; \
    ptr += ret; \
    rem -= ret; \
}

SerializerStatus serialize_to_json(const GatewayMetadata *input_data, 
                                   char *output_buffer, 
                                   size_t buffer_size) 
{
    if (input_data == NULL || output_buffer == NULL || buffer_size == 0) {
        return SERIALIZER_INVALID_INPUT;
    }

    char *ptr = output_buffer;
    size_t rem = buffer_size;

    // Start of the mandatory outer array 
    SAFE_APPEND("[\n");
    SAFE_APPEND(" {\n");

    // Gateway metadata 
    SAFE_APPEND("  \"gatewayId\": \"%s\",\n", input_data->gatewayId);
    SAFE_APPEND("  \"date\": \"%s\",\n", input_data->date);
    SAFE_APPEND("  \"deviceType\": \"%s\",\n", input_data->deviceType);
    SAFE_APPEND("  \"interval_minutes\": %d,\n", input_data->interval_minutes);
    SAFE_APPEND("  \"total_readings\": %d,\n", input_data->total_readings);

    // Values object  
    SAFE_APPEND("  \"values\": {\n");
    SAFE_APPEND("   \"device_count\": %d,\n", input_data->values.device_count);
    SAFE_APPEND("   \"readings\": [\n");

    //Device readings 
    for (int i = 0; i < input_data->values.device_count; i++) {
        const DeviceReading *reading = &input_data->values.readings[i];

        SAFE_APPEND("    {\n");
        SAFE_APPEND("     \"media\": \"%s\",\n", reading->media);
        SAFE_APPEND("     \"meter\": \"%s\",\n", reading->meter);
        SAFE_APPEND("     \"deviceId\": \"%s\",\n", reading->deviceId);
        SAFE_APPEND("     \"unit\": \"%s\",\n", reading->unit);

        //Data points 
        SAFE_APPEND("     \"data\": [\n");

        for (int j = 0; j < reading->data_count; j++) {
            const DataPoint *dp = &reading->data[j];

            SAFE_APPEND("      {\n");
            SAFE_APPEND("       \"timestamp\": \"%s\",\n", dp->timestamp);
            SAFE_APPEND("       \"meter_datetime\": \"%s\",\n", dp->meter_datetime);
            SAFE_APPEND("       \"total_m3\": %.3f,\n", dp->total_m3);        // Numbers NOT serialized as strings
            SAFE_APPEND("       \"status\": \"%s\"\n", dp->status);
            SAFE_APPEND("      }");

            if (j < reading->data_count - 1) {
                SAFE_APPEND(",");
            }
            SAFE_APPEND("\n");
        }

        SAFE_APPEND("     ]\n");
        SAFE_APPEND("    }");

        if (i < input_data->values.device_count - 1) {
            SAFE_APPEND(",");
        }
        SAFE_APPEND("\n");
    }

    // Close all objects 
    SAFE_APPEND("   ]\n");
    SAFE_APPEND("  }\n");
    SAFE_APPEND(" }\n");
    SAFE_APPEND("]\n");

    return SERIALIZER_OK;

}
