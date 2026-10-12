BIN = Proc
SRC = TestJsoncpp.cc ../../Source/Util.cc ../../Source/Log.cc
REQUIRE = -I ../../Include/ -std=c++17 -lpthread -ljsoncpp  -lgtest -lfmt
$(BIN) : $(SRC)
	g++ -o $@ $^ $(REQUIRE)
.PHONY: clean
clean:
	rm -f $(BIN)