# filter test task

## Table of contents
- [Usage](#usage)
- [Features](#features)
- [Algorithm](#algoritm)
- [Environment](#environment)

---
## Usage

Type `make` to build project.
`make start` command starts executable file.

---
## Features

Rebuild project with commented `#undef DEBUG_TRACE` in `include/common.h` to make program print stages of expression evaluation process.

---
## Algorithm

Enter: `5*x-7.3=-12*6`

1) reverse polish notation calculation

`5.000000 x * 7.300000 - = 12.000000 - 6.000000 *`

2) building abstract syntax tree

`
===================
[=
|  [-
|  |  [*
|  |  |  [5.000000
|  |  |  [x
|  |  [7.300000
|  [*
|  |  [-
|  |  |  [12.000000
|  |  [6.000000
\==================
`

3) folding tree

`
===================
[=
|  [-
|  |  [*
|  |  |  [5.000000
|  |  |  [x
|  |  [7.300000
|  [-72.000000
\==================

`

4) `x` evaluation

`
===================
[=
|  [x
|  [-12.940000
\==================
`

---
## Environment

Tested in:
- Windows 10: Cygwin (gcc, make, vim, ctags);
- Ubuntu 22.04.1 LST (required package: build-essential).

