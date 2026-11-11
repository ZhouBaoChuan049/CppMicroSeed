OPTIONS = -lamqpcpp -lev -lpthread
all: SimplePublisher SimlpeSubscriber DeadPublisher DeadSubscriber
SimplePublisher : SimplePublisher.cc
	g++ -std=c++17 $^ -o $@ $(OPTIONS)
SimlpeSubscriber  : SimlpeSubscriber.cc
	g++ -std=c++17 $^ -o $@ $(OPTIONS)
DeadPublisher : DeadPublisher.cc
	g++ -std=c++17 $^ -o $@ $(OPTIONS)
DeadSubscriber : DeadSubscriber.cc
	g++ -std=c++17 $^ -o $@ $(OPTIONS)
.PHONY : clean
clean:
	rm -rf SimplePublisher SimlpeSubscriber DeadPublisher DeadSubscriber