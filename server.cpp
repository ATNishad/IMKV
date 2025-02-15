#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma comment(lib,"ws2_32") //for clang or mvsc

#define CLIENT_PORT_NUM 5000
#define CLIENT_IP_ADDR "127.0.0.1"

#include<winsock2.h>
#include<windows.h>
#include<iostream>
#include<cassert>
#include<cstdint>



namespace full_SR{

    int receive_full(SOCKET handle,char* receive_buffer,size_t r_buffer_size){
        while( r_buffer_size > 0 ){ 
        ssize_t recv_size = recv(handle,receive_buffer,r_buffer_size,0);
        if(recv_size <= 0){
            return -1;  //receive error
        }
        assert(size_t(recv_size) <= r_buffer_size); //check size 
        r_buffer_size -= size_t(recv_size);
        receive_buffer += recv_size;
        }
        return 0;
    }
    
    int send_full(SOCKET handle,char* send_buffer,size_t s_buffer_size){
        while( s_buffer_size > 0){
        ssize_t send_size = send(handle,send_buffer,s_buffer_size,0);
        if(send_size <= 0){
            return -1;
        }
        assert((size_t)send_size <= s_buffer_size);
        s_buffer_size -= size_t(send_size);
        send_buffer += send_size;
        }
        return 0;
    }
    
}

int32_t server_protocol(SOCKET new_comm_socket){
    const size_t MAX_MSG_SIZE=2096; 
    
    //RECEIVE
    char receive_buffer[4+ MAX_MSG_SIZE] = {};
    size_t r_buffer_size = sizeof(receive_buffer);

    //receiving first 4 bytes
    int32_t r_err = full_SR::receive_full(new_comm_socket,receive_buffer,4);
    if(r_err != 0){
        std::cerr<<"FULL RECEIVE FAILED (byte)\n";
        return -1;        
    }
    //storing the 4 bytes
    uint32_t r_len = 0;
    memcpy(&r_len,receive_buffer,4);
    if(r_len > MAX_MSG_SIZE){
        std::cerr<<"RECEIVED MSG EXCEEDS BUFFER SIZE\n";
        return -1;
    }

    //receiving the actual message
    r_err = full_SR::receive_full(new_comm_socket,&receive_buffer[4],r_len);
    if(r_err != 0){
        std::cerr<<"FULL RECEIVE FAILED (msg)\n";
        return -1;
    }
    //printing client message on server    
    std::cout<<"RECEIVED FROM CLIENT:"<<&receive_buffer[4]<<"\n";
    
    //SEND
    char msg[] = "HELLO FROM SERVER";
    char send_buffer[4+sizeof(msg)];
    
    uint32_t s_len = strlen(msg);
    memcpy(send_buffer,&s_len,4);
    memcpy(&send_buffer[4],msg,s_len);

    
    int s_err = full_SR::send_full(new_comm_socket,send_buffer,4+s_len);
    if(s_err == -1){
        std::cerr<<"FULL SEND FAILED\n";
        return -1;        
    }

    return 0;
}

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
    std::cout<<"LISTENING FOR CONNECTION...\n";
    //socket accept
    SOCKET new_comm_socket ={};

    while(true){

    sockaddr_in connecting_socket = {};
    int cs_len = sizeof(connecting_socket);
    new_comm_socket = accept(server_socket_handle,(sockaddr*)&connecting_socket,&cs_len);
    if(new_comm_socket < 0){
        closesocket(new_comm_socket);
        continue;
    }
    std::cout<<"CLIENT CONNECTED\n";

    while(true){
    int32_t err = server_protocol(new_comm_socket);
    if(err != 0){
        std::cerr<<"APPLICATION PROTOCOL ERROR\n";
        break;
    }
    }
    break;
    } 

    std::cin.get();

    shutdown(new_comm_socket,SD_BOTH);
    closesocket(new_comm_socket);
    closesocket(server_socket_handle);    
    WSACleanup();

}