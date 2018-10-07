# dict_roman/Makefile

EXTENSION = dict_roman
PGFILEDESC = "Dictionary for roman dates"

MODULE_big = dict_roman
DOCS = README.dict_roman
REGRESS = dict_roman
OBJS= dict_roman.o
DATA = dict_roman--1.0.sql

ifdef USE_PGXS
PGXS = $(shell pg_config --pgxs)
include $(PGXS)
else
subdir = contrib/dict_regex
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

include $(top_srcdir)/contrib/contrib-global.mk
endif
