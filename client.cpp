#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma comment(lib,"ws2_32") //for clang or mvsc

#define CLIENT_PORT_NUM 5000
#define CLIENT_IP_ADDR "127.0.0.1"


#include<winsock2.h>
#include<windows.h>
#include<iostream>


int main(){
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2,2), &wsadata);

    int optval = 1;
    SOCKET client_socket_handle = socket(AF_INET,SOCK_STREAM,0);
    setsockopt(client_socket_handle,SOL_SOCKET,SO_REUSEADDR,(const char*)&optval,sizeof(optval));
    
    sockaddr_in clientSocketAddressBind = {};
    clientSocketAddressBind.sin_family = AF_INET;
    clientSocketAddressBind.sin_port = htons(CLIENT_PORT_NUM);
    clientSocketAddressBind.sin_addr.s_addr = inet_addr(CLIENT_IP_ADDR);


    connect(client_socket_handle,(const sockaddr*)&clientSocketAddressBind,sizeof(clientSocketAddressBind));

    while(true){
    std::string s_client_msg = "HELLO FROM CLIENT";
    std::string r_server_msg(1024 , '\0');

    send(client_socket_handle,s_client_msg.c_str(),s_client_msg.size(),0);
    size_t recv_byte_size = recv(client_socket_handle,&r_server_msg[0],r_server_msg.size(),0);

    if(recv_byte_size == SOCKET_ERROR || recv_byte_size == 0){
        Sleep(2000);
        continue;
    }

    r_server_msg.resize(recv_byte_size);
    std::cout<<r_server_msg<<"\n";
    Sleep(10000);
    break;
    }

    closesocket(client_socket_handle);
    WSACleanup();
}