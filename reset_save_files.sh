#!/bin/bash

set -e

if [ ! -f settings.txt ] || [ "$1" == "" ]; then
    printf "scale=10\nfov=90\nrender-distance=200\nmap-id=map0\nwall-color=1,1,1\nvsync=true\nrotation-sensitivity=100\nmovement-sensitivity=2\n" > settings.txt
fi
