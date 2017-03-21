#! /bin/bash


./GeneratorWithRating.sh 1500nodes10Edges data/nodes1500Edge10 #nodes\=100AvgEdge\=25 task1/data/nodes1000Edge25
echo "1"
./GeneratorWithRating.sh 1500nodes50Edges data/nodes1500Edge50 #nodes\=100AvgEdge\=25 task1/data/nodes1000Edge25
echo "2"
./GeneratorWithRating.sh 1500nodes100Edges data/nodes1500Edge100 #nodes\=100AvgEdge\=25 task1/data/nodes1000Edge25
./GeneratorWithRating.sh 1500nodes500Edges data/nodes1500Edge500 #nodes\=100AvgEdge\=25 task1/data/nodes1000Edge25
echo "3"
./GeneratorWithRating.sh 1500nodes700Edges data/nodes1500Edge700 #nodes\=100AvgEdge\=25 task1/data/nodes1000Edge25
echo "4"
#echo "25 done"
#./GeneratorWithRating.sh nodes\=100AvgEdge\=75 task1/data/nodes1000Edge75
#echo DONE!!
