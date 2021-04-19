import time
import os
from timeit import default_timer as timer

# ...

path= "/home/uwb6/DJIOSDK/Onboard-SDK/build/bin/buffer.txt"
os.mkfifo(path)
fifo=open(path,'w')
start = timer()
#for i in range(1, 10):
string = "My string to be senterwtwertwertrete434545345"
fifo.write(string)
#time.sleep(1)
fifo.close()

fifo=open(path,'w')
string = "My string to be sent2"
fifo.write(string)
#time.sleep(1)
fifo.close()
end = timer()
print(end - start) # Time in seconds, e.g. 5.38091952400282

fifo=open(path,'w')
string = "My string to be sent3"
fifo.write(string)
#time.sleep(1)
fifo.close()

fifo=open(path,'w')
string = "My string to be sent4"
fifo.write(string)
#time.sleep(1)
fifo.close()

fifo=open(path,'w')
string = "My string to be sent5"
fifo.write(string)
#time.sleep(1)
fifo.close()

#os.remove("buffer.fifo")
