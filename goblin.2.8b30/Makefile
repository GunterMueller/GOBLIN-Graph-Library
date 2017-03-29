
####-----------------------------------------------------------------------####
####                           GOBLIN Makefile                             ####
####-----------------------------------------------------------------------####

# Targets (default is shell):
#
#   goblin      Compile and link the core library libgoblin.a
#   glpk        Compile and link the GLPK library libglpk.a
#   shared      Compile and link the shared objects
#               libgoblin.so and libglpk.so
#   shell       Compile and link the shell tool gosh
#   latex       Generate the reference manual mgoblin.<version>.ps and .pdf
#   html        Generate the html reference
#   srcpkg      Generate a source code tarball goblin.<version>.tgz
#   testpkg     Generate a test code tarball goblin.<version>.test.tgz
#   binpkg      Generate a binary distribution package
#               goblin.<version>.<platform>.tgz
#   install     Generate a binary distribution package and install
#               it to the system
#   private     Private installation to subdirectories ./bin and ./lib
#               PATH must be extended to ./bin manually
#   exe         Compile and link an executable solver
#               Environment variable pr passes the name of the executable
#   clean       Delete all generated files
#   sysclean    Delete a present system installation
#   privclean   Private installation plus deletion of all source files


####-----------------------------------------------------------------------####
####     Configuration (Edit Makefile.conf before using this makefile)     ####
####-----------------------------------------------------------------------####

### GOBLIN Version ###

major_version = 2
minor_version = 8
revision      = b30

# version specifiers

version = $(major_version).$(minor_version)
rel     = $(version)$(revision)


# All external libraries used in this project

LIBS = -lc -lm -lpthread -l$(libtcl) -l$(libtk)


include Makefile.conf


####-----------------------------------------------------------------------####
####                       End Makefile Configuration                      ####
####-----------------------------------------------------------------------####


# location of header files

lib_incl_path  = include
glpk_incl_path = glpk_include

# location of source code

lib_src_path   = lib_src
shell_src_path = shell_src
main_src_path  = main_src
glpk_src_path  = glpk_src
glpk_wrap_path = glpk_wrap

# makefile search path (not used at the moment!)

VPATH = $(lib_src_path) $(shell_src_path) $(main_src_path) \
        $(glpk_src_path) $(glpk_wrap_path)


# all files for core library

