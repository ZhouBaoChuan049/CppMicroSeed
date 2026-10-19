OBJ = Proc
SRC = TestTranscoder.cc ../../Source/Log.cc ../../Source/Util.cc ../../Source/HLSTranscoder.cc
REQUIRE = -std=c++17 \
          -lavformat \
          -lavcodec \
          -lavutil \
          -lpthread \
          -I ../../Include \
          -lspdlog \
          -lfmt \
          -lgflags \
		  -ljsoncpp

$(OBJ) : $(SRC)
	g++ -o $@ $^ $(REQUIRE)

.PHONY: clean
clean:
	rm -f $(OBJ)