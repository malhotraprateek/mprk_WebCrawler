	
IDIR=-I./include
LDIR=-L./deps/lib
SRC=./src/
LIBS=-lhiredis -lcurl
CFLAGS=-o
EXECFILE=Routofy
COMPILER=g++

all: routofy_project

curl:
	cd ./deps/curl && sudo chmod +x configure && ./configure --prefix=$(CURDIR)/deps/curl/compiled/ && sudo make install && cp ./compiled/lib/libcurl.a ../lib/

redis: 
	cd ./deps/redis/deps/hiredis && sudo make install && cp ./libhiredis.a ../../../lib/

routofy_project: curl redis
	$(COMPILER) $(CFLAGS) $(EXECFILE) $(IDIR) $(SRC)*.cc $(LDIR) $(LIBS)
	
cleanroutofy:
	sudo rm $(EXECFILE) && sudo rm ./deps/lib/libcurl.a && sudo rm ./deps/lib/libhiredis.a
	
clean: cleanroutofy
	 cd ./deps/curl && sudo make distclean && cd ../redis/deps/hiredis && sudo make clean
