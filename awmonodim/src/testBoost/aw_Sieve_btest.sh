#! /bin/sh

echo "testing aw_Sieve_btest.cpp"

BOOST_DIR="/Volumes/xdisc/_sync/_x/src/cpp/boost_1_43_0"
AW_CORE="/Volumes/xdisc/_sync/_x/src/arachnewarp/arachnewarp/src/core"

# need AW_CORE b/c i call header files
# library is stored in /usr/local/lib
g++ aw_Sieve_btest.cpp -Wall -I ${BOOST_DIR} -I ${AW_CORE} -l "arachnewarp" -o aw_Sieve_btest


./aw_Sieve_btest


