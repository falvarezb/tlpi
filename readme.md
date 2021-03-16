run Build_ename.sh
gcc  -I./lib fileio/copy.c lib/error_functions.c -o fileio/out/runcopy
gcc  -I./lib fileio/tee.c lib/error_functions.c -o fileio/out/runtee
gcc  -I./lib fileio/seek_io.c lib/error_functions.c lib/get_num.c -o fileio/out/seek_io
gcc  -I./lib fileio/t_truncate.c lib/error_functions.c lib/get_num.c -o fileio/out/t_truncate
gcc  -I./lib fileio/t_writev.c lib/error_functions.c -o fileio/out/t_writev
gcc  -I./lib fileio/atomic_append.c lib/error_functions.c lib/get_num.c -o fileio/out/atomic_append
gcc  -I./lib fileio/multi_descriptors.c lib/error_functions.c -o fileio/out/multi_descriptors