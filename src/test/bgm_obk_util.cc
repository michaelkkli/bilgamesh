#include "bilgamesh_internal.hh"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

int main (int argc, char *argv[])
{
  std::ifstream file (argv[1]);
  std::string line, tmp_str;
  std::stringstream ss;
  std::vector<std::string> moves;
  std::string bw;
  std::vector<_bgm_action<int8_t>> vacts;
  uint64_t x, y;

  while (std::getline (file, line)) {
    if ('#' == line[0]) {
      std::cout << "// " << line.substr (1) << std::endl;
      continue;
    }

    ss.str (line);
    ss >> bw;

    while (ss.good ()) {
      ss >> tmp_str;
      moves.push_back (tmp_str);
    }

    _bgm_board<int8_t> board;

    for (auto i : moves) {
      std::cout << std::hex;
      std::cout << std::showbase;

      if (board.is_black_move ()) {
	if ("b" == bw || "bw" == bw) {
	  board.get_raw (x, y);
	  std::cout << " {" << x << "ULL, " << y << "ULL, \"" << i << "\"},\n";
	}
      } else {
	if ("w" == bw || "bw" == bw) {
	  board.get_raw (x, y);
	  std::cout << " {" << x << "ULL, " << y << "ULL, \"" << i << "\"},\n";
	}
      }

      board.get_actions (vacts);
      auto act = std::find (vacts.begin (), vacts.end (), i);
      if (vacts.end () == act) {
	abort ();
      }

      board.apply (*act);
    }

    ss.clear ();
  }
}
