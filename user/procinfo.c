#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

static const char *statename(int state)
{
  switch (state) {
    case 0: return "UNUSED";
    case 1: return "USED";
    case 2: return "SLEEPING";
    case 3: return "RUNNABLE";
    case 4: return "RUNNING";
    case 5: return "ZOMBIE";
    default: return "?";
  }
}

int
main(void)
{
  struct procinfo buf[NPROC];
  int n, i;

  n = getprocinfo(buf, NPROC);
  if (n < 0) {
    fprintf(2, "getprocinfo failed\n");
    exit(1);
  }
  printf("PID\tSTATE\n");
  for (i = 0; i < n; i++)
    printf("%d\t%s\n", buf[i].pid, statename(buf[i].state));
  exit(0);
}
