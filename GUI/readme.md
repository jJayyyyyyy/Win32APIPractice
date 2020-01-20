##  Compile-and-Run

We can just create an empty `Windows Desktop Application` with `Visual Studio 2017` then write-compile-run it.

If we don't want to install the IDE, we can just use gcc for Windows and write a batch file to compile-and-run it like

```sh
gcc template.c -lgdi32 -mwindows -o template
template
```
