BIN = Proc
SRC = WebSocketppServer.cc
REQUIRE =  -std=c++17 -lpthread 
$(BIN) : $(SRC)
	g++ -o $@ $^ $(REQUIRE)
.PHONY: clean
clean:
	rm -f $(BIN)