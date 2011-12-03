#! /bin/sh

echo "testing aw_Generator_btest.cpp"

BOOST_DIR="/Volumes/xdisc/_sync/_x/src/cpp/boost_1_43_0"
AW_CORE="/Volumes/xdisc/_sync/_x/src/arachnewarp/arachnewarp/src/core"

g++ aw_Generator_btest.cpp  ../core/aw_Add.cpp ../core/aw_Click.cpp ../core/aw_Common.cpp ../core/aw_Constant.cpp  ../core/aw_FilterHighPass.cpp ../core/aw_FilterLowPass.cpp    ../core/aw_Generator.cpp  ../core/aw_GeneratorFactory.cpp ../core/aw_Multiply.cpp ../core/aw_PanStereo.cpp ../core/aw_PolyAdd.cpp ../core/aw_PolyConstant.cpp  ../core/aw_PolyGenerator.cpp  ../core/aw_Output.cpp ../core/aw_RandomUniform.cpp  ../core/aw_Selector.cpp ../core/aw_System.cpp   ../core/aw_WaveSine.cpp  ../core/aw_WaveSquare.cpp    -Wall -I ${BOOST_DIR} -I ${AW_CORE} -l "sndfile" -o aw_Generator_btest

./aw_Generator_btest
