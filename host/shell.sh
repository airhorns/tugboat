#!/usr/bin/env bash
set -ex
host/setup.sh
IP=$(ipconfig getifaddr en0) docker-compose -f docker-compose.host.yml run host bash
