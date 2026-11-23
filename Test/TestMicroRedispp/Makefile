BIN = Proc
SRC = TestMicroRedis.cc ../../Source/Redispp.cc
REQUIRE =  -std=c++17 -lhiredis -lredis++ -lpthread -I ../../Include
$(BIN) : $(SRC)
	g++ -o $@ $^ $(REQUIRE)
.PHONY: clean
clean:
	rm -f $(BIN) 