OBJ = Proc
SRC = HlsRemux.cc ../../Source/Log.cc
REQUIRE = -std=c++17 -lavformat -lavcodec -lavutil -lpthread -I ../../Include -lspdlog -lfmt -lgflags

$(OBJ) : $(SRC)
	g++ -o $@ $^ $(REQUIRE)
.PHONY: clean
clean:
	rm -f $(OBJ) 