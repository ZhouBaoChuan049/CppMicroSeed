BOJ_1 = Server
BOJ_2 = Client

SRC_1 = BrpcServer.cc Calculate.pb.cc ../../Source/Brpc.cc ../../Source/Log.cc
SRC_2 = BrpcClient.cc Calculate.pb.cc ../../Source/Brpc.cc ../../Source/Log.cc
REQUIRE =  -std=c++17 -I ../../Include/ -lbrpc -lpthread -lprotobuf -lfmt -lgflags -lspdlog -lleveldb -ldl -lssl -lcrypto

ALL : $(BOJ_1) $(BOJ_2)
$(BOJ_1) : $(SRC_1)
	g++ -o $@ $^ $(REQUIRE)
$(BOJ_2) : $(SRC_2)
	g++ -o $@ $^ $(REQUIRE)

.PHONY: clean
clean:
	rm -f $(BOJ_1) $(BOJ_2)