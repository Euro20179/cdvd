# CDVD

This is going to be a more generic, and configurable version of [my older project](https://github.com/Euro20179/DVD_Screen)

# Installation

## To Run

* lua
* sdl2

```sh
./build
mkdir -p ${XDG_CONFIG_HOME:-$HOME/.config}/cdvd
sudo mkdir -p /etc/cdvd/
sudo cp config/* /etc/cdvd
./dvd
```


# Goals

* Good lua api
* Change sprites at runtime
* Easily add/remove sprites
* Generic font paths
* Lua api for rendering text
