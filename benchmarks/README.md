# Benchmarking tools for TWINE
This folder contains the experiments illustrated in the paper **TWINE: An Embedded Trusted Runtime for WebAssembly**.
This README helps to reproduce them, by providing the set up required to execute them.

## Prerequisites
### Operating system and software
- Ubuntu 18.04 (tested on 18.04.5)
- Intel SGX SDK 2.11+ and SGX driver 2.6+ (the SDK is expected to be installed in `/opt/intel/sgxsdk`) [[instructions](https://download.01.org/intel-sgx/latest/linux-latest/docs/)]
- Docker (tested on 20.10.1) [[instructions](https://docs.docker.com/engine/install/ubuntu/)]
- git
- SGX-LKL [[instructions]](https://github.com/lsds/sgx-lkl#a-installing-sgx-lkl-oe) (requires the package `cryptsetup`)
- Python3 for running the cost factors analysis (with the additional package `pexpect`)
- Jupyter Notebook (to visualize the results; can be installed on a separate machine)



### Dependencies
#### TWINE
The source code of TWINE must be located in `/opt/wamr-sdk`.
The following commands clone its repository and symlink TWINE.
Note that the default branch for running the benchmarks is `optimized-pfs`.

```
sudo mkdir /opt/twine
sudo sudo chown $(id -u ${USER}):$(id -g ${USER}) /opt/twine
git clone --recurse-submodules --branch optimized-pfs https://github.com/JamesMenetrey/unine-twine.git /opt/twine
sudo ln -s /opt/twine/runtime /opt/wamr-sdk
```

#### The official WAMR runtime
The source code of official WAMR must be located in `/opt/wamr-sdk-official`.
The experiments use a fork of it.

```
sudo mkdir /opt/wamr-sdk-official
sudo chown $(id -u ${USER}):$(id -g ${USER}) /opt/wamr-sdk-official
git clone --depth 1 https://github.com/JamesMenetrey/wasm-micro-runtime.git /opt/wamr-sdk-official
```

## Running the experiments
### PolyBench/C (Figure 3)
```
cd /opt/twine/benchmarks
./polybench_build_docker.sh
./polybench_run_baremetal.sh
```

The output of the experiments are stored in `/opt/twine/benchmarks/polybench/results`.
The Jupyter notebook `polybench_analysis.ipynb` in the same folder can be used to analyse the raw data.
An instance of Jupyter notebook can be launched using the scipt: `/opt/twine/benchmarks/polybench/results/launch.sh`.



### Speedtest1 (Figure 4)
```
cd /opt/twine/benchmarks
./speedtest1_build_docker.sh
./speedtest1_run_baremetal.sh
```

The output of the experiments are stored in `/opt/twine/benchmarks/speedtest1/results`.
The Jupyter notebook `speedtest1_analysis.ipynb` in the same folder can be used to analyse the raw data.
An instance of Jupyter notebook can be launched using the scipt: `/opt/twine/benchmarks/speedtest1/results/launch.sh`.



### Breakdown of SQLite macro-benchmarks (Figure 5 / Table 2)
```
cd /opt/twine/benchmarks
./benchmark_build_docker.sh
./benchmark_run_baremetal.sh
```

The output of the experiments are stored in `/opt/twine/benchmarks/sqlite/results`.
The Jupyter notebook `benchmark_analysis.ipynb` in the same folder can be used to analyse the raw data.
An instance of Jupyter notebook can be launched using the scipt: `/opt/twine/benchmarks/sqlite/results/launch.sh`.



### Normalised run time for SGX variants with in-file database (Figure 6)
This bar chart displays the average of execution time for SGX-LKL and Twine in hardware and software mode for SGX.
The hardware/software mode can be changed following these instructions:

 - For SGX-LKL, edit the file `/opt/twine/benchmarks/sqlite/benchmark-sgx/run-benchmark.sh` and toggle the argument `--hw-debug` for hardware mode or `--sw-debug` for software mode.
 - For Twine, edit the file `/opt/twine/benchmarks/sqlite/benchmark-wasm-sgx/src/Makefile` and toggle `SGX_MODE ?= HW` for hardware mode or `SGX_MODE ?= SW` for software mode.

Afterwards, execute the micro-benchmarks as Figure 5:
```
cd /opt/twine/benchmarks
./benchmark_build_docker.sh
./benchmark_run_baremetal.sh
```

The output of the experiments are stored in `/opt/twine/benchmarks/speedtest1/results`. Average the results based on the category indicated in the first column of the CSV files (i.e., `i` for insertion, `qs` for sequential reading and `qr` for random reading).



### Breakdown of official/optimised IPFS (Figure 7)
The plot is made in two steps: (1) evaluate the performance without optimisation, and (2) evaluate the performance with the optimisation.

#### Evaluation the performance without optimisation (master branch)
```
cd /opt/twine/benchmarks
git checkout master
./benchmark_build_docker.sh
./benchmark_run_baremetal.sh
mv sqlite/results/benchmark-wasm-sgx.csv sqlite/results/profiling-benchmark-wasm-sgx-regular.csv
```

#### Evaluation the performance with optimisation (optimized-pfs branch)
```
cd /opt/twine/benchmarks
git checkout optimized-pfs
./benchmark_build_docker.sh
./benchmark_run_baremetal.sh
mv sqlite/results/benchmark-wasm-sgx.csv sqlite/results/profiling-benchmark-wasm-sgx-nocopy.csv
```

#### Analysis of the results
The output of the experiments are stored in `/opt/twine/benchmarks/sqlite/results`.
The Jupyter notebook `profiling_analysis.ipynb` in the same folder can be used to analyse the raw data.
An instance of Jupyter notebook can be launched using the scipt: `/opt/twine/benchmarks/sqlite/results/launch.sh`.

The execution time are calculated as follows (the calculation can be seen in function `convert_to_report_breakdown`):

- Read (OCALLs): `trusted_fread_node_read_mht_node` + `trusted_fread_node_read_data_node`
- Read (memset): `trusted_memset_read_mht_node` + `trusted_memset_append_mht_node` + `trusted_memset_read_data_node` + `trusted_memset_append_data_node` + `trusted_memset_get_data_node`
- Read (other ops.): `wasi_fd_read` - (*Read (OCALLs):* + *Read (memset)*)
- SQLite inner work: *execution time* - *all wasi colummns*



### Cost factors analysis (Table 3)
```
cd /opt/twine/benchmarks
./cost-factors_build_docker.sh
./cost-factors_run_baremetal.sh
```

The output of the experiments are stored in `/opt/twine/benchmarks/sqlite/cost-factors/results`.
The Jupyter notebook `cost-factors_analysis.ipynb` in the same folder can be used to analyse the raw data.
An instance of Jupyter notebook can be launched using the scipt: `/opt/twine/benchmarks/sqlite/cost-factors/results/launch.sh`.

## License

The source code of TWINE and the source code of the benchmarks are released under the Apache license 2.0.
Check the file `LICENSE` for more information.

## Author

Jämes Ménétrey @ University of Neuchâtel, Switzerland