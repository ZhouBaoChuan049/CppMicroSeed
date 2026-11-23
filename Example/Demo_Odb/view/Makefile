view:View.cc Student-odb.cxx
	g++ -std=c++17 $^ -o $@ -lodb-mysql -lodb -lodb-boost -lpthread
Student-odb.cxx: Student.h
	odb -d mysql --std c++11 --generate-query --generate-schema --profile boost/date-time $^
clean:
	rm -rf *-odb.*  *.sql view