#!/bin/bash

make clean && make monte_carlo # Rebuild the monte_carlo executable .

#Setting up temp files, and variables for easier path referencing (first plot)
mkdir /tmp/plot-temps/
touch /tmp/plot-temps/CircleIn.dat
echo -n "" > /tmp/plot-temps/CircleIn.dat
Turquoise="/tmp/plot-temps/CircleIn.dat"
touch /tmp/plot-temps/CircleOut.dat
echo -n "" > /tmp/plot-temps/CircleOut.dat
Pink="/tmp/plot-temps/CircleOut.dat"
Plot1="/tmp/plot-temps/Plot1.dat"

#Place the data points into a file for the first plot.
./monte_carlo -n 3000 > $Plot1

#Organizing data to set up first plot
k=0
while read -r line
do
    k=$((k+1)) #keeps track of iteration in order
    if [[ $k -eq 1 ]] #to skip the first line
    then
	      continue
    fi
    circle=$(echo $line | awk '{print $5}') #circle check
    if [[ $circle -eq 1 ]] #these points are in the circle
    then
	      echo $line | awk '{print $3,$4}' >> $Turquoise
    else [[ $circle -eq 0 ]] #these points are outside the circle
	      echo $line | awk '{print $3,$4}' >> $Pink
    fi
done < $Plot1

# This is the here - document that is sent to gnuplot for the first plot.
gnuplot <<END
set terminal pdf
set output "FirstPlot.pdf"
set zeroaxis
f(x) = sqrt(1 - (x ** 2))
plot [0:1] [0:1] "/tmp/plot-temps/CircleIn.dat" lt rgb "#30D5C8" with dots title "", "/tmp/plot-temps/CircleOut.dat" lt rgb "#FC03A1" with dots title "", f(x) with lines title ""
END

#Setting up temp files, and variables for easier path referencing (second plot)
touch /tmp/plot-temps/seed1.dat
echo -n "" > /tmp/plot-temps/seed1.dat
seed1="/tmp/plot-temps/seed1.dat"
touch /tmp/plot-temps/seed2.dat
echo -n "" > /tmp/plot-temps/seed2.dat
seed2="/tmp/plot-temps/seed2.dat"
touch /tmp/plot-temps/seed3.dat
echo -n "" > /tmp/plot-temps/seed3.dat
seed3="/tmp/plot-temps/seed3.dat"
Plot2="/tmp/plot-temps/Plot2.dat"

#Place the data points into a file for the first seed of the second plot.
./monte_carlo -n 2000 -r 1 > $Plot2
k=0
PiEst=0
ConcatString=""
while read -r line
do
	  k=$((k+1)) #keeps track of iteration in order
	  if [[ $k -eq 1 ]] #to skip the first line
	  then
	  	  continue
	  fi
	  PiEst=$(echo $line | awk '{print $2}') #monte_carlo's estimation of pi
	  Difference=$(bc <<< "$PiEst - 3.1415926") #finds the difference between estimation and real pi
	  Iteration=$(echo $line | awk '{print $1}') #saves the iteration
	  ConcatString="$Iteration $Difference" #concatonation
	  echo "$ConcatString" >> "$seed1" #logs data
done < $Plot2

#Place the data points into a file for the second seed of the second plot.
./monte_carlo -n 2000 -r 2 > $Plot2
k=0
PiEst=0
ConcatString=""
while read -r line
do
	  k=$((k+1)) #keeps track of iteration in order
	  if [[ $k -eq 1 ]] #to skip the first line
    then
	    	continue
    fi
    PiEst=$(echo $line | awk '{print $2}') #monte_carlo's estimation of pi
    Difference=$(bc <<< "$PiEst - 3.1415926") #finds the difference between estimation and real pi
    Iteration=$(echo $line | awk '{print $1}') #saves the iteration
    ConcatString="$Iteration $Difference" #concatonation
    echo "$ConcatString" >> "$seed2" #logs data
done < $Plot2

#Place the data points into a file for the third seed of the second plot.
./monte_carlo -n 2000 -r 3 > $Plot2
k=0
PiEst=0
ConcatString=""
while read -r line
do
    k=$((k+1)) #keeps track of iteration in order
    if [[ $k -eq 1 ]] #to skip the first line
    then
        continue
    fi
    PiEst=$(echo $line | awk '{print $2}') #monte_carlo's estimation of pi
    Difference=$(bc <<< "$PiEst - 3.1415926") #finds the difference between estimation and real pi
    Iteration=$(echo $line | awk '{print $1}') #saves the iteration
    ConcatString="$Iteration $Difference" #concatonation
    echo "$ConcatString" >> "$seed3" #logs data
done < $Plot2

# This is the here - document that is sent to gnuplot for the second plot.
gnuplot <<END
set terminal pdf
set output "SecondPlot.pdf"
set zeroaxis
plot [0:2000][-1:1] "/tmp/plot-temps/seed1.dat" with lines title "", "/tmp/plot-temps/seed2.dat" with lines title "", "/tmp/plot-temps/seed3.dat" with lines title ""
END