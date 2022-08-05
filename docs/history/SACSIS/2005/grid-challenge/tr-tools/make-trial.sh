#!/bin/sh

PICMAKE=`dirname $0`/make-tr-pic
PICCOUNT=`dirname $0`/make-tr-ans

echo Problem 1
mkdir data01
$PICMAKE data01 1 4 4 512 512 0
$PICCOUNT data01 1 4 4

echo Problem 2
mkdir data02
$PICMAKE data02 2 10 10 4096 4096 0
$PICCOUNT data02 2 10 10

echo Problem 3
mkdir data03
$PICMAKE data03 3 10 10 8192 8192 0
$PICCOUNT data03 3 10 10


echo Problem 4
mkdir data04
$PICMAKE data04 4 4 4 512 512 1
$PICCOUNT data04 4 4 4

echo Problem 5
mkdir data05
$PICMAKE data05 5 10 10 4096 4096 1
$PICCOUNT data05 5 10 10

echo Problem 6
mkdir data06
$PICMAKE data06 6 10 10 8192 8192 1
$PICCOUNT data06 6 10 10

