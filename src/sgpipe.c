#include <stdio.h>

#define BUFSIZE 128

//int parse_output(char *sg_ip, char *opt, struct *cmd_list)
int parse_output(void)
{
  char *cmd = "lanio -n 134.117.62.53";

  char buf[BUFSIZE];
  FILE *fp;

  if ((fp = popen(cmd, "r")) == NULL)
  {
    printf("Error opening pipe!");
    return -1;
  }

  while(fgets(buf, BUFSIZe, fp) != NULL)
  {
    fwrite(cmd, 1, sizeof(cmd), fp);
    printf("OUTPUT: %s", buf);
    wait(5);
  }

  if(pclose(fp))
  {
    printf("Command not found or exited with error status\n");
    return -1;
  }

  return 0;
}
