Proc = TestEmailSend
Src = TestEmailSend.cc ../../Source/EmailSend.cc ../../Source/Util.cc ../../Source/Log.cc
OPTION = -std=c++17 \
          -lpthread \
          -I ../../Include \
          -lgflags \
          -ljsoncpp \
		  -lcurl \
		  -lssl \
		  -lcrypto \
		  -lfmt \
		  -lspdlog 
$(Proc) : $(Src)
	g++ -o $@ $^ $(OPTION)
.PHONY:clean
clean:
	rm -f $(Proc)