# Mini Git

A minimal Git implementation in C++23, supporting core Git plumbing commands.

## Features

- **init** - Initialize a new Git repository
- **hash-object** - Compute object hash and optionally write to object store
- **cat-file** - Retrieve and display object contents
- **ls-tree** - List contents of a tree object
- **write-tree** - Create a tree object from the current working directory
- **commit-tree** - Create a commit object

## Building

```bash
cmake -B build -S .
cmake --build build
```

## Usage

```bash
# Initialize repository
./build/git init

# Create a blob from file
./build/git hash-object -w <file>

# Read object contents
./build/git cat-file -p <sha>

# List tree contents
./build/git ls-tree [--name-only] <tree-sha>

# Create tree from working directory
./build/git write-tree

# Create commit
./build/git commit-tree <tree-sha> -p <parent-sha> -m "<message>"
```

## Architecture

```
include/
├── git.hpp                 # Unified header
├── commands/               # Command implementations
│   ├── command.hpp         # Base class
│   ├── init.hpp
│   ├── cat_file.hpp
│   ├── hash_object.hpp
│   ├── ls_tree.hpp
│   ├── write_tree.hpp
│   └── commit_tree.hpp
└── objects/                # Git object types
    ├── blob.hpp
    ├── tree.hpp
    ├── commit.hpp
    └── object_store.hpp    # Low-level storage

src/
├── main.cpp
├── commands/
└── objects/
```

## Design

- **Layered architecture**: Command -> Object -> Store
- **Factory pattern**: `Command::create()` for command dispatch
- **Modern C++**: Uses `std::expected`, `std::string_view`, `std::span` (C++23)
- **Error handling**: Functional-style error propagation with `std::expected<T, E>`

## Dependencies

- CMake 3.13+
- C++23 compiler
- OpenSSL (SHA-1)
- zlib (compression)
