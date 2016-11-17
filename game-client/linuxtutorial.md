# Installation instructions for an Arch based system

1. Type the following into a terminal:

``` sh
sudo pacman -S sfml base-devel git cmake
git clone https://github.com/University-of-Utah-CS3505/edu-app-unescaped-characters.git

# Instead of manually compiling the tmxlite-git package,
# you may use any AUR app like yaourt to install tmxlite-git instead of doing the below.
# https://aur.archlinux.org/packages/tmxlite-git/

cd edu-app-unescaped-characters/arch/tmxlite-git/
makepkg
sudo pacman -U tmxlite-git-v1*

cd ../..
cmake ./
make
./loque
```
Done!
