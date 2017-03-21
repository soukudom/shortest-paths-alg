#! /bin/bash


./GeneratorWithRating.sh 2000nodes100Edges data/nodes2000Edge100 #nodes\=100AvgEdge\=25 task1/data/nodes1000Edge25
echo "1"
./GeneratorWithRating.sh 2000nodes300Edges data/nodes2000Edge300 #nodes\=100AvgEdge\=25 task1/data/nodes1000Edge25
echo "2"
./GeneratorWithRating.sh 2000nodes500Edges data/nodes2000Edge500 #nodes\=100AvgEdge\=25 task1/data/nodes1000Edge25
./GeneratorWithRating.sh 2000nodes700Edges data/nodes2000Edge700 #nodes\=100AvgEdge\=25 task1/data/nodes1000Edge25
echo "3"
./GeneratorWithRating.sh 2000nodes1000Edges data/nodes2000Edge1000 #nodes\=100AvgEdge\=25 task1/data/nodes1000Edge25
echo "4"
#echo "25 done"
#./GeneratorWithRating.sh nodes\=100AvgEdge\=75 task1/data/nodes1000Edge75
#echo DONE!!
