#!/bin/bash
mkdir -p ./bin/debug/MainApp.app/Contents/Resources/Image
mkdir -p ./bin/release/MainApp.app/Contents/Resources/Image
cp -r ./MainApp/res/Image/* ./bin/debug/MainApp.app/Contents/Resources/Image/
cp -r ./MainApp/res/Image/* ./bin/release/MainApp.app/Contents/Resources/Image/
cp -r ./MainApp/res/MainFrame.xml ./bin/debug/MainApp.app/Contents/Resources/
cp -r ./MainApp/res/MainFrame.xml ./bin/release/MainApp.app/Contents/Resources/
