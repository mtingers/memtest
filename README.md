# memtest
A simple memory throughput tester.

Some uses:
1. Compare memory throughput differences between systems.
2. Check that written memory reads back correctly.

# Example

```
$ ./memtest  1500
Starting memtest: size=1572864000 bytes (1500.00MiB) (this runs forever)...
 write:    1992.39MB/s       0.75 seconds
 write:   11153.83MB/s       0.13 seconds
  read:    1605.40MB/s       0.93 seconds
  read:    1587.00MB/s       0.95 seconds
   l/r:    1408.92MB/s       1.06 seconds
cmp-rw:     897.89MB/s       1.67 seconds
 write:   13894.81MB/s       0.11 seconds
 write:   12284.81MB/s       0.12 seconds
  read:    1608.14MB/s       0.93 seconds
  read:    1606.80MB/s       0.93 seconds
   l/r:    1388.97MB/s       1.08 seconds
cmp-rw:     878.75MB/s       1.71 seconds
```
