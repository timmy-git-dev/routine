# A simple build script to compile the program.
set -e

# Set project directories.
PATH_PWD="$PWD"
PATH_SRC="$PATH_PWD/src"
PATH_INC="$PATH_PWD/inc"
PATH_BIN="$PATH_PWD/bin"
PATH_OBJ="$PATH_BIN/obj"
PATH_EXE="$PATH_BIN/output"

# Set compile types.
COMPILE_VERSION="-std=c++23"
COMPILE_FLAGS="-O3 -s -DNDEBUG -fomit-frame-pointer -funroll-loops"
# COMPILE_FLAGS="-g -O0 -fsanitize=address -fno-inline -fno-omit-frame-pointer"
COMPILE_LIBRARIES=""

# Re-create the object directory.
rm -rf $PATH_OBJ
mkdir -p $PATH_OBJ

# Create the .clangd file to ensure IDE-syntax works correctly.
cat <<EOF > ${PATH_PWD}/.clangd
CompileFlags:
    Add:
        - -std=c++23
        - -I${PATH_INC}
        - -lpthread
        - -Wall
        - -Wextra
        - -Wpedantic

Documentation:
  CommentFormat: Doxygen
EOF

# Gather all .cpp files and compile each one into it's corresponding bin/obj/ location.
PATHS_CPP=$(find "$PATH_SRC" -type f -name "*.cpp")
for PATH_SRC_CPP in $PATHS_CPP; do
    PATH_REL_CPP="${PATH_SRC_CPP#$PATH_SRC}"
    PATH_OBJ_O="$PATH_OBJ${PATH_REL_CPP%.cpp}.o"
    PATH_SUB_OBJ="${PATH_OBJ_O%/*}"

    mkdir -p "$PATH_SUB_OBJ"

    g++ $COMPILE_VERSION $COMPILE_FLAGS $COMPILE_LIBRARIES -I$PATH_INC -c "$PATH_SRC_CPP" -o "$PATH_OBJ_O"
done

# Gather all compiled object files and link the project.
PATHS_O=$(find "$PATH_OBJ" -type f -name "*.o")
g++ $COMPILE_VERSION $COMPILE_FLAGS $COMPILE_LIBRARIES $PATHS_O -o $PATH_EXE

cd "$HOME"
BIN_DIR=".local/bin"
install -d "$BIN_DIR"
install -m 755 "$PATH_EXE" "$BIN_DIR/routine"

$BIN_DIR/routine