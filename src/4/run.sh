mkdir build report
cd build

cmake .. &&
make &&
clear &&
(
echo "\n============ Testing ============"
./test \
--gtest_print_time=0 \
--gtest_output=json:../report/test.json

echo "\n=========== Benchmark ==========="
./benchmark \
--benchmark_out_format=csv --benchmark_out=../report/benchmark.csv

echo "\n============ Program ============"
./app
# ./app -b > ../report/benchmark_main.csv
)

# https://devhints.io/bash
