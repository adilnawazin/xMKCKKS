import numpy as np 
import pandas as pd
import matplotlib.pyplot as plt

# df = pd.read_fwf("/home/adel/xMKCKKS/run/timerec.txt")
df2 = pd.read_csv("/home/adel/xMKCKKS/run/timerec.txt", sep=r"\s{1,}", engine="python", names=["Action", "Time(ms)"])
mse = pd.read_csv("/home/adel/xMKCKKS/run/MSE.txt", engine = "python", names=["Time(ms)"])
pk_gen = df2.loc[df2["Action"] == 'Keysum'].reset_index(drop=True)
ciphers_sum = df2.loc[df2["Action"]=='ct_add_time'].reset_index(drop=True)
dec_share = df2.loc[df2["Action"]=='dec_share_time'].reset_index(drop=True)
data_import = df2.loc[df2["Action"]=='data_import_time'].reset_index(drop=True)
encode = df2.loc[df2["Action"]=='encode_time'].reset_index(drop=True)
pkey = df2.loc[df2["Action"]=='pKey_Gen_time'].reset_index(drop=True)
merge_decode = df2.loc[df2["Action"]=='merge_n_decode_time'].reset_index(drop=True)
encryption = df2.loc[df2["Action"]=='encryption_time'].reset_index(drop=True)

with pd.ExcelWriter("timerecdatasheet_32767.xlsx") as writer:
	data_import.to_excel(writer, sheet_name='data import')
	encode.to_excel(writer, sheet_name='encoding')
	pkey.to_excel(writer, sheet_name='public key (individual)')
	pk_gen.to_excel(writer, sheet_name = 'joint public key')
	encryption.to_excel(writer, sheet_name = 'encryption')
	ciphers_sum.to_excel(writer, sheet_name = 'ciphers_sum')
	dec_share.to_excel(writer, sheet_name = 'dec_share')
	merge_decode.to_excel(writer, sheet_name= 'merge_and_decode')
	mse.to_excel(writer,sheet_name = "MSE")