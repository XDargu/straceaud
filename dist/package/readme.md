# straceaud

## Installation

Install the package usind dpkg

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