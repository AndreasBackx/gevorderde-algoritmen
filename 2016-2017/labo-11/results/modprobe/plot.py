
import matplotlib.pyplot as plt
import csv

x = []
y = []

with open('out_20_200000.csv','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for i, row in enumerate(plots):
        print("Printing line {}".format(i))
        x.append(row[0])
        y.append(row[1])

plt.plot(x,y)
plt.show()
