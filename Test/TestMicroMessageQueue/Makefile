BINS = SimplePublisher SimpleSubscriber DLXPublisher DLXSubscriber
COMMON_SRC = ../../Source/MessageQueue.cc ../../Source/Log.cc
REQUIRE = -std=c++17 -I ../../Include/ -lamqpcpp -lev -lpthread -lspdlog -lfmt

ALL : $(BINS)
SimplePublisher : TestSimpleMQPublisher.cc $(COMMON_SRC)
	g++ -o $@ $^ $(REQUIRE)
SimpleSubscriber : TestSimpleMQSubscriber.cc $(COMMON_SRC)
	g++ -o $@ $^ $(REQUIRE)
DLXPublisher : TestDLXMQPublisher.cc $(COMMON_SRC)
	g++ -o $@ $^ $(REQUIRE)
DLXSubscriber : TestDLXMQSubscriber.cc $(COMMON_SRC)
	g++ -o $@ $^ $(REQUIRE)
.PHONY: clean
clean:
	rm -f $(BINS)
