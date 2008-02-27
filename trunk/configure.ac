AC_INIT([silent], [1.0], [srdrm.cn@gmail.com])
	AM_INIT_AUTOMAKE([-Wall -Werror foreign])
	AC_PROG_CC

	CFLAGS="-g3 -ggdb -O0"

	AC_CONFIG_HEADERS([config.h])
	AC_CONFIG_FILES([
			Makefile

			src/Makefile

			study/Makefile

			study/asm/Makefile
			study/asm/x86/Makefile

			study/epoll/Makefile


			])
	AC_OUTPUT
