rm -rf param_data.out
gcc -g param_data.c -Tparam_manager.lds -o param_data.out
./param_data.out

