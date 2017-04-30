#!/bin/bash

# Captures images from an arbitrary URL, usually a web cam.

# Delay between captures, in seconds
DELAY=30

# Number of images to capture
COUNT=3000

# The URL from where to capture the images
URL="http://server/path"

for i in $(seq 1 $COUNT)
do
   IMGFILE="${i}.jpg"
   # echo $IMGFILE
   wget -O ${IMGFILE} ${URL}
   sleep ${DELAY}
done
