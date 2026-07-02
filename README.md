# Radar_software
By: Petri Miettinen
Original date: 12/05/26

This is the radar software project made for 3rd Year Fontys Eectrical Engineering internship
as part of Distributed Sensor Systems Plasma Boundary project research.

Refer to DSS Radar Program Manual for exact steps in setting up the project.

This is an alternate branch of the DSS Radar Program which already includes all the relevant SDK dependencies required for running the mmWave Program.
Warning: Due to size limitations, not all of the original files are present in this repository, and errors could occur as new functionality is enabled. 
In this case, the full dependency folders may need to be downloaded so missing components can be compared and re-added into the Dependency folder of this repo.

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


