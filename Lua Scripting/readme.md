# Documentation – MMWave Studio Lua

## General Information and usage

Before Booting up MMWave Studio as administrator, you must account for the fact that the actual radar boards have a certain power-up sequence. The (red) DCA1000 must to be connected first, then the (green) xWR2944EUM and finally the USB cable and ethernet cable can be inserted into a PC. Scripts are executed in MMWave’s Lua Shell (load -> load commands file). If the Lua shell is not visible, view -> Lua shell. If the output is not visible, view -> output.

nLinChirpData.csv is an example lookup table generated with the matlab script in \\MATLAB.

## Configuration

Ensure that your PC has been properly configured and has a static IP address `192.168.33.30` and subway mask `255.255.255.0` on the unidentified ethernet network device in (Windows) Control Panel -> Network and Internet -> Network Connections -> Change advanced adaptor settings. If using a different OS, take the appropriate steps for your OS.

---
## Functionality
Note that only the complex functions are documented in detail in this file, since all other commands are documented in MMWave's Lua shell, for instance, all functions in the `ar1` package are documented and accessible using `help ar1` in the Lua shell.
### mmwave_init.lua

This script initializes the radar and connects the radar boards to a computer.

The DCA1000 is connected via ethernet on a specific IP address and port number that may be assigned by the user. The radar is initialized by first resetting the device, then connecting it to a COM port. When the connection is successful, the firmware is uploaded, the radar is powered up and default settings are loaded to the device.
If anything goes wrong, an appropriate error message is printed to the Output console in MMWave, using `WriteToLog(message, colour)`. This is checked based on a `STATUS` variable. If any function does not return 0, then the status will become 1 and all functions afterwards will be terminated.

#### GetCOM()
This function will get all available COM ports and returns them in an array for further processing.

A list of all available COM ports is fetched using the Windows PowerShell command `powershell -Command "[System.IO.Ports.SerialPort]::GetPortNames()"`. Upon successful execution of this command, the list is edited using `gsub` to remove the "COM" substring from the entire table. Afterwards, a regex is used to only insert the actual numbers into an array, and the function returns this array. On failure, the function will return -1.

#### ConnectToCOM()
The result from `GetCOM()` is saved in a table, and this table is used to enter a loop. If the `GetCom()` function returns -1, immediately terminate the function and return 1.

Generally, picking any port below 10 (written in the software as a global constant `SYSTEM_COM`) will crash MMWave, and is used for system peripherals. These ports are therefore skipped in the loop. When the COM port is not a system port, a connection will attempt to be established. If a connection is established, the loop will end and the function will return 0. If no connection can be established, the function will return 2.

#### InitRadar()
This high-level function runs the standard setup protocol of the radar: Reset device -> set SOP mode -> connect to COM (this uses `ConnectToCOM()`) -> wait for 2000ms -> Check for connection.

#### ConnectDCA1000()
This high-level function runs the standard setup protocol of the DCA1000: Select capture device -> Configure capture device -> Verify connection

#### UploadFirmware()
This high-level function uploads the firmware to the radar from the file path provided by the user in the global variables `BSS_PATH` and `MSS_PATH`. 

#### PrepareDataCapture()
This high-level function loads a standard chirp configuration to the device.


### nonlinear_chirp.lua

This Lua scipt imports a .csv lookup table containing a start frequency [GHz], slope frequency [GHz/us] and step time [us] per linear segment, to simulate a non-linear chirp with linear segments, as per thee limitation of MMWave Studio. The lookup table represents a linearized mathematical function, divided in piecewise segments and is generated using the MATLAB script located in \\MATLAB.

#### LoadLookup()
This low-level function imports the lookup table and converts it to an usable array for further processing in the code.

When the lookup table is imported, the entire file is split into separate lines, which are saved in `lines[]`. Since the lines are interpreted as string literals, a regex is used to save each part before a `,` as a substring, and all numbers are converted to actual numbers.
The file is closed, and all numbers in the lines are filtered to their correct array depending on the lookup table structure described in the software. These arrays are all saved as a 2D array `chirpTable[][]` which is returned.

#### ProfileConfiguration()
This high-level function is responsible for implementing the provided lookup table into MMWave using its built-in functions and converting it to a pseudo-non-linear chirp, consisting of several linear chirp segments that approximate a non-linear chirp. 
After `LoadLookup()` is called and has returned an array, it steps into a loop that reads the array and configures a chirp that corresponds to the values saved in the array. After this has finished, the radar frame is configured to use these chirp profiles and the script finishes its execution.

