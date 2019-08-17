Asembler: Asembler.o SecondIteration.o FirstIteration.o Utilities.o Data.o Entry.o Extern.o SymbolTable.o DocumentUtilities.o
	gcc -g -ansi -Wall -pedantic Asembler.o SecondIteration.o FirstIteration.o Utilities.o Data.o Entry.o Extern.o SymbolTable.o DocumentUtilities.o -o Asembler

Asembler.o: Asembler.c FirstIteration.h SecondIteration.h DocumentUtilities.h Constans.h Data.h SymbolTable.h Utilities.h
	gcc -c -ansi -Wall -pedantic Asembler.c -o Asembler.o

FirstIteration.o: FirstIteration.c FirstIteration.h Constans.h Data.h Entry.h Enums.h Extern.h Types.h Utilities.h DocumentUtilities.h SymbolTable.h
	gcc -c -ansi -Wall -pedantic FirstIteration.c -o FirstIteration.o

SecondIteration.o: SecondIteration.c SecondIteration.h Constans.h Data.h Entry.h Extern.h Types.h Utilities.h DocumentUtilities.h SymbolTable.h
	gcc -c -ansi -Wall -pedantic SecondIteration.c -o SecondIteration.o

Data.o: Data.c Data.h Types.h DocumentUtilities.h Constans.h SymbolTable.h
	gcc -c -ansi -Wall -pedantic Data.c -o Data.o

Entry.o: Entry.c Entry.h Types.h Constans.h SymbolTable.h DocumentUtilities.h
	gcc -c -ansi -Wall -pedantic Entry.c -o Entry.o

Extern.o: Extern.c Extern.h Types.h Constans.h SymbolTable.h DocumentUtilities.h
	gcc -c -ansi -Wall -pedantic Extern.c -o Extern.o

SymbolTable.o: SymbolTable.c SymbolTable.h Types.h Constans.h DocumentUtilities.h 
	gcc -c -ansi -Wall -pedantic SymbolTable.c -o SymbolTable.o

Utilities.o: Utilities.c Utilities.h Types.h Constans.h DocumentUtilities.h SymbolTable.h Extern.h
	gcc -c -ansi -Wall -pedantic Utilities.c -o Utilities.o

DocumentUtilities.o: DocumentUtilities.c DocumentUtilities.h Types.h Enums.h Constans.h
	gcc -c -ansi -Wall -pedantic DocumentUtilities.c -o DocumentUtilities.o

clean:
	rm *.o
