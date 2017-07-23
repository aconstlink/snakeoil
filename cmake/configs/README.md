
# Overview

this folder contains cmake config scripts for packages not containing such files for easy external package configuration with snakeoil. 

# How to use

- copy config files into root folder of binary package of certain external library
- use cmake's file_package and point it to the directory containing the copied config file
- targets will be available in your cmake project as import targets
