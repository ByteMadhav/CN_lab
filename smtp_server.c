#include<stdio.h>
#include<string.h>
#include<unistd.h>

#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>

#define PORT 8000
#define bsize 4096

int send_reply(int sock,char* msg){
	
	send(sock,msg,strlen(msg),0);
	printf("server>%s",msg);
	return 0;
}

int main(){
	int server_fd,cl_sock;
	struct sockaddr_in serv_addr,cl_addr;
	socklen_t cl_size=sizeof(cl_addr);
	char buffer[bsize];
	
	server_fd=socket(AF_INET,SOCK_STREAM,0);
	
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(PORT);
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

	
	if (bind(server_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
		perror("Bind failed");
		return -1;
	}
	
	if (listen(server_fd,1)<0){
		perror("Listening failed");
		return -1;
	}
	
	cl_sock=accept(server_fd,(struct sockaddr*)&cl_addr,&cl_size);
	
	if (cl_sock<0){
		perror("Accept failed");
		return -1;
	}
	
	printf("Client accepted\n");
	
	send_reply(cl_sock,"220 example.smtp ready\r\n");
	
	int in_data=0;
	
	while(1){
	
		memset(buffer,0,bsize );
		int bytes=recv(cl_sock,buffer,bsize-1,0);
		if (bytes<=0){
			printf("Client disconnected");
			return -1;
		}
		
		
		if(in_data){
			if(strstr(buffer,"\r\n")!=NULL || strcmp(buffer,".")==0){
				send_reply(cl_sock,"250 message accepted for delivery \r\n");
				
			}
			
		}
		
		else if(strncmp(buffer,"HELO",4)==0){
			send_reply(cl_sock,"250 greeting the user\r\n");
		}
	
		else if(strncmp(buffer,"EHLO",4)==0){
			send_reply(cl_sock,"250 greeting the user\r\n");
		}
		else if(strncmp(buffer,"MAIL FROM:",10)==0){
			send_reply(cl_sock,"250 2.1.0 OK\r\n");
		}
		else if(strncmp(buffer,"RCPT TO:",8)==0){
			send_reply(cl_sock,"250 2.1.5 OK\r\n");
		}
		else if(strncmp(buffer,"DATA",4)==0){
			send_reply(cl_sock,"354 send data and end with<CR><LF>.</LF></CR>\r\n");
			in_data=1;
		}
		else if(strncmp(buffer,"QUIT",4)==0){
			send_reply(cl_sock,"221 2.0.0 bye\r\n");
			break;
		}
		else{
			send_reply(cl_sock,"500 Command Undefined\r\n");
		}
		
			
	}
	
	close(server_fd);
	close(cl_sock); 
	return 0;
	
}
