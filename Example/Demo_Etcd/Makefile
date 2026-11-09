Options = -lcpprest -letcd-cpp-api
All : SimpleGet SimplePut DirGet DirPut
SimpleGet : SimpleGet.cc
	g++ -o $@ $^ $(Options)
SimplePut : SimplePut.cc
	g++ -o $@ $^ $(Options)
DirGet : DirGet.cc
	g++ -o $@ $^ $(Options)
DirPut : DirPut.cc
	g++ -o $@ $^ $(Options)
.PHONY: clean
clean:
	rm -f SimpleGet SimplePut DirPut DirGet