<div align="center">

 ![Image](https://github.com/user-attachments/assets/f7041d21-7e28-4b16-8a7d-c347c3ce6426)
 
[![GitHub License](https://img.shields.io/github/license/camtisocial/p2p-chess)](https://github.com/camtisocial/p2p-chess/blob/main/LICENSE)
[![GitHub last commit](https://img.shields.io/github/last-commit/camtisocial/p2p-chess)](https://github.com/camtisocial/p2p-chess/commits/main)
[![Ubuntu PPA](https://img.shields.io/badge/PPA-available-brightgreen?logo=ubuntu)](https://launchpad.net/~camtisocial/+archive/ubuntu/p2p-chess)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/camtisocial/p2p-chess)](https://github.com/camtisocial/p2p-chess/releases/latest)

 A chess app for terminal with p2p multiplayer and a customizable color pallete
___

 ![Image](https://github.com/user-attachments/assets/645df883-89e5-475c-8d67-c93c8e9d86a7)
</div> 
<br></br>
 
## Install
### PPA (Ubuntu focal, jammy, noble)

```
sudo add-apt-repository ppa:camtisocial/p2p-chess
sudo apt update
sudo apt install p2p-chess
```

### From .deb (Any Debian based system)
```
wget -O p2p-chess.deb https://github.com/camtisocial/p2p-chess/raw/main/releases/p2p-chess_1.0.0_amd64.deb
sudo apt install ./p2p-chess.deb
```

### Manual build (For all other distros)
```
git clone https://github.com/camtisocial/p2p-chess.git
cd p2p-chess/src
g++ std=c++17 -pthread -o /usr/bin/p2p-chess ./*.cpp -I. 
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
