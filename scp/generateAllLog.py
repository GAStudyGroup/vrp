import sys
import os
import testConfigs as cfg
import subprocess

datasetLogFolder = os.listdir("log/")

for datasetLogs in datasetLogFolder:
    logs = os.listdir("log/"+datasetLogs)
    if not os.path.exists("log/"+datasetLogs.replace("-csv", "")+"-csv"):
        os.makedirs("log/"+datasetLogs+"-csv")

    for log in logs:
        logFile = open("log/"+datasetLogs+"/"+log, 'r')

        it=0
        methodRunsCsv=""
        for line in logFile:
            it+=1
            if it < 20:
                if line.find("Run ID:")!=-1:
                    run=int(line.split(' ')[2])
                if line.find("Crossover Method:")!=-1:
                    cross=str(line.split(' ')[2].replace('\n', ''))
                if line.find("Initial Pop Method:")!=-1:
                    initalMethod=int(line.split(' ')[3])
                if line.find("With Mutation?")!=-1:
                    withMutation=line.split(' ')[2].replace('\n', '')

                    methodRunsCsv = str("log/"+datasetLogs+"-csv/"+"cross_"+cross+"_With_"+withMutation+"_InMethod_"+str(initalMethod))
            else:
                if not os.path.exists(methodRunsCsv):
                    os.makedirs(methodRunsCsv)
                
                fileCsv = open(methodRunsCsv+"/run_"+str(run)+".csv", "a")

                if line.find("Generation")!=-1:
                    gen=int(line.split(' ')[1])
                if line.find("BestSolution Fitness:")!=-1:
                    fit=int(line.split(' ')[2])
                    fileCsv.write(str(gen)+","+str(fit)+"\n")