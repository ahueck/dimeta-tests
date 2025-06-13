# SPEC MPI 2007 v2.0.1

## 122.tachyon

### Building the benchmark

The benchmark can be built in vanilla by simply typing

```{.bash}
make
```

and with TypeART instrumentation by typing

```{.bash}
make TYPEART=yes
```

### Running the benchmark

To run tachyon, the data folder holds two different data sets.
To test whether your version starts and computes something, type

```{.bash}
mpirun -np 8 ./122.tachyon data/mtest_scene.dat
```

To run a benchmark test case use

```{.bash}
mpirun -np 15 ./122.tachyon data/mref_dnadof.dat
```

NOTE: To reasonably time the execution, you must use a timing facility, e.g. the unix /bin/time.
