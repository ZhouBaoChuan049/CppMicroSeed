OPTIONS = 	-I ../../Include/ \
			-std=c++17 \
			-lpthread \
			-lspdlog \
			-lfmt \
			-lgflags \
			-ljsoncpp \
			-lcpprest \
			-letcd-cpp-api

ALL : Provider Watcher
Provider : TestEtcdProvider.cc ../../Source/Log.cc ../../Source/Util.cc ../../Source/Etcd.cc
	g++ -o $@ $^ $(OPTIONS)
Watcher : TestEtcdWatcher.cc ../../Source/Log.cc ../../Source/Util.cc ../../Source/Etcd.cc
	g++ -o $@ $^ $(OPTIONS)
.PHONY: clean
clean: 
	rm -f Provider Watcher