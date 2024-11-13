#include "handEvaluators.hpp"

#include "tables.h"

int hash_quinary(const unsigned char q[], int k) {
  int sum = 0;
  const int len = 13;
  int i;

  for (i = 0; i < len; i++) {
    sum += dp[q[i]][len - i - 1][k];

    k -= q[i];

    if (k <= 0) {
      break;
    }
  }

  return sum;
}

int evaluate_7cards(int a, int b, int c, int d, int e, int f, int g) {
  int suit_hash = 0;

  suit_hash += bit_of_mod_4_x_3[a];  // (1 << ((a % 4) * 3))
  suit_hash += bit_of_mod_4_x_3[b];  // (1 << ((b % 4) * 3))
  suit_hash += bit_of_mod_4_x_3[c];  // (1 << ((c % 4) * 3))
  suit_hash += bit_of_mod_4_x_3[d];  // (1 << ((d % 4) * 3))
  suit_hash += bit_of_mod_4_x_3[e];  // (1 << ((e % 4) * 3))
  suit_hash += bit_of_mod_4_x_3[f];  // (1 << ((f % 4) * 3))
  suit_hash += bit_of_mod_4_x_3[g];  // (1 << ((g % 4) * 3))

  if (suits[suit_hash]) {
    int suit_binary[4] = {0};

    suit_binary[a & 0x3] |= bit_of_div_4[a];  // (1 << (a / 4))
    suit_binary[b & 0x3] |= bit_of_div_4[b];  // (1 << (b / 4))
    suit_binary[c & 0x3] |= bit_of_div_4[c];  // (1 << (c / 4))
    suit_binary[d & 0x3] |= bit_of_div_4[d];  // (1 << (d / 4))
    suit_binary[e & 0x3] |= bit_of_div_4[e];  // (1 << (e / 4))
    suit_binary[f & 0x3] |= bit_of_div_4[f];  // (1 << (f / 4))
    suit_binary[g & 0x3] |= bit_of_div_4[g];  // (1 << (g / 4))

    return flush[suit_binary[suits[suit_hash] - 1]];
  }

  unsigned char quinary[13] = {0};

  quinary[(a >> 2)]++;
  quinary[(b >> 2)]++;
  quinary[(c >> 2)]++;
  quinary[(d >> 2)]++;
  quinary[(e >> 2)]++;
  quinary[(f >> 2)]++;
  quinary[(g >> 2)]++;

  const int hash = hash_quinary(quinary, 7);

  return noflush7[hash];
}