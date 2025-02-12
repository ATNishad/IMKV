#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma comment(lib,"ws2_32")   //supported only in clang or MSVC

//replace ip and port with preferred server ip and port
#define SERVER_PORT_NUM 5000        
#define SERVER_IP_ADDR "127.0.0.1"  


#include<winsock2.h>
#include<ws2tcpip.h>
#include<iostream>

    
int main(){
    
    //winsock initialization
    WSADATA wsadata;  
    int iresult;

    iresult = WSAStartup(MAKEWORD(2,2),&wsadata);
    if( iresult != 0){
        std::cout<<"WSASTARTUP FAILED:"<<WSAGetLastError()<<"\n";
        return -1;
    }


    //server socket creation
    int optval = 1;
    SOCKET socket_handle = socket(AF_INET,SOCK_STREAM, 0);
    if(socket_handle == INVALID_SOCKET){
        std::cout<<"SOCKET INITIALIZATION FAILED:"<<WSAGetLastError<<"\n";
        return -1;
    }
    else{
    if(setsockopt(socket_handle,SOL_SOCKET,SO_REUSEADDR,(const char*)(&optval),sizeof(optval)) !- 0){
        std::cout<<"SET SOCKET OPTION FAILED:"<<WSAGetLastError<<"\n";
        return -1;
    }
    }
    
    //binding socket to port and ip
    sockaddr_in socketAddressBind = {};
    socketAddressBind.sin_family = AF_INET; 
    socketAddressBind.sin_port = htons(SERVER_PORT_NUM);
    inet_pton(AF_INET,SERVER_IP_ADDR,&socketAddressBind.sin_addr.s_addr);

    int bindVal = bind(socket_handle,(const sockaddr*)&socketAddressBind,sizeof(socketAddressBind));
    if( bindVal != 0){
        std::cout<<"BIND FAILED:"<<WSAGetLastError()<<"\n";
        return -1;
    }

    
    //set socket to listen 
    if(listen(socket_handle,SOMAXCONN) == SOCKET_ERROR){
        std::cout<<"LISTEN FAILED:"<<WSAGetLastError()<<"\n";
        return -1;
    }

    //loop for the socket to accept client socket
    while(true){
    std::cout<<"Waiting for connection..."<<"\n";
    sockaddr_in client_sock_addr = {};
    int client_sa_len = sizeof(client_sock_addr);
    SOCKET acceptedSocket = accept(socket_handle,(sockaddr*)&client_sock_addr, &client_sa_len);
    if( acceptedSocket == INVALID_SOCKET){
        std::cout<<"ACCEPT FAILED:"<<WSAGetLastError()<<"\n";
        continue;
    }

    //on successful acceptance send()
    else{
    std::cout<<"CLIENT CONNECTED!!"<<"\n";
    
    std::string send_msg = "hello from server";
    std::string recv_msg(1024 , '\0');

    if(send(acceptedSocket,send_msg.c_str(),send_msg.size(),0)== SOCKET_ERROR){
        std::cout<<"SEND FAILED:"<<WSAGetLastError()<<"\n";
        return -1;
    }

    if(recv(acceptedSocket,&recv_msg[0],recv_msg.size(),0) == SOCKET_ERROR){
        std::cout<<"RECEIVE FAILED:"<<WSAGetLastError<<"\n";
        return -1;
    }
    else{
        std::cout<<recv_msg;
    }

    }


    }


    //close socket at end
    if(closesocket(socket_handle) != 0){
        std::cout<<"SOCKET FAILED TO CLOSE:"<< WSAGetLastError()<<"\n";
        return -1;
    }
    else{
        std::cout<<"SOCKET CLOSED"<<"\n";
        WSACleanup();
    }
}

