#include <stdio.h>
#include <string.h>
#include "json_serializer.h"

//Placeholder for an STM32 UART transmit function. 

void uart_log(const char* msg) {
    printf("%s\n", msg); // For simulation/testing purposes
}

int main(void) {
    //  sample input data strictly using variables from pdf given 
    GatewayMetadata myGateway = {
        .gatewayId = "gateway_1234",
        .date = "1970-01-01",
        .deviceType = "stromleser",
        .interval_minutes = 15,
        .total_readings = 1,
        .values.device_count = 1
    };

    // first device reading
    DeviceReading *reading = myGateway.values.readings;
    strcpy(reading->media, "water");
    strcpy(reading->meter, "waterstarm");
    strcpy(reading->deviceId, "stromleser_50898527");
    strcpy(reading->unit, "m3");
    reading->data_count = 1;

    // first data point
    DataPoint *dp = reading->data;
    strcpy(dp->timestamp, "1970-01-01 22:00");
    strcpy(dp->meter_datetime, "1970-01-01 00:00");
    dp->total_m3 = 107.752; // Numbers must not be strings
    strcpy(dp->status, "OK");

    // Preparing caller-provided output buffer 
    //  maximum possible JSON size 

    char json_buffer[1024]; 
    memset(json_buffer, 0, sizeof(json_buffer));   // set all char of Buffer to zero

    // Calling serialization library
    SerializerStatus status = serialize_to_json(&myGateway, json_buffer, sizeof(json_buffer));

    // Handle errors and output results 
    if (status == SERIALIZER_OK) {
        uart_log("Serialization Successful:");
        uart_log(json_buffer);
    } else if (status == SERIALIZER_BUFFER_TOO_SMALL) {
        uart_log("Error: Buffer provided was too small for the JSON data.");
    } else {
        uart_log("Error: An unknown serialization error occurred.");
    }

    return 0;

}
