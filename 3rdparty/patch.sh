#!/bin/bash

for headers in `find . -iname *.hpp`; do
	sed 's/\(\s\+\)emit\(\s\)/\1Q_EMIT\2/g' -i ${headers}
done

for sources in `find . -iname *.cpp`; do
	sed 's/\(\s\+\)emit\(\s\)/\1Q_EMIT\2/g' -i ${sources}
done
