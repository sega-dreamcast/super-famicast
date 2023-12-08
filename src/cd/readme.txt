_/\/\/\/ Super Famicast V1 \/\/\/\_
http://www.sch3rz0.com/superfamicast/

A Super Nintendo Entertainment System Emulator for the Sega Dreamcast
ported and coded by Scherzo using GCC and the KOS Development Library.
Super Famicast is a port of Snes9x, the great SNES emulator for PC. The
current version of snes9x code used is v1.42.

-=< Feature Summary >=-

* Many games emulate at fullspeed with frame skipping.
* Load ROMs from CD; swapping supported
* Sound (not synced or fast)
* Save SRAM to VMU
* Adjustable screen
* Theme system including background music
* Save emulator settings to VMU

-=< Usage >=-

If you obtained the plain files, use your method of choice to create a
self-booting CD. You can find directions to do so at the DCEmulation
message board (http://www.dcemulation.com/phpBB/). ROMs can be included
on the same CD as Super Famicast, and they can be any number of directories
deep, limited by CD format restrictions.

If you downloaded a CD image instead, you are pretty much forced to load
your ROMs from a seperate CD.

-=< Game Control >=-

SNES input maps exactly to DC input in placement, not by button names. Here
is the mapping (perhaps configurable in the future):

DC					SNES	NOTES
-------------------------------------------------------------------------------
A					B
B					A
X					Y
Y					X
L Trigger			L
R Trigger			R
D-Pad or Analog		D-Pad	Depends on configuration

-=< Menu Control >=-

All menus, even the ROM browser, are controlled the same way in Super Famicast:

Directional Pad
	Up - Previous menu item
	Down - Next menu item
	Left - Previous choice item
	Right - Next choice item

Left Trigger - Variable speed through previous menu items
Right Trigger - Variable speed through next menu items
A Button - Selects menu item
B Button = Cancels menu

During gameplay, the main menu can be reached by simultaneously pressing
Analog-Left and START. The in-game menu is Analog-Left and Y. If you have
the controllers configured to use the analog stick for SNES input, then
use D-Pad-Left instead.

-=< Select Game >=-

Starts browsing for a ROM from the root of the CD or from where the last
location browsed. Directories are displayed in a different color (defined by
the theme) than that of files. All files are shown and are allowed to be
chosen. Loading something other than a known ROM can possibly cause the
emulator to crash. So please don't do it.

You can swap the Super Famicast CD for another CD that has your ROMs.Do the
swap before entering the ROM browser. Removing the CD stops the theme's music
if it has it.

-=< Options >=-

Bilinear Filtering:
	Enabling this feature will make the emulated image appear smoother and
	less pixelated. It does not affect performance.

Frameskip:
	Auto - Tells the emulator to skip frames in order to acheive full speed
	emulation. However, there is a limit to the max frames to be dropped,
	which is now 10. So if your game hits about 5/60 fps, it might not be
	running full speed. The max frames allowed to be dropped might be
	configurable in a future release. Hopefully it won't be needed due to speed
	increases.
	
	0-5 - Draw every nth frame as fast as possible. For processing rich games,
	it can often produce smoother frame skipping than auto but full speed
	emulation is less likely. Experiment with your favorite game to find
	which pleases you most.

Display Frame Rate:
	Enabling this feature will display the current frame rate during emulation.
	is auto frame skipping is not enabled, then this feature will always read
	the same ratio and is not a true indicator of frame rate.

Auto Save SRAM:
	If this feature is enabled, games that support SRAM will be checked every
	30 seconds to see if changes have been made to SRAM. If so, then they are
	saved to VMU automatically during gameplay. A slight pause in game play
	occurs do to this.

Enable Sound:
	Enables the SNES APU and produces sound output. This WILL slow down your
	game to just about an unplayable state AND it is out of sync. Future
	optimizations will hopefully free up enough CPU time to make sound a
	positive feature. Also note that if you enable of disable sound while
	a gmae is currently loaded, you will be forced to reset emulation.

Adjust Screen:
	This allows you to adjust to bounds of the area drawn to the screen to
	account for the fact the most TVs clip parts of the image on all sides.
	The D-Pad pans the image while the analog resizes it.

-=< Configure Controllers >=-

This currently allows you to configure whether the analog stick or D-Pad are
used for SNES D-Pad input for each of the 4 players.

-=< Save Settings >=-

Saves all option settings, screen position, and currently selected theme to
VMU. These settings are loaded automatically the next time you run Super
Famicast.

NOTE: Currently, all saves to VMU within Super Famicast are done to the first
VMU found. Later versions will have more control of VMU saving and loading.

-=< Change Theme >=-

This allows you to switch the active theme without restarting Super Famicast.
You can store as many themes as you like under the themes directory.

-=< In-Game Menu >=-

This menu provides some quick and useful functions.

Save SRAM:
	Explicitly save SRAM to VMU regardless of the auto-save feature.

Reset Emulation:
	Perform a soft-reset of emulation. This is equal to pressing reset on
	a real SNES. All SRAM will remain intact.

Adjust Screen:
	Identical to the Adjust Screen found under the options menu except that
	the current game frame is used as the background. White lines are drawn
	on the edges of the frame in case black borders the edges.

-=< Credits >=-
Snes9x Team - for over 95% of the source used in Super Famicast
mastakilla - for his Bowser theme
Mikey - for the clean theme
Rigor Mortis - for the first theme music

-=< Thanks >=-
First, thanks to the authors of snes9x for their years of hard work. Thanks to
all the cool people at the DCEmu boards that have been SUPER supportive!
Without them I might have not purchased the broadband adapter that really
made development faster and less stressful.

-=< Links >=-

Super Famicast: http://www.sch3rz0.com/superfamicast/
Snes9x: http://www.snes9x.com/
KOS: http://cadcdev.sourceforge.net/
DC Emulation: http://www.dcemulation.com/

-=< Contact >=-

I frequently check the DCEmu message boards.
Email: scherzo@sch3rz0.com
AIM: ScherzoQ2

