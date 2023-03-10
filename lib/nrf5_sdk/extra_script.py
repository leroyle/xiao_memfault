
Import("env")
import os
import fnmatch


#print("ENTER NRF5_SDK library extra script")
#print(env.Dump())

if os.path.isdir(env['PIOENV']):
    incdir = env['PIOENV']
else:
    incdir = "."

# only if we need to look in current dir for includes
#env['_CPPINCFLAGS'] = "-I%s $( ${_concat(INCPREFIX, CPPPATH, INCSUFFIX, __env__, RDirs, TARGET, SOURCE, affect_signature=False)} $)" % (incdir)


# remove unwanted nrf5_sdk paths
def update_CPPPATH_NRF():

    NRF_INCS = []
    ADAFRUIT_INCS = []
    pattern = "*framework*"

    # CPPPATH contains Adafriuit include paths, separate out the Adafruit paths
    for item in env.get("CPPPATH", []):
         #print("In test me")
         #print(item)
         if fnmatch.fnmatch(item, pattern):
             #print("MATCH")
             #print(item)
             ADAFRUIT_INCS.append(item)
         else:
             NRF_INCS.append(item)
             #print("NO MATCH")
             #print(item)

    # put only NRF includes back into CPPPATH
    env['CPPPATH'] = NRF_INCS
    # env['ADAFPATHS'] = ADAFRUIT_INCS

    #print("testme CPPPATH")
    #for item in env.get('CPPPATH', []):
    #   print(item)
     
    #print("testme CFLAGS")
    #for item in env.get('CFLAGS', []):
    #   print(item)
     

# this is run for each source file
def filterCPPPath_NRF(env, node):
    update_CPPPATH_NRF()
    #print("NODE NAME")
    #print (node.name)
    return env.Object(
        node,
        CPPPATH=env['CPPPATH']
    )

env.AddBuildMiddleware(filterCPPPath_NRF)

update_CPPPATH_NRF()
