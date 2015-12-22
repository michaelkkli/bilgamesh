#include <cstdint>

void position_to_row_col (int pos, int& row, int& col)
{
  for (int i = 0; i < 8; i++)
    {
      if (pos <= 4 * (i + 1))
	{
	  row = i;
	  break;
	}
    }

  int quot = (pos - 1) / 4; // Integer division with truncation.
  int rem = pos - 1 - 4 * quot;

  col = 2 * rem;

  uint32_t bit_fourfive = (uint32_t)0x0F0F0F0F;

  if (bit_fourfive & ((uint32_t)0x1 << (pos - 1)))
    {
      col += 1;
    }
}
