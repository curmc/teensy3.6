#!/usr/bin/env sh

######################################################################
# @author      : theo (theo@$HOSTNAME)
# @file        : route
# @created     : Saturday Oct 05, 2019 19:09:34 MDT
#
# @description : Create a router
######################################################################

ip link set dev enp0s20f0u2 up
ip addr add 192.168.0.11 dev enp0s20f0u2
ip route add 192.168.0.0/25 via 192.168.0.11 dev enp0s20f0u2
