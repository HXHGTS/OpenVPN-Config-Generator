#include<stdio.h>
#include<stdlib.h>

int main() {
	FILE* conf;
	char server_ip[48];
	int port,port_start, i, port_gap;
	printf("输入服务器地址：");
	scanf("%s", server_ip);
	printf("输入开始端口：");
	scanf("%d", &port_start);
	printf("输入端口间隔：");
	scanf("%d", &port_gap);
	conf = fopen("server.conf", "w");
	for (i=0,port = port_start; i<64; port = port + port_gap,i++) {
		fprintf(conf, "%d ",port);
	}
	fclose(conf);
	conf = fopen("client.conf", "w");
	fprintf(conf, "remote-random\n");
	for (i=0,port = port_start; i<64; port = port + port_gap,i++) {
		fprintf(conf, "remote %s %d\n", server_ip,port);
	}
	fprintf(conf, "\n");
	fclose(conf);
	system("notepad server.conf");
	system("notepad client.conf");
	return 0;
}