#!/bin/bash
#  Short script to toggle a GPIO pin at the highest frequency
#  possible using Bash - by Derek Molloy
echo 17 > /sys/class/gpio/export
sleep 0.5
echo 5 > /sys/class/gpio/export
sleep 0.5
echo "in" > /sys/class/gpio/gpio17/direction
echo "out" > /sys/class/gpio/gpio5/direction
top
while true
do
    VAR=$(cat /sys/class/gpio/gpio17/value)
    if [ $VAR == '1' ]
    then
        echo 1 > /sys/class/gpio/gpio5/value
    fi
    echo 0 > /sys/class/gpio/gpio5/value
done

echo 17 > /sys/class/gpio/unexport



