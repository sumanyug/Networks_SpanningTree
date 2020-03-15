#!/usr/bin/zsh
./bsim < ./Testcases/Practice/Practice\ Testcases/1_inp_test1 >outfiles/out1
diff ./Testcases/Practice/Practice\ Testcases/1_out_test1 outfiles/out1
./bsim < ./Testcases/Practice/Practice\ Testcases/2_inp_test4 >outfiles/out2
diff Testcases/Practice/Practice\ Testcases/2_out_test4 outfiles/out2
./bsim < ./Testcases/Practice/Practice\ Testcases/3_inp_test3 >outfiles/out3
diff Testcases/Practice/Practice\ Testcases/3_out_test3 outfiles/out3
./bsim < ./Testcases/Practice/Practice\ Testcases/4_inp_test2 >outfiles/out4
diff Testcases/Practice/Practice\ Testcases/4_out_test2 outfiles/out4
