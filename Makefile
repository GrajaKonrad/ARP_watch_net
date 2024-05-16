ALL: build link

build:
	g++ -c ./src/AppStart.cpp -o ./bin/cmp/AppStart.o

link:
	g++ -o bin/ArpWatchNet -Wall \
		./bin/cmp/AppStart.o

run_client:
	./bin/ArpWatchNet -c

run_server:
	./bin/ArpWatchNet -s