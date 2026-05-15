#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
void main(){
	
	int PORT=800;
	char* ADDR="127.0.0.1";
	
	int serv_sock;
	struct sockaddr_in serv_addr;
	socklen_t addr_size= sizeof(serv_addr);
	char buffer[1024];
	
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(PORT);
	serv_addr.sin_addr.s_addr=inet_addr(ADDR);
	
	serv_sock=socket(AF_INET,SOCK_STREAM,0);
	
	if (connect(serv_sock,(struct sockaddr*)&serv_addr,addr_size)==0){
		printf("Connection established");	
	
	}
	else{
		perror("connection failed");	
	}
	
	char src_file[1024];
	char dst_file[1024];
	
	printf("Enter source file name:");
	scanf("%s",src_file);
	
	printf("Enter dest file name:");
	scanf("%s",dst_file);
	
	sprintf(buffer,"%s",src_file);
	
	send(serv_sock,buffer,sizeof(buffer),0);
	
	FILE *fp=fopen("op.txt","wb");
	
	while(1){
		int n=recv(serv_sock,buffer,sizeof(buffer),0);
		fwrite(buffer,sizeof(char),n,fp);
		
		if (n<=0){
			break;
		}
	}
	
	fclose(fp);
	close(serv_sock);
	
	
}
