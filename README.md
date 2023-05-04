# Autokey

Program for file encryption and decryption using the Autokey cipher.

## Introduction

This program allows you to encrypt or decrypt files. It offers a few
options - you can choose between a stream cipher encryption or a block
cipher encryption. If you want to use block cipher, you can also choose
between several modes - ECB (Electronic Code Book), CBC (Cipher Block Chaining),
OFB (Output Feedback) or CTR (Counter).

## Usage
Autokey has two modes - **Command line mode** and **Interactive mode**.

### Command line mode
This mode is called straight from the command line. You have to specify
these parameters:
+ if you want to **encrypt** or **decrypt** the file. For encryption, type
`-e` or `--encrypt`. For decryption, type `-d` or `--decrypt`.
+ **cipher key** - specified by option `-k` or `--key` directly followed by `=your_key`
+ **cipher mode** - specified by option `-m` or `--mode` directly followed by `=mode`.
You need to choose one of the following modes: `ecb, cbc, ofb, ctr, stream`.
+ **input file** specified by option `-i` or `--input` directly followed by
`=filename`

There is also an additional optional parameter:
+ **output file** - specified by option `-o` or `--output` directly followed by
`=filename`. If the output file is not specified, the output will automatically
be stored in file `input_file_without_extension`_encrypted.`extension` (resp.
_decrypted).

Example usage of command line mode:
```
./autokey -e -m=ctr -i=input_file.txt --output=output_file.txt -k=KEY
```

If you are not sure about how to use this program, you can also read the help
text. You can access it by typing: `./autokey -h` or `./autokey --help`. Also,
if you want to know a little more about how each mode works, you can display
it's help text by using the *help* option together with *mode* option (for example
`./autokey -h -m=ecb`).

### Interactive mode
You can enter the interactive mode by calling the program without any parameters:
```
./autokey
```
Then, each step is described. Firstly, you have to specify if you want to encrypt
or decrypt a file. Then, you specify input file name and optionally output file
name. After that you specify the key and then you choose what mode to use. After
you do all these steps, the program will encrypt/decrypt the given file.


## Building the project
You can build the project with Makefile. Type `make` for building the entire project.
The executable file will be called *autokey*. For removing the executable, use
`make clean`.

## Source code
The code is divided into several files and classes for better clarity.

###  CLI (Command Line Interface)
Class `CLI` handles the command line. There are methods for both
command line mode and interactive mode. There are also methods for loading and
verifying options entered by the user.

### RandomStringGenerator
This class provides a random string generator that is needed in some of the block
cipher modes (CBC, OFB and CTR).


### Autokey
Class `Autokey` handles encryption and decryption using the
Autokey cipher. There are methods for encrypting/decrypting the whole file,
blocks, or just the next character. Also, for the encryption and decryption, the
cipher key is needed - that's why it's a protected attribute of this class. Some
cipher modes also need the RandomStringGenerator, which is another protected
attribute.

### Cipher modes
Each cipher mode has it's own class. All block cipher modes have very similar codes.
They all use methods from `Autokey` class for encrypting and decrypting blocks.
But because each mode is different, each has to implement their own
`encrypt()` and `decrypt()` methods. There is also a class for the stream
cipher mode - this one doesn't process file in blocks, it processes the whole
file at once.


## How the Autokey cipher works
In this program, the Autokey cipher is used. This cipher was designed for 
use on the english alphabet, but it can be used on any other fixed set of 
characters. I decided to use it on the `char` type, which can hold values from 
0-255. So this alphabet has 256 characters. 

For both encryption and decryption a cipher key is needed. This key has 
to be specified by the user (it cannot be an empty string).

### Encryption
Characters are encrypted one at a time. We take first plaintext character 
and first key character, add those together and take this sum modulo 256. 
That way we obtain a new character, which will be the first character of 
the ciphertext. 

After encrypting a character, we take the used key character and remove it from 
the key. Then, we take the plaintext character that was being encrypted and 
we put it at the end of the key. So the key works like a queue - we take key 
characters from the front for encryption and we put new characters to the back.
This way, the whole plaintext is encrypted into ciphertext. 

### Decryption
The decryption process is very similar. Again, we take the first character from 
the key and the current encrypted character from ciphertext. But this time, 
we subtract the value of key character from the encrypted character. This 
product modulo 256 is the original plaintext character. 

After decrypting a character, we remove the first key character from the queue. 
Then we take the decrypted character and put it at the 
end of the key queue and it will be used for decryption of another character. 
This way, the whole ciphertext is decrypted into original plaintext.
