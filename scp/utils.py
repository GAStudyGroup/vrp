import configs as c
import re

def getTrucks(name):
    splited = name.split("-")
    trucks = splited[len(splited)-1]
    s = re.search(r"\d+(\.\d+)?", trucks)
    return(str(s.group(0)))