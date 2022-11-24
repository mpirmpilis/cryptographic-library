# Cryptographic library
A simple cryptographic library for "CS-457 - Introduction to Information Security Systems" course (https://www.csd.uoc.gr/~hy457/).

The library is implemented from scratch in C and provides five basic cryptographic algorithms:
* One-time pad
* Caesar’s cipher
* Playfair cipher
* Affine cipher
* Feistel cipher


## How to run
```bash
make all
./cipher input
```

* You may provide your own plaintext in the form of a file. The **input** file presented in the directory used is for demonstration only.  
* Shifts for Caesar's are randomly generated between 1-100 and key for Playfair is "Hello COMPUTER SCIENCE DEPARTMENT and HY457" to run the program automatically.  
* If you want to provide your own inputs, then: remove the key initialization, rand()/srand() and the ```#if 0 .... #endif``` lines of the code.  
(as they will let the user input their own data in the program) at test_program.c .  
