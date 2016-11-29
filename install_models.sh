#!/bin/sh

sudo mkdir -p /opt/sensei
sudo chown -R $(whoami) /opt/sensei
cp -r models/* /opt/sensei
