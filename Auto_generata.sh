#! /bin/bash


./GeneratorWithRating.sh 1500nodes10edges task1/data/nodes1500Edge10 #nodes\=100AvgEdge\=25 task1/data/nodes1000Edge25
./GeneratorWithRating.sh 1500nodes50edges task1/data/nodes1500Edge50 #nodes\=100AvgEdge\=25 task1/data/nodes1000Edge25
./GeneratorWithRating.sh 1500nodes100edges task1/data/nodes1500Edge100 #nodes\=100AvgEdge\=25 task1/data/nodes1000Edge25
./GeneratorWithRating.sh 1500nodes500edges task1/data/nodes1500Edge500 #nodes\=100AvgEdge\=25 task1/data/nodes1000Edge25
echo "25 done"
#./GeneratorWithRating.sh nodes\=100AvgEdge\=75 task1/data/nodes1000Edge75
#echo DONE!!
