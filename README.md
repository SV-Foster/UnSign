# PE/COFF executable unsigning tool

Revolutionary command line tool for Windows that is set to change the way developers work with PE/COFF executable files. With the ability to completely remove embedded signatures, including all sections of the signature, this program allows for uncompromised control over the signing process!

![Screenshot](Documents/before-after-GUI.png)

No longer will developers be limited by pesky embedded signatures that are difficult to remove. UnSign tool gives users the power to easily and effectively strip away all traces of a signature, leaving behind a clean and pristine executable file, library or driver.

But that's not all - UnSign tool also offers a solution to the frustrating PE header problem that has plagued developers for years. This problem often renders a PE/COFF executable file unsingable, leaving developers scratching their heads and searching for a solution. With UnSign tool, this issue becomes a thing of the past, as it provides a comprehensive fix for the PE header problem, ensuring that executable files can be signed again without any hiccups.

Say goodbye to the limitations and frustrations that come with working with PE/COFF executable files. UnSign tool takes the power back into the hands of developers, allowing for seamless and hassle-free signing processes. Whether you're a seasoned developer or just getting started, UnSign tool is a must-have tool for anyone working with PE/COFF executable files.


## Usage

UnSign is the command line tool. Open command prompt, navigate to UnSign folder and start the tool with following parameters:

    unsign /<switch 1> /<switch N> <PE/COFF executable file>
    
    <Switches>
      /NoLogo       Don't print copyright logo
      /NoCertPrint  Don't exam and print certificates on the file

WARNING: This program will modify the file inplace on the disk. Please create a backup copy if you want to preserve the original file!

![Screenshot](Documents/screenshot%20001.png)
![Screenshot](Documents/screenshot%20002.png)

## Building

UnSign tool uses the Microsoft Visual Studio 2022 for its builds.

To build UnSign tool from source files with Microsoft Visual Studio, you can use either the graphical mode or the command-line mode. Here are the instructions for both methods:

### Graphical mode
1. Open Microsoft Visual Studio and select Open a project or solution from the start page or the File menu.
2. Browse to the folder where the .sln file is located and select it. This will load the project in Microsoft Visual Studio.
3. Select the configuration and platform for the project by using the drop-down menus on the toolbar. For example, you can choose Debug or Release for the configuration, and x86 or x64 for the platform.
4. Build the project by clicking on the Build menu and selecting Build Solution. You can also use the keyboard shortcut Ctrl+Shift+B.
5. Run the project by clicking on the Debug menu and selecting Start Debugging. You can also use the keyboard shortcut F5

### Command-line mode
1. Open a Developer Command Prompt for Microsoft Visual Studio. You can find it in the Start menu under Microsoft Visual Studio Tools.
2. Navigate to the folder where the .sln file is located by using the cd command.
3. Invoke the MSBuild tool to build the project. You can specify various options and flags for the tool. For example, the following command builds the project with the Release configuration and the x64 platform:
```
msbuild unsign.sln /p:Configuration=Release /p:Platform=x64
```
4. Run your executable by typing its name in the command prompt. For example:
```
unsign-exe-x86-64\unsign /?
```


## Authors

This program was written and is maintained by SV Foster.


## License

This program is available under EULA, see [EULA text file](EULA.txt) for the complete text of the license. This program is free for personal, educational and/or non-profit usage.
