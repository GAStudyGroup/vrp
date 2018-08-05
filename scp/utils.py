import configs as c
import re

def getTrucks(name):
    splited = name.split("-")
    trucks = splited[len(splited)-1]
    s = re.search(r"\d+(\.\d+)?", trucks)
    return(str(s.group(0)))

def getBest(name, path):
    try:
        file = open(path+"/"+name, "r")
    except OSError:
        pass

    for line in file:
        if line.find("Optimal") != -1 or line.find("Best") != -1:
            array = line.split()
            value = array[len(array)-1]
            if value[len(value)-1 == ")"]:
                value = value[:-1]
            return(value)
        
