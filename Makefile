all:	threadSafeHashMain.cpp hashTable.cpp hashUtils.cpp hashUtils.h hashTable.h
	g++ -pthread -o P1 threadSafeHashMain.cpp hashTable.cpp hashUtils.cpp hashUtils.h hashTable.h
debug:	threadSafeHashMain.cpp hashTable.cpp hashUtils.cpp hashUtils.h hashTable.h
	g++ -pthread -g -o P1 threadSafeHashMain.cpp hashTable.cpp hashUtils.cpp hashUtils.h hashTable.h
clean:
	rm -f *.o *~ P1 core
