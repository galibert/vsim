#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <set>

#include "blocks.h"
#include "sched.h"

cstate ref;

void init(cstate &s)
{
  memset(&s, 0, sizeof(s));
  ref = s;
}

void update(cstate &s)
{
  for(;;) {
    std::set<void (*)(cstate &)> f;
    compute_sched(ref, s, f);
    ref = s;
    if(f.empty())
      break;
    for(std::set<void (*)(cstate &)>::const_iterator i = f.begin(); i != f.end(); i++)
      (*i)(s);
  }
}

void show_state(const cstate &s)
{
  printf("%02d.%06d.%d: gtsr=%d%d%d(%d) p80=%d",
	 int(s.ctime/720000/2), int((s.ctime/2) % 720000), int(s.ctime & 1),
	 s.gtsr[1], s.gtsr[3], s.gtsr[5], !s.gtsr_input,
	 s.pulse_80Hz);
  printf(" rom=");
  bool f=true;
  if(!s.rom_hsel_f2) { if(!f) printf(","); printf("f2 "); f=false; }
  if(!s.rom_hsel_f2q){ if(!f) printf(","); printf("f2q"); f=false; }
  if(!s.rom_hsel_f3) { if(!f) printf(","); printf("f3 "); f=false; }
  if(!s.rom_extra)   { if(!f) printf(","); printf("xt "); f=false; }
  if(!s.rom_hsel_fc) { if(!f) printf(","); printf("fc "); f=false; }
  if(!s.rom_hsel_fa) { if(!f) printf(","); printf("fa "); f=false; }
  if(!s.rom_hsel_va) { if(!f) printf(","); printf("va "); f=false; }
  if(!s.rom_hsel_f1) { if(!f) printf(","); printf("f1 "); f=false; }

  printf(" phi1=%d phi2=%d stbsr=%d%d%d pad_stb=%d ipl_s=%d ipl_r=%d p_stb=%02x p_rom=%02x rp=%x rclvd=%x rd=%02x rcl=%d rmux=%d",
	 s.phi1, s.phi2,
	 s.stbsr[0], s.stbsr[1], s.stbsr[2],
	 s.pad_stb,
	 s.input_phone_latch_stb, s.input_phone_latch_rom,
	 s.p_stb, s.p_rom,
	 s.rom_param, s.rom_clvd, s.rom_duration, s.rom_cl,
	 s.rom_muxed_fx_out
	 );
  printf(" sram=%02x.%02x.%02x.%02x.%02x.%02x.%02x di=%02x\n",
	 s.sram[0], s.sram[1], s.sram[2], s.sram[3], s.sram[4], s.sram[5], s.sram[6],
	 s.driver_in);
}

void step(cstate &s, bool verbose)
{
  s.clk_0 = false;
  update(s);
  if(verbose)
    show_state(s);
  s.clk_0 = true;
  update(s);
  s.ctime++;
  s.clk_1 = false;
  update(s);
  if(false && verbose)
    show_state(s);
  s.clk_1 = true;
  update(s);
  s.ctime++;
}

int main()
{
  cstate s;
  init(s);

  s.pad_stb = true;
  s.p_stb = s.p_rom = s.p_input = 0x3f;
  s.clk_0 = s.clk_1 = true;
  for(int i=0; i<1000; i++)
    step(s, false);

  s.p_input = 0x20;
  s.pad_stb = false;
  for(int i=0; i<72; i++)
    step(s, true);
  s.pad_stb = true;
  for(int i=0; i<1000; i++)
    step(s, true);

  return 0;
}
