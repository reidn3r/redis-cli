<h2 align="center"> Redis CLI Implementation in C </h2>
<p align="center"> Command Line Interface for Redis Database with Native TCP Connection (05/2025)</p>
<hr>

### Architecture:
The Redis CLI implementation follows a layered network architecture where communication occurs through multiple protocol layers. The application establishes a reliable connection with the Redis server using TCP sockets and implements the RESP3 protocol for data exchange.

## Network Communication Details:

### 1.0: Protocol Handshakes and Layer Communication
``
    1.1: TCP Three-Way Handshake occurs at the Transport Layer (Layer 4) of the OSI model. This handshake establishes a reliable, connection-oriented communication channel between the client and Redis server, ensuring data integrity and proper sequencing of packets.
    1.2: Redis Application Handshake operates at the Application Layer (Layer 7). After the TCP connection is established, the client negotiates protocol version, performs authentication if required, and selects the target database. This handshake ensures proper application-level communication and security.
``

### 2.0: Socket Implementation and Database Connection
``
    2.1: The application utilizes POSIX sockets as the fundamental communication mechanism. A socket serves as an endpoint for network communication, providing a programming interface to access TCP/IP networking capabilities.
``


### 3.0: RESP3 Data Serialization and Deserialization
``
    3.1: Redis Serialization Protocol version 3 (RESP3) defines how data is formatted for transmission between client and server. All Redis commands and responses are converted to standardized string formats that can be transmitted over the network.
``

``
    3.2: Serialization Process (Future Steps): Client commands are transformed into RESP3 format before transmission. The serializer converts command structures into properly formatted strings with specific prefixes, length indicators, and terminators that Redis can interpret.
``

``
    3.3: Deserialization Process: Server responses in RESP3 format are parsed and converted back into usable data structures. The deserializer identifies data types based on prefix characters and reconstructs the original data for presentation to the user.
``

``
    3.4: The protocol supports multiple data types including simple strings, errors, integers, bulk strings, and arrays. Each type has distinct formatting rules that ensure proper interpretation by both client and server.
``

## Core Features:
``
The CLI aim to provide a complete interface for Redis database operations with native C implementation. The application supports interactive command execution, connection management, and comprehensive error handling. All Redis commands are supported through the standardized protocol implementation.
``


## Usage:
```bash
# Compilation
$ make clean && make all

# Basic connection
$ ./out/redis-cli -h <REDIS_HOST> -p <REDIS_PORT>
```
