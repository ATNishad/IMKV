#include<winsock2.h>
#include<windows.h>
#include<iostream>
#include<cassert>


int receive_full(SOCKET handle,char* receive_buffer,size_t r_buffer_size){
    while( r_buffer_size > 0 ){ 
    ssize_t recv_size = recv(handle,receive_buffer,r_buffer_size,0);
    if(recv_size <= 0){
        return -1;  //receive error
    }
    assert(size_t(recv_size) < r_buffer_size); //check size 
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
    assert((size_t)send_size < s_buffer_size);
    s_buffer_size -= size_t(send_size);
    send_buffer += send_size;
    }
    return 0;
}



int application_protocol(SOCKET new_){

const size_t MAX_MSG_SIZE = 2096;
char send_buffer[] = "HELLO FROM SERVER";
size_t s_buffer_size = sizeof(send_buffer);
char receive_buffer[4+ MAX_MSG_SIZE] = {};
size_t r_buffer_size = sizeof(receive_buffer);

receive_full(new_comm_socket,receive_buffer,r_buffer_size);
send_full(new_comm_socket,send_buffer,s_buffer_size);

}
}
