#
# Test suite for the sequence's type checking
#

# Interface's internal type checking tests
ADD_RATHAXES_CWS_TEST(01_A_sequence_mid_no_type.cws)
ADD_RATHAXES_CWS_TEST(01_B_sequence_mid_ambiguous_type.cws)
ADD_RATHAXES_CWS_TEST(01_C_sequence_mid_no_pointcut.cws)
ADD_RATHAXES_CWS_TEST(01_D_sequence_mid_ambiguous_pointcut.cws)
ADD_RATHAXES_CWS_TEST(01_E_sequence_mid_multiple_definition.cws)
ADD_RATHAXES_CWS_TEST(01_F_sequence_mid_ok.cws)

# Type-checking of the sequences in the Back-end
ADD_RATHAXES_CWS_TEST(02_A_sequence_back_undefined.cws)
ADD_RATHAXES_CWS_TEST(02_B_sequence_back_bad_types.cws)
ADD_RATHAXES_CWS_TEST(02_C_sequence_back_ambiguous_types.cws)
ADD_RATHAXES_CWS_TEST(02_D_sequence_back_ambiguous.cws)
ADD_RATHAXES_CWS_TEST(02_E_sequence_back_ok.cws)

# Type-cheking of the sequences in the Front-end
ADD_RATHAXES_CWS_TEST(03_A_sequence_front_undefined.cws)
ADD_RATHAXES_CWS_TEST(03_B_sequence_front_bad_types.cws)
ADD_RATHAXES_CWS_TEST(03_C_sequence_front_ambiguous_types.cws)
ADD_RATHAXES_CWS_TEST(03_D_sequence_front_ambiguous.cws)
#ADD_RATHAXES_CWS_TEST(03_E_sequence_front_ok.cws)
