ADD_RATHAXES_SOURCES(hello hello.rtx
                     RTI builtin.rti main.rti log.rti
                     BLT log.blt main.blt)

ADD_RATHAXES_EXECUTABLE(Test_Rathaxes_Samples_Helloworld hello)

ADD_TEST(Rathaxes_Samples_Helloworld Test_Rathaxes_Samples_Helloworld "Success")
SET_TESTS_PROPERTIES(Rathaxes_Samples_Helloworld PROPERTIES PASS_REGULAR_EXPRESSION
                     "^Arguments:.*- \\[1\\]: Success.*Environment:.*It works!\n$")
