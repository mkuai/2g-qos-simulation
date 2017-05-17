__author__ = 'Meng Kuai'
# This script is to run the simulation
import os
import matplotlib as mpl

mpl.use('Agg')
import ConfigParser
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

"""
# compile three source code
os.system(
    "gcc cache.c simlib.c -lm -o cache && gcc cache_all.c simlib.c -lm -o cache_all && gcc cache_select.c simlib.c -lm -o cache_select")
"""

# define metric
variable_X = []
result_cache_Satisfied_Ratio = []
result_cache_Cache_Hit_Ratio = []
result_cache_Cache_Replacement_Ratio = []
result_cache_Average_Overhead = []
result_all_Satisfied_Ratio = []
result_all_Cache_Hit_Ratio = []
result_all_Cache_Replacement_Ratio = []
result_all_Average_Overhead = []
result_select_Satisfied_Ratio = []
result_select_Cache_Hit_Ratio = []
result_select_Cache_Replacement_Ratio = []
result_select_Average_Overhead = []


# declare variables
n_Total_SimTime = 10000
n_NodeNo = 10
n_DataNo = 20

range_Max_Cache_Size = list(range(5, 25))
Max_Cache_Size = 15

#range_mean_Data_Interarrival = list(range(10, 30))
#mean_Data_Interarrival = 20.0

range_Data_Arrival_Ratio = list(range(1, 100, 2))
Data_Arrival_Ratio = 50

#range_mean_Request_Interarrival = list(range(15, 25))
#mean_Request_Interarrival = 20.0

range_Request_Arrival_Ratio = list(range(1, 100, 2))
Request_Arrival_Ratio = 50

range_Lambda = list(range(100, 3000, 100))
Lambda = 1000

Density_Range = list(range(1, 12, 1))
Density = 1

variable_X_Title = "Lambda"
for Lambda in range_Lambda:
    variable_X.append(Lambda)

    mean_Data_Interarrival = float(1000) / Data_Arrival_Ratio
    mean_Request_Interarrival = float(1000) / Request_Arrival_Ratio
    Tx_Range = 100 * Density

    # construct input.txt
    file_Input = open("input.txt", "w+")
    print >> file_Input, "%d\n%d\n%d\n%d\n%f\n%f\n%f\n%d" % (
        n_Total_SimTime, n_NodeNo, n_DataNo, Max_Cache_Size, mean_Data_Interarrival, mean_Request_Interarrival,
        Lambda, Tx_Range)
    file_Input.close()

    # run the simulation and get the results: cache
    os.system("nohup ./cache")
    file_Output = ConfigParser.ConfigParser()
    file_Output.read("output.txt")
    f_Satisfied_Ratio = float(file_Output.get("Simulation Results", "Satisfied Ratio"))
    f_Cache_Hit_Ratio = float(file_Output.get("Simulation Results", "Cache Hit Ratio"))
    f_Cache_Replacement_Ratio = float(file_Output.get("Simulation Results", "Cache Replacement Ratio"))
    f_Average_Overhead = float(file_Output.get("Simulation Results", "Average Overhead"))
    result_cache_Satisfied_Ratio.append(f_Satisfied_Ratio)
    result_cache_Cache_Hit_Ratio.append(f_Cache_Hit_Ratio)
    result_cache_Cache_Replacement_Ratio.append(f_Cache_Replacement_Ratio)
    result_cache_Average_Overhead.append(f_Average_Overhead)

    # run the simulation and get the results: all
    os.system("nohup ./cache_all")
    file_Output = ConfigParser.ConfigParser()
    file_Output.read("output.txt")
    f_Satisfied_Ratio = float(file_Output.get("Simulation Results", "Satisfied Ratio"))
    f_Cache_Hit_Ratio = float(file_Output.get("Simulation Results", "Cache Hit Ratio"))
    f_Cache_Replacement_Ratio = float(file_Output.get("Simulation Results", "Cache Replacement Ratio"))
    f_Average_Overhead = float(file_Output.get("Simulation Results", "Average Overhead"))
    result_all_Satisfied_Ratio.append(f_Satisfied_Ratio)
    result_all_Cache_Hit_Ratio.append(f_Cache_Hit_Ratio)
    result_all_Cache_Replacement_Ratio.append(f_Cache_Replacement_Ratio)
    result_all_Average_Overhead.append(f_Average_Overhead)

    # run the simulation and get the results: select
    os.system("nohup ./cache_select")
    file_Output = ConfigParser.ConfigParser()
    file_Output.read("output.txt")
    f_Satisfied_Ratio = float(file_Output.get("Simulation Results", "Satisfied Ratio"))
    f_Cache_Hit_Ratio = float(file_Output.get("Simulation Results", "Cache Hit Ratio"))
    f_Cache_Replacement_Ratio = float(file_Output.get("Simulation Results", "Cache Replacement Ratio"))
    f_Average_Overhead = float(file_Output.get("Simulation Results", "Average Overhead"))
    result_select_Satisfied_Ratio.append(f_Satisfied_Ratio)
    result_select_Cache_Hit_Ratio.append(f_Cache_Hit_Ratio)
    result_select_Cache_Replacement_Ratio.append(f_Cache_Replacement_Ratio)
    result_select_Average_Overhead.append(f_Average_Overhead)

