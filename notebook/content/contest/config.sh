# Adiciona a ~/.bashrc; depois: source ~/.bashrc
compilecp() {
  local f="${1%.cpp}"
  g++ -std=gnu++17 -O2 \
    -Wall -Wextra -Wshadow \
    "$f.cpp" -o "$f"
}

debugcp() {
  local f="${1%.cpp}"
  g++ -std=gnu++17 -O0 -g \
    -Wall -Wextra -Wshadow \
    -DLOCAL -D_GLIBCXX_DEBUG \
    -fsanitize=address,undefined \
    -fno-omit-frame-pointer \
    "$f.cpp" -o "$f.dbg"
}