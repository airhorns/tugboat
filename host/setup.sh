#!/usr/bin/env bash
set -ex
defaults write org.macosforge.xquartz.X11 app_to_run '' # suppress xterm terminal
open -a XQuartz
/opt/X11/bin/xhost +$(ipconfig getifaddr en0)
