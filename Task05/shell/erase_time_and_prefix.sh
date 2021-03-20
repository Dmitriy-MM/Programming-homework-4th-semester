#!/bin/bash

sed -i -e 's/Elapsed = [0-9]*.[0-9]*//' $1
sed -i -e 's/^.*\\\([^\\]\+.out\)/.\/\1/' $1