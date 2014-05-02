import os
import csv

with open("batch/tiles20140501-17h34m25s377196us.dat", "rb") as csvfile:
	reader = csv.reader(csvfile, delimiter=",")
	for row in reader:
		print len(row)
