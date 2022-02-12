#!/bin/bash

(
  cd build;
  cmake ..;
  cmake --build .;
  ctest --output-on-failure
)
