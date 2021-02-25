run Build_ename.sh
gcc  -I./lib fileio/copy.c lib/error_functions.c -o runcopy
gcc  -I./lib fileio/tee.c lib/error_functions.c -o runtee