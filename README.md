# How to compile
To compile the project, just use

```
make all
```

For development purposes, it's recommended to build the project with

```
make build
```

Though it has some dependencies, since it runs a formatter and a linter before compilation. 
For compacting it into a .zip file, use

```
make zip
```

To clean up the directory from binaries and unused files, use

```
make clean
```

To change the compiler being used in the make statements, use

```
make <statement> COMPILER=<custom compiler>
```

Below is the list of dependencies:
- gcc            (replaceable with no guarantee)
- clang-format   (dev)
- cppcheck       (dev)

