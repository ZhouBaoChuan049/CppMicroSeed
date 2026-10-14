BIN = Proc
SRC = AsyncClient.cc AsyncServer.pb.cc
REQUIRE =  -std=c++17 -lbrpc -lpthread -lprotobuf  -lgflags  -lleveldb -ldl -lssl -lcrypto
$(BIN) : $(SRC)
	g++ -o $@ $^ $(REQUIRE)
.PHONY: clean
clean:
	rm -f $(BIN)