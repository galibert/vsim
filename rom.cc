#include "blocks.h"

static const unsigned int rom[64][2] = {
  { 0x361, 0x74688127 },
  { 0x161, 0xd4688127 },
  { 0x9a1, 0xc4688127 },
  { 0x0e0, 0xf0a050a4 },
  { 0x0fb, 0x610316e8 },
  { 0x161, 0x64c9c1a6 },
  { 0x7a1, 0x34c9c1a6 },
  { 0x463, 0xf3cb546c },
  { 0x161, 0xc4e940a3 },
  { 0xb61, 0x806191a6 },
  { 0xa61, 0x906191a6 },
  { 0x9a1, 0x906191a6 },
  { 0x7a3, 0x66a58832 },
  { 0xa61, 0xe6241936 },
  { 0x173, 0x90e19122 },
  { 0x163, 0xf7d36428 },
  { 0x163, 0xfb8b546c },
  { 0x9a2, 0xfb8b546c },
  { 0x163, 0x9cd15860 },
  { 0x8a0, 0x706980a3 },
  { 0x9a0, 0xd4084b36 },
  { 0x8a1, 0x84e940a3 },
  { 0x7a1, 0x30498123 },
  { 0xa21, 0x20498123 },
  { 0x7a1, 0xf409d0a2 },
  { 0xa72, 0x1123642c },
  { 0x0e8, 0xdb7b342c },
  { 0x162, 0xfd2204ac },
  { 0x173, 0xe041c126 },
  { 0x7a2, 0x65832ca8 },
  { 0xb7c, 0x00e89126 },
  { 0x468, 0x489132e0 },
  { 0xa21, 0x84c9c1a6 },
  { 0x561, 0x7069d326 },
  { 0xa61, 0x64a01226 },
  { 0x0e3, 0x548981a3 },
  { 0xcc1, 0x84e940a3 },
  { 0x7b2, 0x631324a8 },
  { 0xa21, 0x84e8c1a2 },
  { 0xa21, 0x806191a6 },
  { 0xa21, 0x80e8c122 },
  { 0x7a1, 0x64015326 },
  { 0x172, 0xe81132e0 },
  { 0x463, 0x54084382 },
  { 0xa20, 0x7049d326 },
  { 0xa66, 0x1460c122 },
  { 0xa20, 0x74e880a7 },
  { 0x7a0, 0x74e880a7 },
  { 0x461, 0x606980a3 },
  { 0x163, 0x548981a3 },
  { 0x7a1, 0xe48981a3 },
  { 0xa21, 0xb48981a3 },
  { 0xa61, 0x34e8c1a2 },
  { 0x9a1, 0x80e8c1a2 },
  { 0x366, 0x106083a2 },
  { 0x461, 0x90e8c122 },
  { 0xa63, 0x88e15220 },
  { 0x168, 0x183800a4 },
  { 0x8a1, 0x2448c382 },
  { 0xa21, 0x94688127 },
  { 0x9a1, 0x9049d326 },
  { 0xcc1, 0xb06980a3 },
  { 0xa23, 0x00a050a4 },
  { 0x0f0, 0x30a058a4 },
};

void rom_read(cstate &s)
{
  s.pause = !(s.p_rom == 0x03 || s.p_rom == 0x3e);
  int slot = -1;
  if(!s.rom_hsel_f1 ) slot = 0;
  if(!s.rom_hsel_va ) slot = 1;
  if(!s.rom_hsel_f2 ) slot = 2;
  if(!s.rom_hsel_fc ) slot = 3;
  if(!s.rom_hsel_f2q) slot = 4;
  if(!s.rom_hsel_f3 ) slot = 5;
  if(!s.rom_hsel_fa ) slot = 6;

  if(slot == -1)
    s.rom_param = 0;
  else {
    unsigned int base = rom[s.p_rom][1] >> slot;
    s.rom_param =
      ((base & 0x000001) ? 8 : 0) |
      ((base & 0x000080) ? 4 : 0) |
      ((base & 0x004000) ? 2 : 0) |
      ((base & 0x200000) ? 1 : 0);
  }
  if(slot != 0 && slot != 6)
    s.rom_clvd = 0xf;
  else {
    unsigned int base = (rom[s.p_rom][1] >> 28) | (rom[s.p_rom][0] << 4);
    if(slot == 6)
      base >>= 1;
    s.rom_clvd =
      ((base & 0x01) ? 1 : 0) |
      ((base & 0x04) ? 2 : 0) |
      ((base & 0x10) ? 4 : 0) |
      ((base & 0x40) ? 8 : 0);
  }
  s.rom_cl = rom[s.p_rom][0] & 0x10;
  s.rom_duration =
    ((rom[s.p_rom][0] & 0x020) ? 0x40 : 0) |
    ((rom[s.p_rom][0] & 0x040) ? 0x20 : 0) |
    ((rom[s.p_rom][0] & 0x080) ? 0x10 : 0) |
    ((rom[s.p_rom][0] & 0x100) ? 0x08 : 0) |
    ((rom[s.p_rom][0] & 0x200) ? 0x04 : 0) |
    ((rom[s.p_rom][0] & 0x400) ? 0x02 : 0) |
    ((rom[s.p_rom][0] & 0x800) ? 0x01 : 0);
}

void rom_param_muxer(cstate &s)
{
  // 111
  // 011
  // 101
  // 010
  // 001
  // 000
  // 100
  // 110

  s.rom_muxed_fx_out = false;
  if((!s.gtsr[1] && !s.gtsr[3] && !s.gtsr[5]) ||
     (!(s.rom_param & 1) && s.gtsr[1] && !s.gtsr[3] && !s.gtsr[5]) ||
     (!(s.rom_param & 2) && !s.gtsr[1] && s.gtsr[3] && !s.gtsr[5]) ||
     (!(s.rom_param & 4) && s.gtsr[1] && !s.gtsr[3] && s.gtsr[5]) ||
     (!(s.rom_param & 8) && s.gtsr[1] && s.gtsr[3] && !s.gtsr[5]) ||
     (s.gtsr[1] && s.gtsr[3] && s.gtsr[5]) ||
     (!s.gtsr[1] && s.gtsr[3] && s.gtsr[5]) ||
     (!s.gtsr[1] && !s.gtsr[3] && s.gtsr[5]))
    s.rom_muxed_fx_out = true;    
}

