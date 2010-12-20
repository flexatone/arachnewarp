/*! \file aw_WaveSquare_unittest.cpp
    \brief Tests for aw_WaveSquare.cpp
    
Created by Christopher Ariza on 6/30/10.
Copyright 2010 Flexatone HFP. All rights reserved.
*/


#include <stdexcept>

#include <boost/shared_ptr.hpp>

#include "aw_Selector.h"
#include "aw_GeneratorFactory.h"

#include <gtest/gtest.h>


using namespace aw;



// provide testCase, name
TEST(BasicTests, SelectorBasic) {

    //System sys;
    // set sampling rate to 1 for testing
    SystemPtr sys(new System(44100, 8));

    Selector gen1(sys); 


    EXPECT_EQ(gen1.isValidGenerator(aw::pNameValueList, aw::gTypePoly), true);
    EXPECT_EQ(gen1.isValidGenerator(aw::pNameValueList, aw::gTypeMono), false);

    EXPECT_EQ(gen1.isValidGenerator(aw::pNameSelectionMethod, aw::gTypeMono), true);


}


TEST(BasicTests, SelectorArgumentAssignment) {


    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance

    GeneratorPtr gen1 = gf.create(aw::gNameSelector);
    GeneratorPtr gen2 = gf.create(aw::gNamePolyConstant);
    GeneratorPtr gen3 = gf.create(aw::gNameConstant);
    GeneratorPtr gen4 = gf.create(aw::gNameConstant);


    // on poly constant, called value 

    gen2->setParameter("value", "8,4,2");
    //gen2->setParameter(aw::pNameValue, "8,4,2");

    // 0 is ordered cyclic
    gen3->setParameter("value", 0.0);

    // permitted b/c this is a poly
    gen1->setParameter("valueList", gen2);
    // permitted b/c this is a mono
    gen1->setParameter("selectionMethod", gen3);


    gen4->setParameter("value", 1);
    gen1->setParameter("refresh", gen4, "trigger");


    EXPECT_EQ(gen1->getValueAtSample(0), 8);
    EXPECT_EQ(gen1->getValueAtSample(1), 4);
    EXPECT_EQ(gen1->getValueAtSample(2), 2);
    EXPECT_EQ(gen1->getValueAtSample(3), 8);


}




TEST(BasicTests, SelectorGetParameterString) {


    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance

    GeneratorPtr gen1 = gf.create(aw::gNameSelector);
    GeneratorPtr gen2 = gf.create(aw::gNamePolyConstant);
    GeneratorPtr gen3 = gf.create(aw::gNameConstant);
    GeneratorPtr gen4 = gf.create(aw::gNameConstant);


    // on poly constant, called value 

    gen2->setParameter("value", "8,4,2");
    gen1->setParameter("valueList", gen2);
    gen1->setParameter("selectionMethod", gen3);

    gen4->setParameter("value", 1);
    gen1->setParameter("refresh", gen4, "trigger");


    EXPECT_EQ(gen1->getParameterString(), "Selector{valueList{PolyConstant{8,4,2}}selectionMethod{Constant{0}}refresh{Constant{1}}{trigger}stride{Constant{1}}}");

    // compact version
    EXPECT_EQ(gen1->getParameterString(true), "Selector{valueList{8,4,2}selectionMethod{0}refresh{1}{trigger}stride{1}}");


}


TEST(BasicTests, SelectorReadParameterString) {


    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance

    // long form works
    GeneratorPtr gen1 = gf.create("Selector{valueList{PolyConstant{value{8,4,2}}}selectionMethod{Constant{0}}refresh{Constant{1}}{trigger}stride{Constant{1}}}");


    GeneratorPtr gen2 = gf.create("Selector{valueList{8,4,2}selectionMethod{1}refresh{1}{trigger}stride{1}}");

    EXPECT_EQ(gen1->getParameter("valueList")->getParameterString(), "PolyConstant{8,4,2}");

    EXPECT_EQ(gen1->getParameter("refresh")->getParameterString(), "Constant{1}");

    }



