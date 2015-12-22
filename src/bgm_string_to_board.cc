#include <cstdint>

template <class T>
void
_bgm_string_to_board (T s, uint64_t& bhboard, uint64_t& whboard)
{
  bhboard = (uint64_t)0x0ULL;
  whboard = (uint64_t)0x0ULL;

  for (int i = 0; i < 32; i++)
    {
      switch (s[i]) {
	case '_':
	  continue;
	  break;
	case 'b':
	  bhboard |= ((uint64_t)0x1 << 2 * i);
	  break;
	case 'w':
	  whboard |= ((uint64_t)0x1 << 2 * (31 - i));
	  break;
	case 'B':
	  bhboard |= ((uint64_t)0x2 << 2 * i);
	  break;
	case 'W':
	  whboard |= ((uint64_t)0x2 << 2 * (31 - i));
	  break;
      }
    }
}

template void _bgm_string_to_board<char*> (char *, uint64_t&, uint64_t&);
