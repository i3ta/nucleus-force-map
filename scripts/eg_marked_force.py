import csv

import matplotlib.pyplot as plt
import numpy as np

with open("../output/force.csv") as file:
    reader = csv.reader(file)
    # Convert each value to a float
    data = [list(map(float, row)) for row in reader]

# Convert to a NumPy array for convenience (optional but recommended)
data = np.array(data)

plt.matshow(data)
plt.title("Resulting force")
plt.colorbar()
plt.savefig("force.png")
