make
LLVM_PROFILE_FILE="test.profraw" ./build/tests
llvm-profdata merge -sparse test.profraw -o test.profdata
llvm-cov show --instr-profile test.profdata build/tests -use-color --format html src/ > coverage.html