# draw figures

# Satisfied Ratio

line_1 = plt.plot(variable_X, result_cache_Satisfied_Ratio)
line_2 = plt.plot(variable_X, result_all_Satisfied_Ratio)
line_3 = plt.plot(variable_X, result_select_Satisfied_Ratio)

# plt.axis([0, 100, 0, 500])

plt.setp(line_1, color='r', marker='s', label='Location-based Cache')
plt.setp(line_2, color='b', marker='o', label='All Cache')
plt.setp(line_3, color='g', marker='^', label='Selective Cache')
plt.legend(loc='best', fancybox=True, framealpha=0.5)

plt.title("%s_vs_Satisfied_Ratio" % variable_X_Title)
plt.ylabel("Satisfied_Ratio")
plt.xlabel("%s" % variable_X_Title)

pp = PdfPages("results/%s_vs_Satisfied_Ratio.pdf" % variable_X_Title)
plt.savefig(pp, format='pdf')
pp.close()
plt.close()

# Cache Hit Ratio

line_1 = plt.plot(variable_X, result_cache_Cache_Hit_Ratio)
line_2 = plt.plot(variable_X, result_all_Cache_Hit_Ratio)
line_3 = plt.plot(variable_X, result_select_Cache_Hit_Ratio)

# plt.axis([0, 100, 0, 500])

plt.setp(line_1, color='r', marker='s', label='Location-based Cache')
plt.setp(line_2, color='b', marker='o', label='All Cache')
plt.setp(line_3, color='g', marker='^', label='Selective Cache')
plt.legend(loc='best', fancybox=True, framealpha=0.5)

plt.title("%s_vs_Cache_Hit_Ratio" % variable_X_Title)
plt.ylabel("Cache_Hit_Ratio")
plt.xlabel("%s" % variable_X_Title)

pp = PdfPages("results/%s_vs_Cache_Hit_Ratio.pdf" % variable_X_Title)
plt.savefig(pp, format='pdf')
pp.close()
plt.close()

# Cache Replacement Ratio

line_1 = plt.plot(variable_X, result_cache_Cache_Replacement_Ratio)
line_2 = plt.plot(variable_X, result_all_Cache_Replacement_Ratio)
line_3 = plt.plot(variable_X, result_select_Cache_Replacement_Ratio)

# plt.axis([0, 100, 0, 500])

plt.setp(line_1, color='r', marker='s', label='Location-based Cache')
plt.setp(line_2, color='b', marker='o', label='All Cache')
plt.setp(line_3, color='g', marker='^', label='Selective Cache')
plt.legend(loc='best', fancybox=True, framealpha=0.5)

plt.title("%s_vs_Cache_Replacement_Ratio" % variable_X_Title)
plt.ylabel("Cache_Replacement_Ratio")
plt.xlabel("%s" % variable_X_Title)

pp = PdfPages("results/%s_vs_Cache_Replacement_Ratio.pdf" % variable_X_Title)
plt.savefig(pp, format='pdf')
pp.close()
plt.close()

# Average Overhead

line_1 = plt.plot(variable_X, result_cache_Average_Overhead)
line_2 = plt.plot(variable_X, result_all_Average_Overhead)
line_3 = plt.plot(variable_X, result_all_Average_Overhead)

#plt.axis([0, 100, 0, 500])

plt.setp(line_1, color='r', marker='s', label='Location-based Cache')
plt.setp(line_2, color='b', marker='o', label='All Cache')
plt.setp(line_3, color='g', marker='^', label='Selective Cache')
plt.legend(loc='best', fancybox=True, framealpha=0.5)

plt.title("%s_vs_Average_Overhead" % variable_X_Title)
plt.ylabel("Average_Overhead")
plt.xlabel("%s" % variable_X_Title)

pp = PdfPages("results/%s_vs_Average_Overhead.pdf" % variable_X_Title)
plt.savefig(pp, format='pdf')
pp.close()
plt.close()