#!/bin/bash
mkdir -p ./bin/debug/Image
mkdir -p ./bin/release/Image
cp -r ./MainApp/res/Image/* ./bin/debug/Image/
cp -r ./MainApp/res/Image/* ./bin/release/Image/
cp -r ./MainApp/res/MainFrame.xml ./bin/debug/
cp -r ./MainApp/res/MainFrame.xml ./bin/release/
