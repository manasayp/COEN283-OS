all:	ThreadSafeHashMain.cpp HashTable.cpp hashUtils.cpp hashUtils.h HashTable.h
	g++ -o P1 ThreadSafeHashMain.cpp HashTable.cpp hashUtils.cpp hashUtils.h HashTable.h
debug:	ThreadSafeHashMain.cpp HashTable.cpp hashUtils.cpp hashUtils.h HashTable.h
	g++ -g -o P1 ThreadSafeHashMain.cpp HashTable.cpp hashUtils.cpp hashUtils.h HashTable.h
clean:
	rm -f *.o *~ P1 core
