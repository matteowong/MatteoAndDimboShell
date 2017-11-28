#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "parse.h"


int main() {

  return execute();
}

/*notes on redirectting
 *close the pipe on the writing end (both 0 and 1)
 *send two things in the pipe is fine
 *use pipe for cd-ing
 *fix trim function, losing p from pwd and separating cd and .., something wrong w/ indexing
 */
