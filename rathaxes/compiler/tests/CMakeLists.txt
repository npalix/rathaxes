# First, do the tests for the internals (algos and such) of the compiler
ADD_SUBDIRECTORY(internals)


# Pure backend compilation and resolution tests
# (without interaction with other parts of the language)
ADD_RATHAXES_CWS_TEST(01_A_test_back_parse.cws)
ADD_RATHAXES_CWS_TEST(01_B_test_back_compile.cws)
ADD_RATHAXES_CWS_TEST(01_C_test_back_meta.cws)
ADD_RATHAXES_CWS_TEST(01_D_test_back_type_hash.cws)
ADD_RATHAXES_CWS_TEST(01_E_test_back_introspect.cws)

# Pure middle-end validation tests
ADD_RATHAXES_CWS_TEST(02_A_test_middle_parse.cws)
ADD_RATHAXES_CWS_TEST(02_B_test_middle_parse_rti_file.cws)
ADD_RATHAXES_CWS_TEST(02_C_test_middle_check_inheritance.cws)
ADD_RATHAXES_CWS_TEST(02_D_test_middle_check_types_declaration.cws)

# Required for test 2_B and the tests which depend on it
FILE_COPY(../../samples/rs232_mockup/simplified_lkm.rti simplified_lkm.rti)

# Tests for configuration matching (against with blocks)
ADD_RATHAXES_CWS_TEST(03_A_test_config_match_lesser.cws)
ADD_RATHAXES_CWS_TEST(03_B_test_config_match_lesser_or_equal.cws)
ADD_RATHAXES_CWS_TEST(03_C_test_config_match_equal.cws)
ADD_RATHAXES_CWS_TEST(03_D_test_config_match_greater_or_equal.cws)
ADD_RATHAXES_CWS_TEST(03_E_test_config_match_greater.cws)
ADD_RATHAXES_CWS_TEST(03_F_test_config_match.cws)

# Tests for the Front-end
ADD_RATHAXES_CWS_TEST(04_A_test_front_parse.cws)
ADD_RATHAXES_CWS_TEST(04_B_test_front_parse_and_check.cws)
ADD_RATHAXES_CWS_TEST(04_C_test_front_all.cws)

# Tests for Link module
ADD_RATHAXES_CWS_TEST(05_A_test_link_merge.cws)
ADD_RATHAXES_CWS_TEST(05_B_test_link_merge_conflict.cws)

# Tests for the typing system (should not do anymore than parsing/checking typing)
ADD_SUBDIRECTORY(typing)
