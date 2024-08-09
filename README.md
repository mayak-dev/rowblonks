# rowblonks
A hack for an old version of a brick game

## About
I wrote this hack so some friends and I can mess around while I stay in-practice with reverse-engineering and game hacking. I have made the repository public to help serve my portfolio.

This hack fixes some bugs, bypasses some of the game's security checks, enables unlocking its frame rate, and extends its Lua API to add and restore functionality, with possibly more to come.

## Usage
Using this hack requires a vanilla copy of the game version and a PE editor (such as [Stud_PE](https://www.cgsoftlabs.ro/studpe.html)). After compiling the DLL, the game's import table should be patched to include the `nothing` function, defined in [dllmain.cpp](Patcher/dllmain.cpp). I recommend setting the `IMAGE_FILE_LARGE_ADDRESS_AWARE` flag in the COFF header's characteristics field to bump the game's memory limit to 4 GB, as well as the `IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE` and `IMAGE_DLLCHARACTERISTICS_NX_COMPAT` flags in the DLL characteristics field for additional security.

For certain reasons, I will not be sharing the game's version info or providing a method to obtain it.
