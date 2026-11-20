REQUEST = -std=c++17 \
		  -lfmt \
		  -lspdlog \
		  -ljsoncpp \
		  -lfastcommon \
		  -lfdfsclient \
		  -pthread \
		  -I ../../Include/ 
Proc : TestFastDFS.cc ../../Source/FastDFS.cc ../../Source/Util.cc ../../Source/Log.cc
	g++ -o $@ $^ $(REQUEST)
.PHONY: clean
clean:
	rm -f Proc Download_file.txt Download_buffer.txt