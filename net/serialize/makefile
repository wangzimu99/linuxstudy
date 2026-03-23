cc=g++
.PHONY:all
all:calclient calserver	
calclient:calClient.cc -ljsoncpp -DMYSELF
	$(cc) -o $@ $^ 
calserver:calServer.cc -ljsoncpp -DMYSELF
	$(cc) -o $@ $^ 
.PHONY:clean
clean:
	rm -f calserver calclient
