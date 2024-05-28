# HW4

```shell
cd $EE6470 && cd riscv-vp/vp/build && rm -r CMakeCache.txt && make install
cd $EE6470 && cd riscv-vp/sw && cd basic-sobel && make && make sim
```
## 1. (80 pt) In this homework, please port the Gaussian Blur Filter module to the "basic-acc" platform in Lab 8. You may simply replace the sobel-filter module in the platform. Please also port your test bench as a RISC-V software.

- In this homework, you should download
```shell
git clone https://gitlab.larc-nthu.net/ee6470_TA/sobel_riscv_vp.git
```
- Download the file from github and replace "basic-acc" file in "ee6470\riscv-vp\vp\src\platform" and replace "basic-sobel" file in "ee6470\riscv-vp\sw"

## 2. (20 pt) Please add the latency of the Gaussian Blur Filter module from HLS.

