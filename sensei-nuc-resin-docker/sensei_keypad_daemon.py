import os
import time
import asyncio
import evdev
import evdev.ecodes as e
from pprint import pprint

SENSEI_START_KEY = e.KEY_KPPLUS
SENSEI_STOP_KEY = e.KEY_KPENTER



def senseiStart():
        print("start sensei")

def senseiStop():
        print("stop sensei")

def getKeypads():
        devices = [evdev.InputDevice(fn) for fn in evdev.list_devices()]
    keypadDevices = []
    for device in devices:
	        c = device.capabilities()
        # Is the device "key-like"? (includes power buttons!)
        # Does the device have a keypad plus button?
        # Then it's probably a numeric keypad.
        if e.EV_KEY in c and e.KEY_KPPLUS in c[e.EV_KEY]:
	                keypadDevices.append(device)
    return keypadDevices

async def print_events(device):
        async for event in device.async_read_loop():
	            if not event.value == evdev.events.KeyEvent.key_up:
			            continue
        if event.code == SENSEI_START_KEY:
	                senseiStart()
        elif event.code == SENSEI_STOP_KEY:
	                senseiStop()

def main():
        for device in getKeypads():
	            asyncio.ensure_future(print_events(device))

if __name__ == '__main__':
        if os.geteuid() == 0:
	            main()
        loop = asyncio.get_event_loop()
        loop.run_forever()
    else:
	        print("you must run as root!")
