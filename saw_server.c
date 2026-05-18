#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>


int main(){
	
	struct sockaddr_in serv_addr,cl_addr;
	memset(&serv_addr,0,sizeof(serv_addr));
	
	int serv_sock=socket(AF_INET,SOCK_DGRAM,0);
	
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(8000);
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	
	if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
		perror("Bind failed\n");
		return 0;
	}
	
	int seq=0;
	char buf[10];
	int drop_once=1;
	socklen_t len=sizeof(cl_addr);
	
	while(1){
		memset(buf,0,sizeof(buf));
		int n=recvfrom(serv_sock,buf,sizeof(buf),0,(struct sockaddr*)&cl_addr,&len);
		
		if(n>0){
			buf[n]='\0';
			int frame=atoi(buf);
			
			if(frame==2 && drop_once){
				printf("Frame %d not received\n",frame);
				drop_once=0;
				continue;
			}
			printf("Received frame %d sending ACK for frame %d\n",frame,frame);
			sendto(serv_sock,buf,strlen(buf),0,(struct sockaddr*)&cl_addr,sizeof(cl_addr));
			seq++;
			
		}
	}
	
	close(serv_sock);
	return 0;
}

