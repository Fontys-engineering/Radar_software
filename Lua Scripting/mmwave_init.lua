----------------------------------------------------------------------
--  Lua script for setting up the DCA1000 and xwr29xx radar device. --
--                    Written by: Chris Carolus                     --
----------------------------------------------------------------------


-- Globals. May need to change SYSTEM_COM, DEVICE_IP, DEVICE_GATEWAY, BSS_PATH and MSS_PATH
-- depending on network config and install path of MMWave Studio.

COM_PORT = 20
SYSTEM_COM = 9
BAUD_RATE = 115200

STATUS = 0

CAPTURE_DEVICE = "DCA1000"
DEVICE_IP = "192.168.33.30"
DEVICE_GATEWAY = "192.168.33.180"
DEVICE_MAC = "12:34:56:78:90:12"
CONFIG_PORT = 4096
RECORD_PORT = 4098
PACKET_DELAY = 25

BSS_PATH = "C:\\ti\\mmwave_studio_03_01_03_01\\rf_eval_firmware\\radarss\\xwr29xx_radarss_rprc.bin"
MSS_PATH = "C:\\ti\\mmwave_studio_03_01_03_01\\rf_eval_firmware\\masterss\\xwr2xxx_masterss.bin"

    -- LL functions --

function GetCOM()
    local command = 'powershell -Command "[System.IO.Ports.SerialPort]::GetPortNames()"'
    local exit_code, output = RSTD.Execute(command)
    local t={}

    if exit_code == 0 then

        output = output:gsub('COM', '')
    else
        WriteToLog("Failed to retrieve COM ports. Error code: ", "red") WriteToLog(exit_code, "red")
        return -1
    end

    output:gsub("([^\n]*)\n",function(c) table.insert(t,c) end)
    return t

end

function ConnectToCOM()
    local comTable = GetCOM()

    if comTable == -1 then
        WriteToLog("\nNo COM ports found!", "red")
        return 1
    end

    local i = 1

    while i <= #comTable do
        local value = tonumber(comTable[i])
        
        if value > SYSTEM_COM then
            WriteToLog("\nattempting connection to COM", "yellow") WriteToLog(value, "yellow") 

            if ar1.Connect(value, BAUD_RATE, 1000) == 0 then
                WriteToLog("\nconnected to COM", "green") WriteToLog(value, "green")
                break
            else
                WriteToLog("\nconnection failed, resuming loop", "yellow")
                i = i+1
            end
        else
            WriteToLog("\nSystem COM port!", "red")
            i=i+1
        end

    end

    if i > #comTable then
        WriteToLog("\nNo connection established!", "red")
        return 2
    else
        return 0
    end
end


    -- HL functions --
function InitRadar()

    WriteToLog("Initializing radar...\n", "yellow")
    ar1.FullReset()
    WriteToLog("fullreset\n")
    RSTD.Sleep(2000)

    ar1.SOPControl(2)
    WriteToLog("sop mode\n")
    RSTD.Sleep(1000) 
    
    WriteToLog("Connection to COM ports\n")
    if ConnectToCOM() ~= 0 then
        WriteToLog("COM Connection Failed\n", "red")
    end
    RSTD.Sleep(2000)

    if ar1.Calling_IsConnected() == true then
        WriteToLog("Radar connected successfully.\n", "green")
    else
        WriteToLog("Radar connection failed.\n", "red")
        return 1
    end
        
    return 0
end

function ConnectDCA1000()
    WriteToLog("Connecting to DCA1000...", "yellow")
    
    WriteToLog("Selecting capture device\n\r")
    ar1.SelectCaptureDevice(CAPTURE_DEVICE)
    
    
    WriteToLog("Selecting ethernet config\n\r")
    ar1.CaptureCardConfig_EthInit(DEVICE_IP, DEVICE_GATEWAY, DEVICE_MAC, CONFIG_PORT, RECORD_PORT)


    WriteToLog("capture card config mode\n\r")
    ar1.CaptureCardConfig_Mode(1, 2, 1, 2, 3, 30)

    WriteToLog("packet delay\n\r")    
    ar1.CaptureCardConfig_PacketDelay(PACKET_DELAY)

    if ar1.GetCaptureCardFPGAVersion() == 0
    then
        WriteToLog("DCA1000 connected successfully.\n", "green")
    else
        WriteToLog("Failed to connect to DCA1000.\n", "red")
        return 1
    end
    
    return 0
end

function UploadFirmware()
    WriteToLog("Uploading firmware", "yellow")

    if ar1.DownloadBSSFw(BSS_PATH) ~= 0 then
        return 1
    end
    if ar1.DownloadMSSFw(MSS_PATH) ~= 0 then
        return 2
    end
    WriteToLog("firmware uploaded", "green")
    return 0

    
end

function PrepareDataCapture()

    ar1.ChanNAdcConfig(1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0)
    ar1.LPModConfig(0, 0)
    ar1.RfInit()
    ar1.DataPathConfig(513, 1216644097, 0)
    ar1.LVDSLaneConfig(0, 1, 0, 0, 0, 1, 0, 0)

    WriteToLog("static config completed.\n")
    ar1.ProfileConfig(0, 77, 100, 6, 60, 0, 0, 0, 0, 0, 0, 0, 0, 29.982, 0, 256, 10000, 2216755200, 0, 30, 0, 0, 0)
    ar1.ChirpConfig(0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0)
    ar1.DisableTestSource(0)
    ar1.FrameConfig(0, 0, 8, 128, 40, 0, 1)

    WriteToLog("Sensor config completed\n")

    return 0

end

    --------------------
    --script execution--
    --------------------
WriteToLog("Starting mmWave Studio automation script...\n")

if ConnectDCA1000() == 0 then
    WriteToLog("connection made, proceeding\n")
    STATUS = 0
else 
    WriteToLog("No connection made\n")
    STATUS = 1
end

if STATUS == 0 then
    if InitRadar() == 0 then
        WriteToLog("radar initialised\n")
        STATUS = 0
    else
        STATUS = 1
    end
end

if STATUS == 0 then
    if UploadFirmware() == 0 then
        WriteToLog("firmware initialised\n")
        STATUS = 0
    else
        STATUS = 1
    end
end

if STATUS == 0 then
    ar1.PowerOn(0, 100, 0, 0)
    ar1.RfEnable()
end

if STATUS ~= 0 then
    WriteToLog("mmWave failed\n")
end

WriteToLog("mmWave Studio setup finished.\n")

if STATUS == 0 then
    if PrepareDataCapture() == 0 then
        WriteToLog("Sensor setup finished\n", "green")
        STATUS = 0
    else
        WriteToLog("Sensor setup failed\n", "red")
        STATUS = 1
    end
end
