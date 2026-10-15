BOJ_1 = Server
BOJ_2 = Client

SRC_1 = HttpServer.cc HttpService.pb.cc
SRC_2 = HttpClient.cc HttpService.pb.cc
REQUIRE =  -std=c++17 -lbrpc -lpthread -lprotobuf  -lgflags  -lleveldb -ldl -lssl -lcrypto

ALL : $(BOJ_1) $(BOJ_2)
$(BOJ_1) : $(SRC_1)
	g++ -o $@ $^ $(REQUIRE)
$(BOJ_2) : $(SRC_2)
	g++ -o $@ $^ $(REQUIRE)

.PHONY: clean
clean:
	rm -f $(BOJ_1) $(BOJ_2)