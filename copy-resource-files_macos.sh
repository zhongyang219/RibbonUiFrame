#!/bin/bash
mkdir -p ./bin/debug/MainApp.app/Contents/MacOS/Image
mkdir -p ./bin/release/MainApp.app/Contents/MacOS/Image
cp -r ./MainApp/res/Image/* ./bin/debug/MainApp.app/Contents/MacOS/Image/
cp -r ./MainApp/res/Image/* ./bin/release/MainApp.app/Contents/MacOS/Image/
cp -r ./MainApp/res/MainFrame.xml ./bin/debug/MainApp.app/Contents/MacOS/
cp -r ./MainApp/res/MainFrame.xml ./bin/release/MainApp.app/Contents/MacOS/
