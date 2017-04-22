
#!/usr/bin/env python3

# Edit line 31 in csv.h from
#
# // sprintf(formstring,"%%%ie",kolombreedte);
#
# into
#
# // sprintf(formstring,"%%%if",kolombreedte);
#
# or this script won't be able to read the scientific notations

from matplotlib import pyplot as plt

with open("dieptedata.csv") as f:
    dieptes = [ float(d) for d in f.read().splitlines() ]

    plt.plot(dieptes, color='blue', marker=None, linestyle='solid')

    plt.xlabel("Datapunt")
    plt.ylabel("Diepte")
    plt.show()
