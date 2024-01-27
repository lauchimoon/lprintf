#!/usr/bin/sh
CC=gcc
SRCS='lprintf-test.c lprintf.c'
OUT=lprintf-test

$CC $SRCS -o $OUT
