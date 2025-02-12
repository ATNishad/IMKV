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
    //wsa startup
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2,2), &wsadata);
    
    //socket init
    int optval = 1;
    SOCKET server_socket_handle = socket(AF_INET,SOCK_STREAM,0);
    setsockopt(server_socket_handle,SOL_SOCKET,SO_REUSEADDR,(const char*)&optval,sizeof(optval));
    
    //socket bind
    sockaddr_in serverSocketAddressBind = {};
    serverSocketAddressBind.sin_family = AF_INET;
    serverSocketAddressBind.sin_port = htons(CLIENT_PORT_NUM);
    serverSocketAddressBind.sin_addr.s_addr = inet_addr(CLIENT_IP_ADDR);
    bind(server_socket_handle,(const sockaddr*)&serverSocketAddressBind,sizeof(serverSocketAddressBind));


    //socket listen
    listen(server_socket_handle,SOMAXCONN);
    //socket accept
    sockaddr_in connecting_socket = {};
    int cs_len = sizeof(connecting_socket);
    SOCKET new_comm_socket = accept(server_socket_handle,(sockaddr*)&connecting_socket,&cs_len);
    
    while(true){
    std::string s_server_msg = "HELLO FROM SERVER";
    std::string r_client_msg(1024, '\0');

    send(new_comm_socket,s_server_msg.c_str(),s_server_msg.size(),0);
    size_t recv_byte_size = recv(new_comm_socket,&r_client_msg[0],r_client_msg.size(),0);
    r_client_msg.resize(recv_byte_size);
    std::cout<<r_client_msg<<"\n";
    Sleep(10000);
    break;
    }

    shutdown(new_comm_socket,SD_BOTH);
    closesocket(new_comm_socket);
    closesocket(server_socket_handle);    
    WSACleanup();
}