• /* Start Header ------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: EncryptSCT.c
Purpose: To create a program that will take in any file, read the
content and perform a simple columnar tranposition cipher on it. Then, write 
the result in a specified output file. 
Language: C programming
Platform: GCC c89
Project: weizhe.goh_cs120_4
Author: Goh Wei Zhe 
Student Login: weizhe.goh / 440000119
StudentID: 1900806
Creation date: 31 Jan 2020
End Header --------------------------------------------------------*/ 


How to run the program:
gcc -Wall -Werror -Wextra -Wconversion -ansi -pedantic -o EncryptSCT.exe EncryptSCT.c

Encrypt: EncryptSCT.exe -e Input.txt Output.txt 
Decrypt: EncryptSCT.exe -d Input.txt Output.txt

- Ensure Input.txt file is created. 

Compile in visual studio: 
1. Change directory to where the folder is and enter.
2. type: cl /W4 /WX /nologo /Za /FeEncryptSCT.exe /TC EncryptSCT.c
3. Encrypt: EncryptSCT.exe -e Input.txt Output.txt 
3. Decrypt: EncryptSCT.exe -d Input.txt Output.txt

- Ensure Input.txt file is created. 
