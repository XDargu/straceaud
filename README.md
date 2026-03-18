# straceaud
straceaud is a small utility Linux tool to summarize and audit the output of strace.

## Compilation

Compile with gcc:

```
 gcc -o straceaud straceaud.c
```

Remember to give permissions:

```
 chmod +x straceaud
```

## Installation

You can also install straceaud as a package. Install `dist/package/straceaud-deb.deb.` usind dpkg

```
sudo dpkg -i straceaud-deb.deb
```

## Usage

Simply store the strace output to a file called trace.txt and call straceaud with that file

```
strace -o trace.txt your_program
straceaud trace.txt
```

Alternatively, pipe the output of strace to the parser:

```
strace -f your_program 2>&1 | ./straceaud
```

## Supported calls

Currently, the following calls are supported:
 - open/openat
 - read/pread64/readv
 - write/pwrite64/writev
 - close
 - getdents64
 - stat/lstat/statx/newfstatat/fstatat
 - fstat
 - access
 - readlink
 - mkdir/mkdirat
 - link/linkat/symlink/symlinkat/creat
 - unlink/unlinkat/rmdir
 - rename/renameat/renameat2
 - execve/execveat
 - fork/vfork/clone/clone3
 - socket/connect/bind/listen/accept/accept4
 - pipe/pipe2
 - dup/dup2/dup3
 - mmap
 - ioctl
 - fcntl
 - mount/umount/umount2
 - ptrace/capset/reboot/swapon/setuid/setgid

## Example

A simple example of usage can be seen with this simple python program:

```
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
```

Running it with `strace` reveals several hundred of calls. This offers plenty of detail, but it's hard to get an overall idea of which resources are being accesed:

