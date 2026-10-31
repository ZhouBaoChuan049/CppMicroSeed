Proc = CurlMail
Src = CurlMail.cc
OPTION = -lcurl -lssl -lcrypto
$(Proc) : $(Src)
	g++ -o $@ $^ $(OPTION)
.PHONY:clean
clean:
	rm -f $(Proc)