#!/bin/sh
PATH=/bin:/usr/bin:/usr/sbin:/usr/local/bin

rsync -avzuHS --cvs-exclude -e ssh ./ sacsis@www.hpcc.jp:/usr/local/PHASE/WWW/data/sacsis/2005
