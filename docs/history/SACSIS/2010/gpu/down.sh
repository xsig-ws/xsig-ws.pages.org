#!/bin/sh
ssh -i test endo@131.112.35.1 "scp -r boogie:public_html/2010tmp/gpu ./tmp/"
scp -r -i test endo@131.112.35.1:tmp/gpu/* ./
