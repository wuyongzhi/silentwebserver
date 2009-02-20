#define _GNU_SOURCE 1
#include <assert.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <libaio.h>

enum {
	IO_CMD_PREADV = 7,
	IO_CMD_PWRITEV = 8
};

void io_prep_preadv(struct iocb *iocb, int fd, struct iovec *iov, int nr_segs,
		    long long offset)
{
	memset(iocb, 0, sizeof(*iocb));
	iocb->aio_fildes = fd;
	iocb->aio_lio_opcode = IO_CMD_PREADV;
	iocb->aio_reqprio = 0;
	iocb->u.v.vec = iov;
	iocb->u.v.nr = nr_segs;
	iocb->u.v.offset = offset;
}

void io_prep_pwritev(struct iocb *iocb, int fd, struct iovec *iov, int nr_segs,
		     long long offset)
{
	memset(iocb, 0, sizeof(*iocb));
	iocb->aio_fildes = fd;
	iocb->aio_lio_opcode = IO_CMD_PWRITEV;
	iocb->aio_reqprio = 0;
	iocb->u.v.vec = iov;
	iocb->u.v.nr = nr_segs;
	iocb->u.v.offset = offset;
}

io_context_t io_ctx;
void perror_exit(const char *msg)
{
	perror(msg);
	exit(1);
}


enum {
	BLOCK_SIZE = 1024,
};

void set_buffer(unsigned char *buf, int seed)
{
	int j;
	for (j = 0; j < BLOCK_SIZE; j++) {
		buf[j] = seed + j;
	}
}

int check_buffer(unsigned char *buf, int seed)
{
	int i;
	for (i = 0; i < BLOCK_SIZE; i++) {
		if (buf[i] != (unsigned char)(seed + i)) {
			return i;
		}
	}
	return -1;
}

