#!/bin/bash
let count 0
let sum 0
for param in "$@"
do
count=$(( $count + 1 ))
let 'sum = sum+param'
done

let 'ans = sum/count'
echo "$ans"
echo "$count"