#!/bin/bash

grep 0079 /sys/bus/usb/devices/*/idVendor

sudo echo '0' > /sys/bus/hid/drivers_autoprobe
sudo echo "1-3:1.0" >  /sys/bus/usb/drivers/usbhid/unbind
sudo echo "1-3:1.0" >  /sys/bus/usb/drivers/pen_driver/bind
