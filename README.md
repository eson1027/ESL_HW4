# HW4
## 1. (80 pt) In this homework, please port the Gaussian Blur Filter module to the "basic-acc" platform in Lab 8. You may simply replace the sobel-filter module in the platform. Please also port your test bench as a RISC-V software.

- In this homework, you should download
```shell
git clone https://gitlab.larc-nthu.net/ee6470_TA/sobel_riscv_vp.git
git clone https://github.com/eson1027/ESL_HW4
```
- Replace "basic-acc" file in "riscv-vp\vp\src\platform" and "basic-sobel" file in "riscv-vp\sw"
```shell
cd $EE6470 && cd riscv-vp/vp/build && rm -r CMakeCache.txt && make install
cd $EE6470 && cd riscv-vp/sw && cd basic-sobel && make && make sim
```

## 2. (20 pt) Please add the latency of the Gaussian Blur Filter module from HLS.
```shell
cd $EE6470 && cd riscv-vp/vp/build && rm -r CMakeCache.txt && make install
cd $EE6470 && cd riscv-vp/sw && cd basic-sobel && make && make sim
```
