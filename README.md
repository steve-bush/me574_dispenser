# Plague Destroyer 3000

Below are the steps for setting up group 6's hand sanitizer dispenser for either home use or company use. The files for setting up the logoff scripts are located in the `logoff_scripts` directory. The part files for 3D printing are located in the `printing` directory. Lastly, the Arduino code is located in the `dispenser` directory.

## Printing

Use whatever printing software you prefer for slicing the STL files. We used Cura for our own set up to get usable G code. We used PLA+ at 225 C ejection temperature, 75 C bed temperature, 55% infill, a wall count of 3, and a 45 mm/s print speed.

## Assembly

1. Watch instruction [video](https://youtu.be/RnLYFoYw21w) for more information on the following steps.
2. Assemble PCB board according to the [schematic](https://github.com/steve-bush/me574_dispenser/tree/master/pcb) and [BOM](https://github.com/steve-bush/me574_dispenser/tree/master/pcb) provided.
3. Drill 3/8in hole into top corner of sanitizer bottle.
4. Assemble Base Lid to Top using provided dowel pins.
5. Route USB cable through base inserting mini USB through bottom of base leaving 3 in of cable through top of base. Make sure USB cable is secured into notch on bottom of base.
6. Attach Pump to Pump Holder, making sure the suction port of the pump is at the base of the Pump Mount.
7. Install tubing into pump outlet, use twisting motion for proper installation.
8. Insert Pump and Pump mount into sanitizer bottle. Making sure that pump mount is sitting flush with the sanitizer bottle and pump outlet is facing towards hole in bottle.
9. Route wiring from pump and tubing through grommet hole in sanitizer bottle from the inside of the bottle to the outside.
10. Slide grommet over electrical cable beyond heat shrink tubing, then squeeze and insert tubing through grommet.
11. Insert grommet securely into hole of bottle.
12. Install grommet into sanitizer bottle.
13. Install sanitizer bottle into base with tubing towards right side of base when viewing from bottle side.
14. Insert LED holder into Top.
15. Mount assembled Base Lid and Top to the base using 2 of the provided screws, making sure USB cable and tubing is routed through slot in base and to not overtighten screws.
16. Flip assembly onto top.
17. Route tubing to far side of Top ensuring enough tube is allowed to reach nozzle.
18. Plug USB cord into Arduino Nano on PCB Board.
19. Insert PCB board into Top above tubing and secure with 2 of the provided screws making sure that the red/green Led is secured into LED holder and tubing and wiring is not pinched.
20. Install top cover making sure tubing is aligned with nozzle.
21. Plug in motor to motor wire from PCB board with correct polarity.
22. Install sanitizer bottle lid onto bottle.
23. Install mid cover onto front of sanitizer.
24. Plug in USB to 5V 1A power source or Computer USB port.

## Software

There are 2 different scripts than can be chosen from to enable the device to block login. The first script is `dispenser_ping_secure.ps1` which is the recommended script to use. It will block login whether the dispenser is plugged in or not. The other script named, `dispenser_ping.ps1`, will stop working when the device is unpluggled which may be desired for people without much computer knowledge.

When setting up the script to be run on login, the company's administrative software should be used and set up to run for non-admin or non-IT users. I give steps below to setting it up with Windows Group Policy and also with Task Scheduler. These would be available to anyone and the steps should be similar for whatever other administrative software used.

To setup using Group Policy:
1. Download files from GitHub.
2. Open mmc from the Windows search bar
3. Select File > Add/Remove Snap-in..
4. Select the Group Policy Object from the snap-in list
5. Click Add >
6. Select Browse.. then Users on the top options
7. Select Non-Administrators then OK
8. Click OK and double-click on the policy object
9. Select User Configuration > Windows Settings > Scripts (Logon/Logoff) > Logon
10. Select add and move the .ps1 file into the default browsing location (it will be hidden)
11. Now go back to the add screen and select the .ps1 file
12. Close and save the mmc screen

To setup with Task Scheduler:
1. Open Task Scheduler from the Windows search bar
2. Select Create Task...
3. Set the name to whatever you like, the group that is desired, and configuration to Windows 10.
4. Under the Triggers tab, select New and then set task to run on login
5. Under the Actions tab, select New and set the task to run either Powershell script
6. Close and save the new task.

Lastly, the Arduino [code](https://github.com/steve-bush/me574_dispenser/tree/master/dispenser) will need to be uploaded to either an Arduino Nano or Mega.
