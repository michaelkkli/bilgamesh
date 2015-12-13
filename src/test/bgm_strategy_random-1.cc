#include "bilgamesh_internal.hh"
#include <iostream>
#include <cstdlib>

int main (int argc, char *argv[])
{
  _bgm_board<int8_t> board;
  std::vector<_bgm_action<int8_t>> vacts;
  _bgm_strategy_random<int8_t> strat;
  _bgm_action<int8_t> tmp;

  int num_games = 200;
  int max_moves = 200;

  int black_wins = 0;
  int white_wins = 0;
  int hit_max_moves = 0;

  for (int i = 0; i < num_games; i++) {
    board.set ({3}, {32}, {}, {18, 19}, false);
    for (int j = 0; j < max_moves; j++) {
      board.get_actions (vacts);
      if (vacts.empty ()) {
	//std::cout << "Game ended on move " << j + 1 << std::endl;
	if (board.is_black_move ()) {
	  white_wins++;
	} else {
	  black_wins++;
	}
	break;
      }
      tmp = strat (board, vacts);
      board.apply (tmp);
      if ( max_moves - 1 == j ) {
	hit_max_moves++;
      }
    }
  }

  std::cout << "Black wins   : " << black_wins << "\n";
  std::cout << "White wins   : " << white_wins << "\n";
  std::cout << "Hit max moves: " << hit_max_moves << std::endl;
}
