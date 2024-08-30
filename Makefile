include makefile_mini.mk


$(call mm_start_parameters_t,a)
$(call mm_start,a)

$(call mm_add_library_parameters_t,b)
b.filetypes:=EMMLibraryfiletype_Static
b.c:=test_mini.c
b.h:=test_mini.h
$(call mm_add_library,test-mini,b)

$(call mm_stop_parameters_t,e)
$(call mm_stop,e)
