Cleps-Video-Player
==================

A simple (bloat-free) Video Player. Inspired by Kde's Dragon Player, Cleps is built on the idea that video players are for viewing videos. Cleps does not claim to play exotic formats like Vlc or other players but simply play the ones that matter most. 

Cleps does not support Cd/Dvd i/o and will not transcode files. It depends on Qt5/Qt5 Multimedia and indirectly on gstreamer.



Building
=================
 Cleps needs only 2 steps to build
 1) run cmake on the sources
 2) run make to compile
 
=>note that building requires a c++11 compiler (a few lines were better expressed in c++11)

=>pkgbuild for ArchLinux is available in the AUR

=>Building from qmake is still supported for compatibility reasons