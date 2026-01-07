#ifndef DATA_MODELS_H
#define DATA_MODELS_H

#include <stdint.h>

// Constants for fixed-size arrays to ensure controlled memory usage 
#define MAX_STR_LEN         64
#define MAX_DATE_LEN        11
#define MAX_TIMESTAMP_LEN   17
#define MAX_READINGS        5
#define MAX_DATA_POINTS     5


 // Data point structure 
 
typedef struct {
    char timestamp[MAX_TIMESTAMP_LEN];       // "timestamp"
    char meter_datetime[MAX_TIMESTAMP_LEN];  // "meter_datetime"
    double total_m3;                         // "total_m3" 
    char status[MAX_STR_LEN];                // "status"
} DataPoint;


  //Device reading structure
 
typedef struct {
    char media[MAX_STR_LEN];                 // "media"
    char meter[MAX_STR_LEN];                 // "meter"
    char deviceId[MAX_STR_LEN];              // "deviceId"
    char unit[MAX_STR_LEN];                  // "unit"
    DataPoint data[MAX_DATA_POINTS];         // List of data points
    uint8_t data_count;
} DeviceReading;


  //Gateway-level metadata
 
typedef struct {
    char gatewayId[MAX_STR_LEN];             // "gatewayId"
    char date[MAX_DATE_LEN];                 // "date" (YYYY-MM-DD)
    char deviceType[MAX_STR_LEN];            // "deviceType"
    int interval_minutes;                    // "interval_minutes"
    int total_readings;                      // "total_readings"
    
    struct {
        int device_count;                    // "device_count"
        DeviceReading readings[MAX_READINGS];
    } values;                                // Nested "values" object     
} GatewayMetadata;


#endif 
