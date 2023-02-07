
import os
Import("env")
import fnmatch

print("Enter MAIN extra script")
#print("Dump env")
#print(env.Dump())

if os.path.isdir(env['PIOENV']):
    incdir = env['PIOENV']
else:
    incdir = "."

def filterCPPPathMain(env, node):
    #print("Main NODE NAME")
    #print (node.name)
    #print (env.Dump())
    if node.name is None:
       return

    NRF_INCS = []
    ADAFRUIT_INCS = []
    pattern = "*framework*"

    # CPPPATH contains Adafriuit include paths, separate out the Adafruit paths
    for item in env.get("CPPPATH", []):
         #print("In test me")
         #print(item)
         if fnmatch.fnmatch(item, pattern):
             #print("MAIN MATCH")
             #print(item)
             ADAFRUIT_INCS.append(item)
         else:
             #print("MAIN NO MATCH")
             #print(item)
             NRF_INCS.append(item)

    # put only NRF includes back into CPPPATH
    env['CPPPATH'] = ADAFRUIT_INCS
    # env['ADAFPATHS'] = ADAFRUIT_INCS

    #print("testme CPPPATH")
    #for item in env.get('CPPPATH', []):
    #   print(item)

    return env.Object(
        node,
        CPPPATH=env['CPPPATH'],
        #CCFLAGS=env['CCFLAGS'] + ["-fno-builtin-printf"]
    )

print("MAIN END")
#env.AddBuildMiddleware(filterCPPPathMain)

