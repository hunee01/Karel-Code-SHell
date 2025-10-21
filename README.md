# Karel-Code-SHell
**A toy command line shell based off Bash.**<br/>
Made with very minimal C experience, and on CodeHS, hence the name!<br/>
<br/>
All required scripts contained in the `karel` folder.<br/>
Type `'help'` for the list internally defined commands.<br/>
<br/>
Any reported bugs would be **greatly appreciated**.<br/>
This is a beginner project, and I'll be glad to increase my skills through improvement.<br/>
<br/>
<sub>A 'mens et manus' project</sub><br/>
Thanks to [Polecat](https://github.com/polecatttt) for help and inspiration!

---

# Installation
**Requirements:**<br/>
- C compiler (e.g. gcc, clang, or tcc)
- POSIX Environment (Linux, MacOS)
- Standard C headers (no external libraries)

<!-- -->

**Build:**<br/>
From the root folder of the downloaded release:<br/>
```console
$ cd karel
$ gcc -o kcsh main.c
```
**Run:**<br/>
```console
$ ./kcsh
```
> If your compiler produces an error or warns about missing errors, please make sure you are on a POSIX compatible environment.  
> On Windows, use WSL or MinGW, as native 32-bit and 64-bit Windows builds are not supported. 