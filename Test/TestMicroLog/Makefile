BIN = Proc
SRC = Testlog.cc ../../Source/Log.cc
REQUIRE = -I ../../Include/ -std=c++17 -lpthread -lspdlog -lfmt -lgflags
$(BIN) : $(SRC)
	g++ -o $@ $^ $(REQUIRE)
.PHONY: clean
clean:
	rm -f $(BIN)