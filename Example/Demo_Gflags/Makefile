BIN = Proc
SRC = Testgflags.cc Anotherfile.cc
$(BIN):$(SRC) 
	g++ -o $@ $^ -lgflags
.PHONY:clean
clean:
	rm -f $(BIN)