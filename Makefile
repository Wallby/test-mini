ifndef OS # linux
EXECUTABLE_EXTENSION=.exe
LIBRARY_EXTENSION=.a
RM=rm -f $(1)
else ifeq ($(OS), Windows_NT) # windows
EXECUTABLE_EXTENSION=
LIBRARY_EXTENSION=.lib
RM=if exist $(1) del $(1)
else
$(error os not supported)
endif


libtest-mini$(LIBRARY_EXTENSION): test_mini.o
	$(call RM,libtest-mini$(LIBRARY_EXTENSION))
	ar rcs libtest-mini$(LIBRARY_EXTENSION) test_mini.o

test_mini.o: test_mini.c
	gcc -c test_mini.c

test$(EXECUTABLE_EXTENSION): test.o libtest-mini$(LIBRARY_EXTENSION)
	gcc -Wl,--wrap=malloc,--wrap=free,--wrap=main -o test$(EXECUTABLE_EXTENSION) test.o -L ./ -ltest-mini

test.o: test.c
	gcc -c test.c

#******************************************************************************

release: test$(EXECUTABLE_EXTENSION)
	./test$(EXECUTABLE_EXTENSION)

# don't associate clean with a file with filename "clean"
# v
.PHONY: clean
clean:
	$(call RM,test_mini.o)
	$(call RM,libtest_mini.a)
	$(call RM,libtest_mini.lib)
	
	$(call RM,test.o)
	$(call RM,test)
	$(call RM,test.exe)