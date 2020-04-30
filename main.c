#include<stdio.h>
#include<stdlib.h>
#include<io.h>

char server_ip[25],ovpn_name[100],command[151];
int port_start,port_gap,port,i;
FILE* server_portnum,*server_config, * client_config, *settings,* server_cert,*client_cert, * client_key;


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
	printf("------------------------------------------------------------------\n");
	printf("使用说明：软件中需要的证书与私钥可通过SoftEther或OpenVPN生成后导入\n");
	printf("生成的portnum文件用于在SoftEther GUI界面修改OpenVPN端口\n");
	printf("生成的config文件用于直接在远程服务器上以覆盖方式修改OpenVPN端口\n");
	printf("------------------------------------------------------------------\n\n");
	printf("请输入服务器域名或ip地址：\n");
	scanf("%s", server_ip);
	Input:printf("请输入起始端口号：\n");
	scanf("%d", &port_start);
	if (port_start < 10000 || port_start>65535) {
		printf("起始端口号输入不合法！\n");
		goto Input;
	}
	printf("请输入端口号间隔：\n");
	scanf("%d", &port_gap);
	printf("请输入OpenVPN配置文件名称：\n");
	scanf("%s", ovpn_name);
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
		fprintf(settings,"#此配置为系统默认配置，可根据自身需要修改，如修改后需恢复默认可直接删除本文件！\n");
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
		fprintf(settings, "keepalive 5 10\n");
		fprintf(settings, "block-outside-dns\n");
		fprintf(settings, "connect-retry 0\n");
		fclose(settings);
	}
	printf("请在弹出窗口中导入服务器证书（利用SoftEther证书生成工具生成后导入）. . .\n");
	system("notepad save\\server.cer");
	printf("请在弹出窗口中导入客户端证书（利用SoftEther证书生成工具生成后导入）. . .\n");;
	system("notepad save\\client.cer");
	printf("请在弹出窗口中导入客户端私钥（利用SoftEther证书生成工具生成后导入）. . .\n");
	system("notepad save\\client.key");
	printf("请在弹出窗口中自定义客户端配置（需要与服务器一致，不知道请不要修改！）. . .\n");
	system("notepad save\\settings.ini");
	printf("数据导入完成，正在生成配置文件. . .\n");
	return 0;
}

int server() {
	server_portnum = fopen("config\\server_portnum.txt", "w");
	for (i = 0, port = port_start; i < 64; port = port + port_gap, i++) {
		if (port <= 65535)fprintf(server_portnum, "%d,", port);
		else {
			printf("由于端口号大小溢出，自动取最小值10001！\n");
			port = 10001;
			i = i - 1;
		}
	}
	fclose(server_portnum);
	server_config = fopen("config\\server_config.txt", "w");
	fprintf(server_config, "string OpenVPN_UdpPortList ");
	for (i = 0, port = port_start; i < 64; port = port + port_gap, i++) {
		if ((port <= 65535)&&(i<63))fprintf(server_config, "%d,$20", port);
		else if ((port <= 65535) && (i == 63))fprintf(server_config, "%d", port);
		else {
			printf("由于端口号大小溢出，自动取最小值10001！\n");
			port = 10001;
			i = i - 1;
		}
	}
	fclose(server_config);
	sprintf(command, "copy \"config\\server_portnum.txt\" \"config\\%s_server_portnum.txt\"", ovpn_name);
	system(command);
	sprintf(command, "copy \"config\\server_config.txt\" \"config\\%s_server_config.txt\"", ovpn_name);
	system(command);
	system("del config\\server_portnum.txt");
	system("del config\\server_config.txt");
	printf("服务器端配置文件已生成！\n");
	return 0;
}

int client() {
	system("type save\\settings.ini>config\\client_config.ovpn");
	client_config = fopen("config\\client_config.ovpn", "a");
	fprintf(client_config, "\n\n");
	for (i = 0, port = port_start; i < 64; port = port + port_gap, i++) {
		if (port <= 65535)fprintf(client_config, "remote %s %d\n", server_ip,port);
		else {
			printf("由于端口号大小溢出，自动取最小值10001！\n");
			port = 10001;
			i = i - 1;
		}
	}
	fclose(client_config);
	client_config = fopen("config\\client_config.ovpn", "a");
	fprintf(client_config, "\n\n<ca>\n");
	fclose(client_config);
	system("type save\\server.cer>>config\\client_config.ovpn");
	client_config = fopen("config\\client_config.ovpn", "a");
	fprintf(client_config, "\n</ca>\n\n<cert>\n");
	fclose(client_config);
	system("type save\\client.cer>>config\\client_config.ovpn");
	client_config = fopen("config\\client_config.ovpn", "a");
	fprintf(client_config, "\n</cert>\n\n<key>\n");
	fclose(client_config);
	system("type save\\client.key>>config\\client_config.ovpn");
	client_config = fopen("config\\client_config.ovpn", "a");
	fprintf(client_config, "\n</key>\n\n\n");
	fclose(client_config);
	sprintf(command, "copy \"config\\client_config.ovpn\" \"config\\%s.ovpn\"", ovpn_name);
	system(command);
	system("del config\\client_config.ovpn");
	printf("客户端端配置文件已生成！\n");
	return 0;
}
