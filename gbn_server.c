#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>

int main(){
	
	int sock=socket(AF_INET,SOCK_DGRAM,0);
	
	struct sockaddr_in serv,cl;
	socklen_t len=sizeof(cl);
	
	memset(&serv,0,sizeof(serv));
	
	serv.sin_family=AF_INET;
	serv.sin_port=htons(8000);
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	char buf[10];
	int expected_frame=0;
	int drop_once=1;
	
	if(bind(sock,(struct sockaddr*)&serv,sizeof(serv))<0){
		perror("Bind failed");
		return 0;
	}
	
	while(1){
		
		int n=recvfrom(sock,buf,sizeof(buf),0,(struct sockaddr*)&cl,&len);
		
		if(n>0){
			buf[n]='\0';
			int frame=atoi(buf);
			
			if(frame==2 && drop_once){
				drop_once=0;
				continue;
			}
			
			if(frame==expected_frame){
				printf("Received frame %d, sending ACK for frame %d \n",frame,frame);
				sprintf(buf,"%d",frame);
				sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&cl,sizeof(cl));
				expected_frame++;
			}
			
			else{
				printf("Frame out of order\n");
				int last_ack=expected_frame -1;
				printf("Last frame in order frame %d\n",last_ack);
				sprintf(buf,"%d",last_ack);
				sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&cl,sizeof(cl));
			}
		}
	}
	
	close(sock);
	return 0;
}
