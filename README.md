<div align="center">

 ![Image](https://github.com/user-attachments/assets/f7041d21-7e28-4b16-8a7d-c347c3ce6426)

 ![GitHub License](https://img.shields.io/github/license/camtisocial/p2p-chess)
 ![GitHub last commit](https://img.shields.io/github/last-commit/camtisocial/p2p-chess)
 ![Debian package](https://img.shields.io/debian/v/p2p-chess)

 A chess app for terminal with p2p multiplayer and a customizable color pallete
</div> 

## Install

```
sudo add-apt-repository ppa:camtisocial/p2p-chess
sudo apt update
sudo apt install p2p-chess
```

## Usage 
- Just run p2p-chess in your terminal and navigate with your choice of wasd, arrows, or hjkl.
- use /commands in game (/help, /resign, /draw, /t, etc.)
- run with the -c flag to see the color options for your terminal
- adjust options in usr/share/p2p-chess/settings.ini

## issues 
- if p2p-chess is crashing on startup, its likely an issue with the filepath for stockfish. Try:
```
which stockfish
```
and make sure it is the same path as what's in /usr/share/p2p/chess/settings.ini
if not, install it with:
```
sudo apt install stockfish
```
Otherwise, please make a bug report and I will address it as fast as I can
