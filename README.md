ThreeRing
=========

ThreeRing aims to be an open-source (GPL v3) cross-platform note-taking
application that uses HTML, SVG, and similar web technologies as the native
file format so that notes can be read in any standards-compliant browser.


State of ThreeRing
------------------

ThreeRing is very early in development, and many of its planned features have
not yet been implemented.

#### What works
1. Drawing and erasing ink with the pen and eraser of a Wacom tablet.
2. Saving and opening notes to and from SVG files.
3. A lasso tool (with the Shift key) to select a free-form area of ink.
4. A move tool (active when there is selected ink) that can move and delete ink.

#### What doesn't
1. Undo and redo.
2. Text boxes and other note types.
3. Support for mobile platforms (iOS, Android).

Building from source
--------------------

ThreeRing is a standard Qt 4.7.x/4.8.x application with no external
dependencies and has been tested on Windows, Mac OS X, and Linux.

### Setup
1. Install a C++ compiler, such as GCC (on Linux),
[Clang][xcode] (on Mac OS X), or
[Microsoft Visual C++ 2010][msvc] (on Windows).
2. Install the [Qt 4.8.x libraries][qt48] for your platform.
    * Mac OS X note: Qt tablet support is [broken][macbug] in the 4.8.x line.
    Install the [4.7.x libraries][qt47mac] instead.
3. (Optional) Install Qt Creator.

[xcode]: https://itunes.apple.com/us/app/xcode/id497799835
[msvc]: http://www.microsoft.com/visualstudio/eng/downloads#d-2010-express
[qt48]: http://qt-project.org/downloads#qt-lib
[macbug]: https://bugreports.qt-project.org/browse/QTBUG-26532
[qt47mac]: ftp://ftp.qt-project.org/qt/source/qt-mac-opensource-4.7.4.dmg

### Compiling

#### From the command line
1. Clone ThreeRing into a directory.
2. On Windows, open a Qt 4.8 Command Prompt and do the following:

        cd [cloned-threering]
        qmake
        nmake

3. On Linux, open a shell and do the following:

        cd [cloned-threering]
        qmake
        make

4. On Mac, open a shell and do the following:

        cd [cloned-threering]
        qmake -spec macx-g++
        make

For release mode, replace `make` with `make release`. You may need to copy some
Qt shared libraries from their install directories to the ThreeRing directory
for it to run.

#### With Qt Creator
1. Open ThreeRing.pro in Qt Creator.
2. Click Run (or press Ctrl+R).
