import os

vector_size = input("Please enter vector_size: ")
for trial in range(1,10):
	#print(trial)
	for rnd in range(1,4):
		print("=================================================")
		print(f"round = {rnd}, trial = {trial}, vector_size = {vector_size}")
		print("=================================================")
		os.system('./TestClient Test '+ str(rnd)+' '+str(trial)+' '+vector_size)
