/*
 *  pollrdhup-test.c by Davide Libenzi ( simple test for Linux POLLRDHUP )
 *  Copyright (C) 2006  Davide Libenzi
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Davide Libenzi <davidel@xmailserver.org>
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/un.h>



#define USOCK_PATH "/tmp/pollrdhup-test"

#ifndef POLLRDHUP
#define POLLRDHUP 0x2000
#endif



static int prdht_connect(char const *uskfile) {
	int susk;
	struct sockaddr_un rmtaddr;

	if ((susk = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return -1;
	}
	memset(&rmtaddr, 0, sizeof(rmtaddr));
	rmtaddr.sun_family = AF_UNIX;
	strcpy(rmtaddr.sun_path, uskfile);
	if (connect(susk, (struct sockaddr *) &rmtaddr,
		    strlen(rmtaddr.sun_path) + sizeof(rmtaddr.sun_family)) == -1) {
		perror("connect");
		close(susk);
		return -1;
	}

	return susk;
}

static int prdht_sockcreate(char const *uskpath) {
	int usk;
	struct sockaddr_un uskname;

	if ((usk = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return -1;
	}
	memset(&uskname, 0, sizeof(uskname));
	uskname.sun_family = AF_UNIX;
	strncpy(uskname.sun_path, uskpath, sizeof(uskname.sun_path) - 1);
	unlink(uskname.sun_path);
	if (bind(usk, (struct sockaddr *) &uskname,
		 strlen(uskname.sun_path) + sizeof(uskname.sun_family)) == -1) {
		perror("bind");
		close(usk);
		return -1;
	}
	if (listen(usk, 8) == -1) {
		perror("listen");
		close(usk);
		return -1;
	}

	return usk;
}

static void prdht_events_print(unsigned int events, FILE *fout) {

	if (events & POLLIN)
		fprintf(fout, " POLLIN");
	if (events & POLLOUT)
		fprintf(fout, " POLLOUT");
	if (events & POLLHUP)
		fprintf(fout, " POLLHUP");
	if (events & POLLRDHUP)
		fprintf(fout, " POLLRDHUP");
}

int main(int ac, char **av) {
	int ufd, cufd, c;
	socklen_t addrsize;
	pid_t cpid;
	char const *upath = USOCK_PATH;
	struct pollfd pfds[1];
	struct sockaddr_un rmtaddr;

	if ((ufd = prdht_sockcreate(upath)) == -1)
		return 1;
	if ((cpid = fork()) == 0) {
		if ((cufd = prdht_connect(upath)) == -1) {
			perror("connect");
			exit(1);
		}
		shutdown(cufd, SHUT_WR);
		read(cufd, &c, 1);
		close(cufd);
		exit(0);
	}
	if (cpid == -1) {
		perror("fork");
		return 2;
	}
	addrsize = sizeof(rmtaddr);
	if ((cufd = accept(ufd, (struct sockaddr *) &rmtaddr,
			   &addrsize)) == -1) {
		perror("accept");
		return 3;
	}
	memset(pfds, 0, sizeof(pfds));
	pfds[0].fd = cufd;
	pfds[0].events = POLLIN | POLLRDHUP;
	poll(pfds, 1, -1);
	fprintf(stdout, "events:");
	prdht_events_print(pfds[0].revents, stdout);
	fprintf(stdout, "\n");
	write(cufd, "r", 1);
	close(cufd);
	close(ufd);
	waitpid(cpid, NULL, 0);
	remove(upath);

	return 0;
}

