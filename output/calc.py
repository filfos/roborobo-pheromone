import os
import csv
from math import sqrt

def toFloatArray( stringArray ):
	return [float(n) for n in stringArray]
	
def avg( numbers ):
	return sum(numbers)/len(numbers)

def std ( numbers, average ):
	sum = 0
	for n in numbers:
		sum += (n-average)**2
	sum /= len(numbers)
	return sqrt(sum)
	


with open("tiles.csv", "rb") as csvfile:
	reader = csv.reader(csvfile, delimiter=",")
	outputwriter = csv.writer(open("tiles_calc.csv", "wb"), delimiter="\t")
	outputwriter.writerow(["Average", "Stdev min", "Stdev plus"])	
	for row in reader:
		try:
			numbers = toFloatArray(row)
		except ValueError:
			break;
		else:
			average = avg(numbers)
			stdev = std(numbers, average)
			std_below = average - stdev
			std_above = average + stdev
			outputwriter.writerow([average, std_below, std_above])

with open("dispersion.csv", "rb") as csvfile:
	reader = csv.reader(csvfile, delimiter=",")
	outputwriter = csv.writer(open("dispersion_calc.csv", "wb"), delimiter="\t")
	outputwriter.writerow(["average", "stdmin", "stdplus"])	
	for row in reader:
		try:
			numbers = toFloatArray(row)
		except ValueError:
			break;
		else:
			average = avg(numbers)
			stdev = std(numbers, average)
			std_below = average - stdev
			std_above = average + stdev
			outputwriter.writerow([average, std_below, std_above])