TEST(BasicTests, SelectorCyclic) {


    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance

    // default trigger is refresh 1, trigger
    GeneratorPtr gen1 = gf.create("Selector{valueList{1,20,300,4000}selectionMethod{0}}");

    EXPECT_EQ(gen1->getValueAtSample(0), 1);
    EXPECT_EQ(gen1->getValueAtSample(1), 20);
    EXPECT_EQ(gen1->getValueAtSample(2), 300);
    EXPECT_EQ(gen1->getValueAtSample(3), 4000);
    EXPECT_EQ(gen1->getValueAtSample(4), 1);

    // after reset, start w/ position in cycle, independent of sample time
    gen1->reset(); 
    EXPECT_EQ(gen1->getValueAtSample(5), 1);
    EXPECT_EQ(gen1->getValueAtSample(6), 20);

    // create new selection method, ordered cyclic reverse
    gen1 = gf.create("Selector{valueList{1,20,300,4000}selectionMethod{1}}");

    EXPECT_EQ(gen1->getValueAtSample(0), 4000);
    EXPECT_EQ(gen1->getValueAtSample(1), 300);
    EXPECT_EQ(gen1->getValueAtSample(2), 20);
    EXPECT_EQ(gen1->getValueAtSample(3), 1);
    EXPECT_EQ(gen1->getValueAtSample(4), 4000);
    EXPECT_EQ(gen1->getValueAtSample(5), 300);


    // create new selection method, ordered oscillate
    gen1 = gf.create("Selector{valueList{50,51,52,53}selectionMethod{2}}");
    EXPECT_EQ(gen1->getValueAtSample(0), 50);
    EXPECT_EQ(gen1->getValueAtSample(1), 51);
    EXPECT_EQ(gen1->getValueAtSample(2), 52);
    EXPECT_EQ(gen1->getValueAtSample(3), 53);
    EXPECT_EQ(gen1->getValueAtSample(4), 52);
    EXPECT_EQ(gen1->getValueAtSample(5), 51);
    EXPECT_EQ(gen1->getValueAtSample(6), 50);
    EXPECT_EQ(gen1->getValueAtSample(7), 51);
    EXPECT_EQ(gen1->getValueAtSample(8), 52);
    EXPECT_EQ(gen1->getValueAtSample(9), 53);
    EXPECT_EQ(gen1->getValueAtSample(10), 52);
    EXPECT_EQ(gen1->getValueAtSample(11), 51);
    EXPECT_EQ(gen1->getValueAtSample(12), 50);
    EXPECT_EQ(gen1->getValueAtSample(13), 51);
    EXPECT_EQ(gen1->getValueAtSample(14), 52);

    gen1->printSample(10, 0); // display 10 sample outputs, b/n 0 and 1


    // ordered oscillate with 2 values
    gen1 = gf.create("Selector{valueList{2,8}selectionMethod{2}}");
    EXPECT_EQ(gen1->getValueAtSample(0), 2);
    EXPECT_EQ(gen1->getValueAtSample(1), 8);
    EXPECT_EQ(gen1->getValueAtSample(2), 2);
    EXPECT_EQ(gen1->getValueAtSample(3), 8);
    EXPECT_EQ(gen1->getValueAtSample(4), 2);
    EXPECT_EQ(gen1->getValueAtSample(5), 8);

}






TEST(BasicTests, SelectorRandomChoice) {


    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance

    // section method 3 is random choice
    GeneratorPtr gen1 = gf.create("Selector{valueList{1,2,3,4,5,6.2,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28.1,29,30}selectionMethod{3}}");

    double a = gen1->getValueAtSample(0);
    double b = gen1->getValueAtSample(1);
    double c = gen1->getValueAtSample(2);

    // this is probabilistic: assuming chance of getting same value is small
    EXPECT_EQ(a == b, false);
    EXPECT_EQ(a == c, false);

    gen1->printSample(10, 0); // display 10 sample outputs, b/n 0 and 1
    gen1->printSample(10, 0); // display 10 sample outputs, b/n 0 and 1



}




