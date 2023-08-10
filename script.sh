#!/bin/bash

for i in {1..10}
do
    gcc -o "client$i" "client.c"
    echo -n "Compilation completed successfully"
done