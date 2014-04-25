

import os
import csv
from itertools import izip

#print os.getcwd()

directory = os.getcwd()#"/home/filip/Roborobo-Pheromone/output"
dispersion_files = []
tiles_files = []

#combine all files
for file in os.listdir(directory+"/batch"):
    if file.endswith(".dat"):
        if (file.startswith('dispersion')):
            dispersion_files.append(file)
        elif (file.startswith('tiles')):
            tiles_files.append(file);
        
#sort (by name i.e timestamp)
dispersion_sorted = sorted(dispersion_files)
tiles_sorted = sorted(tiles_files)


#----------DISPERSION---------
dispersion_file = open("dispersion_temp.csv", "w")
for file in dispersion_sorted:
    file_object = open(directory+"/batch/"+file, 'r')
    dispersion_file.write( file_object.readline()+"\n" )
    os.remove(file_object.name)
   
dispersion_file.close() 
#Transpose
d = izip(*csv.reader(open("dispersion_temp.csv", "rb")))
csv.writer(open("dispersion.csv", "wb")).writerows(d)
os.remove(dispersion_file.name)

#--------TILES-------------
tiles_file = open("tiles_temp.csv", "w")
for file in tiles_sorted:
    file_object = open(directory+"/batch/"+file, 'r')
    tiles_file.write(file_object.readline()+"\n" )
    os.remove(file_object.name)

tiles_file.close()
#Transpose	
t = izip(*csv.reader(open("tiles_temp.csv", "rb")))
csv.writer(open("tiles.csv", "wb")).writerows(t)
os.remove(tiles_file.name)


#TRANSPOSE
#t = izip(*csv.reader(open("tiles_temp.csv", "rb")))
#csv.writer(open("tiles.csv", "wb")).writerows(t)

#TRANSPOSE
#d = izip(*csv.reader(open("dispersion_temp.csv", "rb")))
#csv.writer(open("dispersion.csv", "wb")).writerows(d)




	
