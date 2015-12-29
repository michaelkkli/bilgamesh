#include "bilgamesh_internal.hh"
#include <algorithm>
#include <iostream>
#include <initializer_list>
#include <string>
#include <tuple>

void _bgm_hrki_output_action (_bgm_action<int8_t>& tmp_act)
{
  int pos, row, col;

  if (!tmp_act.is_capture ()) {
    std::cout << "1\n";

    pos = tmp_act.first ();
    _bgm_position_to_row_col (pos, row, col);
    std::cout << row << " " << col << "\n";

    pos = tmp_act.last ();
    _bgm_position_to_row_col (pos, row, col);
    std::cout << row << " " << col << std::endl;
  } else {
    std::vector<int> positions;

    tmp_act.get_all_hop_positions (positions);
    std::cout << positions.size () - 1 << "\n";

    for (auto i : positions)
      {
	_bgm_position_to_row_col (i, row, col);
	std::cout << row << " " << col << "\n";
      }
  }
}

typedef struct {
  uint64_t first;
  uint64_t second;
  std::string third;
} obk_ins;

void
_bgm_hrki (int num_games, int num_moves, double multiplier, int num_games_capture, int num_moves_capture, bool use_opening_book)
{
  char bw;
  int L;
  std::string line1, line2;
  char positions[] = "________________________________";

  std::cin >> bw;
  std::cin >> L;

  for (int i = 0; i < 4; i++)
    {
      std::cin >> line1 >> line2;

      positions[8*i] = line1[1];
      positions[8*i + 1] = line1[3];
      positions[8*i + 2] = line1[5];
      positions[8*i + 3] = line1[7];

      positions[8*i + 4] = line2[0];
      positions[8*i + 5] = line2[2];
      positions[8*i + 6] = line2[4];
      positions[8*i + 7] = line2[6];
    }

  uint64_t tmp_board[2];
  _bgm_string_to_board (positions, tmp_board[0], tmp_board[1]);

  _bgm_board<int8_t> board;
  board.set_raw (tmp_board[0], tmp_board[1]);
  std::vector<_bgm_action<int8_t>> vacts;

  _bgm_obk obk;
  std::vector<std::string> accepted;
  uint64_t ignored_ui64 = 0x0;

  if ('b' == bw) {
    if (use_opening_book) {
      std::initializer_list<obk_ins> black_obk = {
	{0x555555ULL, 0x555555ULL, "11-15"},
	{0x10455555ULL, 0x10455555ULL, "15-22"},
	{0x455555ULL, 0x10451555ULL, "8-11"},
	{0x551555ULL, 0x10455515ULL, "4-8"}
      };

      for (auto i : black_obk) {
	obk.insert_accepted (i.second, i.third, i.first);
      }

      obk.find (tmp_board[1], tmp_board[0], accepted);

      if (accepted.size () >= 1) {
	board.get_actions (vacts);
	int tmp_ind = (int)std::round((double)(accepted.size () - 1)*rand()/RAND_MAX);
	auto act_it = std::find (vacts.begin (), vacts.end (), accepted[tmp_ind]);

	if (vacts.end () != act_it) {
	  _bgm_hrki_output_action (*act_it);
	  return;
	} 
      }
    }
  } else {
    board.swap_turn ();

    if (use_opening_book) {
      if (0x10455555ULL == tmp_board[0]) {
	// 11-15 23-18.
	std::cout << "1\n5 4\n4 3" << std::endl;
	return;
      } else if (0x40155555ULL == tmp_board[0]) {
	// 12-16 24-20.
	std::cout << "1\n5 6\n4 7" << std::endl;
	return;
      } else if (0x1545555ULL == tmp_board[0]) {
	// 2-13 22-18.
	std::cout << "1\n5 2\n4 3" << std::endl;
	return;
      }
      board.get_actions (vacts);
    }
  }
  
  board.get_actions (vacts);

  _bgm_strategy_monte_carlo<int8_t> mc_strat (num_games, num_moves, multiplier, num_games_capture, num_moves_capture);

  auto tmp_act = mc_strat (board, vacts);

  _bgm_hrki_output_action (tmp_act);
}
