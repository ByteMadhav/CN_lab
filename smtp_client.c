#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>

int serv_sock;

int send_msg(char* msg){
	if(msg!=NULL){
		send(serv_sock,msg,strlen(msg),0);
		printf("client>%s",msg);
	}
	
	char buffer[4096];
	memset(buffer,0,sizeof(buffer));
	recv(serv_sock,buffer,sizeof(buffer),0);
	printf("server>%s",buffer);
	return 0;
}

int main(){
	
	struct sockaddr_in serv_addr;
	socklen_t serv_size=sizeof(serv_addr);	
	
	serv_sock=socket(AF_INET,SOCK_STREAM,0);
	
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(8000);
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	if(connect(serv_sock,(struct sockaddr*)&serv_addr,serv_size)==0){
		printf("Connection established\n");
	}

	
	send_msg(NULL);
	
	send_msg("HELO from sample.example.org\r\n");
	
	send_msg("MAIL FROM:<bob@example.org>\r\n");
	
	send_msg("RCPT TO:<sam@example.org>\r\n");
	
	send_msg("RCPT TO:<dean@example.com>\r\n");
	
	send_msg("DATA\r\n");
	
	char* email=
	"From:bob@example.org\r\n"
	"To:sam@example.org\r\n"
	"Cc:dean@example.com\r\n"
	"Date:16/05/2026 02:33:45\r\n"
	"Subject: Deadline for work\r\n"
	"\r\n"
	"The job has been done, submit the report in 5 days\r\n"
	"\r\n"
	".";
	
	send_msg(email);
	
	send_msg("QUIT\r\n");
	
	close(serv_sock);
	
	return 0;
}
