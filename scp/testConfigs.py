'''
    All configurations of tests will be placed in this file

    Change the options below according with necessary
'''

# LOG CONTROL
LOG_DIR = "log/"
LOG_MODE = "1"

# Parameters of run
TOTAL_RUNS = 10
POP_SIZE = [10]#[50, 100, 200]
MAX_IT = [5]#[50]
WITH_MUTATION = ["false", "true"] # mutation will be used during run
'''
    0 - Initial pop only using mutation
    1 - Initial pop using kmeans and mutation
    2 - Initial pop using only random generation
'''
INITIAL_METHOD = [0, 1, 2] 
'''
    1 - Order Crossover
    2 - Generalized Partition Crossover With Population reset (based on initial pop method)
'''
CROSS = [0, 1]

# change configurations according with instances of test
# the .vrp is not necessary
setsVector = ["A-Sets", "B-Sets", "F-Sets", "M-Sets"]
DATASETS = {
    setsVector[0] : ["A-n34-k5", "E-n22-k4"]
    # setsVector[1] : [""],
    # setsVector[2] : [""],
    # setsVector[3] : [""]
}