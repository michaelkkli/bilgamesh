TESTS = src/test/success.test \
	src/test/bgm_board-1.test \
	src/test/bgm_action-1.test \
	src/test/xfail.test 

XFAIL_TESTS = src/test/xfail.test

check_PROGRAMS = $(TESTS)

noinst_LIBRARIES = libbgm_internal.a

libbgm_internal_a_SOURCES = src/bgm_action.cc src/bgm_board.cc src/bgm_get_reverse_bitpairs.cc src/bgm_get_occupied_positions.cc
libbgm_internal_a_CPPFLAGS = -I"$(srcdir)/src/include"

src_test_success_test_SOURCES = src/test/success.cc
src_test_xfail_test_SOURCES = src/test/xfail.cc

src_test_bgm_board_1_test_SOURCES = src/test/bgm_board-1.cc
src_test_bgm_board_1_test_CPPFLAGS = -I"$(srcdir)/src/include"
src_test_bgm_board_1_test_LDADD = libbgm_internal.a

src_test_bgm_action_1_test_SOURCES = src/test/bgm_action-1.cc
src_test_bgm_action_1_test_CPPFLAGS = -I"$(srcdir)/src/include"
src_test_bgm_action_1_test_LDADD = libbgm_internal.a
