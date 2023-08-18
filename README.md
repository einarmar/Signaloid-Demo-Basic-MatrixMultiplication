[<img src="https://assets.signaloid.io/add-to-signaloid-cloud-logo-dark-v6.png#gh-dark-mode-only" alt="[Add to signaloid.io]" height="30">](https://signaloid.io/repositories?connect=https://github.com/einarmar/Signaloid-Demo-Basic-MatrixMultiplication#gh-dark-mode-only)
[<img src="https://assets.signaloid.io/add-to-signaloid-cloud-logo-light-v6.png#gh-light-mode-only" alt="[Add to signaloid.io]" height="30">](https://signaloid.io/repositories?connect=https://github.com/einarmar/Signaloid-Demo-Basic-MatrixMultiplication#gh-light-mode-only)

# MICRO Benchmark: `matrix-multiplication`

Benchmark from Tsoutsouras et al. MICRO paper.[^0]

The program calculates the multiplication of two matrices using the naive matrix multiplication algorithm.

## Arguments

```
matrix-multiplication -a <file prefix for A> -m <A rows> -n <A columns> -b <file prefix for B> -x <B rows> -y <B columns>
	-a <file prefix for A>: default is "gaussian/input-A"
	-m <A rows>:            default is 8
	-n <A columns>:         default is 8
	-b <file prefix for B>: default is "gaussian/input-B"
	-x <B rows>:            default is 8
	-y <B columns>:         default is 8
```

## Inputs

The inputs make up two 8x8 matrices: matrix A and matrix B. There is one input file corresponding to each matrix element.
For example, file `input-A-0-0.txt` holds samples for the top left element of matrix A.
All samples in these files are from a Gaussian distribution of mean `1.0` and variance `1.0`.

## Ouputs

The program prints the means of the matrix multiplication product.

[^0]: Vasileios Tsoutsouras, Orestis Kaparounakis, Bilgesu Arif Bilgin, Chatura Samarakoon, James Timothy Meech, Jan Heck, Phillip Stanley-Marbell: The Laplace Microarchitecture for Tracking Data Uncertainty and Its Implementation in a RISC-V Processor. MICRO 2021: 1254-1269

## Version

This project is a C++ implementation of the C example from Signaloid.