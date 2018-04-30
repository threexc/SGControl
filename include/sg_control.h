#ifndef _SG_CONTROL_H
#define _SG_CONTROL_H

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

#ifndef COMMAND_ERROR
#define COMMAND_ERROR 1
#endif

#ifndef NO_CMD_ERROR
#define NO_CMD_ERROR 0
#endif

#ifndef SCPI_PORT
#define SCPI_PORT 5025
#endif

#ifndef INPUT_BUF_SIZE
#define INPUT_BUF_SIZE 64*1024
#endif

typedef  int SOCKET;

SOCKET open_socket(const char *hostname, int port_number);
int command_instrument(SOCKET sock, const char *command);
char * recv_line(SOCKET sock, char * result, int maxLength);
long query_instrument(SOCKET sock, const char *command, char *result, size_t max_length);
void show_errors(SOCKET sock);
unsigned char is_query(char* cmd);

endif
