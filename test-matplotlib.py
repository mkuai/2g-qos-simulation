import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
import csv
from matplotlib.backends.backend_pdf import PdfPages

result1 = {}
result2 = {}
result3 = {}

for i in range(0, 100):
    result1[i] = i
    result2[i] = i * 2
    result3[i] = i * 3


line_1 = plt.plot(result1.keys(), result1.values())
line_2 = plt.plot(result2.keys(), result2.values())
line_3 = plt.plot(result3.keys(), result3.values())
plt.axis([0, 100, 0, 500])

plt.setp(line_1, color='r', marker='s', label='line1')
plt.setp(line_2, color='b', marker='o', label='line2')
plt.setp(line_3, color='g', marker='^', label='line3')
plt.legend()

plt.title("Text vs Time")
plt.ylabel("Time")
plt.xlabel("Text")


pp = PdfPages('result.pdf')
plt.savefig(pp, format='pdf')
pp.close()

w = csv.writer(open("output.csv", "w"))
w.writerow(["Time", "metric1", "metric2", "metric3"])
for key in result1.keys():
    w.writerow([key, result1[key], result2[key], result3[key]])