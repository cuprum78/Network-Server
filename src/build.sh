
#Just get object files here.
OBJS="listener.o connmanager.o connection.o"
rm ${OBJS}

SRCS="listener.cpp listener.h connmanager.h connmanager.cpp connection.h connection.cpp"
g++ -c -Wall -fPIC -std=c++11 ${SRCS} -I/usr/local/include
