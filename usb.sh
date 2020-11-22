#!/bin/bash

grep 10c4 /sys/bus/usb/devices/*/idVendor

sudo echo '0' > /sys/bus/hid/drivers_autoprobe
sudo echo "3-2:1.0" >  /sys/bus/usb/drivers/usbhid/unbind
sudo echo "3-2:1.0" >  /sys/bus/usb/drivers/pen_driver/bind
