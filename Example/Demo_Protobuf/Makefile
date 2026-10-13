BIN = Proc
SRC = Serialize.cc Person.pb.cc
REQUIRE =  -std=c++17 -lpthread -lprotobuf
$(BIN) : $(SRC)
	g++ -o $@ $^ $(REQUIRE)
.PHONY: clean
clean:
	rm -f $(BIN)