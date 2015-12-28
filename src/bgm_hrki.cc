#include "bilgamesh_internal.hh"

#include <iostream>
#include <string>

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
  if ('b' == bw) {
    if (use_opening_book) {
      if (0x555555ULL == tmp_board[1]) {
	// Old Faithful: open 11-15.
	std::cout << "1\n2 5\n3 4" << std::endl;
	return;
      }
    }
  } else {
    if (use_opening_book) {
      if (0x10455555ULL == tmp_board[0]) {
	// Play 23-18 against 11-15.
	std::cout << "1\n5 4\n4 3" << std::endl;
	return;
      }
      board.swap_turn ();
    }
  }
  board.set_raw (tmp_board[0], tmp_board[1]);

  std::vector<_bgm_action<int8_t>> vacts;
  board.get_actions (vacts);

  _bgm_strategy_monte_carlo<int8_t> mc_strat (num_games, num_moves, multiplier, num_games_capture, num_moves_capture);

  _bgm_action<int8_t> tmp_act = mc_strat (board, vacts);

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
