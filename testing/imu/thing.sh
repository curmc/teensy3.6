#!/usr/bin/env sh

######################################################################
# @author      : theo (theo.j.lincke@gmail.com)
# @file        : thing
# @created     : Friday Dec 20, 2019 13:29:56 MST
#
# @description : 
######################################################################


#!/bin/sh

# Usage: ./start_minicom 1 ACM # uses /dev/ttyACM1

#Start a minicom to show arduino serial output
# first argument to set the proper /dev/tty index
# second argument to set the name of /dev/tty index

Didx="0"
Dtype="USB"

if [ ! -z "$1" ]
then
  Didx="$1"
fi

if [ ! -z "$2" ]
then
  Dtype="$2"
fi


minicom -b 57600 -o -D "/dev/tty$Dtype$Didx"



