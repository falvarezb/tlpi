## Pre-requisites

- build static library *libtlpi.a* containing functionality of *lib/*
- generate *ename.c.inc* with list of error codes
- install library [cmocka](https://cmocka.org/) to run the tests

This is achieved by running the goal `make all` inside *lib/*