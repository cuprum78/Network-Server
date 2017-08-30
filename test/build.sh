OBJS="../src/listener.o ../src/connmanager.o ../src/connection.o"

g++ -std=c++11 ../src/listener.h client.h test1.cpp ${OBJS} -I/home/bmecci/BIM/boost/src/ -I/usr/local/include -L /usr/local/lib/ -L /usr/lib/ -L /home/bmecci/BIM/boost/src/ -lboost_system -lboost_unit_test_framework -lpthread -o test1
