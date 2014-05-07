import os
import csv


def toFloatArray( stringArray ):
	return [float(n) for n in stringArray]

def addToArray (outputArray, inputArray):
	arr = []
	for i in range(len(outputArray)):
		arr.append(outputArray[i] + inputArray[i])
	return arr


	


directory = os.getcwd()
heat_files = []

#gather all files
for file in os.listdir(directory+"/heatmap"):
    if file.endswith(".dat"):
        if (file.startswith('heat')):
            heat_files.append(file)

avgGrid = []
noofFiles = 0
firstPass = True
for file in heat_files:
	noofFiles+=1
	with open("heatmap/"+file, "rb") as csvfile:
		reader = csv.reader(csvfile, delimiter=",")
		rowNb=0
		for row in reader:
			numbers = toFloatArray(row)
			if (firstPass):
				avgGrid.append(numbers)
			else:
				avgGrid[rowNb] = addToArray(avgGrid[rowNb], numbers)
			rowNb+=1

	firstPass=False

for row in range(len(avgGrid)):
	for n in range(len(avgGrid[row])):
		avgGrid[row][n] /= noofFiles




outputwriter = csv.writer(open("heatmap/heatmap.csv", "wb"), delimiter=",")
for row in avgGrid:
	outputwriter.writerow(row)	

#Delete old files
for file in heat_files:
    file_object = open(directory+"/heatmap/"+file, 'r')
    os.remove(file_object.name)
