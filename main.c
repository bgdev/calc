#include "calc.h"
#include <stdio.h>
#include <stdlib.h>

int
main ()
{
  char *line;
  size_t n;
  CALC_NEW (c);

  line = malloc (n = 512);
  while (getline (&line, &n, stdin) > 0)
    printf ("%d\n", calc_process_line (&c, line));

  free (line);
  return 0;
}
