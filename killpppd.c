/* (C) thetrues0l@gmail.com 2011 */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <alloca.h>
#include <getopt.h>
#include <sys/socket.h>
#include <cutils/sockets.h>
#include <termios.h>

#define LOG_NDEBUG 0
#define LOG_TAG "KILLPPPD"
#include <utils/Log.h>

int main() {

	while(1) {

		int pid = -1;

		FILE* f = popen("ps | grep /system/bin/pppd", "r");

		if (f) {
			char* buf = (char*)malloc(1024);
			while (!feof(f)) {
				// ps on flytouch output:
				// user<spaces>pid<spaces...>
				*buf = 0;
				if (fgets(buf, 1024, f) != buf)
					break;
				//LOGD("[inf] buf:");
				//LOGD("%s", buf);
				char* pos = buf;
				while(!isdigit(*pos))
					++pos;
				if (*pos == 0) // invalid output
					continue;
				// ok, pid
				char *pos_e = pos;
				while(isdigit(*pos_e++));
				if (*pos_e == 0)
					continue; // invalid output
				*pos_e = 0;
				//LOGD("pos: %s", pos);
				pid = atoi(pos); // use atoi strtoul is better but)
				char *cmd = (char*)malloc(256);
				sprintf(cmd, "kill %d", pid);
				LOGD("[inf] found pppd with pid %d, do: '%s'", pid, cmd);
				system(cmd);
                                free(cmd);
			}
			free(buf);
			fclose(f);
		}

		if (pid != -1)	{
			// retry
			sleep(1);
			continue;
		}

		break;
	}

	// ok
	return 0;
}
