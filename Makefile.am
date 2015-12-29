TESTS = src/test/success.test \
	src/test/bgm_board-3.test \
	src/test/bgm_strategy_monte_carlo-1.test \
	src/test/bgm_strategy_monte_carlo-2.test \
	src/test/bgm_strategy_monte_carlo-3.test \
	src/test/bgm_strategy_random-1.test \
	src/test/bgm_board-2.test \
	src/test/bgm_board-1.test \
	src/test/bgm_action-1.test \
	src/test/xfail.test 

XFAIL_TESTS = src/test/xfail.test

check_PROGRAMS = $(TESTS) \
	src/test/bgm_obk_util.test \
	src/test/bgm_hrki_util.test 

noinst_LIBRARIES = libbgm_internal.a

libbgm_internal_a_SOURCES = src/bgm_action.cc src/bgm_board.cc src/bgm_get_reverse_bitpairs.cc src/bgm_get_occupied_positions.cc src/bgm_strategy_random.cc src/bgm_strategy_monte_carlo.cc src/bgm_string_to_board.cc src/bgm_position_to_row_col.cc src/bgm_hrki.cc src/bgm_obk.cc
libbgm_internal_a_CPPFLAGS = -I"$(srcdir)/src/include"

src_test_success_test_SOURCES = src/test/success.cc
src_test_xfail_test_SOURCES = src/test/xfail.cc

src_test_bgm_obk_util_test_SOURCES = src/test/bgm_obk_util.cc
src_test_bgm_obk_util_test_CPPFLAGS = -I"$(srcdir)/src/include"
src_test_bgm_obk_util_test_LDADD = libbgm_internal.a

src_test_bgm_hrki_util_test_SOURCES = src/test/bgm_hrki_util.cc
src_test_bgm_hrki_util_test_CPPFLAGS = -I"$(srcdir)/src/include"
src_test_bgm_hrki_util_test_LDADD = libbgm_internal.a

src_test_bgm_strategy_monte_carlo_3_test_SOURCES = src/test/bgm_strategy_monte_carlo-3.cc
src_test_bgm_strategy_monte_carlo_3_test_CPPFLAGS = -I"$(srcdir)/src/include"
src_test_bgm_strategy_monte_carlo_3_test_LDADD = libbgm_internal.a

src_test_bgm_strategy_monte_carlo_2_test_SOURCES = src/test/bgm_strategy_monte_carlo-2.cc
src_test_bgm_strategy_monte_carlo_2_test_CPPFLAGS = -I"$(srcdir)/src/include"
src_test_bgm_strategy_monte_carlo_2_test_LDADD = libbgm_internal.a

src_test_bgm_strategy_monte_carlo_1_test_SOURCES = src/test/bgm_strategy_monte_carlo-1.cc
src_test_bgm_strategy_monte_carlo_1_test_CPPFLAGS = -I"$(srcdir)/src/include"
src_test_bgm_strategy_monte_carlo_1_test_LDADD = libbgm_internal.a

src_test_bgm_strategy_random_1_test_SOURCES = src/test/bgm_strategy_random-1.cc
src_test_bgm_strategy_random_1_test_CPPFLAGS = -I"$(srcdir)/src/include"
src_test_bgm_strategy_random_1_test_LDADD = libbgm_internal.a

src_test_bgm_board_1_test_SOURCES = src/test/bgm_board-1.cc
src_test_bgm_board_1_test_CPPFLAGS = -I"$(srcdir)/src/include"
src_test_bgm_board_1_test_LDADD = libbgm_internal.a

src_test_bgm_board_3_test_SOURCES = src/test/bgm_board-3.cc
src_test_bgm_board_3_test_CPPFLAGS = -I"$(srcdir)/src/include"
src_test_bgm_board_3_test_LDADD = libbgm_internal.a

src_test_bgm_board_2_test_SOURCES = src/test/bgm_board-2.cc
src_test_bgm_board_2_test_CPPFLAGS = -I"$(srcdir)/src/include"
src_test_bgm_board_2_test_LDADD = libbgm_internal.a

src_test_bgm_action_1_test_SOURCES = src/test/bgm_action-1.cc
src_test_bgm_action_1_test_CPPFLAGS = -I"$(srcdir)/src/include"
src_test_bgm_action_1_test_LDADD = libbgm_internal.a
