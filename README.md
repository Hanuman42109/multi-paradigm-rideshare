# Basic Statistics Calculator - C, OCaml, and Python

This project implements a program that calculates **mean**, **median**,
and **mode** for a list of integers in three languages, each demonstrating
a different programming paradigm:

| Language | Paradigm     | File                                 |
|----------|--------------|---------------------------------------|
| C        | Procedural   | [`C/statistics.c`](C/statistics.c)   |
| OCaml    | Functional   | [`OCaml/statistics.ml`](OCaml/statistics.ml) |
| Python   | Object-Oriented | [`Python/statistics.py`](Python/statistics.py) |

All three implementations operate on the same sample dataset and produce identical results, which makes it easy to compare the three approaches
directly.

```
Dataset: [4, 8, 15, 16, 23, 42, 4, 8, 15, 16, 8]

Mean:   14.4545
Median: 15.0000
Mode(s) (frequency = 3): [8]
```

## How to Run

### C (Procedural)

```bash
cd c
gcc -Wall -Wextra -std=c11 -o statistics statistics.c
./statistics
```

The C implementation manages all dynamic memory manually with
`malloc`/`realloc`/`free` (used for the mode-result buffer) and has been
verified leak-free with `valgrind --leak-check=full`.

### OCaml (Functional)

```bash
cd ocaml

# Option 1: run directly via the toplevel interpreter
ocaml statistics.ml

# Option 2: compile to a native executable, then run
ocamlopt -o statistics statistics.ml
./statistics
```

The OCaml implementation uses only immutable `int list` values and
higher-order functions (`List.fold_left`, `List.map`, `List.filter`,
`List.sort`) — there are no `ref` cells, no array mutation, and no loops.

### Python (Object-Oriented)

```bash
cd python
python3 statistics_calculator.py
```

Requires Python 3.7+. No external dependencies. The `StatisticsCalculator`
class encapsulates the dataset and exposes `calculate_mean()`,
`calculate_median()`, `calculate_mode()`, and a convenience `summary()`
method.

## Design Notes

* All three implementations correctly handle **ties in mode** (returning
  every value tied for the highest frequency) and **even-length lists**
  for median (averaging the two middle values).
* The same sample dataset is hard-coded into each program's
  driver/`main` so that outputs can be compared directly across
  languages; in a production setting this would instead be read from
  stdin, a file, or command-line arguments.
* Each source file is heavily commented to call out where the
  paradigm's defining characteristics show up in the code (manual
  memory management in C, immutability/higher-order functions in
  OCaml, encapsulation in Python).

## Running Online (OnlineGDB)
 
All three implementations can be run directly in the browser using [OnlineGDB](https://www.onlinegdb.com) — no local installation needed.
 
1. Go to [https://www.onlinegdb.com](https://www.onlinegdb.com)
2. Select the appropriate language from the dropdown (C, Python 3, or OCaml)
3. Paste the source code into the editor
4. Click **Run** to execute and see the output

This is a convenient way to test and take screenshots of the code and its output for documentation or submission purposes.
