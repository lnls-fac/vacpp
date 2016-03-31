CXX = g++
MACHINE = -m64

OPTFLAG = -O3 -std=c++11
DBGFLAG = -O0 -g3 -std=c++11

INCDIR = include
SRCDIR = src
OBJDIR = build
SWIGDIR = swig
PKGDIR = package

ifeq ($(MAKECMDGOALS), debug)
MODEFLAG = $(DBGFLAG)
TGTDIR = debug
else
MODEFLAG = $(OPTFLAG)
TGTDIR = release
endif

LDFLAGS = $(MACHINE)
CFLAGS = $(MACHINE) $(MODEFLAG) $(DFLAGS) -pthread -fPIC

INC = -I./$(INCDIR) -I/usr/include/python3.4 -I../trackcpp/include/trackcpp
LIBS = -lpthread -ltrackcpp

SRCS = \
	driver.cpp \
	beamcharge.cpp \
	ring_model.cpp \
	sirius_models.cpp

BINSRC = main.cpp

OBJS = $(addprefix $(OBJDIR)/$(TGTDIR)/, $(SRCS:.cpp=.o))
BINOBJ = $(addprefix $(OBJDIR)/$(TGTDIR)/, $(BINSRC:.cpp=.o))

.NOTPARALLEL:

.PHONY: vacpp debug package swig doc clean


all: vacpp package

vacpp: $(OBJDIR)/$(TGTDIR)/vacpp

package: $(OBJDIR)/$(TGTDIR)/$(PKGDIR)/vacpp.py $(OBJDIR)/$(TGTDIR)/$(PKGDIR)/_vacpp.so

debug: vacpp

doc:
	cd doc; doxygen Doxyfile


# Find dependencies
$(shell $(CXX) -MM $(CFLAGS) $(INC) $(addprefix $(SRCDIR)/, $(SRCS)) | sed 's/.*\.o/$(OBJDIR)\/$(TGTDIR)\/&/' > .depend)
-include .depend


$(OBJDIR)/$(TGTDIR)/vacpp: $(OBJS) $(BINOBJ) | $(OBJDIR)/$(TGTDIR)
	$(CXX) $(LDFLAGS) $(OBJS) $(BINOBJ) $(LIBS) -o $@

$(OBJDIR)/$(TGTDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)/$(TGTDIR)
	$(CXX) -c $(CFLAGS) $(INC) $< -o $@


$(OBJDIR)/$(TGTDIR)/$(PKGDIR)/vacpp.py: $(SWIGDIR)/vacpp.py | $(OBJDIR)/$(TGTDIR)/$(PKGDIR)
	cp $(SWIGDIR)/vacpp.py $(OBJDIR)/$(TGTDIR)/$(PKGDIR)

$(OBJDIR)/$(TGTDIR)/$(PKGDIR)/_vacpp.so: $(OBJDIR)/$(TGTDIR)/libvacpp.so | $(OBJDIR)/$(TGTDIR)/$(PKGDIR)
	cp $(OBJDIR)/$(TGTDIR)/libvacpp.so $(OBJDIR)/$(TGTDIR)/$(PKGDIR)/_vacpp.so

$(OBJDIR)/$(TGTDIR)/libvacpp.so: $(OBJDIR)/$(TGTDIR)/vacpp_wrap.o $(OBJS) | $(OBJDIR)/$(TGTDIR)
	$(CXX) -shared $(LDFLAGS) $(OBJS)  $(OBJDIR)/$(TGTDIR)/vacpp_wrap.o $(LIBS) -o $@

$(OBJDIR)/$(TGTDIR)/vacpp_wrap.o: $(SWIGDIR)/vacpp_wrap.cxx | $(OBJDIR)/$(TGTDIR)
	$(CXX) -c $(CFLAGS) $(INC) $< -o $@

$(SWIGDIR)/vacpp.py $(SWIGDIR)/vacpp_wrap.cxx: $(SWIGDIR)/vacpp.i $(OBJS)
	swig -c++ -python $(INC) $(SWIGDIR)/vacpp.i

$(OBJDIR)/$(TGTDIR):
	mkdir -p $(OBJDIR)/$(TGTDIR)

$(OBJDIR)/$(TGTDIR)/$(PKGDIR):
	mkdir -p $(OBJDIR)/$(TGTDIR)/$(PKGDIR)

clean:
	-rm -rf build .depend $(SWIGDIR)/vacpp.py $(SWIGDIR)/vacpp_wrap.cxx
