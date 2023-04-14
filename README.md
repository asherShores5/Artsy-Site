# Artsy Coding: Learn the art of programming
Website created by Andrew Esch, Asher Shores, Albert Gonzales, and Karan Sharma

## Operating System
Artsy is intended to run on Ubuntu 20.04. Other flavors of Debian may work too, but these operating systems are untested with Artsy.

## Web Server and PHP
Artsy runs on PHP 8.2 with either Apache or Nginx.

## Package Dependencies
The following packages need to be installed in Ubuntu for Artsy to function properly:
- make
- flex
- bison
- wabt
- build-essential
- gcc
To install these packages, run the following command in your terminal:
```
sudo apt-get install make flex bison wabt build-essential gcc
```

## Permissions
Ensure that PHP has full read, write, and execute permissions for the /src directory. This can be done using the following command:
```
sudo chmod -R 777 /path/to/artsy/src
```

For more detailed instructions on how to use Artsy as a programming language, please refer to the [Artsy documentation website](https://drewesch.notion.site/Artsy-Coding-7b9021e7eb11475ca68ca2d7a60bdd1e).