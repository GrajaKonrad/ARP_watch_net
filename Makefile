ALL: build link

build:
	g++ -c ./src/AppStart.cpp -o ./bin/cmp/AppStart.o
	g++ -c ./src/StartupOptions.cpp -o ./bin/cmp/StartupOptions.o
	g++ -c ./src/Client.cpp -o ./bin/cmp/Client.o
	g++ -c ./src/Server.cpp -o ./bin/cmp/Server.o

link:
	g++ -o bin/ArpWatchNet -Wall \
		./bin/cmp/AppStart.o \
		./bin/cmp/StartupOptions.o \
		./bin/cmp/Client.o \
		./bin/cmp/Server.o \

run_client:
	sudo ./bin/ArpWatchNet -c 127.0.0.1 12345 -v

run_client_test:
	./bin/ArpWatchNet -c 127.0.0.1 12345 -a ./tests/mock_arp.txt -v

run_server:
	./bin/ArpWatchNet -s 12345