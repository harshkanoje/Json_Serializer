## Overview 

This project involves the design and implementation of an embedded-friendly software library specifically built to serialize structured meter measurement data into a predefined JSON format. It serves as a critical component in a smart-meter / w-M-Bus data pipeline, bridging the gap between internal data structures and gateway/backend interfaces.

The library is developed with a focus on clean software architecture and predictable serialization behavior for embedded environments.

### Key Objectives
• **Structured Serialization**: Convert complex gateway and measurement metadata (including IDs, timestamps, and meter values) into a strictly formatted JSON string. 

• **Embedded Suitability**: The implementation prioritizes controlled memory usage and error handling (`snprintf ` and `SAFE_APPEND` macros) without relying on any external JSON libraries.

• **Transport-Agnostic Design**: The core logic is entirely independent of hardware communication protocols like UART, MQTT, or Radio, making it highly portable across different embedded platforms.


## Chosen Platform and Programming Language

### Programming Language: C

The project is implemented in C, as it is the language I am most familiar with compared to the other available options.
it provides direct control over memory, which is important for embedded and firmware-oriented applications.

Using C made it possible to:

• Work with fixed-size data structures, which helps in calculating the required output buffer size  
• Detect and prevent buffer overflows more easily  
• Avoid dynamic memory allocation  

Special care was taken to make the implementation as safe as possible, especially with respect to memory usage and string handling. All JSON serialization is performed using bounded operations to ensure deterministic and secure behavior.

### IDE : VS Code

For development, I chose VS Code beacause it provides a clean and lightweight development environment without enforcing any platform- or framework-specific structure. This allowed the project to maintain a clear and portable folder architecture and kept the focus on the core serialization logic rather than IDE-specific tooling. Using VS Code also supports a transport-agnostic design approach, as the code is not tightly coupled to any particular embedded framework and can be integrated into different environments without modification.

## Build and Run Instructions
This project can be built and executed on a standard desktop system for testing and validation

### Build 
From the root directory of the project, compile the source files using the following command:

```bash
gcc -Iinclude src/json_serializer.c examples/test.c -o json_test1
```
This command:

• Includes the public header files from the `include/` directory  
• Compiles the JSON serialization library  
• Compiles the example application  
• Produces an executable named `json_test1`  

### Run Instructions

After successful compilation, run the executable:

```bash
./json_test1
```

### Expected Output

```bash
Serialization Successful:
[
 {
  "gatewayId": "gateway_1234",
  "date": "1970-01-01",
  "deviceType": "stromleser",
  "interval_minutes": 15,
  "total_readings": 1,
  "values": {
   "device_count": 1,
   "readings": [
    {
     "media": "water",
     "meter": "waterstarm",
     "deviceId": "stromleser_50898527",
     "unit": "m3",
     "data": [
      {
       "timestamp": "2026-01-06 22:00",
       "meter_datetime": "2026-01-06 22:00",
       "total_m3": 107.752,
       "status": "OK"
      }
     ]
    }
   ]
  }
 }
]
```

### Note

• The example application (`examples/main.c`) simulates output using standard console printing.
• In a real embedded environment, the output function can be replaced with a hardware-specific interface (e.g., UART) without modifying the serialization library.
• No external JSON libraries or platform-specific dependencies are required.

## Description of the Public API

• The public API of the library is exposed through the header file `json_serializer.h`.  
• It provides a single function, `serialize_to_json`, which converts structured gateway and meter data into a JSON string that strictly follows   the format defined in the assignment specification.  
• The function writes the serialized JSON into a caller-provided output buffer.    
• The buffer size is explicitly calculated and passed to prevent buffer overflows, and no dynamic memory allocation is used.  
• The function returns a status code to indicate successful serialization, invalid input parameters, or insufficient buffer size.  
• All write operations are performed using bounded functions to ensure safe and deterministic behavior.  
• This minimal API design keeps the library easy to use, portable, and suitable for embedded firmware integration.  

## Design Decisions and Assumptions

The sizes defined in `data_models.h` were selected to match the maximum expected length of each JSON field and to allow an explicit estimation of the serialized output size. All fields map directly to the mandatory JSON structure specified in the assignment.

For the provided example application (main.c), the implementation assumes one device reading containing one data point. Based on the defined data structures and JSON syntax, the expected output size is approximately:
```
Gateway metadata and JSON structural characters: ~300 bytes  
One device reading object: ~316 bytes  
One data point object: ~160 bytes  
```
This results in a total JSON size of approximately **780 bytes**. Consequently, the 1024-byte output buffer used in the example application is sufficient for this configuration and includes additional margin.

It is assumed that applications using the full capacity of the data model (up to five device readings with five data points each) will provide a larger output buffer. If the provided buffer is insufficient, the serializer reports this condition via an error code and does not produce partial or invalid output.

