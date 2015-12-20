#include "bilgamesh_internal.hh"
#include <iostream>
#include <cstdlib>

int main (int argc, char *argv[])
{
  _bgm_board<int8_t> board;
  std::vector<_bgm_action<int8_t>> vacts;
  _bgm_strategy_random<int8_t> rand_strat;
  _bgm_strategy_monte_carlo<int8_t> mc_strat (10, 7, 1.5);
  _bgm_action<int8_t> tmp;

  const int num_games = 100;
  const int max_moves = 100;

  int black_wins = 0;
  int white_wins = 0;
  int hit_max_moves = 0;

  for (int i = 0; i < num_games; i++) {
    board.set ({3}, {32}, {}, {18, 19}, false);
    for (int j = 0; j < (int)(max_moves/2); j++) {
      board.get_actions (vacts);
      if (vacts.empty ()) {
	if (board.is_black_move ()) {
	  white_wins++;
	} else {
	  black_wins++;
	}
	break;
      }
      tmp = mc_strat (board, vacts);
      board.apply (tmp);

      board.get_actions (vacts);
      if (vacts.empty ()) {
	if (board.is_black_move ()) {
	  white_wins++;
	} else {
	  black_wins++;
	}
	break;
      }
      tmp = mc_strat (board, vacts);
      board.apply (tmp);


      if ( (int)(max_moves/2) - 1 == j ) {
	hit_max_moves++;
      }
    }
  }

  std::cout << "Black wins   : " << black_wins << "\n";
  std::cout << "White wins   : " << white_wins << "\n";
  std::cout << "Hit max moves: " << hit_max_moves << std::endl;
}
