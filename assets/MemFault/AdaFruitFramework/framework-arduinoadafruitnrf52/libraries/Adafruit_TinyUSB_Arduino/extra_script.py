
Import("env", "projenv")
import os
import fnmatch


print("ENTER TINY USB library extra script")
#print(env.Dump())

if os.path.isdir(env['PIOENV']):
    incdir = env['PIOENV']
else:
    incdir = "."

# only if we need to look in current dir for includes
#env['_CPPINCFLAGS'] = "-I%s $( ${_concat(INCPREFIX, CPPPATH, INCSUFFIX, __env, RDirs, TARGET, SOURCE, affect_signature=False)} $)" % (incdir)


# update the CPP Path, remove unwanted Adafruit paths
def update_CPPPATH_TINY():

    NRF_INCS = []
    ADAFRUIT_INCS = []
    pattern = "*lib/nrf5_sdk/*"

    # CPPPATH contains Adafriuit include paths, separate out the Adafruit paths
    for item in env.get("CPPPATH", []):
         if fnmatch.fnmatch(item, pattern):
             #print("MATCH")
             #print(item)
             #ADAFRUIT_INCS.append(item)
             NRF_INCS.append(item)
         else:
             #print("NO MATCH")
             #print(item)
             ADAFRUIT_INCS.append(item)
             #NRF_INCS.append(item)


    # put only NRF includes back into CPPPATH
    # env['CPPPATH'] = NRF_INCS
    env['CPPPATH'] = ADAFRUIT_INCS

    #print("TINY testme CPPPATH")
    #for item in env.get('CPPPATH', []):
    #    print(item)

    #print("TINY testme projenv CPPPATH")
    #for item in projenv.get('CPPPATH', []):
    #    print(item)
     

def filterCPPPath_TINY(env, node):
    #print("TINY NODE NAME")
    #print (node.name)
    update_CPPPATH_TINY()
    #print("TINY callback testme CPPPATH")
    #for item in env.get('CPPPATH', []):
    #    print(item)

    return env.Object(
        node,
        CPPPATH=env['CPPPATH']
    )

env.AddBuildMiddleware(filterCPPPath_TINY)

update_CPPPATH_TINY()
