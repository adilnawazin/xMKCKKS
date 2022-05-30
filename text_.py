import os
from random import randint
import numpy as np

for party in range(0,10):
	for rnd in range (1,45):
		for trial in range(1, 30):
			with open("/home/adel/heaan_test/doc_T"+str(party)+"_"+str(rnd)+"_"+str(trial)+".txt", 'w') as f:
				f.writelines(f"{np.random.uniform(1,100)}\n" for _ in range(32768))
				print('writing file doc_T'+str(rnd)+'1'+str(trial)+'.txt')


# from random import randint

# with open("numbers.txt", "w") as file:
#     file.writelines(f"{randint(1, 100)}\n" for _ in range(100))

# with open("numbers.txt", "r") as fin,\
#      open("filtered_numbers.txt", "w") as fout:
#     numbers = [n for n in map(int, fin) if 20 < n < 80]
#     print(f"Found {len(numbers)} numbers between 20 and 80.")
#     fout.write(",".join(map(str, numbers)))