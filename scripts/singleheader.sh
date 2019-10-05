#!/bin/bash

if [ -f hashmap.h ]; then
    echo 'hashmap.h already exists'
    exit
fi

[ ! -d lib ] && mkdir lib

touch lib/hashmap.h
cat inc/hashmap.h >> lib/hashmap.h
sed -e 's/\(#include "hashmap.h"\|#include <stddef.h>\)//g' inc/map_iter.h >> lib/hashmap.h