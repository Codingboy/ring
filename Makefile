LIBDIR=lib
OBJDIR=obj
INCLUDEDIR=include
SRCDIR=src
DOCDIR=doc
CFLAGS=-std=c++0x -g -Wall -c -Os -c -I\$(INCLUDEDIR)
CP=cp
RM=rm -f
RMDIR=rm -rf
MKDIR=mkdir -p
CC=g++
CHMOD=chmod -R 777

.PHONY: install clean uninstall

$(LIBDIR)/libring.a: $(OBJDIR)/ring.o
	$(MKDIR) $(LIBDIR)
	ar r $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(MKDIR) $(OBJDIR)
	$(CC) $(CFLAGS) -o $@ $<

install:
	$(CP) $(LIBDIR)/libring.a /usr/lib/libring.a
	$(CHMOD) /usr/lib/libring.a
	$(MKDIR) /usr/include/ring/
	$(CHMOD) /usr/include/ring/
	$(CP) $(INCLUDEDIR)/* /usr/include/ring/

uninstall:
	$(RMDIR) /usr/include/ring
	$(RM) /usr/lib/libring.a

clean:
	$(RMDIR) $(LIBDIR)
	$(RMDIR) $(OBJDIR)
	$(RMDIR) $(DOCDIR)
