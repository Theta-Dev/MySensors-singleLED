# MySensors-singleLED
12V/2A 1-channel LED wireless switch for MySensors

## How to build
### Step 1: PCB assembly
Download the Gerber files for the PCB from this repository. Get the PCB manufactured by your favourite boardhouse. I chose AllPCB who sent me 12 50x100mm boards (green soldermask, HASL with lead, 1.2mm thickness) for about 15€.
Note that you will save some cost if you order a larger board with multiple designs placed next to each other like I did.

Prepare the electronics components listed on the BOM. Start by soldering the male 8pin JST connector to the side of the board. Be careful not to melt the plastic with your iron. Also don't apply solder to the top part of the contact blades since that will lead to bridges.
Then continue with the ATmega 328p microcontroller, the two 100nF capacitors C1/C3, the resistors R1, R4 and R5 and the dual LED D1.

Double check your solder connections, then continue with step 2
### Step 2: Programming and testing the microcontroller
To program your ATmega you will need adapter cables for ICSP and Serial. Solder them according to [these] instructions. You'll also need to add the barebone ATmega328p to the board definitions of your Arduino IDE.

Plug the ICSP adapter into your board and connect a 3.3V FTDI adapter to it. Then connect the FTDI adapter to your computer via USB.
Select "ATmega328p (8 MHz internal clock)" from the Board menu and make sure the correct serial port is selected. Then select Tools > Burn Bootloader and wait for the process to complete.
Then unplug the USB cable and the ICSP adapter and connect the Serial adapter to your board. Plug the adapter's header into your FTDI board and reconnect the USB cable.
To test your micro load the Blink sketch, change the LED pin to 7 and load it to your board. Now the LED should blink green.

Once you have tested your microcontroller you can continue soldering the other components.
### Step 3: PCB assembly goes on
Assemble all the other parts: begin with the small parts on the top side (voltage regulator U3, capacitors C2/C4, mosfet Q1). Flip the PCB and continue with R2, R3 and the RF69HW module.
If you want to have an expansion port (external buttons/serial communication) you'll have to solder a 4-pin header and the pullup resistors R6/R7.
Then connect the small pin next to the power terminal (NOT the one labeled "ANT") to the pin next to the output using solid core wire.
To complete the assembly solder the tactile switch S1, a 2pin screw terminal for the LED output and either a screw terminal or a DC barrel jack for the power input.
Thn you need to make your RF antenna.
To do this, cut a piece of solid core wire to the quarter of your RF module's wavelength. The easiest way to calculate the antenna length in meters is dividing 75 by the frequency of you module in (in my case 868).

Example: 75 / 868 MHz = 0,0864m = 8,64cm

Then solder your antenna to the pin labled "ANT", wrap it around a pencil to make it more compact and you're done!
As always: check your soldering, then continue to testing.
### Step 4: Testing
Flash the Arduino sketch to your board using the Serial adapter. Then connect some 12V LED strip to the output and apply 12V to the input terminal. If you have a working MySensors Gateway nearby you should see the LED flashing green a couple of times and the LED switch appear in the *Devices* section of your smart home server. Once you have set it up you will be able to control your LEDs from your computer or phone.
You can also manually control the LEDs using the button. Pressing it should switch the LEDs on/off, holding it enables you to dim them.

**Common errors (at least the ones I've encountered):**

**LED flashing red/orange:**
error with the wireless connection to your gateway. Try decreasing the distance or bending the antenna. Also make sure your gateway+server is working.
### Step 4: Printing the case
Download the provided STL files for the case, place the bottom part next to the top part in your slicer (rotate the top 180° around Y-axis), and print them using a 3D printer. I've used my Prusa MK3, blue and white PLA filament and 0.15mm layer height.

To get the cool 2-color look you can use Joe Prusas ColorPrint tool (https://www.prusaprinters.org/color-print/) and add a color change at 0.5 mm.
Start the print with the colorful filament (in my case blue). The printer will print the first two layers and then ask you to change the filament. Now load white or light grey filament and continue the print.
The whole print will take about 1h (depends on your printer).
### Step 5: Putting it all together
Put the top piece upside down on your table. Then insert the PCB (make sure button, LED, terminals and the mounting holes line up withe the case). Finally close the case using the bottom part and 2 self-tapping screws.
Now you can install your module whereever you have some 12V leds to control. Mine is doing it's job at the 1m led strip over my bed ;-)

## How to modify
This project is licensed under the CC-BY-SA license, so you can modify and re-publish it, as long as you'll keep it under that license and give me, the ThetaDev, credits.

I've created the PCB design using EasyEDA. The source files are located in the *PCB* folder. But you can also directly visit my project on EasyEDA (https://easyeda.com/ThetaDev/MySen_ledV2-e1ad562137ac4dbb967d829ceed0bd0b) to edit it.

The microcontroller gets programmed using the Arduino IDE, you'll find the sketch in the *Arduino* folder. The required libraries are:
- MySensors (https://github.com/mysensors/MySensors)
- OneButton (https://github.com/mathertel/OneButton)

The case was designed using Autodesk Fusion 360. Source files are in the *Case* folder.
