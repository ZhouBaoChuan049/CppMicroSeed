ALL: upload download remove
upload:Upload.cc
	g++ -std=c++11 $^ -o $@ -lfdfsclient -lfastcommon -pthread
download:Download.cc
	g++ -std=c++11 $^ -o $@ -lfdfsclient -lfastcommon -pthread
remove:Remove.cc
	g++ -std=c++11 $^ -o $@ -lfdfsclient -lfastcommon -pthread
.PHONY:clean
clean:
	rm -rf upload download remove Buffer.bak