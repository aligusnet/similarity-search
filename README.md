# Similarity Search

## Usage

```
% make ui         # build the project and run UI for 100 million people
% make benchmark  # build the project and run benchmark tests for 100 million people
% make test       # build the project and run tests for 1 million people
```

## Benchmark results

### 100 million people

```
100000000 persons generation took 12455 ms
tree construction took 113591 ms
average value: 10.5462 microseconds
0.95 quantile: 18 microseconds
0.99 quantile: 43 microseconds
```

### 300 million people

```
300000000 persons generation took 36679 ms
tree construction took 401211 ms
average value: 11.3878 microseconds
0.95 quantile: 18 microseconds
0.99 quantile: 66 microseconds
```