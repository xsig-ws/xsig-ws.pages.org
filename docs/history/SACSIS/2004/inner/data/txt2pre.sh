#!/bin/sh

#for f in *.txt; do
for f in $*; do
  h=`echo $f | sed s/\.txt/\.html/`
  echo "<html><body bgcolor=\"white\"><pre>" > $h
  cat $f >> $h
  echo "</pre></body></html>" >> $h
done
