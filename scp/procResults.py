import os
import sys
import statistics as st
from decimal import *

processedFolder = "dataProcessed"

if not os.path.exists(processedFolder):
    os.makedirs(processedFolder)

getcontext().prec = 3
output = {}
process = {}
it=0

datasetsFolders = os.listdir("log/")

for datasets in datasetsFolders:
    datas = os.listdir("log/"+datasets+"/data")

    if "finish" in datas:
        datas.remove("finish")
        for data in datas:
            fileRead = open("log/"+datasets+"/data/"+data, "r")

            for line in fileRead:
                if line.find("Best: ")!=-1:
                    output["best"]=int(line.split(' ')[1].replace("\n", ""))
                if line.find("Cross: ")!=-1:
                    output["cross"]=str(line.split(' ')[1].replace('\n', ''))
                if line.find("InitialMethod: ")!=-1:
                    output["initialMethod"]=int(line.split(' ')[1])
                if line.find("Mutation: ")!=-1:
                    output["mutation"]=line.split(' ')[1].replace("\n", " ")
            
            fileWrite = open(processedFolder+"/"+datasets+"_"+str(output["best"])+".csv", "a")
            if it==0:
                fileWrite.write("cross,initialMethod,mutation,convergency,convergencyPercentage,convergencyDesvPad,optimals,optimalsPercentage\n")
                it=+1

            fileRead.close()

            fileRead = open("log/"+datasets+"/data/"+data, "r")

            file = fileRead.read().split("\nInit Alg\n")
            runs = [x for x in file[1].split("\n") if x != '']

            runs = list(map(int, runs))

            process["convergency"] = format(st.mean(runs), ".3f")

            process["convergency_perc"] = format(float(process["convergency"])/output["best"] * 100 - 100, ".2f")

            process["convergency_desvPad"] = format(st.pstdev(runs), ".3f")

            process["optCount"] = len([run for run in runs if run == output["best"]])

            process["optPercentage"] = format(process["optCount"]/len(runs) * 100, ".2f")

            fileWrite.write(output["cross"]+","+str(output["initialMethod"])+","+output["mutation"]+","+str(process["convergency"])+","+str(process["convergency_perc"])+","+str(process["convergency_desvPad"])+","+str(process["optCount"])+","+process["optPercentage"]+"\n")

        fileRead.close()
        print("?")
        it=0
                

