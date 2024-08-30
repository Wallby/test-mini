include makefile_mini.mk


$(call mm_start_parameters_t,a)
a.ignoredbinaries:=^test$(MM_EXECUTABLE_EXTENSION)$$
$(call mm_start,a)

$(call mm_add_library_parameters_t,b)
b.filetypes:=EMMLibraryfiletype_Static
b.c:=test_mini.c
b.h:=test_mini.h
$(call mm_add_library,test-mini,b)

$(call mm_add_executable_parameters_t,c)
c.c:=test.c
c.gccOrG++:=-Wl,--wrap=malloc,--wrap=free,--wrap=main
c.libraries:=test-mini
$(call mm_add_executable,test,c)

$(call mm_add_test_parameters_t,d)
d.executables:=test
$(call mm_add_test,test,d)

$(call mm_stop_parameters_t,e)
$(call mm_stop,e)
