
import os
Import("env")
import fnmatch

print("ENTER NRF5_SDK script")

# print(env.Dump())
if os.path.isdir(env['PIOENV']):
    incdir = env['PIOENV']
else:
    incdir = "."

#env['_CPPINCFLAGS'] = "-I%s $( ${_concat(INCPREFIX, CPPPATH, INCSUFFIX, __env__, RDirs, TARGET, SOURCE)} $)" % (incdir)


print("LAL  NRF5_SDK TEST CPPPATH")
NEW_CPPPATH = []
NEW_NRFINCS = []
pattern = "*framework*"

for item in env.get("CPPPATH", []):
    
    if fnmatch.fnmatch(item, pattern):
        NEW_NRFINCS.append(item)
    else:
        NEW_CPPPATH.append(item)

env['CPPPATH'] = NEW_CPPPATH

#print("LAL NRF5_SDK TEST CPPDEFINES")
for item in env.get("CPPDEFINES", []):
    if item == "NRF5_SDK_INCS":
        # env['_CPPINCFLAGS'] = "-I%s $( ${_concat(INCPREFIX, INCSUFFIX, __env__, RDirs, TARGET, SOURCE)} $)" % (incdir)
        env['_CPPINCFLAGS'] = "-I%s $( ${_concat(INCPREFIX, CPPPATH, INCSUFFIX, __env__, RDirs, TARGET, SOURCE)} $)" % (incdir)
        break



print("LAL NRF5_SDK CPPPATH")
for item in env.get("CPPPATH", []):
    print(item)


print("LAL NRF5_SDK SAVE_CPPPATH")
for item in env.get("SAVE_CPPPATH", []):
    print(item)

