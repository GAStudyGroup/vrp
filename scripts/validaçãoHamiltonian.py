#!/usr/bin/env python
import sys

try:
    file = open("Debug/Hamiltonian/test1.tour","r")
except OSError:
    pass

red=[]
blue=[]

def get(list, element):
    result=[]
    offset=-1
    while True:
        try:
            offset = list.index(element, offset+1)
        except ValueError:
            return result
        result.append(list[offset])

for line in file:
    if len(line)!=0:
        if line.find("RED ")!=-1:
            line = line.split(" ")
            line.pop(0)
            line.pop(len(line)-1)
            for c in line:
                red.append(c)

        elif line.find("BLUE ")!=-1:
            line = line.split(" ")
            line.pop(0)
            line.pop(len(line)-1)
            for c in line:
                blue.append(c)

print("Red em relação ao blue")
for c in red:
    getRet = get(blue, c)
    if len(getRet) == 0 or len(getRet) > 1:
        print("Elemento procurado: "+str(c))
        print(getRet)

print("\nBlue em relação ao Red")
for c in blue:
    getRet = get(red, c)
    if len(getRet) == 0 or len(getRet) > 1:
        print("Elemento procurado: "+str(c))
        print(getRet)