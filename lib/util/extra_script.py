
Import("env", "projenv")
import os
import fnmatch


#print("ENTER MEMFAULT library extra script")
#print(env.Dump())

if os.path.isdir(env['PIOENV']):
    incdir = env['PIOENV']
else:
    incdir = "."

# only if we need to look in current dir for includes
#env['_CPPINCFLAGS'] = "-I%s $( ${_concat(INCPREFIX, CPPPATH, INCSUFFIX, __env, RDirs, TARGET, SOURCE, affect_signature=False)} $)" % (incdir)


# update the CPP Path, remove unwanted Adafruit paths
def update_CPPPATH():

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
    #env['CPPPATH'] = NRF_INCS
    env['CPPPATH'] = ADAFRUIT_INCS

    #print("testme CPPPATH")
    #for item in env.get('CPPPATH', []):
    #    print(item)
     

def filterCPPPath_UTIL(env, node):
    #print("NODE NAME")
    #print (node.name)
    return env.Object(
        node,
        CPPPATH=env['CPPPATH']
    )

env.AddBuildMiddleware(filterCPPPath_UTIL)

update_CPPPATH()
