# PlatformIO prebuild script. Will check to see if some of the build  pre-reqs 
# are satisfied. Stop the build if not
Import("env")
import os
from pathlib import Path
from platformio import fs

PROJECT_DIR=fs.normalize_path(os.getcwd())
print(PROJECT_DIR)

def check_files():
    print("***************** PRE BUILD CHECK FOR CUSTOM FILES ****************")
    FRAMEWORK_DIR = env.PioPlatform().get_package_dir("framework-arduinoadafruitnrf52")

    # Tiny USB library
    ADA_TINY_USB='libraries/Adafruit_TinyUSB_Arduino/extra_script.py'
    # SPI library
    ADA_SPI='libraries/SPI/extra_script.py'
    # Memfault files
    MEMF_FILES='lib/memfault/components'
    # Nordic SDK files
    NORD_FILES='lib/nrf5_sdk/components'

    # look for Tiny USB library script
    scriptPath=os.path.join(FRAMEWORK_DIR, ADA_TINY_USB)

    path = Path(scriptPath)
    resUsb=path.is_file()

    if resUsb == False:
        print("\nError: Custom script file not found" )
        print(scriptPath)
        print("\tPlease ensure the script file has been added and the \"library.json\" file has been updated")
     

    # look for SPI library script
    scriptPath=os.path.join(FRAMEWORK_DIR, ADA_SPI)

    path = Path(scriptPath)
    resSpi=path.is_file()
    
    if resSpi == False:
        print("\nError: Custom script file not found" )
        print(scriptPath)
        print("\tPlease ensure the script file has been added and the \"library.json\" file has been updated")


    # look for Memfault library files
    dirPath=os.path.join(PROJECT_DIR, MEMF_FILES)

    path = Path(dirPath)
    memfDir=path.is_dir()
    
    if memfDir == False:
        print("\nError: Missing required Memfault SDK files: ")
        print("\t" + dirPath + "\n")

        scriptPath=os.path.join(PROJECT_DIR, 'assets/MemFault/cpMemfltSrcFiles')

        print("\tPlease ensure you have run the script file: " +  scriptPath)
        print("\tthis should have copied the required Memfault files")


    # look for Nordic NRF SDK library files
    dirPath=os.path.join(PROJECT_DIR, NORD_FILES)

    path = Path(dirPath)
    nordDir=path.is_dir()
    
    if nordDir == False:
        print("\nError: Missing required Nordic SDK files" )
        print("\t" + dirPath + "\n")

        scriptPath=os.path.join(PROJECT_DIR, 'assets/MemFault/cpNrfSrcFiles')

        print("\tPlease ensure you have run the script file: " +  scriptPath)
        print("\tthis should have copied the required Memfault files")



    #if (not resUsb) and ( not resSpi):
    if not ( resUsb and resSpi and memfDir and nordDir):
        print("\nExiting build\n")
        exit(1)
    else:
        print("***************** PRE BUILD CHECK PASSED ****************")

check_files()
