import os
import time

path = "invisiblefile"

# need to assign the result to a variable, otherwise file is closed just after calling 'open'
# this behaviour can be verified by calling lsof
f = open(path, 'x')
print(f"file {path} created")
time.sleep(5)

os.unlink(path)
f.write("hello")
f.flush()
print(f"file {path} unlinked")

time.sleep(5)
# ls returns nothing
# BUT
# file is listed in the list of files held open by the process
# lsof -p $(ps -ef | awk '/[u]nlink_file_python/ {print $2}')
# lsof -p $(ps -ef | awk '/[u]nlink_file_python/ {print $2}') | awk '/invisiblefile/ {print "size:" $7; print "inode:" $8}'

f.close()
print(f"file {path} closed")
time.sleep(5)

print("end")
