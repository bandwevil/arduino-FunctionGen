import math, sys

for i in range(0,41):
   out = str(math.floor((math.sin(math.pi*i/20)+1) * 2047))
   strout = out + ", "
   sys.stdout.write(strout)
