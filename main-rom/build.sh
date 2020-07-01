#!/bin/bash

# YES, this is awful, and this point we have no time to care.
# If someone wants to fix the Makefile, be my guest...

make clean
make assets
make || true
make
