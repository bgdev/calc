#include "calc.h"
#include <ctype.h>
#include <stdio.h>              /* TODO */

#define STATE_CLEAR 3
#define STATE_APPEND 4
#define STATE_EQ 5

#define FLAG_PENDING_EQ 0x1
#define FLAG_FIRST 0x2
#define FLAG_PREV_EQ 0x4
#define FLAG_ERR 0x8

int copy_x (struct calc *c);
int eq (struct calc *c);
int is_operation (char c);
void swap_xy (struct calc *c);

int is_flag_on (struct calc *c, int flag);
int set_flag (struct calc *c, int flag, int on);

void calc_init (struct calc *c)
{
  c->x = c->y = 0;
  c->state = STATE_APPEND;
  c->op = '+';
  c->flags = FLAG_FIRST;
}

int
calc_process_line (struct calc *calc, char *line)
{
  char c;
  float res;

  while ((c = *line++) != '\0')
    res = calc_process_cmd (calc, c);

  return res;
}

int
calc_process_cmd (struct calc *c, char cmd)
{
  if (is_flag_on (c, FLAG_ERR))
    return 0;

  if (isdigit (cmd))
    {
      if (c->state != STATE_APPEND)
        c->x = cmd - '0';
      else
        c->x = c->x * 10 + (cmd - '0');
      c->state = STATE_APPEND;

      if (is_flag_on (c, FLAG_FIRST))
        set_flag (c, FLAG_FIRST, 0);
      else
        set_flag (c, FLAG_PENDING_EQ, 1);
    }
  else if (is_operation (cmd))
    {
      set_flag (c, FLAG_FIRST, 0);

      if (is_flag_on (c, FLAG_PENDING_EQ))
        eq (c);

      c->y = c->x;
      c->op = cmd;

      c->state = STATE_CLEAR;
      set_flag (c, FLAG_PREV_EQ, 0);
    }
  else if (cmd == '=')
    {
      eq (c);
      c->state = STATE_EQ;
    }

  /* printf ("cmd=%c, x=%d, y=%d\n", cmd, c->x, c->y); */
  return c->x;
}

int
copy_x (struct calc *c)
{
  int tmp = c->y;
  c->y = c->x;
  return tmp;
}

int
eq (struct calc *c)
{
  if (c->op == '*')
    c->x *= c->y;
  else if (c->op == '+')
    c->x += !set_flag (c, FLAG_PREV_EQ, 1) ? copy_x (c) : c->y;
  else if (c->op == '-')
    {
      if (!set_flag (c, FLAG_PREV_EQ, 1))
        swap_xy (c);
      c->x -= c->y;
    }
  else if (c->op == '/')
    {
      if (!set_flag (c, FLAG_PREV_EQ, 1))
        swap_xy (c);
      if (c->y == 0)
        set_flag (c, FLAG_ERR, 1);
      else
        c->x /= c->y;
    }
  set_flag (c, FLAG_PENDING_EQ, 0);
  return c->x;
}

int
is_operation (char c)
{
  return c == '*' || c == '+' || c == '-' || c == '/';
}

void
swap_xy (struct calc *c)
{
  int tmp = c->x;
  c->x = c->y;
  c->y = tmp;
}


/*********/
/* Flags */
/*********/

int
is_flag_on (struct calc *c, int flag)
{
  return !!(c->flags & flag);
}

int
set_flag (struct calc *c, int flag, int on)
{
  int prev;
  prev = is_flag_on (c, flag);

  if (on)
    c->flags |= flag;
  else
    c->flags &= ~flag;

  return prev;
}
