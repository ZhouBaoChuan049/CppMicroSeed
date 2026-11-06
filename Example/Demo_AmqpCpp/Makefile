OPTIONS = -lamqpcpp -lev -lpthread
all: SimplePublisher SimlpeSubscriber 
SimplePublisher : SimplePublisher.cc
	g++ -std=c++17 $^ -o $@ $(OPTIONS)
SimlpeSubscriber  : SimlpeSubscriber.cc
	g++ -std=c++17 $^ -o $@ $(OPTIONS)
.PHONY : clean
clean:
	rm -rf SimplePublisher SimlpeSubscriber  