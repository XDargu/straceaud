import os, socket

os.mkdir("mydir")
with open("mydir/file1.txt", "w") as f:
    f.write("hello\n")

os.rename("mydir/file1.txt", "mydir/file2.txt")
os.chmod("mydir/file2.txt", 0o600)
os.chown("mydir/file2.txt", os.getuid(), os.getgid())
s = socket.socket()
try:
    s.connect(("example.com", 80))
except:
    pass