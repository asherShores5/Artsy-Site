# Artsy Coding: Learn the art of programming
Website created by Andrew Esch, Asher Shores, Albert Gonzales, and Karan Sharma

## Operating System
Artsy is intended to run on Ubuntu 20.04. Other flavors of Debian may work as well, but these operating systems are untested with Artsy.

## Web Server and PHP
Artsy is intended to run on PHP 8.x with either the LAMP or LEMP stack (i.e., Apache or Nginx).

## Package Dependencies
The following packages need to be installed in Ubuntu for Artsy to function properly:
- make
- flex
- bison
- wabt
- build-essential
- gcc


To install these packages, run the following commands in your terminal:
```
sudo apt-get update
sudo apt-get install make flex bison wabt build-essential gcc
```

In addition, the Artsy site requires the use of Composer for PHP package management. If running this from a local device, make sure to install [Composer](https://getcomposer.org/) and run the following command.
```
composer update
```

## Permissions
Ensure that PHP has full read, write, and execute permissions for the /src directory. This can be done using the following command:
```
sudo chmod -R 777 /path/to/artsy/src
```

## Using the Artsy Programming Language
For detailed instructions on how to use the Artsy programming language, please refer to our [documentation website](https://drewesch.notion.site/Artsy-Coding-7b9021e7eb11475ca68ca2d7a60bdd1e).
