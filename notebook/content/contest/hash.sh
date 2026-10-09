# Hashes a file, ignoring all whitespace and comments. Use for
# verifying that code was correctly typed.
cpp -x c++ -std=c++17 -dD -P -fpreprocessed | tr -d '[:space:]'| md5sum |cut -c-6
