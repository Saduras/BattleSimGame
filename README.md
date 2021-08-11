# BattleSimGame

The core of the engine code is inspired by The Cherno's Game Engine Series (https://thecherno.com/engine).

## Setup

In order to run unit tests as a post build step the path to `vstest.console.exe` must be set as an evironment variable. The path depends on the Visual Studio version and looks as follows
```
%Program Files(x86)%\Microsoft Visual Studio\<version>\<edition>\common7\ide\CommonExtensions\<Platform | Microsoft>\TestWindow\
```
[[Reference]](https://docs.microsoft.com/en-us/visualstudio/test/vstest-console-options?view=vs-2019)

## How to run

1. execute `GenerateProjets.bat` to generate Visual Studio 2019 project
2. open `BattleSimGame.sln`
3. Select "Build > Build solution (F6)"
4. Run app with "Debug > Start Debugging (F5)"