```
execve("/usr/bin/python3", ["python3", "test.py"], 0x7fff181a2170 /* 25 vars */) = 0
brk(NULL)                               = 0x2f540000
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7ac147ad3000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=20179, ...}) = 0
mmap(NULL, 20179, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7ac147ace000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libm.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0644, st_size=952616, ...}) = 0
mmap(NULL, 950296, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7ac1479e5000
mmap(0x7ac1479f5000, 520192, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x10000) = 0x7ac1479f5000
mmap(0x7ac147a74000, 360448, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x8f000) = 0x7ac147a74000
mmap(0x7ac147acc000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0xe7000) = 0x7ac147acc000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libz.so.1", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0644, st_size=113000, ...}) = 0
mmap(NULL, 110744, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7ac1479c9000
mmap(0x7ac1479cb000, 73728, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x2000) = 0x7ac1479cb000
mmap(0x7ac1479dd000, 24576, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x14000) = 0x7ac1479dd000
mmap(0x7ac1479e3000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1a000) = 0x7ac1479e3000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libexpat.so.1", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0644, st_size=174336, ...}) = 0
mmap(NULL, 176256, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7ac14799d000
mmap(0x7ac1479a1000, 118784, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x4000) = 0x7ac1479a1000
mmap(0x7ac1479be000, 32768, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x21000) = 0x7ac1479be000
mmap(0x7ac1479c6000, 12288, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x28000) = 0x7ac1479c6000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\220\243\2\0\0\0\0\0"..., 832) = 832
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
fstat(3, {st_mode=S_IFREG|0755, st_size=2125328, ...}) = 0
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
mmap(NULL, 2170256, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7ac147600000
mmap(0x7ac147628000, 1605632, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x28000) = 0x7ac147628000
mmap(0x7ac1477b0000, 323584, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1b0000) = 0x7ac1477b0000
mmap(0x7ac1477ff000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1fe000) = 0x7ac1477ff000
mmap(0x7ac147805000, 52624, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7ac147805000
close(3)                                = 0
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7ac14799b000
arch_prctl(ARCH_SET_FS, 0x7ac14799c080) = 0
set_tid_address(0x7ac14799c350)         = 610
set_robust_list(0x7ac14799c360, 24)     = 0
rseq(0x7ac14799c9a0, 0x20, 0, 0x53053053) = 0
mprotect(0x7ac1477ff000, 16384, PROT_READ) = 0
mprotect(0x7ac1479c6000, 8192, PROT_READ) = 0
mprotect(0x7ac1479e3000, 4096, PROT_READ) = 0
mprotect(0x7ac147acc000, 4096, PROT_READ) = 0
mprotect(0xa27000, 4096, PROT_READ)     = 0
mprotect(0x7ac147b0b000, 8192, PROT_READ) = 0
prlimit64(0, RLIMIT_STACK, NULL, {rlim_cur=8192*1024, rlim_max=RLIM64_INFINITY}) = 0
munmap(0x7ac147ace000, 20179)           = 0
getrandom("\x02\x0d\xe3\xa0\xd3\xfd\x3c\x16", 8, GRND_NONBLOCK) = 8
brk(NULL)                               = 0x2f540000
brk(0x2f561000)                         = 0x2f561000
openat(AT_FDCWD, "/usr/lib/locale/locale-archive", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/usr/share/locale/locale.alias", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=2996, ...}) = 0
read(3, "# Locale name alias data base.\n#"..., 4096) = 2996
read(3, "", 4096)                       = 0
close(3)                                = 0
openat(AT_FDCWD, "/usr/lib/locale/C.UTF-8/LC_CTYPE", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/usr/lib/locale/C.utf8/LC_CTYPE", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=360460, ...}) = 0
mmap(NULL, 360460, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7ac147942000
close(3)                                = 0
openat(AT_FDCWD, "/usr/lib/x86_64-linux-gnu/gconv/gconv-modules.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=27028, ...}) = 0
mmap(NULL, 27028, PROT_READ, MAP_SHARED, 3, 0) = 0x7ac14793b000
close(3)                                = 0
futex(0x7ac14780472c, FUTEX_WAKE_PRIVATE, 2147483647) = 0
getcwd("/home/testuser", 4096)            = 13
getrandom("\xb0\xbd\x5c\xfb\x7f\xcb\x8b\x9c\x01\xcc\xc3\x01\x17\xc1\xb9\xc8\x18\xc2\x52\x77\x0e\x4c\x27\x43", 24, GRND_NONBLOCK) = 24
gettid()                                = 610
mmap(NULL, 1048576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7ac14783b000
mmap(NULL, 266240, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7ac1475bf000
mmap(NULL, 135168, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7ac14781a000
brk(0x2f582000)                         = 0x2f582000
mmap(NULL, 16384, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7ac147acf000
brk(0x2f5a3000)                         = 0x2f5a3000
newfstatat(AT_FDCWD, "/usr/local/sbin/python3", 0x7ffd4bd44200, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/local/bin/python3", 0x7ffd4bd44200, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/sbin/python3", 0x7ffd4bd44200, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/bin/python3", {st_mode=S_IFREG|0755, st_size=8020928, ...}, 0) = 0
openat(AT_FDCWD, "/usr/pyvenv.cfg", O_RDONLY) = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/usr/bin/pyvenv.cfg", O_RDONLY) = -1 ENOENT (No such file or directory)
readlink("/usr/bin/python3", "python3.12", 4096) = 10
readlink("/usr/bin/python3.12", 0x7ffd4bd3f220, 4096) = -1 EINVAL (Invalid argument)
openat(AT_FDCWD, "/usr/bin/python3._pth", O_RDONLY) = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/usr/bin/python3.12._pth", O_RDONLY) = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/usr/bin/pybuilddir.txt", O_RDONLY) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/bin/Modules/Setup.local", 0x7ffd4bd44200, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/bin/lib/python312.zip", 0x7ffd4bd43fc0, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib/python312.zip", 0x7ffd4bd44020, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/bin/lib/python3.12/os.py", 0x7ffd4bd44020, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/bin/lib/python3.12/os.pyc", 0x7ffd4bd44020, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib/python3.12/os.py", {st_mode=S_IFREG|0644, st_size=39786, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/bin/lib/python3.12/lib-dynload", 0x7ffd4bd44020, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib/python3.12/lib-dynload", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/lib-dynload", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
brk(0x2f5c4000)                         = 0x2f5c4000
openat(AT_FDCWD, "/etc/localtime", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=2298, ...}) = 0
fstat(3, {st_mode=S_IFREG|0644, st_size=2298, ...}) = 0
read(3, "TZif2\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\t\0\0\0\t\0\0\0\0"..., 4096) = 2298
lseek(3, -1449, SEEK_CUR)               = 849
read(3, "TZif2\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\t\0\0\0\t\0\0\0\0"..., 4096) = 1449
close(3)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python312.zip", 0x7ffd4bd43a50, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python312.zip", 0x7ffd4bd43dd0, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib/python3.12", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12", O_RDONLY|O_NONBLOCK|O_CLOEXEC|O_DIRECTORY) = 3
fstat(3, {st_mode=S_IFDIR|0755, st_size=4096, ...}) = 0
getdents64(3, 0x2f5a59d0 /* 202 entries */, 32768) = 6752
getdents64(3, 0x2f5a59d0 /* 0 entries */, 32768) = 0
close(3)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/encodings/__init__.cpython-312-x86_64-linux-gnu.so", 0x7ffd4bd43dd0, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib/python3.12/encodings/__init__.abi3.so", 0x7ffd4bd43dd0, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib/python3.12/encodings/__init__.so", 0x7ffd4bd43dd0, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib/python3.12/encodings/__init__.py", {st_mode=S_IFREG|0644, st_size=5884, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/encodings/__init__.py", {st_mode=S_IFREG|0644, st_size=5884, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/encodings/__pycache__/__init__.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 3
fcntl(3, F_GETFD)                       = 0x1 (flags FD_CLOEXEC)
fstat(3, {st_mode=S_IFREG|0644, st_size=5790, ...}) = 0
ioctl(3, TCGETS, 0x7ffd4bd43a10)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(3, 0, SEEK_CUR)                   = 0
lseek(3, 0, SEEK_CUR)                   = 0
fstat(3, {st_mode=S_IFREG|0644, st_size=5790, ...}) = 0
read(3, "\313\r\r\n\0\0\0\0F\217ri\374\26\0\0\343\0\0\0\0\0\0\0\0\0\0\0\0\6\0\0"..., 5791) = 5790
read(3, "", 1)                          = 0
close(3)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/encodings", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/encodings", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/encodings", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/encodings", O_RDONLY|O_NONBLOCK|O_CLOEXEC|O_DIRECTORY) = 3
fstat(3, {st_mode=S_IFDIR|0755, st_size=4096, ...}) = 0
getdents64(3, 0x2f5af190 /* 125 entries */, 32768) = 4224
getdents64(3, 0x2f5af190 /* 0 entries */, 32768) = 0
close(3)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/encodings/aliases.py", {st_mode=S_IFREG|0644, st_size=15677, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/encodings/aliases.py", {st_mode=S_IFREG|0644, st_size=15677, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/encodings/__pycache__/aliases.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=12408, ...}) = 0
ioctl(3, TCGETS, 0x7ffd4bd42f90)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(3, 0, SEEK_CUR)                   = 0
lseek(3, 0, SEEK_CUR)                   = 0
fstat(3, {st_mode=S_IFREG|0644, st_size=12408, ...}) = 0
read(3, "\313\r\r\n\0\0\0\0F\217ri==\0\0\343\0\0\0\0\0\0\0\0\0\0\0\0\5\0\0"..., 12409) = 12408
read(3, "", 1)                          = 0
close(3)                                = 0
brk(0x2f5e5000)                         = 0x2f5e5000
newfstatat(AT_FDCWD, "/usr/lib/python3.12/encodings", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/encodings/utf_8.py", {st_mode=S_IFREG|0644, st_size=1005, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/encodings/utf_8.py", {st_mode=S_IFREG|0644, st_size=1005, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/encodings/__pycache__/utf_8.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=2147, ...}) = 0
ioctl(3, TCGETS, 0x7ffd4bd43aa0)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(3, 0, SEEK_CUR)                   = 0
lseek(3, 0, SEEK_CUR)                   = 0
fstat(3, {st_mode=S_IFREG|0644, st_size=2147, ...}) = 0
read(3, "\313\r\r\n\0\0\0\0F\217ri\355\3\0\0\343\0\0\0\0\0\0\0\0\0\0\0\0\5\0\0"..., 2148) = 2147
read(3, "", 1)                          = 0
close(3)                                = 0
rt_sigaction(SIGPIPE, {sa_handler=SIG_IGN, sa_mask=[], sa_flags=SA_RESTORER|SA_ONSTACK, sa_restorer=0x7ac147645330}, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGXFSZ, {sa_handler=SIG_IGN, sa_mask=[], sa_flags=SA_RESTORER|SA_ONSTACK, sa_restorer=0x7ac147645330}, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
mmap(NULL, 1048576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7ac1474bf000
rt_sigaction(SIGHUP, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGINT, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGQUIT, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGILL, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGTRAP, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGABRT, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGBUS, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGFPE, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGKILL, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGUSR1, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGSEGV, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGUSR2, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGPIPE, NULL, {sa_handler=SIG_IGN, sa_mask=[], sa_flags=SA_RESTORER|SA_ONSTACK, sa_restorer=0x7ac147645330}, 8) = 0
rt_sigaction(SIGALRM, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGTERM, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGSTKFLT, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGCHLD, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGCONT, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGSTOP, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGTSTP, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGTTIN, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGTTOU, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGURG, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGXCPU, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGXFSZ, NULL, {sa_handler=SIG_IGN, sa_mask=[], sa_flags=SA_RESTORER|SA_ONSTACK, sa_restorer=0x7ac147645330}, 8) = 0
rt_sigaction(SIGVTALRM, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGPROF, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGWINCH, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGIO, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGPWR, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGSYS, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_2, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_3, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_4, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_5, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_6, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_7, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_8, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_9, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_10, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_11, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_12, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_13, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_14, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_15, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_16, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_17, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_18, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_19, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_20, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_21, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_22, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_23, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_24, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_25, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_26, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_27, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_28, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_29, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_30, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_31, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGRT_32, NULL, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
rt_sigaction(SIGINT, {sa_handler=0x6e7140, sa_mask=[], sa_flags=SA_RESTORER|SA_ONSTACK, sa_restorer=0x7ac147645330}, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
fstat(0, {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0), ...}) = 0
fcntl(0, F_GETFD)                       = 0
fstat(0, {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0), ...}) = 0
ioctl(0, TCGETS, {c_iflag=ICRNL|IXON, c_oflag=NL0|CR0|TAB0|BS0|VT0|FF0|OPOST|ONLCR, c_cflag=B38400|CS8|CREAD, c_lflag=ISIG|ICANON|ECHO|ECHOE|ECHOK|IEXTEN|ECHOCTL|ECHOKE, ...}) = 0
lseek(0, 0, SEEK_CUR)                   = -1 ESPIPE (Illegal seek)
ioctl(0, TCGETS, {c_iflag=ICRNL|IXON, c_oflag=NL0|CR0|TAB0|BS0|VT0|FF0|OPOST|ONLCR, c_cflag=B38400|CS8|CREAD, c_lflag=ISIG|ICANON|ECHO|ECHOE|ECHOK|IEXTEN|ECHOCTL|ECHOKE, ...}) = 0
fcntl(1, F_GETFD)                       = 0
fstat(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0), ...}) = 0
ioctl(1, TCGETS, {c_iflag=ICRNL|IXON, c_oflag=NL0|CR0|TAB0|BS0|VT0|FF0|OPOST|ONLCR, c_cflag=B38400|CS8|CREAD, c_lflag=ISIG|ICANON|ECHO|ECHOE|ECHOK|IEXTEN|ECHOCTL|ECHOKE, ...}) = 0
lseek(1, 0, SEEK_CUR)                   = -1 ESPIPE (Illegal seek)
ioctl(1, TCGETS, {c_iflag=ICRNL|IXON, c_oflag=NL0|CR0|TAB0|BS0|VT0|FF0|OPOST|ONLCR, c_cflag=B38400|CS8|CREAD, c_lflag=ISIG|ICANON|ECHO|ECHOE|ECHOK|IEXTEN|ECHOCTL|ECHOKE, ...}) = 0
fcntl(2, F_GETFD)                       = 0
fstat(2, {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0), ...}) = 0
ioctl(2, TCGETS, {c_iflag=ICRNL|IXON, c_oflag=NL0|CR0|TAB0|BS0|VT0|FF0|OPOST|ONLCR, c_cflag=B38400|CS8|CREAD, c_lflag=ISIG|ICANON|ECHO|ECHOE|ECHOK|IEXTEN|ECHOCTL|ECHOKE, ...}) = 0
lseek(2, 0, SEEK_CUR)                   = -1 ESPIPE (Illegal seek)
ioctl(2, TCGETS, {c_iflag=ICRNL|IXON, c_oflag=NL0|CR0|TAB0|BS0|VT0|FF0|OPOST|ONLCR, c_cflag=B38400|CS8|CREAD, c_lflag=ISIG|ICANON|ECHO|ECHOE|ECHOK|IEXTEN|ECHOCTL|ECHOKE, ...}) = 0
newfstatat(AT_FDCWD, "/usr/bin/pyvenv.cfg", 0x7ffd4bd43a10, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/pyvenv.cfg", 0x7ffd4bd43a70, 0) = -1 ENOENT (No such file or directory)
geteuid()                               = 1000
getuid()                                = 1000
getegid()                               = 1000
getgid()                                = 1000
newfstatat(AT_FDCWD, "/home/testuser/.local/lib/python3.12/site-packages", 0x7ffd4bd43c40, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/local/lib/python3.12/dist-packages", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
openat(AT_FDCWD, "/usr/local/lib/python3.12/dist-packages", O_RDONLY|O_NONBLOCK|O_CLOEXEC|O_DIRECTORY) = 3
fstat(3, {st_mode=S_IFDIR|0755, st_size=4096, ...}) = 0
getdents64(3, 0x2f5d29a0 /* 2 entries */, 32768) = 48
getdents64(3, 0x2f5d29a0 /* 0 entries */, 32768) = 0
close(3)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3/dist-packages", {st_mode=S_IFDIR|0755, st_size=12288, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3/dist-packages", O_RDONLY|O_NONBLOCK|O_CLOEXEC|O_DIRECTORY) = 3
fstat(3, {st_mode=S_IFDIR|0755, st_size=12288, ...}) = 0
getdents64(3, 0x2f5d29a0 /* 155 entries */, 32768) = 6376
getdents64(3, 0x2f5d29a0 /* 0 entries */, 32768) = 0
close(3)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3/dist-packages/distutils-precedence.pth", {st_mode=S_IFREG|0644, st_size=151, ...}, AT_SYMLINK_NOFOLLOW) = 0
openat(AT_FDCWD, "/usr/lib/python3/dist-packages/distutils-precedence.pth", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=151, ...}) = 0
ioctl(3, TCGETS, 0x7ffd4bd438e0)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(3, 0, SEEK_CUR)                   = 0
read(3, "import os; var = 'SETUPTOOLS_USE"..., 8192) = 151
newfstatat(AT_FDCWD, "/usr/lib/python3.12", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/lib-dynload", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/lib-dynload", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/lib-dynload", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/lib-dynload", O_RDONLY|O_NONBLOCK|O_CLOEXEC|O_DIRECTORY) = 4
fstat(4, {st_mode=S_IFDIR|0755, st_size=4096, ...}) = 0
getdents64(4, 0x2f5dc0d0 /* 49 entries */, 32768) = 3104
getdents64(4, 0x2f5dc0d0 /* 0 entries */, 32768) = 0
close(4)                                = 0
newfstatat(AT_FDCWD, "/usr/local/lib/python3.12/dist-packages", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/local/lib/python3.12/dist-packages", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/local/lib/python3.12/dist-packages", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
openat(AT_FDCWD, "/usr/local/lib/python3.12/dist-packages", O_RDONLY|O_NONBLOCK|O_CLOEXEC|O_DIRECTORY) = 4
fstat(4, {st_mode=S_IFDIR|0755, st_size=4096, ...}) = 0
getdents64(4, 0x2f5dc0d0 /* 2 entries */, 32768) = 48
getdents64(4, 0x2f5dc0d0 /* 0 entries */, 32768) = 0
close(4)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3/dist-packages", {st_mode=S_IFDIR|0755, st_size=12288, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3/dist-packages", {st_mode=S_IFDIR|0755, st_size=12288, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3/dist-packages", {st_mode=S_IFDIR|0755, st_size=12288, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3/dist-packages", O_RDONLY|O_NONBLOCK|O_CLOEXEC|O_DIRECTORY) = 4
fstat(4, {st_mode=S_IFDIR|0755, st_size=12288, ...}) = 0
getdents64(4, 0x2f5dc0d0 /* 155 entries */, 32768) = 6376
getdents64(4, 0x2f5dc0d0 /* 0 entries */, 32768) = 0
close(4)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3/dist-packages/_distutils_hack/__init__.cpython-312-x86_64-linux-gnu.so", 0x7ffd4bd434f0, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib/python3/dist-packages/_distutils_hack/__init__.abi3.so", 0x7ffd4bd434f0, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib/python3/dist-packages/_distutils_hack/__init__.so", 0x7ffd4bd434f0, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib/python3/dist-packages/_distutils_hack/__init__.py", {st_mode=S_IFREG|0644, st_size=6299, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3/dist-packages/_distutils_hack/__init__.py", {st_mode=S_IFREG|0644, st_size=6299, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3/dist-packages/_distutils_hack/__pycache__/__init__.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 4
fstat(4, {st_mode=S_IFREG|0644, st_size=9998, ...}) = 0
ioctl(4, TCGETS, 0x7ffd4bd43190)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(4, 0, SEEK_CUR)                   = 0
lseek(4, 0, SEEK_CUR)                   = 0
fstat(4, {st_mode=S_IFREG|0644, st_size=9998, ...}) = 0
read(4, "\313\r\r\n\0\0\0\0\n_\337d\233\30\0\0\343\0\0\0\0\0\0\0\0\0\0\0\0\6\0\0"..., 9999) = 9998
read(4, "", 1)                          = 0
close(4)                                = 0
read(3, "", 8192)                       = 0
close(3)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3/dist-packages/zope.interface-6.1-nspkg.pth", {st_mode=S_IFREG|0644, st_size=529, ...}, AT_SYMLINK_NOFOLLOW) = 0
openat(AT_FDCWD, "/usr/lib/python3/dist-packages/zope.interface-6.1-nspkg.pth", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=529, ...}) = 0
ioctl(3, TCGETS, 0x7ffd4bd43940)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(3, 0, SEEK_CUR)                   = 0
read(3, "import sys, types, os;has_mfs = "..., 8192) = 529
brk(0x2f607000)                         = 0x2f607000
newfstatat(AT_FDCWD, "/usr/lib/python3.12", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/types.py", {st_mode=S_IFREG|0644, st_size=10993, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/types.py", {st_mode=S_IFREG|0644, st_size=10993, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/__pycache__/types.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 4
fstat(4, {st_mode=S_IFREG|0644, st_size=14939, ...}) = 0
ioctl(4, TCGETS, 0x7ffd4bd432c0)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(4, 0, SEEK_CUR)                   = 0
lseek(4, 0, SEEK_CUR)                   = 0
fstat(4, {st_mode=S_IFREG|0644, st_size=14939, ...}) = 0
read(4, "\313\r\r\n\0\0\0\0F\217ri\361*\0\0\343\0\0\0\0\0\0\0\0\0\0\0\0\6\0\0"..., 14940) = 14939
read(4, "", 1)                          = 0
close(4)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/importlib/__init__.cpython-312-x86_64-linux-gnu.so", 0x7ffd4bd430e0, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib/python3.12/importlib/__init__.abi3.so", 0x7ffd4bd430e0, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib/python3.12/importlib/__init__.so", 0x7ffd4bd430e0, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib/python3.12/importlib/__init__.py", {st_mode=S_IFREG|0644, st_size=4774, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/importlib/__init__.py", {st_mode=S_IFREG|0644, st_size=4774, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/importlib/__pycache__/__init__.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 4
fstat(4, {st_mode=S_IFREG|0644, st_size=4565, ...}) = 0
ioctl(4, TCGETS, 0x7ffd4bd42d80)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(4, 0, SEEK_CUR)                   = 0
lseek(4, 0, SEEK_CUR)                   = 0
fstat(4, {st_mode=S_IFREG|0644, st_size=4565, ...}) = 0
read(4, "\313\r\r\n\0\0\0\0F\217ri\246\22\0\0\343\0\0\0\0\0\0\0\0\0\0\0\0\5\0\0"..., 4566) = 4565
read(4, "", 1)                          = 0
close(4)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/warnings.py", {st_mode=S_IFREG|0644, st_size=21760, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/warnings.py", {st_mode=S_IFREG|0644, st_size=21760, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/__pycache__/warnings.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 4
fstat(4, {st_mode=S_IFREG|0644, st_size=23792, ...}) = 0
ioctl(4, TCGETS, 0x7ffd4bd42710)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(4, 0, SEEK_CUR)                   = 0
lseek(4, 0, SEEK_CUR)                   = 0
fstat(4, {st_mode=S_IFREG|0644, st_size=23792, ...}) = 0
read(4, "\313\r\r\n\0\0\0\0F\217ri\0U\0\0\343\0\0\0\0\0\0\0\0\0\0\0\0\6\0\0"..., 23793) = 23792
read(4, "", 1)                          = 0
close(4)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/importlib", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/importlib", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/importlib", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/importlib", O_RDONLY|O_NONBLOCK|O_CLOEXEC|O_DIRECTORY) = 4
fstat(4, {st_mode=S_IFDIR|0755, st_size=4096, ...}) = 0
getdents64(4, 0x2f5ed9d0 /* 14 entries */, 32768) = 456
getdents64(4, 0x2f5ed9d0 /* 0 entries */, 32768) = 0
close(4)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/importlib/_abc.py", {st_mode=S_IFREG|0644, st_size=1354, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/importlib/_abc.py", {st_mode=S_IFREG|0644, st_size=1354, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/importlib/__pycache__/_abc.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 4
fstat(4, {st_mode=S_IFREG|0644, st_size=1643, ...}) = 0
ioctl(4, TCGETS, 0x7ffd4bd42bc0)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(4, 0, SEEK_CUR)                   = 0
lseek(4, 0, SEEK_CUR)                   = 0
fstat(4, {st_mode=S_IFREG|0644, st_size=1643, ...}) = 0
read(4, "\313\r\r\n\0\0\0\0F\217riJ\5\0\0\343\0\0\0\0\0\0\0\0\0\0\0\0\5\0\0"..., 1644) = 1643
read(4, "", 1)                          = 0
close(4)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/threading.py", {st_mode=S_IFREG|0644, st_size=60123, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/threading.py", {st_mode=S_IFREG|0644, st_size=60123, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/__pycache__/threading.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 4
fstat(4, {st_mode=S_IFREG|0644, st_size=65346, ...}) = 0
ioctl(4, TCGETS, 0x7ffd4bd42bc0)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(4, 0, SEEK_CUR)                   = 0
lseek(4, 0, SEEK_CUR)                   = 0
fstat(4, {st_mode=S_IFREG|0644, st_size=65346, ...}) = 0
read(4, "\313\r\r\n\0\0\0\0F\217ri\333\352\0\0\343\0\0\0\0\0\0\0\0\0\0\0\0\5\0\0"..., 65347) = 65346
read(4, "", 1)                          = 0
close(4)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/functools.py", {st_mode=S_IFREG|0644, st_size=38126, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/functools.py", {st_mode=S_IFREG|0644, st_size=38126, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/__pycache__/functools.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 4
fstat(4, {st_mode=S_IFREG|0644, st_size=40506, ...}) = 0
ioctl(4, TCGETS, 0x7ffd4bd42550)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(4, 0, SEEK_CUR)                   = 0
lseek(4, 0, SEEK_CUR)                   = 0
fstat(4, {st_mode=S_IFREG|0644, st_size=40506, ...}) = 0
read(4, "\313\r\r\n\0\0\0\0F\217ri\356\224\0\0\343\0\0\0\0\0\0\0\0\0\0\0\0\7\0\0"..., 40507) = 40506
read(4, "", 1)                          = 0
close(4)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/collections/__init__.cpython-312-x86_64-linux-gnu.so", 0x7ffd4bd42240, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib/python3.12/collections/__init__.abi3.so", 0x7ffd4bd42240, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib/python3.12/collections/__init__.so", 0x7ffd4bd42240, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib/python3.12/collections/__init__.py", {st_mode=S_IFREG|0644, st_size=52378, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/collections/__init__.py", {st_mode=S_IFREG|0644, st_size=52378, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/collections/__pycache__/__init__.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 4
fstat(4, {st_mode=S_IFREG|0644, st_size=73089, ...}) = 0
ioctl(4, TCGETS, 0x7ffd4bd41ee0)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(4, 0, SEEK_CUR)                   = 0
lseek(4, 0, SEEK_CUR)                   = 0
fstat(4, {st_mode=S_IFREG|0644, st_size=73089, ...}) = 0
brk(0x2f635000)                         = 0x2f635000
read(4, "\313\r\r\n\0\0\0\0F\217ri\232\314\0\0\343\0\0\0\0\0\0\0\0\0\0\0\0\5\0\0"..., 73090) = 73089
read(4, "", 1)                          = 0
close(4)                                = 0
brk(0x2f623000)                         = 0x2f623000
newfstatat(AT_FDCWD, "/usr/lib/python3.12", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/keyword.py", {st_mode=S_IFREG|0644, st_size=1073, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/keyword.py", {st_mode=S_IFREG|0644, st_size=1073, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/__pycache__/keyword.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 4
fstat(4, {st_mode=S_IFREG|0644, st_size=1041, ...}) = 0
ioctl(4, TCGETS, 0x7ffd4bd41870)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(4, 0, SEEK_CUR)                   = 0
lseek(4, 0, SEEK_CUR)                   = 0
fstat(4, {st_mode=S_IFREG|0644, st_size=1041, ...}) = 0
read(4, "\313\r\r\n\0\0\0\0F\217ri1\4\0\0\343\0\0\0\0\0\0\0\0\0\0\0\0\3\0\0"..., 1042) = 1041
read(4, "", 1)                          = 0
close(4)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/operator.py", {st_mode=S_IFREG|0644, st_size=10965, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/operator.py", {st_mode=S_IFREG|0644, st_size=10965, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/__pycache__/operator.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 4
fstat(4, {st_mode=S_IFREG|0644, st_size=17364, ...}) = 0
ioctl(4, TCGETS, 0x7ffd4bd41870)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(4, 0, SEEK_CUR)                   = 0
lseek(4, 0, SEEK_CUR)                   = 0
fstat(4, {st_mode=S_IFREG|0644, st_size=17364, ...}) = 0
read(4, "\313\r\r\n\0\0\0\0F\217ri\325*\0\0\343\0\0\0\0\0\0\0\0\0\0\0\0\4\0\0"..., 17365) = 17364
read(4, "", 1)                          = 0
close(4)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/reprlib.py", {st_mode=S_IFREG|0644, st_size=6569, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/reprlib.py", {st_mode=S_IFREG|0644, st_size=6569, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/__pycache__/reprlib.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 4
fstat(4, {st_mode=S_IFREG|0644, st_size=9909, ...}) = 0
ioctl(4, TCGETS, 0x7ffd4bd41870)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(4, 0, SEEK_CUR)                   = 0
lseek(4, 0, SEEK_CUR)                   = 0
fstat(4, {st_mode=S_IFREG|0644, st_size=9909, ...}) = 0
read(4, "\313\r\r\n\0\0\0\0F\217ri\251\31\0\0\343\0\0\0\0\0\0\0\0\0\0\0\0\4\0\0"..., 9910) = 9909
read(4, "", 1)                          = 0
close(4)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/_weakrefset.py", {st_mode=S_IFREG|0644, st_size=5893, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/_weakrefset.py", {st_mode=S_IFREG|0644, st_size=5893, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/__pycache__/_weakrefset.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 4
fstat(4, {st_mode=S_IFREG|0644, st_size=11751, ...}) = 0
ioctl(4, TCGETS, 0x7ffd4bd42550)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(4, 0, SEEK_CUR)                   = 0
lseek(4, 0, SEEK_CUR)                   = 0
fstat(4, {st_mode=S_IFREG|0644, st_size=11751, ...}) = 0
read(4, "\313\r\r\n\0\0\0\0F\217ri\5\27\0\0\343\0\0\0\0\0\0\0\0\0\0\0\0\4\0\0"..., 11752) = 11751
read(4, "", 1)                          = 0
close(4)                                = 0
gettid()                                = 610
newfstatat(AT_FDCWD, "/usr/lib/python3/dist-packages", {st_mode=S_IFDIR|0755, st_size=12288, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3/dist-packages/zope/__init__.cpython-312-x86_64-linux-gnu.so", 0x7ffd4bd439e0, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib/python3/dist-packages/zope/__init__.abi3.so", 0x7ffd4bd439e0, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib/python3/dist-packages/zope/__init__.so", 0x7ffd4bd439e0, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib/python3/dist-packages/zope/__init__.py", {st_mode=S_IFREG|0644, st_size=56, ...}, 0) = 0
read(3, "", 8192)                       = 0
close(3)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/dist-packages", 0x7ffd4bd43ca0, 0) = -1 ENOENT (No such file or directory)
newfstatat(AT_FDCWD, "/usr/lib/python3.12", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/sitecustomize.py", {st_mode=S_IFREG|0644, st_size=155, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/sitecustomize.py", {st_mode=S_IFREG|0644, st_size=155, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/__pycache__/sitecustomize.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=300, ...}) = 0
ioctl(3, TCGETS, 0x7ffd4bd43580)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(3, 0, SEEK_CUR)                   = 0
lseek(3, 0, SEEK_CUR)                   = 0
fstat(3, {st_mode=S_IFREG|0644, st_size=300, ...}) = 0
read(3, "\313\r\r\n\0\0\0\0\273$\26f\233\0\0\0\343\0\0\0\0\0\0\0\0\0\0\0\0\4\0\0"..., 301) = 300
read(3, "", 1)                          = 0
close(3)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/lib-dynload", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/local/lib/python3.12/dist-packages", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3/dist-packages", {st_mode=S_IFDIR|0755, st_size=12288, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3/dist-packages/apport_python_hook.py", {st_mode=S_IFREG|0644, st_size=8695, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3/dist-packages/apport_python_hook.py", {st_mode=S_IFREG|0644, st_size=8695, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3/dist-packages/__pycache__/apport_python_hook.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=8876, ...}) = 0
ioctl(3, TCGETS, 0x7ffd4bd42f10)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(3, 0, SEEK_CUR)                   = 0
lseek(3, 0, SEEK_CUR)                   = 0
fstat(3, {st_mode=S_IFREG|0644, st_size=8876, ...}) = 0
read(3, "\313\r\r\n\0\0\0\0\247\22!f\367!\0\0\343\0\0\0\0\0\0\0\0\0\0\0\0\2\0\0"..., 8877) = 8876
read(3, "", 1)                          = 0
close(3)                                = 0
getcwd("/home/testuser", 1024)            = 13
newfstatat(AT_FDCWD, "/usr/lib/python3.12", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/lib-dynload", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/local/lib/python3.12/dist-packages", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3/dist-packages", {st_mode=S_IFDIR|0755, st_size=12288, ...}, 0) = 0
newfstatat(AT_FDCWD, "/home/testuser/test.py", {st_mode=S_IFREG|0644, st_size=327, ...}, 0) = 0
openat(AT_FDCWD, "/home/testuser/test.py", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=327, ...}) = 0
ioctl(3, TCGETS, 0x7ffd4bd440e0)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(3, 0, SEEK_CUR)                   = 0
lseek(3, 0, SEEK_CUR)                   = 0
lseek(3, -22, SEEK_END)                 = 305
lseek(3, 0, SEEK_CUR)                   = 305
read(3, "0))\r\nexcept:\r\n    pass", 4096) = 22
lseek(3, 0, SEEK_END)                   = 327
lseek(3, 0, SEEK_CUR)                   = 327
lseek(3, 0, SEEK_SET)                   = 0
lseek(3, 0, SEEK_CUR)                   = 0
fstat(3, {st_mode=S_IFREG|0644, st_size=327, ...}) = 0
read(3, "import os, socket\r\n\r\nos.mkdir(\"m"..., 328) = 327
read(3, "", 1)                          = 0
lseek(3, 0, SEEK_SET)                   = 0
close(3)                                = 0
newfstatat(AT_FDCWD, "/home/testuser/test.py", {st_mode=S_IFREG|0644, st_size=327, ...}, 0) = 0
readlink("test.py", 0x7ffd4bd33880, 4096) = -1 EINVAL (Invalid argument)
getcwd("/home/testuser", 1024)            = 13
readlink("/home/testuser/test.py", 0x7ffd4bd33420, 1023) = -1 EINVAL (Invalid argument)
openat(AT_FDCWD, "/home/testuser/test.py", O_RDONLY) = 3
ioctl(3, FIOCLEX)                       = 0
fstat(3, {st_mode=S_IFREG|0644, st_size=327, ...}) = 0
ioctl(3, TCGETS, 0x7ffd4bd44810)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(3, 0, SEEK_CUR)                   = 0
fstat(3, {st_mode=S_IFREG|0644, st_size=327, ...}) = 0
read(3, "import os, socket\r\n\r\nos.mkdir(\"m"..., 4096) = 327
lseek(3, 0, SEEK_SET)                   = 0
read(3, "import os, socket\r\n\r\nos.mkdir(\"m"..., 4096) = 327
read(3, "", 4096)                       = 0
close(3)                                = 0
newfstatat(AT_FDCWD, "/home/testuser", {st_mode=S_IFDIR|0750, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/home/testuser", {st_mode=S_IFDIR|0750, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/home/testuser", {st_mode=S_IFDIR|0750, st_size=4096, ...}, 0) = 0
openat(AT_FDCWD, "/home/testuser", O_RDONLY|O_NONBLOCK|O_CLOEXEC|O_DIRECTORY) = 3
fstat(3, {st_mode=S_IFDIR|0750, st_size=4096, ...}) = 0
brk(0x2f647000)                         = 0x2f647000
getdents64(3, 0x2f61e060 /* 21 entries */, 32768) = 720
getdents64(3, 0x2f61e060 /* 0 entries */, 32768) = 0
brk(0x2f63f000)                         = 0x2f63f000
close(3)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/socket.py", {st_mode=S_IFREG|0644, st_size=37411, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/socket.py", {st_mode=S_IFREG|0644, st_size=37411, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/__pycache__/socket.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=41817, ...}) = 0
ioctl(3, TCGETS, 0x7ffd4bd43de0)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(3, 0, SEEK_CUR)                   = 0
lseek(3, 0, SEEK_CUR)                   = 0
fstat(3, {st_mode=S_IFREG|0644, st_size=41817, ...}) = 0
read(3, "\313\r\r\n\0\0\0\0F\217ri#\222\0\0\343\0\0\0\0\0\0\0\0\0\0\0\0\5\0\0"..., 41818) = 41817
read(3, "", 1)                          = 0
close(3)                                = 0
newfstatat(AT_FDCWD, "/home/testuser", {st_mode=S_IFDIR|0750, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/selectors.py", {st_mode=S_IFREG|0644, st_size=19671, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/selectors.py", {st_mode=S_IFREG|0644, st_size=19671, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/__pycache__/selectors.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=26128, ...}) = 0
ioctl(3, TCGETS, 0x7ffd4bd43770)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(3, 0, SEEK_CUR)                   = 0
lseek(3, 0, SEEK_CUR)                   = 0
fstat(3, {st_mode=S_IFREG|0644, st_size=26128, ...}) = 0
read(3, "\313\r\r\n\0\0\0\0F\217ri\327L\0\0\343\0\0\0\0\0\0\0\0\0\0\0\0\5\0\0"..., 26129) = 26128
read(3, "", 1)                          = 0
close(3)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/collections", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/collections", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/collections", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/collections", O_RDONLY|O_NONBLOCK|O_CLOEXEC|O_DIRECTORY) = 3
fstat(3, {st_mode=S_IFDIR|0755, st_size=4096, ...}) = 0
brk(0x2f660000)                         = 0x2f660000
getdents64(3, 0x2f6375e0 /* 5 entries */, 32768) = 144
getdents64(3, 0x2f6375e0 /* 0 entries */, 32768) = 0
brk(0x2f658000)                         = 0x2f658000
close(3)                                = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/collections/abc.py", {st_mode=S_IFREG|0644, st_size=119, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/collections/abc.py", {st_mode=S_IFREG|0644, st_size=119, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/collections/__pycache__/abc.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=249, ...}) = 0
ioctl(3, TCGETS, 0x7ffd4bd43100)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(3, 0, SEEK_CUR)                   = 0
lseek(3, 0, SEEK_CUR)                   = 0
fstat(3, {st_mode=S_IFREG|0644, st_size=249, ...}) = 0
read(3, "\313\r\r\n\0\0\0\0F\217riw\0\0\0\343\0\0\0\0\0\0\0\0\0\0\0\0\2\0\0"..., 250) = 249
read(3, "", 1)                          = 0
close(3)                                = 0
epoll_create1(EPOLL_CLOEXEC)            = 3
close(3)                                = 0
newfstatat(AT_FDCWD, "/home/testuser", {st_mode=S_IFDIR|0750, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/enum.py", {st_mode=S_IFREG|0644, st_size=81636, ...}, 0) = 0
newfstatat(AT_FDCWD, "/usr/lib/python3.12/enum.py", {st_mode=S_IFREG|0644, st_size=81636, ...}, 0) = 0
openat(AT_FDCWD, "/usr/lib/python3.12/__pycache__/enum.cpython-312.pyc", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=80708, ...}) = 0
ioctl(3, TCGETS, 0x7ffd4bd43770)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(3, 0, SEEK_CUR)                   = 0
lseek(3, 0, SEEK_CUR)                   = 0
fstat(3, {st_mode=S_IFREG|0644, st_size=80708, ...}) = 0
read(3, "\313\r\r\n\0\0\0\0F\217ri\344>\1\0\343\0\0\0\0\0\0\0\0\0\0\0\0\10\0\0"..., 80709) = 80708
read(3, "", 1)                          = 0
close(3)                                = 0
mkdir("mydir", 0777)                    = 0
openat(AT_FDCWD, "mydir/file1.txt", O_WRONLY|O_CREAT|O_TRUNC|O_CLOEXEC, 0666) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=0, ...}) = 0
ioctl(3, TCGETS, 0x7ffd4bd44310)        = -1 ENOTTY (Inappropriate ioctl for device)
lseek(3, 0, SEEK_CUR)                   = 0
lseek(3, 0, SEEK_CUR)                   = 0
write(3, "hello\n", 6)                  = 6
close(3)                                = 0
rename("mydir/file1.txt", "mydir/file2.txt") = 0
chmod("mydir/file2.txt", 0600)          = 0
getuid()                                = 1000
getgid()                                = 1000
chown("mydir/file2.txt", 1000, 1000)    = 0
socket(AF_INET, SOCK_STREAM|SOCK_CLOEXEC, IPPROTO_IP) = 3
newfstatat(AT_FDCWD, "/etc/resolv.conf", {st_mode=S_IFREG|0644, st_size=197, ...}, 0) = 0
openat(AT_FDCWD, "/etc/host.conf", O_RDONLY|O_CLOEXEC) = 4
fstat(4, {st_mode=S_IFREG|0644, st_size=92, ...}) = 0
read(4, "# The \"order\" line is only used "..., 4096) = 92
read(4, "", 4096)                       = 0
close(4)                                = 0
futex(0x7ac14780b66c, FUTEX_WAKE_PRIVATE, 2147483647) = 0
openat(AT_FDCWD, "/etc/resolv.conf", O_RDONLY|O_CLOEXEC) = 4
fstat(4, {st_mode=S_IFREG|0644, st_size=197, ...}) = 0
read(4, "# This file was automatically ge"..., 4096) = 197
read(4, "", 4096)                       = 0
uname({sysname="Linux", nodename="LAPTOP-6OTQQ4LT", ...}) = 0
fstat(4, {st_mode=S_IFREG|0644, st_size=197, ...}) = 0
close(4)                                = 0
socket(AF_UNIX, SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK, 0) = 4
connect(4, {sa_family=AF_UNIX, sun_path="/var/run/nscd/socket"}, 110) = -1 ENOENT (No such file or directory)
close(4)                                = 0
socket(AF_UNIX, SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK, 0) = 4
connect(4, {sa_family=AF_UNIX, sun_path="/var/run/nscd/socket"}, 110) = -1 ENOENT (No such file or directory)
close(4)                                = 0
newfstatat(AT_FDCWD, "/etc/nsswitch.conf", {st_mode=S_IFREG|0644, st_size=526, ...}, 0) = 0
newfstatat(AT_FDCWD, "/", {st_mode=S_IFDIR|0755, st_size=4096, ...}, 0) = 0
openat(AT_FDCWD, "/etc/nsswitch.conf", O_RDONLY|O_CLOEXEC) = 4
fstat(4, {st_mode=S_IFREG|0644, st_size=526, ...}) = 0
read(4, "# /etc/nsswitch.conf\n#\n# Example"..., 4096) = 526
read(4, "", 4096)                       = 0
fstat(4, {st_mode=S_IFREG|0644, st_size=526, ...}) = 0
close(4)                                = 0
openat(AT_FDCWD, "/etc/hosts", O_RDONLY|O_CLOEXEC) = 4
fstat(4, {st_mode=S_IFREG|0644, st_size=427, ...}) = 0
lseek(4, 0, SEEK_SET)                   = 0
read(4, "# This file was automatically ge"..., 4096) = 427
read(4, "", 4096)                       = 0
close(4)                                = 0
socket(AF_INET, SOCK_DGRAM|SOCK_CLOEXEC|SOCK_NONBLOCK, IPPROTO_IP) = 4
setsockopt(4, SOL_IP, IP_RECVERR, [1], 4) = 0
connect(4, {sa_family=AF_INET, sin_port=htons(53), sin_addr=inet_addr("172.24.64.1")}, 16) = 0
poll([{fd=4, events=POLLOUT}], 1, 0)    = 1 ([{fd=4, revents=POLLOUT}])
sendto(4, "\31\277\1\0\0\1\0\0\0\0\0\0\7example\3com\0\0\1\0\1", 29, MSG_NOSIGNAL, NULL, 0) = 29
poll([{fd=4, events=POLLIN}], 1, 5000)  = 1 ([{fd=4, revents=POLLIN}])
ioctl(4, FIONREAD, [72])                = 0
recvfrom(4, "\31\277\201\0\0\1\0\2\0\0\0\0\7example\3com\0\0\1\0\1\7ex"..., 1024, 0, {sa_family=AF_INET, sin_port=htons(53), sin_addr=inet_addr("172.24.64.1")}, [28 => 16]) = 72
close(4)                                = 0
openat(AT_FDCWD, "/etc/gai.conf", O_RDONLY|O_CLOEXEC) = 4
fstat(4, {st_mode=S_IFREG|0644, st_size=2584, ...}) = 0
fstat(4, {st_mode=S_IFREG|0644, st_size=2584, ...}) = 0
read(4, "# Configuration for getaddrinfo("..., 4096) = 2584
read(4, "", 4096)                       = 0
close(4)                                = 0
futex(0x7ac147810ec4, FUTEX_WAKE_PRIVATE, 2147483647) = 0
socket(AF_NETLINK, SOCK_RAW|SOCK_CLOEXEC, NETLINK_ROUTE) = 4
bind(4, {sa_family=AF_NETLINK, nl_pid=0, nl_groups=00000000}, 12) = 0
getsockname(4, {sa_family=AF_NETLINK, nl_pid=610, nl_groups=00000000}, [12]) = 0
sendto(4, [{nlmsg_len=20, nlmsg_type=RTM_GETADDR, nlmsg_flags=NLM_F_REQUEST|NLM_F_DUMP, nlmsg_seq=1773869573, nlmsg_pid=0}, {ifa_family=AF_UNSPEC, ...}], 20, 0, {sa_family=AF_NETLINK, nl_pid=0, nl_groups=00000000}, 12) = 20
recvmsg(4, {msg_name={sa_family=AF_NETLINK, nl_pid=0, nl_groups=00000000}, msg_namelen=12, msg_iov=[{iov_base=[[{nlmsg_len=76, nlmsg_type=RTM_NEWADDR, nlmsg_flags=NLM_F_MULTI, nlmsg_seq=1773869573, nlmsg_pid=610}, {ifa_family=AF_INET, ifa_prefixlen=8, ifa_flags=IFA_F_PERMANENT, ifa_scope=RT_SCOPE_HOST, ifa_index=if_nametoindex("lo")}, [[{nla_len=8, nla_type=IFA_ADDRESS}, inet_addr("127.0.0.1")], [{nla_len=8, nla_type=IFA_LOCAL}, inet_addr("127.0.0.1")], [{nla_len=7, nla_type=IFA_LABEL}, "lo"], [{nla_len=8, nla_type=IFA_FLAGS}, IFA_F_PERMANENT], [{nla_len=20, nla_type=IFA_CACHEINFO}, {ifa_prefered=4294967295, ifa_valid=4294967295, cstamp=55, tstamp=55}]]], [{nlmsg_len=88, nlmsg_type=RTM_NEWADDR, nlmsg_flags=NLM_F_MULTI, nlmsg_seq=1773869573, nlmsg_pid=610}, {ifa_family=AF_INET, ifa_prefixlen=20, ifa_flags=IFA_F_PERMANENT, ifa_scope=RT_SCOPE_UNIVERSE, ifa_index=if_nametoindex("eth0")}, [[{nla_len=8, nla_type=IFA_ADDRESS}, inet_addr("172.24.69.10")], [{nla_len=8, nla_type=IFA_LOCAL}, inet_addr("172.24.69.10")], [{nla_len=8, nla_type=IFA_BROADCAST}, inet_addr("172.24.79.255")], [{nla_len=9, nla_type=IFA_LABEL}, "eth0"], [{nla_len=8, nla_type=IFA_FLAGS}, IFA_F_PERMANENT], [{nla_len=20, nla_type=IFA_CACHEINFO}, {ifa_prefered=4294967295, ifa_valid=4294967295, cstamp=55, tstamp=55}]]]], iov_len=4096}], msg_iovlen=1, msg_controllen=0, msg_flags=0}, 0) = 164
recvmsg(4, {msg_name={sa_family=AF_NETLINK, nl_pid=0, nl_groups=00000000}, msg_namelen=12, msg_iov=[{iov_base=[[{nlmsg_len=80, nlmsg_type=RTM_NEWADDR, nlmsg_flags=NLM_F_MULTI, nlmsg_seq=1773869573, nlmsg_pid=610}, {ifa_family=AF_INET6, ifa_prefixlen=128, ifa_flags=IFA_F_PERMANENT, ifa_scope=RT_SCOPE_HOST, ifa_index=if_nametoindex("lo")}, [[{nla_len=20, nla_type=IFA_ADDRESS}, inet_pton(AF_INET6, "::1")], [{nla_len=20, nla_type=IFA_CACHEINFO}, {ifa_prefered=4294967295, ifa_valid=4294967295, cstamp=55, tstamp=55}], [{nla_len=8, nla_type=IFA_FLAGS}, IFA_F_PERMANENT], [{nla_len=5, nla_type=IFA_PROTO}, "\x01"]]], [{nlmsg_len=80, nlmsg_type=RTM_NEWADDR, nlmsg_flags=NLM_F_MULTI, nlmsg_seq=1773869573, nlmsg_pid=610}, {ifa_family=AF_INET6, ifa_prefixlen=64, ifa_flags=IFA_F_PERMANENT, ifa_scope=RT_SCOPE_LINK, ifa_index=if_nametoindex("eth0")}, [[{nla_len=20, nla_type=IFA_ADDRESS}, inet_pton(AF_INET6, "fe80::215:5dff:fef3:80b1")], [{nla_len=20, nla_type=IFA_CACHEINFO}, {ifa_prefered=4294967295, ifa_valid=4294967295, cstamp=55, tstamp=55}], [{nla_len=8, nla_type=IFA_FLAGS}, IFA_F_PERMANENT], [{nla_len=5, nla_type=IFA_PROTO}, "\x03"]]]], iov_len=4096}], msg_iovlen=1, msg_controllen=0, msg_flags=0}, 0) = 160
recvmsg(4, {msg_name={sa_family=AF_NETLINK, nl_pid=0, nl_groups=00000000}, msg_namelen=12, msg_iov=[{iov_base=[{nlmsg_len=20, nlmsg_type=NLMSG_DONE, nlmsg_flags=NLM_F_MULTI, nlmsg_seq=1773869573, nlmsg_pid=610}, 0], iov_len=4096}], msg_iovlen=1, msg_controllen=0, msg_flags=0}, 0) = 20
close(4)                                = 0
socket(AF_INET, SOCK_DGRAM|SOCK_CLOEXEC, IPPROTO_IP) = 4
connect(4, {sa_family=AF_INET, sin_port=htons(0), sin_addr=inet_addr("104.18.26.120")}, 16) = 0
getsockname(4, {sa_family=AF_INET, sin_port=htons(39608), sin_addr=inet_addr("172.24.69.10")}, [28 => 16]) = 0
connect(4, {sa_family=AF_UNSPEC, sa_data="\0\0\0\0\0\0\0\0\0\0\0\0\0\0"}, 16) = 0
connect(4, {sa_family=AF_INET, sin_port=htons(0), sin_addr=inet_addr("104.18.27.120")}, 16) = 0
getsockname(4, {sa_family=AF_INET, sin_port=htons(49682), sin_addr=inet_addr("172.24.69.10")}, [28 => 16]) = 0
close(4)                                = 0
connect(3, {sa_family=AF_INET, sin_port=htons(80), sin_addr=inet_addr("104.18.26.120")}, 16) = 0
rt_sigaction(SIGINT, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=SA_RESTORER|SA_ONSTACK, sa_restorer=0x7ac147645330}, {sa_handler=0x6e7140, sa_mask=[], sa_flags=SA_RESTORER|SA_ONSTACK, sa_restorer=0x7ac147645330}, 8) = 0
getsockname(3, {sa_family=AF_INET, sin_port=htons(53684), sin_addr=inet_addr("172.24.69.10")}, [16]) = 0
getpeername(3, {sa_family=AF_INET, sin_port=htons(80), sin_addr=inet_addr("104.18.26.120")}, [16]) = 0
close(3)                                = 0
munmap(0x7ac147acf000, 16384)           = 0
exit_group(0)                           = ?
+++ exited with 0 +++
```

