# ifndef CONNECT_H
# define CONNECT_H
#include "include/library.h"

using namespace std;
int server_fd =0;
struct sockaddr_in address;
uint32_t ip_port = (uint32_t)8080;
char *server_ip = (char*)"127.0.0.1";

int ServerConnect() {
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
                perror("Initializing Socket Communication Failed");
                return 1;
        }
        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
                perror("Failed To Attach To The Port");
		return 2;
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(ip_port);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
                perror("Couldn't Connect To Port");
		return 3;
        }
        if (listen(server_fd, 3) < 0) {
                perror("listen");
		return 4;
        }
	return 0;
}

int client_socket = 0;
int ClientConnect() {
	struct sockaddr_in serv_addr;
	if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    		cout<<"Socket Ereation Error"<<endl;
		return 1;
	}
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(ip_port);
	if(inet_pton(AF_INET, reinterpret_cast<const char*>(server_ip), &serv_addr.sin_addr)<=0) {
    		cout<<"Invalid Address Not Supported"<<endl;
    		return 1;
	}
	if (connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    		cout<<"Connection Failed";
    		return -1;
	}
	return 0;
}
#endif