TEST(BasicTests, SelectorRefresh) {


    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance

    // ordered cyclic
    GeneratorPtr gen1 = gf.create("Selector{ \
        valueList{1,2,3,4,5,6,7,8,9,10} \
        selectionMethod{0}  \
        refresh{1}{sec}  \
    }");

    EXPECT_EQ(gen1->getValueAtSample(0), 1);
    EXPECT_EQ(gen1->getValueAtSample(44100*.5), 1);

    EXPECT_EQ(gen1->getValueAtSample(44100*2+1), 2);
    EXPECT_EQ(gen1->getValueAtSample(44100*2.6), 2);

    EXPECT_EQ(gen1->getValueAtSample(44100*3+1), 3);
    EXPECT_EQ(gen1->getValueAtSample(44100*3.2), 3);

    EXPECT_EQ(gen1->getValueAtSample(44100*4+1), 4);
    EXPECT_EQ(gen1->getValueAtSample(44100*4.2), 4);

    EXPECT_EQ(gen1->getValueAtSample(44100*5+1), 5);
    EXPECT_EQ(gen1->getValueAtSample(44100*5.2), 5);

    EXPECT_EQ(gen1->getValueAtSample(44100*6+1), 6);
    EXPECT_EQ(gen1->getValueAtSample(44100*6.2), 6);

    // the next time shift past boundary gets the next value;
    // this is consistent 
    EXPECT_EQ(gen1->getValueAtSample(44100*10+1), 7);
    EXPECT_EQ(gen1->getValueAtSample(44100*10.2), 7);


    // ordered cyclic
    GeneratorPtr gen2 = gf.create("Selector{ \
        valueList{6,7,8,9} \
        selectionMethod{0}  \
        refresh{1}{samples}  \
    }");
    EXPECT_EQ(gen2->getValueAtSample(0), 6);
    EXPECT_EQ(gen2->getValueAtSample(1), 7);
    EXPECT_EQ(gen2->getValueAtSample(2), 8);
    EXPECT_EQ(gen2->getValueAtSample(3), 9);
    EXPECT_EQ(gen2->getValueAtSample(8), 6);
    EXPECT_EQ(gen2->getValueAtSample(9), 7);




    // ordered cyclic reverse
    gen2 = gf.create("Selector{ \
        valueList{4,5,6} \
        selectionMethod{1}  \
        refresh{1}{sec}  \
    }");

    EXPECT_EQ(gen2->getValueAtSample(0), 6);
    EXPECT_EQ(gen2->getValueAtSample(44100*2+1), 5);
    EXPECT_EQ(gen2->getValueAtSample(44100*2.3), 5);
    EXPECT_EQ(gen2->getValueAtSample(44100*2.8), 5);
    EXPECT_EQ(gen2->getValueAtSample(44100*3.2), 4);
    EXPECT_EQ(gen2->getValueAtSample(44100*3.9), 4);
    EXPECT_EQ(gen2->getValueAtSample(44100*4.2), 6);
    EXPECT_EQ(gen2->getValueAtSample(44100*5.2), 5);

    // ordered cyclic reverse
    gen2 = gf.create("Selector{ \
        valueList{4,5,6} \
        selectionMethod{1}  \
        refresh{1}{samples}  \
    }");
    EXPECT_EQ(gen2->getValueAtSample(0), 6);
    EXPECT_EQ(gen2->getValueAtSample(1), 5);
    EXPECT_EQ(gen2->getValueAtSample(2), 4);
    EXPECT_EQ(gen2->getValueAtSample(3), 6);
    EXPECT_EQ(gen2->getValueAtSample(8), 5);
    EXPECT_EQ(gen2->getValueAtSample(9), 4);



    // ordered oscillate
    gen2 = gf.create("Selector{ \
        valueList{4,5,6} \
        selectionMethod{2}  \
        refresh{1}{sec}  \
    }");

    EXPECT_EQ(gen2->getValueAtSample(0), 4);
    EXPECT_EQ(gen2->getValueAtSample(44100*2+1), 5);
    EXPECT_EQ(gen2->getValueAtSample(44100*2.3), 5);
    EXPECT_EQ(gen2->getValueAtSample(44100*2.8), 5);
    EXPECT_EQ(gen2->getValueAtSample(44100*3.2), 6);
    EXPECT_EQ(gen2->getValueAtSample(44100*3.9), 6);
    EXPECT_EQ(gen2->getValueAtSample(44100*4.2), 5);
    EXPECT_EQ(gen2->getValueAtSample(44100*5.2), 4);

    // ordered cyclic reverse
    gen2 = gf.create("Selector{ \
        valueList{4,5,6} \
        selectionMethod{2}  \
        refresh{1}{samples}  \
    }");
    EXPECT_EQ(gen2->getValueAtSample(0), 4);
    EXPECT_EQ(gen2->getValueAtSample(1), 5);
    EXPECT_EQ(gen2->getValueAtSample(2), 6);
    EXPECT_EQ(gen2->getValueAtSample(3), 5);
    EXPECT_EQ(gen2->getValueAtSample(8), 4);
    EXPECT_EQ(gen2->getValueAtSample(9), 5);


}






TEST(BasicTests, SelectorPermutations) {


    SystemPtr sys(new System(44100)); // smart pointer
    GeneratorFactory gf(sys); // one instance


//     EXPECT_EQ(gen2->getValueAtSample(0), 6);
//     EXPECT_EQ(gen2->getValueAtSample(1), 7);
//     EXPECT_EQ(gen2->getValueAtSample(2), 8);
//     EXPECT_EQ(gen2->getValueAtSample(3), 9);
//     EXPECT_EQ(gen2->getValueAtSample(8), 6);
//     EXPECT_EQ(gen2->getValueAtSample(9), 7);

    double sum(0);
    int count(0);
    double avg(0);
    double x(0);

    

    GeneratorPtr gen1 = gf.create("Selector{ \
        valueList{1,2,3,4} \
        selectionMethod{4}  \
        refresh{1}{samples}  \
    }");
    for (int i=0; i < 20; i++) {    
        // reset for each call
        sum = 0;
        count = 0;
        // 4 here is the number of unique values
        for (int i=0; i<4; i++) {
            // try to shuffle in place
            x = gen1->getValueAtSample(i);
            //std::cout << "x " << x << std::endl;
            sum += x;
            count += 1;
            }
        avg = sum / count;
        EXPECT_EQ(avg, 2.5);
        EXPECT_EQ(sum, 10);
        }



    gen1 = gf.create("Selector{ \
        valueList{1,2,3,4,5,6,7,8,9,10,11,12} \
        selectionMethod{4}  \
        refresh{1}{samples}  \
    }");
    for (int i=0; i < 20; i++) {    
        // reset for each call
        sum = 0;
        count = 0;
        // 4 here is the number of unique values
        for (int i=0; i<12; i++) {
            // try to shuffle in place
            x = gen1->getValueAtSample(i);
            //std::cout << "y " << x << std::endl;
            sum += x;
            count += 1;
            }
        avg = sum / count;
        EXPECT_EQ(avg, 6.5);
        EXPECT_EQ(sum, 78);
        }




}


