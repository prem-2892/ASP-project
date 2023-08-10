#!/bin/bash

for i in {1..12}
do
    gcc -o "client$i" "client.c"
done
echo -n "Compilation completed successfully"