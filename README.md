# Winsock Client-Server Communication (Learning Project)

## Overview
This is a minimal client-server communication prototype using Winsock in C++. It establishes a basic TCP connection, where the client sends a 'Hello' message, the server responds with a 'Hello,' and then the connection terminates.

## Why I Built This
This project was an exercise to learn the fundamentals of **TCP socket programming** and **Winsock API** in C++. It lays the foundation for more advanced networking applications.

## How It Works
1. The server starts and listens for connections on **port 5000**.
2. The client connects to the server.
3. The client sends a message: **"HELLO FROM CLIENT"**.
4. The server receives the message and responds with: **"HELLO FROM SERVER"**.
5. The connection closes after one exchange.

## Requirements
- Windows OS
- Microsoft Visual Studio (or any compiler that supports Winsock)
- Winsock2 Library (ws2_32.lib)

## How to Run
1. **Compile the Server:**
   ```sh
   g++ server.cpp -o server.exe -lws2_32
   ```
2. **Compile the Client:**
   ```sh
   g++ client.cpp -o client.exe -lws2_32
   ```
3. **Start the Server:**
   ```sh
   server.exe
   ```
4. **Start the Client:**
   ```sh
   client.exe
   ```

---

