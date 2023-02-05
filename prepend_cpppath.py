
import os
Import("env")
import fnmatch

print("Enter MAIN extra script")
#print(env.Dump())

if os.path.isdir(env['PIOENV']):
    incdir = env['PIOENV']
else:
    incdir = "."


def filterCPPPath(env, node):
    print("NODE NAME")
    print (node.name)

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
    env['CPPPATH'] = ADAFRUIT_INCS
    # env['ADAFPATHS'] = ADAFRUIT_INCS

    print("testme CPPPATH")
    for item in env.get('CPPPATH', []):
       print(item)

    return env.Object(
        node,
        CPPPATH=env['CPPPATH'],
        CCFLAGS=env['CCFLAGS'] + ["-fno-builtin-printf"]
    )

env.AddBuildMiddleware(filterCPPPath)

