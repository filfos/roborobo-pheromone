from PIL import Image, ImageDraw
import os
import csv

def toFloatArray( stringArray ):
	return [float(n) for n in stringArray]

def findMax (grid):
	maxi = 0
	for row in grid:
		for n in row:
			if (n > maxi):
				maxi = n
	return maxi
				
	
def normalize( grid ):
	normGrid = []
	m = findMax(grid)
	col=0
	for row in grid:
		normGrid.append([])
		for n in row:
			normGrid[col].append(n/m)
		col+=1
	return normGrid

def getColorTriple (normValue):
	if (normValue <= 0.5):
		return (int(510*normValue), 255, 0)
	elif (normValue > 0.5):
		invert = 1-normValue
		return (255, int(invert*510) , 0)
		



i = Image.open("heatmap/env.png")

cellSize = 50

pixels = i.load() # this is not a list, nor is it list()'able
img_width, img_height = i.size

all_pixels = []
for x in range(img_width):
    all_pixels.append([])
    for y in range(img_height):
        cpixel = pixels[x, y]
        all_pixels[x].append(cpixel)

grid = None
with open("heatmap/heatmap.csv", "rb") as csvfile:
	reader = csv.reader(csvfile, delimiter=",")

	floatGrid = []
	for row in reader:
		floatGrid.append(toFloatArray(row))
		
	grid = normalize(floatGrid)


#in memory
img = Image.new("RGB", (img_width, img_height), (255, 255, 255))
draw = ImageDraw.Draw(img)


#Color cells
for y in range(img_height):
	yCell = int(y/cellSize)
	for x in range(img_width):
		xCell = int(x/cellSize)
		img.putpixel( (y, x), getColorTriple(grid[xCell][yCell]) )

#Draw Grid		
for x in range(img_width):
	draw.line((x*cellSize, 1, x*cellSize, img_height-1), fill=(0,0,0), width=1)
			
for y in range(img_height):
	draw.line((1, y*cellSize, img_width-1, y*cellSize), fill=(0,0,0), width=1)

#Draw environment
for x in range(img_width):
	for y in range(img_height):
		if (all_pixels[x][y] == (0, 0, 0)):
			img.putpixel((x, y), (0,0,0) )
		
img.save("heatmap/heatmap.png")


























