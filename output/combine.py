

import os

#print os.getcwd()

directory = "/home/filip/Roborobo-Pheromone/output"
dispersion_files = []
tiles_files = []

for file in os.listdir(directory+"/batch"):
    if file.endswith(".dat"):
        if (file.startswith('dispersion')):
            dispersion_files.append(file)
        elif (file.startswith('tiles')):
            tiles_files.append(file);
        

dispersion_sorted = sorted(dispersion_files)
tiles_sorted = sorted(tiles_files)

dispersion_file = open("dispersion.csv", "w")
for file in dispersion_sorted:
    file_object = open(directory+"/batch/"+file, 'r')
    dispersion_file.write( file_object.readline()+"\n" )
    os.remove(file_object.name)
   
dispersion_file.close 

tiles_file = open("tiles.csv", "w")
for file in tiles_sorted:
    file_object = open(directory+"/batch/"+file, 'r')
    tiles_file.write(file_object.readline()+"\n" )
    os.remove(file_object.name)
	
tiles_file.close()



	
