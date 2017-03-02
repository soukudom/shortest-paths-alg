#! /bin/bash

min=5
max=40
n=`cat $1 | head -1`
file=`echo "$2"`
matrix=`tail -n +2 $1`

echo "$n" > $file

for line in $matrix; do

line=`echo "$line" | sed -r 's/0/0 /g' | sed -r 's/1/1 /g'`

for c in $line; do
	if [ "$c" -eq 0 ]; then
		echo -n "0 " >> $file 
	elif [ "$c" -eq 1 ]; then
		rating=`shuf -i$min-$max -n1`
		echo -n "$rating " >> $file
	fi
done 
	echo "" >> $file
done
