/* 
 * File:   ArgumentParserTest
 * Author: Philipp Kaaden
 *
 * Created on 24.11.2020, 08:15:44
 */

#include <gtest/gtest.h>

#include "../src/ArgumentParser.h"

namespace encoder {

    class ArgumentParserTest : public testing::Test {
    protected:
    };

    TEST_F(ArgumentParserTest, Size0Argc) {
        char c[10] = "hallo";
        char * pn = c;
        char ** ppn = &pn;
        auto files = getFilesToEncode(0, ppn);
        EXPECT_EQ(0, files.size());
    }

    TEST_F(ArgumentParserTest, Size1Argc) {
        char c[10] = "hallo";
        char * pn = c;
        char ** ppn = &pn;
        auto files = getFilesToEncode(1, ppn);
        EXPECT_EQ(0, files.size());
    }

    TEST_F(ArgumentParserTest, Size3_ArgumentsAdded) {
        char c1[20] = "Para1";
        char c2[20] = "../testscripts";
        char c3[20] = "Para3";
        char * pn[3];
        pn[0] = c1;
        pn[1] = c2;
        pn[2] = c3;
        char ** ppn = &pn[0];
        auto files = getFilesToEncode(3, ppn);
        EXPECT_GT(files.size(), 0);
    }


}//end namespace