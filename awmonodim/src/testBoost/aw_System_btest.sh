#! /bin/sh

echo "testing aw_System_btest.cpp"

BOOST_DIR="/Volumes/xdisc/_sync/_x/src/cpp/boost_1_43_0"
AW_CORE="/Volumes/xdisc/_sync/_x/src/arachnewarp/arachnewarp/src/core"

g++ aw_System_btest.cpp ../core/aw_System.cpp -Wall -I ${BOOST_DIR} -I ${AW_CORE} -o aw_System_btest


./aw_System_btest

