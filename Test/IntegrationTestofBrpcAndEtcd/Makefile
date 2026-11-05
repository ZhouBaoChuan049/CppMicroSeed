BOJ_1 = Server
BOJ_2 = Client

SRC_1 = ServiceProvider.cc Calculate.pb.cc ../../Source/Brpc.cc ../../Source/Log.cc ../../Source/Etcd.cc ../../Source/Util.cc
SRC_2 = ServiceConsumer.cc Calculate.pb.cc ../../Source/Brpc.cc ../../Source/Log.cc ../../Source/Etcd.cc ../../Source/Util.cc
REQUIRE =	-I ../../Include/ \
			-std=c++17 \
			-lbrpc \
			-lpthread \
			-lprotobuf \
			-lfmt \
			-lgflags \
			-lspdlog \
			-lleveldb \
			-ljsoncpp \
			-ldl \
			-lssl \
			-lcrypto \
			-lcpprest \
			-letcd-cpp-api

ALL : $(BOJ_1) $(BOJ_2)
$(BOJ_1) : $(SRC_1)
	g++ -o $@ $^ $(REQUIRE)
$(BOJ_2) : $(SRC_2)
	g++ -o $@ $^ $(REQUIRE)

.PHONY: clean
clean:
	rm -f $(BOJ_1) $(BOJ_2)