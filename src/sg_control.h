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
#define SCPI_PORT  5025
#endif

/* Used only for debug via the echo_server tool */
#ifndef ECHO_PORT
#define ECHO_PORT 5026
#endif

#ifndef INPUT_BUF_SIZE
#define INPUT_BUF_SIZE (64*1024)
#endif

/* Multiple potential typedef calls will be fine, and possibly optimized out,
as long as there are no changes to the definition */
typedef int SOCKET;

#endif
