	AC_INIT([Silent Server], [1.0], [srdrm.cn@gmail.com])
	AM_INIT_AUTOMAKE([-Wall -Werror foreign])
	

	AC_PROG_CXX
	CXXFLAGS="-g3 -ggdb -O0"

	AC_CHECK_HEADERS([sys/epoll.h])

	AC_CONFIG_HEADERS([config.h])
	AC_CONFIG_FILES([
			Makefile
			src/Makefile
			study/Makefile
			study/epoll/Makefile
			])
	AC_OUTPUT
