#include<stdio.h>
#include<stdlib.h>
#include<io.h>

char server_ip[25],ovpn_name[100],command[151];
int port_start,port_gap,port,i;
FILE* server_config, * client_config, *settings,* server_cert,*client_cert, * client_key;


int main() {
MainMenu:system("cls");
	UserInterface();
	server();
	client();
	system("explorer config\\");
	system("pause");
	goto MainMenu;
}

int UserInterface(){
	if (access("config", 0)) {
		system("mkdir config");
	}
	if (access("save", 0)) {
		system("mkdir save");
	}
	printf("�����������������ip��ַ��\n");
	scanf("%s", server_ip);
	Input:printf("��������ʼ�˿ںţ�\n");
	scanf("%d", &port_start);
	if (port_start < 10000 || port_start>65535) {
		printf("��ʼ�˿ں����벻�Ϸ���\n");
		goto Input;
	}
	printf("������˿ںż����\n");
	scanf("%d", &port_gap);
	printf("������OpenVPN�����ļ����ƣ�\n");
	scanf("%s", ovpn_name);
	sprintf(command, "copy \"config\\client_config.ovpn\" \"config\\%s.ovpn\"",ovpn_name);
	if ((server_cert = fopen("save\\server.cer", "r")) == NULL) {
		server_cert = fopen("save\\server.cer", "w");
		fclose(server_cert);
	}
	if ((client_cert = fopen("save\\client.cer", "r")) == NULL) {
		client_cert = fopen("save\\client.cer", "w");
		fclose(client_cert);
	}
	if ((client_key = fopen("save\\client.key", "r")) == NULL) {
		client_key = fopen("save\\client.key", "w");
		fclose(client_key);
	}
	if ((settings = fopen("save\\settings.ini", "r")) == NULL) {
		settings = fopen("save\\settings.ini", "w");
		fprintf(settings, "dev tun\n");
		fprintf(settings, "proto udp\n");
		fprintf(settings, "remote-random\n");
		fprintf(settings, "cipher AES-128-CBC\n");
		fprintf(settings, "auth SHA1\n");
		fprintf(settings, "resolv-retry 60\n");
		fprintf(settings, "nobind\n");
		fprintf(settings, "persist-key\n");
		fprintf(settings, "persist-tun\n");
		fprintf(settings, "client\n");
		fprintf(settings, "verb 3\n");
		fprintf(settings, "auth-user-pass\n");
		fprintf(settings, "keepalive 10 60\n");
		fprintf(settings, "block-outside-dns\n");
		fclose(settings);
	}
	printf("���ڵ��������е��������֤�飨���Դ�֮ǰ�����������ļ��е������������뵼�룩. . .\n");
	system("notepad save\\server.cer");
	printf("���ڵ��������е���ͻ���֤�飨���Դ�֮ǰ�����������ļ��е����ڶ������뵼�룩. . .\n");;
	system("notepad save\\client.cer");
	printf("���ڵ��������е���ͻ���˽Կ�����Դ�֮ǰ�����������ļ������һ�����뵼�룩. . .\n");
	system("notepad save\\client.key");
	printf("���ڵ����������Զ���ͻ������ã���Ҫ�������һ�£���֪���벻Ҫ�޸ģ���. . .\n");
	system("notepad save\\settings.ini");
	printf("���ݵ�����ɣ��������������ļ�. . .\n");
	return 0;
}

int server() {
	server_config = fopen("config\\server_config.txt", "w");
	for (i = 0, port = port_start; i < 64; port = port + port_gap, i++) {
		if (port <= 65535)fprintf(server_config, "%d,", port);
		else {
			printf("���ڶ˿ںŴ�С������Զ�ȡ��Сֵ10001��\n");
			port = 10001;
			i = i - 1;
		}
	}
	fclose(server_config);
	printf("�������������ļ������ɣ�\n");
	return 0;
}

int client() {
	system("type save\\settings.ini>config\\client_config.ovpn");
	client_config = fopen("config\\client_config.ovpn", "a");
	fprintf(client_config, "\n\n");
	for (i = 0, port = port_start; i < 64; port = port + port_gap, i++) {
		if (port <= 65535)fprintf(client_config, "remote %s %d\n", server_ip,port);
		else {
			printf("���ڶ˿ںŴ�С������Զ�ȡ��Сֵ10001��\n");
			port = 10001;
			i = i - 1;
		}
	}
	fclose(client_config);
	client_config = fopen("config\\client_config.ovpn", "a");
	fprintf(client_config, "\n\n");
	fclose(client_config);
	system("type save\\server.cer>>config\\client_config.ovpn");
	client_config = fopen("config\\client_config.ovpn", "a");
	fprintf(client_config, "\n\n");
	fclose(client_config);
	system("type save\\client.cer>>config\\client_config.ovpn");
	client_config = fopen("config\\client_config.ovpn", "a");
	fprintf(client_config, "\n\n");
	fclose(client_config);
	system("type save\\client.key>>config\\client_config.ovpn");
	client_config = fopen("config\\client_config.ovpn", "a");
	fprintf(client_config, "\n\n");
	fclose(client_config);
	system(command);
	printf("�ͻ��˶������ļ������ɣ�\n");
	return 0;
}