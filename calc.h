#ifndef _CALC_H_
#define _CALC_H_

#define CALC_NEW(name) struct calc name; calc_init (&name);

struct calc {
  int x, y;
  int op;
  int flags;
};

void calc_init (struct calc *);
int calc_process_line (struct calc *c, char *line);
int calc_process_cmd (struct calc *c, char cmd);

#endif /* _CALC_H_ */
