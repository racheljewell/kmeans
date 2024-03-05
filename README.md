# kmeans

A small program implementing the k-means algorithm.
This program is written in C++ and can be run on
any system with C++11 installed.

In a Linux environment:

To compile:

```bash
g++ -o kmeans kmeans.cpp
```

To run:\
The program expects an integer value as a "k" value, and a
text file continaing point values. It expects the text file
to be in the following format:

point1x  point1y\
point2x point2y\
...

```bash
./kmeans 2 input1.txt
```
