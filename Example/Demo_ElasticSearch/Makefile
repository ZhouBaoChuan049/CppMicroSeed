OBJ = Proc
SRC = ESClient.cc 
REQUIRE = -std=c++17 -lcpr -ljsoncpp -lelasticlient -lpthread 

$(OBJ) : $(SRC)
	g++ -o $@ $^ $(REQUIRE)
.PHONY: clean
clean:
	rm -f $(OBJ) 