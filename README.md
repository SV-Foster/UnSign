# Windows executable files unsigning tool

Revolutionary command line tool for Windows that is set to change the way developers work with executable files like .exe, .dll, .sys, .drv and others, both 32 and 64 bit. With the ability to completely strip embedded signatures, including all sections of the signature, this program allows for uncompromised control over the signing process!

![Screenshot](Documents/file%20signature%20before%20and%20after.png)

No longer will developers be limited by pesky digital signatures that are difficult to remove. UnSign tool gives users the power to easily and effectively strip away all traces of a signature, leaving behind a clean and pristine executable file, library or driver.

But that's not all — UnSign tool also offers a solution to the frustrating "optional PE header problem" that has plagued developers for years. This problem often renders an executable file unsignable, leaving developers scratching their heads and searching for a solution. With UnSign tool, this issue becomes a thing of the past, as it provides a comprehensive fix for the problem, ensuring that executable files can be signed again without any hiccups.

Say goodbye to the limitations and frustrations that come with Windows executable files signatures. UnSign tool takes the power back into the hands of developers, allowing for seamless and hassle-free unsigning processes. Whether you're a seasoned developer or just getting started, UnSign tool is a must-have tool for anyone working with PE32/PE32+ format executable files.


## Download

Latest prebuilt ready-to-use 32 and 64 bit versions are available for download via the links below. The builds are provided as standard `.zip` archives.

Please note: Most modern systems run 64-bit Windows. If you're unsure, download the 64 bit build first.

- **64 bit (x64) – Recommended for Windows 10/11**
  - [Download latest x64 build](https://github.com/SV-Foster/UnSign/releases/download/1.3/SV-Foster-UnSign-1.3-x64.zip)

- **32 bit (x86)**
  - [Download latest x86 build](https://github.com/SV-Foster/UnSign/releases/download/1.3/SV-Foster-UnSign-1.3-x86.zip)

Each archive contains:

* Precompiled `.exe` binaries
* Required runtime resources
* Documentation and README files

### Older Versions

If you need a specific version or an archived release, visit the project's Releases page:

* [All Releases](https://github.com/SV-Foster/UnSign/releases)


## Installation

This application is provided as a portable ZIP package and does not require installation. To unzip it:

1. Download the appropriate archive for your system
2. Right-click the downloaded `.zip` file
3. Select "Extract All..." from the context menu
4. Choose a destination folder and click "Extract"

The extracted folder can be moved later to any location.


## Usage

UnSign is the command line tool. Open a Command Prompt (terminal) window, navigate to UnSign's .exe folder with the `cd` command and start the tool with following parameters:

```unsign /<switch 1> /<switch N> <PE32/PE32+ executable file>```

WARNING: This program will modify the file inplace on the disk! Please create a backup copy if you want to preserve the original file!

Any version of the tool, 32 or 64 bit, works with both 32 and 64 bit executables (in PE32 and PE32+ format, also called PE/PE+ or PE/COFF).

![Screenshot](Documents/screenshot%20001.png)
![Screenshot](Documents/screenshot%20002.png)

### Switches
```
/NoLogo       Don't print copyright logo
/NoCertPrint  Don't exam and print certificates on the file
/CRC          Checksum recalculation policy
                Zero    - forcefully zero out the CRC field in the optional PE header
                Calc    - always recalculate the valid checksum
                Default - recalculate the checksum only if it was not 0 (default)
```

### Exit codes

An exit code is a number that is returned by a program when it finishes running. This code is used to indicate the success or failure of the program's execution.

The exit code is often used by other programs or scripts to determine how to proceed after running a particular program. For example, a batch script may check the exit code of a program and take different actions based on the result.

```
0             The requested operation completed successfully
1             The file to process was not found
2             The file is not signed
4             An initialization error has occurred. There is not enough memory or disk space, the path
              to the file is invalid, or the syntax of the arguments passed on the command line is invalid
5             Disruption or inability to read, write, analyze, or transfer data from or to the disk,
              or the file has a corrupted or incompatible format
```


## What's new

### Version 1.3

* Added detailed checks on the provided file for better error reporting while opening the file
* Added an exit code description
* Some spelling fixed

### Version 1.2

* Added the CRC of the executable file recalculation feature
* Added the friendly error message if switches were set but no file was provided to process
* Some spelling fixed

### Version 1.1

* Added the check for the number of directories in the PE32/PE32+ optional header, as suggested by documentation


## Building

UnSign tool uses the `Microsoft Visual Studio 2022` for its builds.

To build UnSign tool from source files with Microsoft Visual Studio, you can use either the graphical or the command-line mode. Here are the instructions for both methods:

### Graphical mode
1. Open Microsoft Visual Studio and select `Open a project or solution` from the start page or the `File` menu
2. Browse to the folder where the `unsign.sln` file is located and select it. This will load the project in Microsoft Visual Studio
3. Select the `configuration` and `platform` for the project by using the drop-down menus on the toolbar. For example, you can choose Debug or Release for the configuration, and x86 or x64 for the platform
4. Build the project by clicking on the `Build` menu and selecting `Build Solution`. You can also use the keyboard shortcut `Ctrl+Shift+B`
5. Run the project by clicking on the `Debug` menu and selecting `Start Debugging`. You can also use the keyboard shortcut `F5`

### Command-line mode
1. Open a `Developer Command Prompt` for Microsoft Visual Studio. You can find it in the Start menu under Microsoft Visual Studio Tools
2. Navigate to the folder where the `unsign.sln` file is located by using the `cd` command
3. Invoke the MSBuild tool to build the project. You can specify various options and flags for the tool. For example, the following command builds the project with the Release configuration and the x64 platform:
```
msbuild unsign.sln /p:Configuration=Release /p:Platform=x64
```
4. Run your executable by typing its name and path in the command prompt. For example:
```
unsign-exe-x86-64\unsign /?
```


## Authors

This program was written and is maintained by SV Foster.


## License

This program is available under EULA, see [EULA text file](EULA.txt) for the complete text of the license. This program is free for personal, educational and/or non-profit usage.
