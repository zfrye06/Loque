# How to compile loque on Windows

**Warning**: C++ programming sucks on windows. Highly recommend dual booting Linux or Mac.
Beware all who wish to brave the tutorial below.

---

1. Download and install a visual studio version. Make sure you know which version you've got.
2. Download a precompiled SFML library from here: http://www.sfml-dev.org/download/sfml/2.4.1/ **Make sure that visual studio version matches up with the SFML binaries.**
3. Clone https://github.com/fallahn/tmxlite.git
4. Clone https://github.com/University-of-Utah-CS3505/edu-app-unescaped-characters.git
6. Run cmake, and attempt to configure tmxlite as Visual Studio build files. It should fail due to not finding any compilation tools.
7. Open visual studio's development console, and cd into the tmxlite directory, and run `cmake ./`. This should set up the compilation tools.
8. Open the generated .sln files in Visual Studio, you may have to delete extraneous projects in order to compile.
9. Attempt to compile the library as a .dll somehow... probably has to do with the project settings somewhere.
10. Re-open cmake, and now attempt to configure the edu-app-unescaped-characters folder.
11. Like last time, you'll have to use the Visual Studio development console in order to let cmake find the build tools.
12. After the build tools are set up, you'll have to use the graphical cmake again in order to point cmake to the directories containing your libraries.
13. First, you'll have to set up a custom variable called "SFML_ROOT" as a path that directs cmake to the SFML folder you downloaded.
14. Second, you'll have to tell cmake where tmxlite's libary, and include folders are.
15. Finally you should be able to configure and generate the solution.
16. Open visual studio, and open the loque.sln. Delete any extreneous projects like "ALL_BUILD", and "ZERO_TEST".
17. Press play! Hopefully it should work....
