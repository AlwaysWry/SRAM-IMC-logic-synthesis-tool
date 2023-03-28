# a Python script to remove the head and tail information of .pla files

print("input file name:")
filename_in = input()
print("output file name:")
filename_out = input()

fi = open(filename_in, mode = 'r')
fo = open(filename_out,mode = 'w')
fr = fi.readlines()
for line in fr:
	if line.startswith('0') or line.startswith('1') or line.startswith('-'):
		fo.write(line)
	else:
		pass

fi.close()
fo.close()