Running `straceaud` with that output gives an overview of which resources are accesed:

```
==== READ FILES ====

/etc/gai.conf
/etc/host.conf
/etc/hosts
/etc/ld.so.cache
/etc/ld.so.preload
/etc/localtime
/etc/nsswitch.conf
/etc/resolv.conf
/home/daniel
/home/daniel/test.py
/lib/x86_64-linux-gnu/libc.so.6
/lib/x86_64-linux-gnu/libexpat.so.1
/lib/x86_64-linux-gnu/libm.so.6
/lib/x86_64-linux-gnu/libz.so.1
/usr/bin/pybuilddir.txt
/usr/bin/python3
/usr/bin/python3.12
/usr/bin/python3.12._pth
/usr/bin/python3._pth
/usr/bin/pyvenv.cfg
/usr/lib/locale/C.UTF-8/LC_CTYPE
/usr/lib/locale/C.utf8/LC_CTYPE
/usr/lib/locale/locale-archive
/usr/lib/python3.12
/usr/lib/python3.12/__pycache__/_weakrefset.cpython-312.pyc
/usr/lib/python3.12/__pycache__/enum.cpython-312.pyc
/usr/lib/python3.12/__pycache__/functools.cpython-312.pyc
/usr/lib/python3.12/__pycache__/keyword.cpython-312.pyc
/usr/lib/python3.12/__pycache__/operator.cpython-312.pyc
/usr/lib/python3.12/__pycache__/reprlib.cpython-312.pyc
/usr/lib/python3.12/__pycache__/selectors.cpython-312.pyc
/usr/lib/python3.12/__pycache__/sitecustomize.cpython-312.pyc
/usr/lib/python3.12/__pycache__/socket.cpython-312.pyc
/usr/lib/python3.12/__pycache__/threading.cpython-312.pyc
/usr/lib/python3.12/__pycache__/types.cpython-312.pyc
/usr/lib/python3.12/__pycache__/warnings.cpython-312.pyc
/usr/lib/python3.12/collections
/usr/lib/python3.12/collections/__pycache__/__init__.cpython-312.pyc
/usr/lib/python3.12/collections/__pycache__/abc.cpython-312.pyc
/usr/lib/python3.12/encodings
/usr/lib/python3.12/encodings/__pycache__/__init__.cpython-312.pyc
/usr/lib/python3.12/encodings/__pycache__/aliases.cpython-312.pyc
/usr/lib/python3.12/encodings/__pycache__/utf_8.cpython-312.pyc
/usr/lib/python3.12/importlib
/usr/lib/python3.12/importlib/__pycache__/__init__.cpython-312.pyc
/usr/lib/python3.12/importlib/__pycache__/_abc.cpython-312.pyc
/usr/lib/python3.12/lib-dynload
/usr/lib/python3/dist-packages
/usr/lib/python3/dist-packages/__pycache__/apport_python_hook.cpython-312.pyc
/usr/lib/python3/dist-packages/_distutils_hack/__pycache__/__init__.cpython-312.pyc
/usr/lib/python3/dist-packages/distutils-precedence.pth
/usr/lib/python3/dist-packages/zope.interface-6.1-nspkg.pth
/usr/lib/x86_64-linux-gnu/gconv/gconv-modules.cache
/usr/local/lib/python3.12/dist-packages
/usr/pyvenv.cfg
/usr/share/locale/locale.alias
test.py

==== WRITE FILES ====

mydir/file1.txt

==== CREATED FILES ====

mydir/file1.txt

==== CREATED DIRECTORIES ====

mydir

==== DELETED FILES ====


==== RENAMED FILES ====

mydir/file1.txt -> mydir/file2.txt

==== METADATA ====

/
/etc/gai.conf
/etc/host.conf
/etc/hosts
/etc/ld.so.cache
/etc/localtime
/etc/nsswitch.conf
/etc/resolv.conf
/home/daniel
/home/daniel/.local/lib/python3.12/site-packages
/home/daniel/test.py
/lib/x86_64-linux-gnu/libc.so.6
/lib/x86_64-linux-gnu/libexpat.so.1
/lib/x86_64-linux-gnu/libm.so.6
/lib/x86_64-linux-gnu/libz.so.1
/usr/bin/Modules/Setup.local
/usr/bin/lib/python3.12/lib-dynload
/usr/bin/lib/python3.12/os.py
/usr/bin/lib/python3.12/os.pyc
/usr/bin/lib/python312.zip
/usr/bin/python3
/usr/bin/pyvenv.cfg
/usr/lib
/usr/lib/locale/C.utf8/LC_CTYPE
/usr/lib/python3.12
/usr/lib/python3.12/__pycache__/_weakrefset.cpython-312.pyc
/usr/lib/python3.12/__pycache__/enum.cpython-312.pyc
/usr/lib/python3.12/__pycache__/functools.cpython-312.pyc
/usr/lib/python3.12/__pycache__/keyword.cpython-312.pyc
/usr/lib/python3.12/__pycache__/operator.cpython-312.pyc
/usr/lib/python3.12/__pycache__/reprlib.cpython-312.pyc
/usr/lib/python3.12/__pycache__/selectors.cpython-312.pyc
/usr/lib/python3.12/__pycache__/sitecustomize.cpython-312.pyc
/usr/lib/python3.12/__pycache__/socket.cpython-312.pyc
/usr/lib/python3.12/__pycache__/threading.cpython-312.pyc
/usr/lib/python3.12/__pycache__/types.cpython-312.pyc
/usr/lib/python3.12/__pycache__/warnings.cpython-312.pyc
/usr/lib/python3.12/_weakrefset.py
/usr/lib/python3.12/collections
/usr/lib/python3.12/collections/__init__.abi3.so
/usr/lib/python3.12/collections/__init__.cpython-312-x86_64-linux-gnu.so
/usr/lib/python3.12/collections/__init__.py
/usr/lib/python3.12/collections/__init__.so
/usr/lib/python3.12/collections/__pycache__/__init__.cpython-312.pyc
/usr/lib/python3.12/collections/__pycache__/abc.cpython-312.pyc
/usr/lib/python3.12/collections/abc.py
/usr/lib/python3.12/dist-packages
/usr/lib/python3.12/encodings
/usr/lib/python3.12/encodings/__init__.abi3.so
/usr/lib/python3.12/encodings/__init__.cpython-312-x86_64-linux-gnu.so
/usr/lib/python3.12/encodings/__init__.py
/usr/lib/python3.12/encodings/__init__.so
/usr/lib/python3.12/encodings/__pycache__/__init__.cpython-312.pyc
/usr/lib/python3.12/encodings/__pycache__/aliases.cpython-312.pyc
/usr/lib/python3.12/encodings/__pycache__/utf_8.cpython-312.pyc
/usr/lib/python3.12/encodings/aliases.py
/usr/lib/python3.12/encodings/utf_8.py
/usr/lib/python3.12/enum.py
/usr/lib/python3.12/functools.py
/usr/lib/python3.12/importlib
/usr/lib/python3.12/importlib/__init__.abi3.so
/usr/lib/python3.12/importlib/__init__.cpython-312-x86_64-linux-gnu.so
/usr/lib/python3.12/importlib/__init__.py
/usr/lib/python3.12/importlib/__init__.so
/usr/lib/python3.12/importlib/__pycache__/__init__.cpython-312.pyc
/usr/lib/python3.12/importlib/__pycache__/_abc.cpython-312.pyc
/usr/lib/python3.12/importlib/_abc.py
/usr/lib/python3.12/keyword.py
/usr/lib/python3.12/lib-dynload
/usr/lib/python3.12/operator.py
/usr/lib/python3.12/os.py
/usr/lib/python3.12/reprlib.py
/usr/lib/python3.12/selectors.py
/usr/lib/python3.12/sitecustomize.py
/usr/lib/python3.12/socket.py
/usr/lib/python3.12/threading.py
/usr/lib/python3.12/types.py
/usr/lib/python3.12/warnings.py
/usr/lib/python3/dist-packages
/usr/lib/python3/dist-packages/__pycache__/apport_python_hook.cpython-312.pyc
/usr/lib/python3/dist-packages/_distutils_hack/__init__.abi3.so
/usr/lib/python3/dist-packages/_distutils_hack/__init__.cpython-312-x86_64-linux-gnu.so
/usr/lib/python3/dist-packages/_distutils_hack/__init__.py
/usr/lib/python3/dist-packages/_distutils_hack/__init__.so
/usr/lib/python3/dist-packages/_distutils_hack/__pycache__/__init__.cpython-312.pyc
/usr/lib/python3/dist-packages/apport_python_hook.py
/usr/lib/python3/dist-packages/distutils-precedence.pth
/usr/lib/python3/dist-packages/zope.interface-6.1-nspkg.pth
/usr/lib/python3/dist-packages/zope/__init__.abi3.so
/usr/lib/python3/dist-packages/zope/__init__.cpython-312-x86_64-linux-gnu.so
/usr/lib/python3/dist-packages/zope/__init__.py
/usr/lib/python3/dist-packages/zope/__init__.so
/usr/lib/python312.zip
/usr/lib/x86_64-linux-gnu/gconv/gconv-modules.cache
/usr/local/bin/python3
/usr/local/lib/python3.12/dist-packages
/usr/local/sbin/python3
/usr/pyvenv.cfg
/usr/sbin/python3
/usr/share/locale/locale.alias
mydir/file1.txt

==== EXEC ====

/usr/bin/python3

==== PROCESS ====


==== NETWORK CONNECT ====

socket

==== NETWORK BIND ====

bind

==== IPC ====

fcntl
ioctl
mmap

==== MOUNT ====


==== PRIVILEGED ====
```