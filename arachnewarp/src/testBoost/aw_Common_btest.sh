#! /bin/sh

echo "testing aw_Common_btest.cpp"

BOOST_DIR="/Volumes/xdisc/_sync/_x/src/cpp/boost_1_43_0"
AW_CORE="/Volumes/xdisc/_sync/_x/src/arachnewarp/arachnewarp/src/core"

g++ aw_Common_btest.cpp ../core/aw_Common.cpp -Wall -I ${BOOST_DIR} -I ${AW_CORE} -o aw_Common_btest

./aw_Common_btest

