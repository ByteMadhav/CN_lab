#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

int main(){
	
	int serv_sock=socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in serv_addr,cl_addr;
	socklen_t len=sizeof(cl_addr);
	char buf[10];
	int drop_once=1;
	
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(8000);
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	
	memset(&serv_addr,0,sizeof(serv_addr));
	
	if((bind(&serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)))<0){
		perror("Bind failed");
		return -1;
	}
	
	while(1){
		
		int n=recvfrom(serv_sock,buf,sizeof(buf),0,&cl_addr,&len);
		
		if(n>0){
			
			buf[n]='\0';
			int frame=atoi(buf);
			
			if(frame==2 && drop_once){
				printf("Frame lost!!");
				drop_once=0;
			}
			
		}
		
		printf("Received frame %d, sending ACK %d",frame,frame);
		sendto(serv_sock,buf,strlen(buf),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
		
	}

	close(serv_sock);
	
	return 0; 
}
