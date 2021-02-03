# RSA Algorithm

RSA 算法在1977年由麻省理工学院的 Ron Rivest、Adi Shamir 和 Leonard Adleman 一起提出，并以他们三人姓氏开头字母命名，是一种获得广泛使用的非对称加密算法。其加密安全性取决于对极大整数进行因数分解的难度。

## Quick Start

该项目在 linux 环境下构建，运行程序需要安装 linux 下的大数运算库 [gmp](https://gmplib.org/)。安装对应的库后，执行下列指令可以运行 demo。

```shell
git clone git@github.com:warpmatrix/rsa.git
cd rsa
make
./bin/demo
```

## RSA 算法中 p 和 q 满足的一些基本原则

- (D. Coppersmith) p 和 q 不能太近。如果 N 的位数为 k，那么 |p - q| 要同时满足 log|p - q| > k/2-100 以及 log|p - q| > k/3。
- (D. Coppersmith) 较短的 e 可以提高加密算法计算 n, e 的速度，但可能存在计算 d 的快速算法。
  - PKCS#1 建议 e = 2^16 + 1 = 65537，即 e = 0x10001。
- (M. Wiener) d 不能太小，否则有可能通过快速算法得到 d。如果 N 的位数为 k，那么 d 的值要满足 d > 2^(k/2)
- (O. Schirokauer) N 的非相邻形式 (Non-Adjacent Form，NAF) 的海明权重 (Hamming weight，非 0 元总数) 不能太小，否则应用数筛法可能可以快速对 N 进行质因数分解。一般要求 N 的 NAF 表述权重大于 N/4。

> 算法实现了 RSA 的基本功能，也满足上述选取 p 和 q 的原则。但没有完全遵循 RFC 8017: PKCS #1。
