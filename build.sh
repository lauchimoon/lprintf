#!/usr/bin/sh
CC=gcc
SRCS=main.c lprintf.c
OUT=lprintf-test

$CC $SRCS -o $OUT
