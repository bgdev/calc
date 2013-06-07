#include "calc.h"
#include <assert.h>
#include <stdio.h>

#define FLAG_1 0x1
#define FLAG_2 0x2
#define FLAG_3 0x4

int is_flag_on (struct calc *c, int flag);
int set_flag (struct calc *c, int flag, int on);

void
test_flags (void)
{
  CALC_NEW (c);
  c.flags = 0;

  assert (set_flag (&c, FLAG_1, 1) == 0);
  assert (c.flags == FLAG_1);
  assert (is_flag_on (&c, FLAG_1) == 1);
  assert (set_flag (&c, FLAG_1, 0) == 1);
  assert (c.flags == 0);
  assert (is_flag_on (&c, FLAG_1) == 0);

  assert (set_flag (&c, FLAG_2, 1) == 0);
  assert (c.flags == FLAG_2);
  assert (is_flag_on (&c, FLAG_2) == 1);
  assert (set_flag (&c, FLAG_2, 0) == 1);
  assert (c.flags == 0);
  assert (is_flag_on (&c, FLAG_2) == 0);

  assert (set_flag (&c, FLAG_1, 1) == 0);
  assert (set_flag (&c, FLAG_2, 2) == 0);
  assert (set_flag (&c, FLAG_3, 3) == 0);
  assert (c.flags == (FLAG_1 | FLAG_2 | FLAG_3));
}

void
test_input_append (void)
{
  struct calc c;
  calc_init (&c);
  assert (calc_process_cmd (&c, '1') == 1);
  assert (calc_process_cmd (&c, '2') == 12);
  assert (calc_process_cmd (&c, '3') == 123);
  assert (calc_process_cmd (&c, '=') == 123);
  assert (calc_process_cmd (&c, '3') == 3);
  assert (calc_process_cmd (&c, '2') == 32);
  assert (calc_process_cmd (&c, '1') == 321);
}

void
test_input_zero (void)
{
  struct calc c;
  calc_init (&c);
  assert (calc_process_cmd (&c, '0') == 0);
  assert (calc_process_cmd (&c, '0') == 0);
  assert (calc_process_cmd (&c, '0') == 0);
}

void
test_plus1 (void)
{
  CALC_NEW (c);
  assert (calc_process_line (&c, "2+") == 2);
}

void
test_plus2 (void)
{
  CALC_NEW (c);
  assert (calc_process_line (&c, "+2") == 2);
}

void
test_plus3 (void)
{
  CALC_NEW (c);
  assert (calc_process_line (&c, "1+2=") == 3);
}

void
test_plus4 (void)
{
  CALC_NEW (c);
  assert (calc_process_line (&c, "1+5+10=") == 16);
}

void
test_plus5 (void)
{
  CALC_NEW (c);
  assert (calc_process_line (&c, "1+5=10=") == 15);
}

void
test_plus6 (void)
{
  CALC_NEW (c);
  assert (calc_process_line (&c, "1+5+10=20=") == 30);
  assert (calc_process_cmd (&c, '=') == 40);
  assert (calc_process_cmd (&c, '=') == 50);
}

void
test_mult1 (void)
{
  struct calc c;
  calc_init (&c);
  assert (calc_process_line (&c, "*2=") == 0);
}

void
test_mult2 (void)
{
  struct calc c;
  calc_init (&c);
  assert (calc_process_line (&c, "2*=") == 4);
}

void
test_mult3 (void)
{
  CALC_NEW (c);
  assert (calc_process_line (&c, "2*3=") == 6);
}

void
test_mult4 (void)
{
  CALC_NEW (c);
  assert (calc_process_line (&c, "2*3*") == 6);
  assert (calc_process_line (&c, "=") == 36);
  assert (calc_process_line (&c, "=") == 216);
}

void
test_mult5 (void)
{
  CALC_NEW (c);
  assert (calc_process_line (&c, "1*2*") == 2);
  assert (calc_process_line (&c, "3*") == 6);
  assert (calc_process_line (&c, "4*") == 24);
  assert (calc_process_line (&c, "5*") == 120);
  assert (calc_process_line (&c, "6*") == 720);
  assert (calc_process_line (&c, "=") == 518400);
}

void
test_minus1 ()
{
  CALC_NEW (c);
  assert (calc_process_cmd (&c, '-') == 0);
  assert (calc_process_line (&c, "123") == 123);
  assert (calc_process_cmd (&c, '=') == -123);
  assert (calc_process_cmd (&c, '=') == -246);
  assert (calc_process_cmd (&c, '=') == -369);
}

void
test_minus2 ()
{
  CALC_NEW (c);
  assert (calc_process_line (&c, "5-1=") == 4);
  assert (calc_process_cmd (&c, '=') == 3);
  assert (calc_process_cmd (&c, '=') == 2);
  assert (calc_process_cmd (&c, '=') == 1);
  assert (calc_process_cmd (&c, '=') == 0);
  assert (calc_process_cmd (&c, '=') == -1);
}

void
test_minus3 ()
{
  CALC_NEW (c);
  assert (calc_process_line (&c, "10-5-1=") == 4);
  assert (calc_process_cmd (&c, '=') == 3);
  assert (calc_process_cmd (&c, '=') == 2);
  assert (calc_process_line (&c, "20=") == 19);
}

void
test_minus4 ()
{
  CALC_NEW (c);
  assert (calc_process_line (&c, "10-50=") == -40);
  assert (calc_process_line (&c, "=") == -90);
  assert (calc_process_line (&c, "=") == -140);
}

void
test_minus5 ()
{
  CALC_NEW (c);
  assert (calc_process_line (&c, "123-=") == -123);
}

void
test_div1 ()
{
  CALC_NEW (c);
  assert (calc_process_line (&c, "/20=") == 0);
  assert (calc_process_cmd (&c, '=') == 0);
}

void
test_div2 ()
{
  CALC_NEW (c);
  assert (calc_process_line (&c, "256/2=") == 128);
  assert (calc_process_cmd (&c, '=') == 64);
  assert (calc_process_cmd (&c, '=') == 32);
}

void
test_div3 ()
{
  CALC_NEW (c);
  assert (calc_process_line (&c, "256/2=") == 128);
  assert (calc_process_line (&c, "/=") == 0); /* 2 / 256 */
}

void
test_all1 ()
{
  CALC_NEW (c);
  assert (calc_process_line (&c, "123*32-66*-+100=") == 3970);
}

void
test_all2 ()
{
  CALC_NEW (c);
}

int
main ()
{
  test_flags();

  test_input_append ();
  test_input_zero ();

  test_plus1 ();
  test_plus2 ();
  test_plus3 ();
  test_plus4 ();
  test_plus5 ();
  test_plus6 ();

  test_mult1 ();
  test_mult2 ();
  test_mult3 ();
  test_mult4 ();
  test_mult5 ();

  test_minus1 ();
  test_minus2 ();
  test_minus3 ();
  test_minus4 ();
  /* test_minus5 (); */

  test_div1 ();
  test_div2 ();
  /* test_div3 (); */

  test_all1 ();
  test_all2 ();

  return 0;
}
