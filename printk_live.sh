#!/bin/bash
#
# invoke this script to show your dmesg live
# this this very nice and smooth for debugging
#
# Refernces:
# LDD3 P: 77
# https://stackoverflow.com/a/51783894/5688267
sudo echo "8" > /proc/sys/kernel/printk
sudo dmesg -wH &