lib_sources = $(wildcard $(lib_src_path)/*.cpp)
lib_objects = $(patsubst %.cpp,%.o, $(lib_sources))


# all files for GLPK library

glpk_sources = $(wildcard $(glpk_src_path)/*.c)
glpk_objects = $(patsubst %.c,%.o, $(glpk_sources))


# all files for GLPK wrapper

wrap_sources = $(wildcard $(glpk_wrap_path)/*.cpp)
wrap_objects = $(patsubst %.cpp,%.o, $(wrap_sources))


# all files for shell tool

shell_sources = $(wildcard $(shell_src_path)/*.cpp)
shell_objects = $(patsubst %.cpp,%.o, $(shell_sources))


ifdef glpk
  gosh_objects = $(shell_objects) $(glpk_objects) \
                 $(wrap_objects) $(main_src_path)/goshAppInit2.o
else
  gosh_objects = $(shell_objects) \
                 $(main_src_path)/goshAppInit.o
endif


# all files for solver main procedures

solve_sources = $(wildcard $(main_src_path)/*.cpp)
solve_objects = $(patsubst %.cpp,%.o, $(solve_sources))


# all source files

SRCS = $(lib_sources) $(glpk_sources) $(wrap_sources) $(shell_sources) \
       $(solve_sources)


# all object files

OBJS = $(lib_objects) $(glpk_objects) $(wrap_objects) $(shell_objects) \
       $(solve_objects)


# all libraries to build

ifdef glpk

static_archives = libgoblin.a libglpk.a
shared_objects = libgoblin.$(so_suffix) libglpk.$(so_suffix)

else

static_archives = libgoblin.a
shared_objects = libgoblin.$(so_suffix)

endif


# some more short cuts for file and directory names

rel_depended = goblin.$(rel).build
conf_depended = goblin.$(version).conf

rname = goblin.$(rel)
vname = goblin.$(version)

iscript = goblin_install.sh
uscript = goblin_uninstall.sh

dpath = $(rname)/doku
ppath = $(rname)/project
hpath = $(rname)/html
vfile = $(ppath)/history


####-----------------------------------------------------------------------####
####                           Rules start here                            ####
####-----------------------------------------------------------------------####


# default target

.PHONY : silent
silent :
	@$(MAKE) -s all

.PHONY : all
all : private
	@echo


# make dependencies file .depend

.PHONY : depend create_depend
depend :
	rm -f .depend
	@$(MAKE) -s create_depend

# if .depend file is not existing the .depend file will be created automatically
# by the include statement at the end of this file; so do nothing
create_depend: ;

.depend :
	@rm -f .depend
	@touch .depend
ifeq ($(CC),gcc)
	@echo -e "$(lib_src_path)/\c" > dir.depend
	@for srcfile in $(lib_sources); do \
		cat dir.depend >> .depend; \
		$(CXX) $(CXXFLAGS) $(CMFLAGS) -I$(lib_incl_path) -I$(glpk_incl_path) -o part.depend $$srcfile; \
		cat part.depend >> .depend; \
	done
	@echo -e "$(glpk_src_path)/\c" > dir.depend;
	@for srcfile in $(glpk_sources); do \
		cat dir.depend >> .depend; \
		$(CXX) $(CXXFLAGS) $(CMFLAGS) -I$(lib_incl_path) -I$(glpk_incl_path) -o part.depend $$srcfile; \
		cat part.depend >> .depend; \
	done
	@echo -e "$(glpk_wrap_path)/\c" > dir.depend
	@for srcfile in $(wrap_sources); do \
		cat dir.depend >> .depend; \
		$(CXX) $(CXXFLAGS) $(CMFLAGS) -I$(lib_incl_path) -I$(glpk_incl_path) -o part.depend $$srcfile; \
		cat part.depend >> .depend; \
	done
	@echo -e "$(shell_src_path)/\c" > dir.depend
	@for srcfile in $(shell_sources); do \
		cat dir.depend >> .depend; \
		$(CXX) $(CXXFLAGS) $(CMFLAGS) -I$(lib_incl_path) -I$(glpk_incl_path) -o part.depend $$srcfile; \
		cat part.depend >> .depend; \
	done
	@echo -e "$(main_src_path)/\c" > dir.depend
	@for srcfile in $(solve_sources); do \
		cat dir.depend >> .depend; \
		$(CXX) $(CXXFLAGS) $(CMFLAGS) -I$(lib_incl_path) -I$(glpk_incl_path) -o part.depend $$srcfile; \
		cat part.depend >> .depend; \
	done
	@rm part.depend
	@rm dir.depend
else
	@for srcfile in $(lib_sources); do \
		makedepend -a -f'.depend' -I$(lib_incl_path) -I$(glpk_incl_path) $$srcfile; \
	done
	@echo -e "$(glpk_src_path)/\c" > dir.depend;
	@for srcfile in $(glpk_sources); do \
		makedepend -a -f'.depend' -I$(lib_incl_path) -I$(glpk_incl_path) $$srcfile; \
	done
	@echo -e "$(glpk_wrap_path)/\c" > dir.depend
	@for srcfile in $(wrap_sources); do \
		makedepend -a -f'.depend' -I$(lib_incl_path) -I$(glpk_incl_path) $$srcfile; \
	done
	@echo -e "$(shell_src_path)/\c" > dir.depend
	@for srcfile in $(shell_sources); do \
		makedepend -a -f'.depend' -I$(lib_incl_path) -I$(glpk_incl_path) $$srcfile; \
	done
	@echo -e "$(main_src_path)/\c" > dir.depend
	@for srcfile in $(solve_sources); do \
		makedepend -a -f'.depend' -I$(lib_incl_path) -I$(glpk_incl_path) $$srcfile; \
	done
endif
	@echo "...Dependencies generated"


.PHONY : intro newline
intro : $(conf_depended)
	@echo -e "\nGOBLIN Makefile (Release $(rel)) ...\n"


# generic compilation rule

$(filter-out %/goblinController.o,$(lib_objects)) : %.o : %.cpp
	$(CXX) $(CXXFLAGS) -I$(lib_incl_path) -c $< -o $@
	@echo -n .


# compile with version string

$(lib_src_path)/goblinController.o : goblinController.cpp $(rel_depended)
	$(CXX) $(CXXFLAGS) -I$(lib_incl_path) -c $< -o $@
	@echo -n .


# compile with tcl headers

$(shell_objects) $(solve_objects) : %.o : %.cpp
	$(CXX) $(CXXFLAGS) -I$(lib_incl_path) $(tcl_flags) -c $< -o $@
	@echo -n .


# compile with GLPK headers

$(glpk_objects) : %.o : %.c
	$(CC) $(CCFLAGS) -I$(glpk_incl_path) -c $< -o $@
	@echo -n .

$(wrap_objects) : %.o : %.cpp
	$(CXX) $(CXXFLAGS) -I$(lib_incl_path) -I$(glpk_incl_path) $(tcl_flags) -c $< -o $@
	@echo -n .



# build GOBLIN library

.PHONY : goblin

ifeq ($(link_mode),static)

goblin : intro libgoblin.a

else

goblin : intro libgoblin.a libgoblin.$(so_suffix)

endif


# build GLPK library

.PHONY : glpk

ifeq ($(link_mode),static)

glpk : intro libglpk.a

else

glpk : intro libglpk.a libglpk.$(so_suffix)

endif



# build static archives

.PHONY : archives
archives : intro $(static_archives)

libgoblin.a : $(lib_objects)
	ar rc $@ $?
	@echo "...Archive ($@) has been successfully built"

libglpk.a : $(glpk_objects)
	ar rc $@ $?
	@echo "...Archive ($@) has been successfully built"



# build shared objects

.PHONY : shared
shared : intro $(shared_objects)

libgoblin.$(so_suffix) : $(lib_objects) $(shell_objects)
ifeq ($(CC),gcc)
	$(LD) -shared -Wl,-soname,$@ -o $@ $^ $(LDFLAGS) $(LIBS)
else
	$(LD) -G                     -o $@ $^ $(LDFLAGS) $(LIBS)
endif
	@echo "...Shared object ($@) has been successfully linked"

libglpk.$(so_suffix) : $(glpk_objects) $(wrap_objects)
ifeq ($(CC),gcc)
	$(LD) -shared -Wl,-soname,$@ -o $@ $(shell_objects) $^ -L. $(LDFLAGS) $(LIBS) -lgoblin
else
	$(LD) -G                     -o $@ $(shell_objects) $^ -L. $(LDFLAGS) $(LIBS) -lgoblin
endif
	@echo "...Shared object ($@) has been successfully linked"



# link shell tool

.PHONY : shell
shell : intro gosh

ifeq ($(link_mode),static)

gosh : $(gosh_objects) $(static_archives)
	$(LD) -o $@ $(gosh_objects) -L. $(LDFLAGS) -lgoblin $(glpk) $(LIBS)
	@echo "Gosh objects ($(gosh_objects))"
	@echo "...Shell tool ($@) has been linked with static archive"

else

gosh : $(gosh_objects) $(shared_objects)
ifeq ($(CC),gcc)
	$(LD)    -o $@ $(gosh_objects) -L. $(LDFLAGS) -lgoblin $(glpk) $(LIBS)
else
	$(LD) -G -o $@ $(gosh_objects) -L. $(LDFLAGS) -lgoblin $(glpk) $(LIBS)
endif
	@echo "...Shell tool ($@) has been linked with shared object"

endif


# link executable

.PHONY : exe
.SILENT : exe
exe : intro $(pr)
	@echo -e "...Solver '$(pr)' has been successfully built\n"

$(pr) : $(pr).cpp $(static_archives)
	$(CXX) $(CXXFLAGS) -I$(lib_incl_path) -c $< -o $(main_src_path)/$(pr).o
	$(LD) $(main_src_path)/$(pr).o -o $(pr) -L. $(LDFLAGS) -lgoblin $(glpk) $(LIBS)



# link test project

try : $(static_archives) try.o
	$(LD) $(main_src_path)/try.o -o try -L. $(LDFLAGS) -lgoblin $(glpk) $(LIBS)



# delete generated files

.PHONY : clean
.SILENT : clean
clean : intro
	rm -f $(OBJS) gosh libgoblin.a libglpk.a libgoblin.$(so_suffix) libglpk.$(so_suffix)
	rm -f mgoblin.$(rel).ps $(iscript) $(uscript) $(rel_depended)
	rm -f mgoblin.$(rel).pdf
	@echo -e "...All object files have been deleted\n"



# generate reference manual

.PHONY : latex
.SILENT : latex
latex : intro
	sh doku/manual.sh $(rel)
	@echo -e "...Reference manual generated (mgoblin.$(rel).ps)\n"



# generate doxygen html

.PHONY : html
.SILENT : html
html : intro
	doxygen doku/doxygen/doxygen.cfg
	@echo -e "...Doxygen html generated\n"



# generate source code package

.PHONY : srcpkg
.SILENT : srcpkg
srcpkg : intro
	rm -f -r $(rname)
	mkdir $(rname)
	cp goblet $(rname)
	cp Makefile $(rname)
	cp Makefile.conf $(rname)
	cp .depend $(rname)
	cp display $(rname)
	mkdir $(rname)/$(lib_src_path)
	cp $(lib_src_path)/*.cpp $(rname)/$(lib_src_path)
	mkdir $(rname)/$(shell_src_path)
	cp $(shell_src_path)/*.cpp $(rname)/$(shell_src_path)
	mkdir $(rname)/$(glpk_src_path)
	cp $(glpk_src_path)/*.c $(rname)/$(glpk_src_path)
	mkdir $(rname)/$(glpk_wrap_path)
	cp $(glpk_wrap_path)/*.cpp $(rname)/$(glpk_wrap_path)
	mkdir $(rname)/$(main_src_path)
	cp $(main_src_path)/*.cpp $(rname)/$(main_src_path)
	mkdir $(rname)/$(lib_incl_path)
	cp $(lib_incl_path)/*.h $(rname)/$(lib_incl_path)
	mkdir $(rname)/$(glpk_incl_path)
	cp $(glpk_incl_path)/*.h $(rname)/$(glpk_incl_path)
	mkdir $(rname)/tcl
	cp tcl/* $(rname)/tcl
	mkdir $(rname)/samples
	cp samples/*.gob $(rname)/samples
	mkdir $(ppath)
	cp project/change.log $(ppath)
	cp project/to.do $(ppath)
	cp project/licence $(ppath)
	cp project/report.form $(ppath)
	cp project/history $(vfile)
	echo "" >> $(vfile)
	echo "Release "$(rel)" of " >> $(vfile)
	date >> $(vfile)
	mkdir $(dpath)
	mkdir $(dpath)/doxygen
	cp doku/doxygen/doxygen.cfg $(dpath)/doxygen
	cp doku/doxygen/doxygen.css $(dpath)/doxygen
	cp doku/doxygen/header.html $(dpath)/doxygen
	cp doku/doxygen/footer.html $(dpath)/doxygen
	mkdir $(dpath)/gif
	cp doku/gif/*.gif $(dpath)/gif
	touch $(dpath)/manual.ind
	cp doku/*.tex $(dpath)
	mkdir $(dpath)/eps
	cp doku/eps/*.eps $(dpath)/eps
	cp doku/*.eps $(dpath)
	cp doku/*.ps $(dpath)
	cp doku/manual.sh $(dpath)
	mkdir $(hpath)
	cp html/index.html $(hpath)
	cp html/menu.html $(hpath)
	cp html/headline.html $(hpath)
	cp html/logo.html $(hpath)
	cp html/hostref.html $(hpath)
	cp html/wallpaper.jpg $(hpath)
	cp html/logo.gif $(hpath)
	$(tar) cf $(rname).tar $(rname)
	gzip -f $(rname).tar
	mv $(rname).tar.gz $(rname).tgz
	rm -f -r $(rname)
	@echo -e "...Source code distribution generated ($(rname).tgz)\n"



# generate binaries package

.SILENT : $(iscript)
$(iscript) :
	echo "#!/bin/sh" > $(iscript)
	echo "" >> $(iscript)
	echo "if test -e $(goblin_bin_path)/$(uscript)" >> $(iscript)
	echo "then sh $(goblin_bin_path)/$(uscript)" >> $(iscript)
	echo "fi" >> $(iscript)
	echo "" >> $(iscript)
	echo "$(tar)  --extract --directory=/ --file=$(tmp_path)goblin.tmp.tar" >> $(iscript)
	echo "" >> $(iscript)
	echo "chmod 755 $(goblin_bin_path)/$(vname)/*" >> $(iscript)
	echo "chmod 755 $(goblin_bin_path)/goblet" >> $(iscript)
	echo "chmod 755 $(goblin_bin_path)/$(uscript)" >> $(iscript)
	echo "chmod 644 $(goblin_include_path)/$(vname)/*.h" >> $(iscript)
	echo "" >> $(iscript)
	echo "rm $(tmp_path)goblin.tmp.tar" >> $(iscript)
	echo "rm $(tmp_path)$(iscript)" >> $(iscript)
	echo "echo ...GOBLIN $(version) system installation complete" >> $(iscript)
	echo "echo" >> $(iscript)
	chmod 755 $(iscript)



.PHONY : binpkg
.SILENT : binpkg
binpkg : shell $(static_archives) $(shared_objects) $(iscript) $(uscript)
	rm -f -r $(vname) && mkdir $(vname)
	mkdir -p $(vname)$(goblin_bin_path)/$(vname)
	mkdir -p $(vname)$(goblin_lib_path)
	mkdir -p $(vname)$(goblin_include_path)/$(vname)
	cp libgoblin.a $(vname)$(goblin_lib_path)
ifdef glpk
	cp libglpk.a $(vname)$(goblin_lib_path)
endif
ifneq ($(link_mode),static)
ifneq ($(os),cygwin)
	cp libgoblin.$(so_suffix) $(vname)$(goblin_lib_path)
ifdef glpk
	cp libglpk.$(so_suffix) $(vname)$(goblin_lib_path)
endif
else
	cp libgoblin.$(so_suffix) $(vname)$(goblin_bin_path)
ifdef glpk
	cp libglpk.$(so_suffix) $(vname)$(goblin_bin_path)
endif
endif
endif
ifneq ($(link_mode),dynamic)
	cp gosh$(exe_suffix) $(vname)$(goblin_bin_path)
endif
	cp goblet $(vname)$(goblin_bin_path)
ifneq ($(link_mode),dynamic)
	echo "#!$(goblin_bin_path)/gosh" > $(vname)$(goblin_bin_path)/goblet
else
	echo "#!$(wish_name)" > $(vname)$(goblin_bin_path)/goblet
	echo "" >> $(vname)$(goblin_bin_path)/goblet
	echo "load $(goblin_lib_path)/libgoblin.$(so_suffix) goblin" >> $(vname)$(goblin_bin_path)/goblet
ifdef glpk
	echo "load $(goblin_lib_path)/libglpk.$(so_suffix) glpk" >> $(vname)$(goblin_bin_path)/goblet
endif
endif
	echo "" >> $(vname)$(goblin_bin_path)/goblet
	echo "set GoblinVersion $(version)" >> $(vname)$(goblin_bin_path)/goblet
	echo "set GoblinTclPath [file join $(goblin_bin_path) $(vname)]" \
	       >> $(vname)$(goblin_bin_path)/goblet
	echo "" >> $(vname)$(goblin_bin_path)/goblet
	cat goblet >> $(vname)$(goblin_bin_path)/goblet
	mv $(uscript) $(vname)$(goblin_bin_path)
	cp tcl/* $(vname)$(goblin_bin_path)/$(vname)
	cp $(lib_incl_path)/*.h $(vname)$(goblin_include_path)/$(vname)
	$(tar) --create --directory=$(vname) --file=goblin.tmp.tar \
	    ./$(goblin_bin_path) ./$(goblin_lib_path) ./$(goblin_include_path)
	$(tar) cf $(rname).$(os).tar $(iscript) goblin.tmp.tar
ifeq ($(zip),gz)
	gzip -f $(rname).$(os).tar
	mv -f $(rname).$(os).tar.gz $(rname).$(os).tgz
	@echo -e "...Binary distribution generated ($(rname).$(os).tgz)\n"
else
	bzip2 -f $(rname).$(os).tar
	mv -f $(rname).$(os).tar.bz2 $(rname).$(os).tbz2
	@echo -e "...Binary distribution generated ($(rname).$(os).tbz2)\n"
endif
	rm -f goblin.tmp.tar
	rm -f -r $(vname)
	rm -f $(iscript)



# generate unit test code package

.PHONY : testpkg
.SILENT : testpkg
testpkg : intro
	rm -f -r $(rname)
	mkdir $(rname)
	mkdir $(rname)/test
	cp test/*.tcl $(rname)/test
	cp -a test/input $(rname)/test/input
	cp -a test/scripts $(rname)/test/scripts
	cd $(rname) ; \
	$(tar) cf $(rname).test.tar test ; \
	gzip -f $(rname).test.tar ; \
	mv $(rname).test.tar.gz ../$(rname).test.tgz ; \
	cd ..
	rm -f -r $(rname)
	@echo -e "...Unit test code distribution generated ($(rname).test.tgz)\n"



# system installation simulates the installation procedure of a binary package

.PHONY : install
.SILENT : install
install : binpkg
ifeq ($(zip),gz)
	cp -f $(rname).$(os).tgz $(tmp_path)
	gunzip -f $(tmp_path)$(rname).$(os).tgz
else
	cp -f $(rname).$(os).tbz2 $(tmp_path)
	bunzip2 -f $(tmp_path)$(rname).$(os).tbz2
endif
	$(tar) --extract --directory=$(tmp_path) --file=$(tmp_path)$(rname).$(os).tar
	sh $(tmp_path)$(iscript)
	rm -f $(tmp_path)$(rname).$(os).*



# Private installation to ./bin and ./lib, using ./tcl

.PHONY : private privclean
.SILENT : private privclean

private : shell goblin glpk
	mkdir -p lib
	mv libgoblin.a lib
ifdef glpk
	mv libglpk.a lib
endif
ifneq ($(link_mode),static)
	mv libgoblin.$(so_suffix) lib
ifdef glpk
	mv libglpk.$(so_suffix) lib
endif
endif
	mkdir -p bin
ifneq ($(link_mode),dynamic)
	mv gosh$(exe_suffix) bin
	echo "#!$(PWD)/bin/gosh" > bin/goblet
else
	echo "#!$(wish_name)" > bin/goblet
	echo "" >> bin/goblet
	echo "load $(PWD)/lib/libgoblin.$(so_suffix) goblin" >> bin/goblet
ifdef glpk
	echo "load $(PWD)/lib/libglpk.$(so_suffix) glpk" >> bin/goblet
endif
endif
	echo "" >> bin/goblet
	echo "set GoblinVersion $(version)" >> bin/goblet
	echo "set GoblinTclPath [file join $(PWD) tcl]" \
	       >> bin/goblet
	echo "" >> bin/goblet
	cat goblet >> bin/goblet
	chmod 755 bin/goblet
	@echo -e "...GOBLIN $(version) private installation complete\n"


privclean : intro private
	rm -f -r lib_src
	rm -f -r shell_src
	rm -f -r main_src
	rm -f -r glpk_src
	rm -f -r glpk_wrap
	rm -f *.*
	rm -f Makefile
	rm -f goblet
	rm -f gosh
	rm -f display
	@echo -e "...All source files have been deleted\n"


# Adjust file generation times

.PHONY : touch
touch : 
	touch lib_src/*
	touch shell_src/*
	touch main_src/*
	touch glpk_src/*
	touch glpk_wrap/*
	touch include/*
	touch .depend
	touch Makefile.conf
	rm goblin.*.conf


# system deinstallation

.SILENT : $(uscript) sysclean
$(uscript) :
	echo "#!/bin/sh" > $(uscript)
	echo "" >> $(uscript)
	echo "rm -f -r $(goblin_bin_path)/goblin.$(version)" >> $(uscript)
ifneq ($(link_mode),dynamic)
	echo "rm -f $(goblin_bin_path)/gosh$(exe_suffix)" >> $(uscript)
endif
	echo "rm -f $(goblin_bin_path)/goblet" >> $(uscript)
	echo "rm -f -r $(goblin_include_path)/goblin.$(version)" >> $(uscript)
	echo "rm -f $(goblin_lib_path)/libgoblin.a" >> $(uscript)
ifdef glpk
	echo "rm -f $(goblin_lib_path)/libglpk.a" >> $(uscript)
endif
ifneq ($(link_mode),static)
ifneq ($(os),cygwin)
	echo "rm -f $(goblin_lib_path)/libgoblin.$(so_suffix)" >> $(uscript)
ifdef glpk
	echo "rm -f $(goblin_lib_path)/libglpk.$(so_suffix)" >> $(uscript)
endif
else
	echo "rm -f $(goblin_bin_path)/libgoblin.$(so_suffix)" >> $(uscript)
ifdef glpk
	echo "rm -f $(goblin_bin_path)/libglpk.$(so_suffix)" >> $(uscript)
endif
endif
endif
	echo "rm -f $(goblin_bin_path)/$(uscript)" >> $(uscript)
	chmod 755 $(uscript)
	echo "echo ...GOBLIN $(version) system deinstallation complete" >> $(uscript)
	echo "echo" >> $(uscript)

.PHONY : sysclean
sysclean : intro
	sh $(goblin_bin_path)/$(uscript)



# detecting changes of version/revision number and Makefile.conf

versionHeader = $(lib_incl_path)/version.h

.SILENT : $(rel_depended) $(conf_depended)
$(rel_depended) :
	rm -f goblin.*.build
	@echo "Do not delete this file! It is used by Make to detect version/revision number changes." > $@
	@echo "/// \file   version.h" > $(versionHeader)
	@echo "/// \brief  Macro version information to be compiled into the core library" >> $(versionHeader)
	@echo "" >> $(versionHeader)
	@echo "#ifndef _VERSION_H_" >> $(versionHeader)
	@echo "#define _VERSION_H_" >> $(versionHeader)
	@echo "" >> $(versionHeader)
	@echo "#define _GOBLIN_MAJOR_VERSION_ $(major_version)" >> $(versionHeader)
	@echo "#define _GOBLIN_MINOR_VERSION_ $(minor_version)" >> $(versionHeader)
	@echo "#define _GOBLIN_REVISION_ \"$(revision)\"" >> $(versionHeader)
	@echo "" >> $(versionHeader)
	@echo "#endif" >> $(versionHeader)


#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_


$(conf_depended) : Makefile.conf
	rm -f $(OBJS) lib*.$(so_suffix) lib*.a gosh goblin.*.conf
	@echo "Do not delete this file! It is used by Make to detect configuration changes." > $@


include .depend
