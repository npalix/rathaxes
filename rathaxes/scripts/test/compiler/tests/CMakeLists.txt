# Pure backend compilation and resolution tests
ADD_RATHAXES_TEST(01_A_test_back_parse.cws)
ADD_RATHAXES_TEST(01_B_test_back_compile.cws)
ADD_RATHAXES_TEST(01_C_test_back_meta.cws)
ADD_RATHAXES_TEST(01_D_test_back_type_hash.cws)
ADD_RATHAXES_TEST(01_E_test_back_introspect.cws)
ADD_RATHAXES_TEST(01_F_test_back_gen.cws)
ADD_RATHAXES_TEST(01_G_test_back_registerToCache.cws)
ADD_RATHAXES_TEST(01_H_test_back_loadFromCache.cws)
ADD_RATHAXES_TEST(01_I_test_back_resolveCall.cws)
ADD_RATHAXES_TEST(01_J_test_back_resolvePointcut.cws)
ADD_RATHAXES_TEST(01_K_test_back_resolvePointcutDefault.cws)
ADD_RATHAXES_TEST(01_L_test_back_resolveTypeMapping.cws)
ADD_RATHAXES_TEST(01_L_test_back_resolveTypeChunkCall.cws)

# Pure middle-end validation tests
ADD_RATHAXES_TEST(02_A_test_middle_parse.cws)
ADD_RATHAXES_TEST(02_B_test_middle_parse_rti_file.cws)
ADD_RATHAXES_TEST(02_C_test_middle_check_inheritance.cws)

# Required for test 2_B
FILE_COPY(full_lkm/simplified_lkm.rti full_lkm/simplified_lkm.rti)

# Tests for configuration matching (against with blocks)
ADD_RATHAXES_TEST(03_A_test_config_match_lesser.cws)
ADD_RATHAXES_TEST(03_B_test_config_match_lesser_or_equal.cws)
ADD_RATHAXES_TEST(03_C_test_config_match_equal.cws)
ADD_RATHAXES_TEST(03_D_test_config_match_greater_or_equal.cws)
ADD_RATHAXES_TEST(03_E_test_config_match_greater.cws)
ADD_RATHAXES_TEST(03_F_test_config_match.cws)

# Tests for the Front-end
ADD_RATHAXES_TEST(04_A_test_front_parse.cws)

# Tests for Link module
ADD_RATHAXES_TEST(05_A_test_link_merge.cws)
ADD_RATHAXES_TEST(05_B_test_link_merge_conflict.cws)
