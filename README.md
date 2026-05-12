# Radar_software
By: Petri Miettinen
Original date: 12/05/26

This is the radar software project made for 3rd Year Fontys Eectrical Engineering internship
as part of Distributed Sensor Systems Plasma Boundary project research.

For importing and running the code, refer to AWR2944EVM Out of Box Manual by Petri Miettinen, accessible through DSS Teams channel.
This manual was written to be used for the base Out of Box code, but will also effectively work with enabling these project files as well.

Notice: Due to the code still utilizing much of code written by Texas Instruments, external distribution of this code is likely not adviced.
But, the program should be fine to use for DSS internal usage.


This code was put together by importing the Out of Box demo code for AWR2944EVM radar board into CCS 20
This was used to import the "project structure", before the source code was replaced from the non-project code file contained
in the MCUPlus 4.7 SDK.

Originally the SDK Code was only used as a way to Debug the code when using the premade Binaries and 
Appdata for the mmWDemo_DDM/DDM and was not intended to be a functional set of code.

However, effectively pasting this debug code on top of the Radar Toolbox code made it accessible and usable.
Following this, the code functioned as a version of the mmWDemo_DDM code.

Following this, by changing the include and define settings within the CCS itself, the TDM Functionality of the code could be enabled.
Afterwards, the code's linker file was manipulated to better facilitate the memory management, as TDM processing chain required greater deal of memory compared to
DDM.

The radar code now compiles and effectively works as a customizable version of the mmWDemo_TDM out of box code.

ENET Functionality has been experimented with and can be enabled and used by uncommenting the initEnetTsk in the main code.
However, at the time this does not work effectively until the memory of the system can be further played around with.

Another major change is, much of the "original" code of the OOB Demo was relocated into RadarSetup.c file. 
With this the idea is that for customizing and personal code, the mss_main.c is still used athe primary location for the code, but with much less bloat
from the original code.

Thanks to the constructor based approach to their task interrupts, basic functions can be implemented separately in the main code. 
But some functions may require altering the RadarSetup.c code, depending on functional requirements.


