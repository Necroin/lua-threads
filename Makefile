build:
	g++ -I/usr/include/lua5.3 src/threads.cpp -o libthreads.so -std=c++17 -Wall -Wextra -fPIC -shared -pthread