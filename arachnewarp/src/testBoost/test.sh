#! /bin/sh

# http://www.boost.org/doc/libs/1_34_1/libs/test/doc/components/test_tools/reference/
#BOOST_CHECK( predicate )

# two argument comparison
# BOOST_CHECK_EQUAL(a, b)

# comparing assertions
# BOOST_CHECK_THROW()

# replace ASSERT_NEAR
# BOOST_CHECK_CLOSE( v1, v2, 0.0001 );

# TODO: recompile .dylib

./aw_Add_btest.sh
./aw_Click_btest.sh
./aw_Common_btest.sh
./aw_Constant_btest.sh
./aw_FilterHighPass_btest.sh

./aw_Generator_btest.sh
./aw_GeneratorFactory_btest.sh

./aw_Multiply_btest.sh
./aw_Output_btest.sh
./aw_PanStereo_btest.sh
./aw_PolyAdd_btest.sh

./aw_PolyConstant_btest.sh
./aw_PolyGenerator_btest.sh
./aw_RandomUniform_btest.sh
./aw_ResidualClass_btest.sh
./aw_Selector_btest.sh

./aw_Sieve_btest.sh
./aw_System_btest.sh


./aw_WaveSine_btest.sh

./aw_WaveSquare_btest.sh


