#include <set>
#include <cstdint>

void
_bgm_get_occupied_positions (uint64_t in, std::set<uint8_t>& out, int max)
{
  out.clear();

  for (int i=0; i<32; i++) {
    if (in & (0x3ULL << 2*i)) {
      out.insert (i+1);
      max--;
      if (max <= 0) {
	break;
      }
    }
  }
}
