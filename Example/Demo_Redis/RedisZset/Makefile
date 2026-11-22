BIN = Proc
SRC = RedisZset.cc
REQUIRE =  -std=c++17 -lhiredis -lredis++ -lpthread
$(BIN) : $(SRC)
	g++ -o $@ $^ $(REQUIRE)
.PHONY: clean
clean:
	rm -f $(BIN) 