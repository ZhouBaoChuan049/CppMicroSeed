OBJ = Proc
SRC = ./RealTestElasticSearch.cc ../../Source/ElasticSearch.cc ../../Source/Util.cc ../../Source/Log.cc
REQUIRE = -std=c++17 \
          -lpthread \
          -I ../../Include \
          -lspdlog \
          -lfmt \
          -lgflags \
          -ljsoncpp \
          -lcpr \
          -lelasticlient

$(OBJ) : $(SRC)
	g++ -o $@ $^ $(REQUIRE)

.PHONY: clean
clean:
	rm -f $(OBJ)