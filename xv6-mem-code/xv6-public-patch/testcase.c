#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(void)
{
  printf(1, "Hello, world!\n");
  
  printf(1, "%d\n", numvp());
  printf(1, "%d\n", numpp());

  exit();
}
