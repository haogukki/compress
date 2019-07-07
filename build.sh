#!/bin/bash

PROJECT=$(pwd)
TARGET=$PROJECT/Jarvis
echo `du -h $PROJECT/data/png/0001.png`&&$TARGET $PROJECT/data/png/0001.png
echo `du -h $PROJECT/data/png/0002.png`&&$TARGET $PROJECT/data/png/0002.png
echo `du -h $PROJECT/data/png/0003.png`&&$TARGET $PROJECT/data/png/0003.png
echo `du -h $PROJECT/data/png/0004.png`&&$TARGET $PROJECT/data/png/0004.png
echo `du -h $PROJECT/data/png/0005.png`&&$TARGET $PROJECT/data/png/0005.png
echo `du -h $PROJECT/data/pdf/0001.pdf`&&$TARGET $PROJECT/data/pdf/0001.pdf
echo `du -h $PROJECT/data/txt/0001.txt`&&$TARGET $PROJECT/data/txt/0001.txt
echo `du -h $PROJECT/data/txt/0002.txt`&&$TARGET $PROJECT/data/txt/0002.txt
echo `du -h $PROJECT/data/txt/0003.txt`&&$TARGET $PROJECT/data/txt/0003.txt
echo `du -h $PROJECT/data/txt/0004.txt`&&$TARGET $PROJECT/data/txt/0004.txt
echo `du -h $PROJECT/data/txt/0005.txt`&&$TARGET $PROJECT/data/txt/0005.txt
