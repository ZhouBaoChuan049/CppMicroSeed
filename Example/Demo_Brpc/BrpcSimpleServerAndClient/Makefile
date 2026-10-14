BIN = Proc
SRC = Server.cc Server.pb.cc
REQUIRE =  -std=c++17 -lbrpc -lpthread -lprotobuf  -lgflags  -lleveldb -ldl -lssl -lcrypto
$(BIN) : $(SRC)
	g++ -o $@ $^ $(REQUIRE)
.PHONY: clean
clean:
	rm -f $(BIN)