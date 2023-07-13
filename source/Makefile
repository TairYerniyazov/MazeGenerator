# Plik Makefile
#----------------------------------------------------------------------
# Autor: Tair Yerniyazov,                            Krakow, 30.05.2023
#----------------------------------------------------------------------

.SUFFIXES: .cpp .o .x .hpp
DIR = $(notdir $(CURDIR))
SYSNAME = $(shell uname -s)

# Nazwy plików źródłowych
GRAPH = graph

# Pliki wykonywalne
EXEC_GRAPH = $(GRAPH).x

# Pliki powstałe po kompilacji plikow źródłowych
OBJS_GRAPH = $(GRAPH).o

# Pliki nagłówkowe
GRAPH_HEADER = graph.hpp
SETS_HEADER = sets.hpp

# Opcje kompilatora i linkera;
CXXFLAGS = $(LDFLAGS) -c
LDFLAGS = -Wall -std=c++11 -O3 -march=native

# Definiowanie reguł zależności plików wykonywalnych
%.x: %.o
	$(CXX) $(LDFLAGS) -o $@ $^

# Definiowanie reguł zależności plików obiektowych
%.o: %.cpp $(GRAPH_HEADER) $(SETS_HEADER)
	$(CXX) $(CXXFLAGS) -o $@ $<

# Akcje
.PHONY: all clean tar
all: $(EXEC_GRAPH)
	./$(EXEC_GRAPH) 

# Zostawienie plików obiektowych po wykonaniu programu
.PRECIOUS: %.o

# Czyszczenie
clean:
	$(RM) *.o *.x

# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR))