void writetest(int fd, int max_size, int nr_iov)
{
	enum {
		IO_UNIT = 1024 * BLOCK_SIZE,
	};
	int i;

	struct iovec iov[nr_iov];
	for (i = 0; i < nr_iov; i++) {
		iov[i].iov_base = mmap(NULL, IO_UNIT / nr_iov, 
				       PROT_READ|PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (iov[i].iov_base == MAP_FAILED)
			perror_exit("mmap");
		iov[i].iov_len = IO_UNIT / nr_iov;
	}

	unsigned char *read_buf = malloc(IO_UNIT);
	for (i = 0; i < 100; i++) {
		int seed[IO_UNIT / BLOCK_SIZE];

		int j;
		for (j = 0; j < IO_UNIT; j += BLOCK_SIZE) {
			int s;
			s = seed[j / BLOCK_SIZE] = random();
			unsigned char *p = iov[j / (IO_UNIT / nr_iov)].iov_base;
			p += j % (IO_UNIT / nr_iov);
			set_buffer(p, s);
		}

		struct io_event events[128];
		struct iocb iocb, *iocb_ptr;
		iocb_ptr = &iocb;
		off_t off = random() % (max_size / BLOCK_SIZE) * BLOCK_SIZE;
		io_prep_pwritev(&iocb, fd, iov, nr_iov, off);
		if (io_submit(io_ctx, 1, &iocb_ptr) < 0)
			perror_exit("io_submit");
		int nr = io_getevents(io_ctx, 1, 128, events, NULL);
		if (nr != 1)
			perror_exit("io_getevents");
		if (events[0].obj != iocb_ptr) 
			perror_exit("event");
		
		int n = pread(fd, read_buf, IO_UNIT, off);
		if (n != IO_UNIT)
			perror_exit("pread");
		for (j = 0; j < IO_UNIT; j += BLOCK_SIZE) {
			check_buffer(read_buf + j, seed[j / BLOCK_SIZE]);
		}
		printf(".");
		fflush(stdout);
	}
	printf("%s: ok\n", __FUNCTION__);
}

void readtest2(int fd, int max_size, int nr_iov)
{
	enum {
		WRITE_BATCH_UNIT = 1024 * BLOCK_SIZE,
		READ_UNIT = 1024 * BLOCK_SIZE,
	};
	int i;

	assert(max_size % BLOCK_SIZE == 0);
	assert(max_size >= WRITE_BATCH_UNIT);
	assert(max_size >= READ_UNIT);

	int seed[max_size / BLOCK_SIZE];

	/* Fill the disk with deterministic garbage. */
	unsigned char *buf = mmap(NULL, WRITE_BATCH_UNIT,
				  PROT_READ|PROT_WRITE,
				  MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	for (i = 0; i < max_size; i += BLOCK_SIZE) {
		int s = seed[i / BLOCK_SIZE] = random() % 256;
		set_buffer(buf + i % WRITE_BATCH_UNIT, s);
		if ((i + BLOCK_SIZE) % WRITE_BATCH_UNIT == 0) {
			ssize_t n = write(fd, buf, WRITE_BATCH_UNIT);
			if (n != WRITE_BATCH_UNIT)
				perror_exit("write");
		}
	}
	
	/* Try reading */
	struct iovec iov[nr_iov];
	for (i = 0; i < nr_iov; i++) {
		iov[i].iov_base = mmap(NULL, READ_UNIT / nr_iov, 
				       PROT_READ|PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (iov[i].iov_base == MAP_FAILED)
			perror_exit("mmap");
		iov[i].iov_len = READ_UNIT / nr_iov;
	}

	for (i = 0; i < 100; i++) {
		int j;
		for (j = 0; j < nr_iov; j++) {
			memset(iov[j].iov_base, -1, READ_UNIT / nr_iov);
		}

		struct io_event events[128];
		off_t off = random() % (max_size / BLOCK_SIZE) * BLOCK_SIZE;
		off -= READ_UNIT;
		if (off < 0) off = 0;

		struct iocb iocb, *iocb_ptr;
		iocb_ptr = &iocb;
		io_prep_preadv(&iocb, fd, iov, nr_iov, off);
		if (io_submit(io_ctx, 1, &iocb_ptr) < 0)
			perror_exit("io_submit");
		int nr = io_getevents(io_ctx, 1, 128, events, NULL);
		if (nr != 1)
			perror_exit("io_getevents");
		if (events[0].obj != iocb_ptr) 
			perror_exit("event");

		for (j = 0; j < READ_UNIT; j += BLOCK_SIZE) {
			unsigned char *p = (unsigned char*)iov[j / (READ_UNIT/nr_iov)].iov_base;
			int k = j % (READ_UNIT/nr_iov);
			int x = check_buffer(p + k, seed[(off + j)/ BLOCK_SIZE]);
			if (x >= 0) {
				printf("wrong value at off=%d+%d+%d v=%x:%x:%x\n",
				       off, j, x,
				       p[k + x], p[k + x + 1], p[k + x + 2]);
				perror_exit("read");
			}
		}

		printf(".");
		fflush(stdout);
	}
	munmap(buf, WRITE_BATCH_UNIT);
	for (i = 0; i < nr_iov; i++) {
		munmap(iov[i].iov_base, READ_UNIT / nr_iov);
	}
	printf("%s: ok\n", __FUNCTION__);
}

void readtest1()
{
	char buf1[256];
	char buf21[128], buf22[128];
	srandom(0);
	struct iocb iocb, *iocb_ptr;
	int nr;
	struct io_event events[128];

	int fd = open("/etc/services", O_RDONLY);
	if (fd < 0)
		perror_exit("open /etc/services");

	{
		io_prep_pread(&iocb, fd, buf1, sizeof buf1, 0);
		iocb_ptr = &iocb;
		if (io_submit(io_ctx, 1, &iocb_ptr) < 0)
			perror_exit("io_submit");
		nr = io_getevents(io_ctx, 1, 128, events, NULL);
		if (nr != 1)
			perror_exit("io_getevents");
		if (events[0].obj != iocb_ptr) 
			perror_exit("event");
	}

	{
		struct iovec iov[2] = {
			{buf21, sizeof buf21},
			{buf22, sizeof buf22}
		};
		
		io_prep_preadv(&iocb, fd, iov, 2, 0);
		if (io_submit(io_ctx, 1, &iocb_ptr) < 0)
			perror_exit("io_submit");
		
		nr = io_getevents(io_ctx, 1, 128, events, NULL);
		if (nr != 1)
			perror_exit("io_getevents");
		if (events[0].obj != iocb_ptr) 
			perror_exit("event");
	}
	if (memcmp(buf1, buf21, sizeof buf21) != 0)
		perror_exit("memcmp1");
	if (memcmp(buf1 + sizeof buf21, buf22, sizeof buf22) != 0)
		perror_exit("memcmp1");
	printf("%s: ok\n", __FUNCTION__);
	close(fd);
}
int main()
{
	if (io_setup(128, &io_ctx) < 0)
		perror_exit("io_setup");
	readtest1();

	{
		const char *path_name = "/tmp/blah";
		int fd = open(path_name, O_RDWR | O_CREAT, 0777);
		if (fd < 0)
			perror_exit(path_name);
		ftruncate(fd, 128 * 1024 * 1024);
		writetest(fd, 128 * 1024 * 1024, 1);
		close(fd);
	}
	{
		const char *path_name = "/tmp/blah";
		int fd = open(path_name, O_RDWR | O_CREAT, 0777);
		if (fd < 0)
			perror_exit(path_name);
		ftruncate(fd, 128 * 1024 * 1024);
		readtest2(fd, 128 * 1024 * 1024, 16);
		close(fd);
	}
#if 0
	/* Caution! this test does direct I/O on a raw device.  Don't do this
	   on a drive with valid data. */
	{
		const char *dev_name = "/dev/sdb";
		int fd = open(dev_name, O_RDWR | O_DIRECT);
		if (fd < 0)
			perror_exit(dev_name);
		readtest2(fd, 128 * 1024 * 1024, 16);
		close(fd);
	}
#endif
}

