OUTDIR = ./out
CPP = clang++
CPPFLAGS = -Wall -std=c++17 -I$(INCDIR)
DEBUGFLAGS = -g
RELEASEFLAGS = -O3
LDFLAGS = 
BINDIR = $(OUTDIR)/bin
OBJDIR = $(OUTDIR)/obj
SRCDIR = src
INCDIR = include
FILES = main.cpp lexer.cpp token.cpp

all:
	make $(BINDIR)/debug/spl

$(BINDIR)/debug/spl: $(patsubst %.cpp,$(OBJDIR)/debug/%.o,$(FILES))
	@mkdir -p $(BINDIR)/debug
	$(CPP) $(DEBUGFLAGS) $(CPPFLAGS) -o $@ $^ $(LDFLAGS) 

$(OBJDIR)/debug/%.o: $(SRCDIR)/%.cpp
	@if [ ! -d $(OBJDIR)/debug ]; then \
		mkdir -p $(OBJDIR)/debug; \
	fi
	
	$(CPP) -c $(DEBUGFLAGS) $(CPPFLAGS) -o $@ $<

$(BINDIR)/release/spl: $(patsubst %.cpp,$(OBJDIR)/release/%.o,$(FILES))
	@mkdir -p $(BINDIR)/release
	$(CPP) $(RELEASEFLAGS) $(CPPFLAGS) -o $@ $^ $(LDFLAGS) 

$(OBJDIR)/release/%.o: $(SRCDIR)/%.cpp
	@if [ ! -d $(OBJDIR)/release ]; then \
		mkdir -p $(OBJDIR)/release; \
	fi
	
	$(CPP) -c $(RELEASEFLAGS) $(CPPFLAGS) -o $@ $<

run: $(BINDIR)/debug/spl $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(INCDIR)/*.h)
	./$<

release: $(BINDIR)/release/spl $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(INCDIR)/*.h)
debug: $(BINDIR)/debug/spl $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(INCDIR)/*.h)

clean:
	@if [ -d $(OUTDIR) ]; then \
		rm -rf $(OUTDIR); \
	fi
	@echo RM $(OUTDIR)
