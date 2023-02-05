
Import("env")
import os
import fnmatch

print("ENTER MEMFAULT script")
# print(env.Dump())
if os.path.isdir(env['PIOENV']):
    incdir = env['PIOENV']
else:
    incdir = "."

env['_CPPINCFLAGS'] = "-I%s $( ${_concat(INCPREFIX, CPPPATH, INCSUFFIX, __env__, RDirs, TARGET, SOURCE)} $)" % (incdir)

tcopy = env.get("CPPPATH").copy()
env['SAVE_CPPPATH'] = tcopy


print("LAL MEMFAULT CPPINCFLAGS BEFORE")
print(env['_CPPINCFLAGS'])
print("LAL MEMFAULT pre-adjust CPPPATH")

for item in env.get("CPPPATH", []):
    print(item)
#print("LAL MEMFAULT TEST CPPPATH")
NEW_CPPPATH = []
NEW_NRFINCS = []
pattern = "*framework*"

for item in env.get("CPPPATH", []):
    
    if fnmatch.fnmatch(item, pattern):
        NEW_NRFINCS.append(item)
    else:
        NEW_CPPPATH.append(item)

env['CPPPATH'] = NEW_CPPPATH


#print("LAL MEMFAULT TEST CPPDEFINES")
for item in env.get("CPPDEFINES", []):
    if item == "MEMFAULT_SDK_INCS":
        # env['_CPPINCFLAGS'] = "-I%s $( ${_concat(INCPREFIX, INCSUFFIX, __env__, RDirs, TARGET, SOURCE)} $)" % (incdir)
        env['_CPPINCFLAGS'] = "-I%s $( ${_concat(INCPREFIX, CPPPATH, INCSUFFIX, __env__, RDirs, TARGET, SOURCE)} $)" % (incdir)
        break

print("LAL MEMFAULT Adjusted CPPPATH")
for item in env.get("CPPPATH", []):
    print(item)


print("LAL MEMFAULT SAVE_CPPPATH")
for item in env.get("SAVE_CPPPATH", []):
    print(item)

#print("LAL MEMFAULT TCOPY")
#for item in env.get("tcopy", []):
#    print(item)

print("LAL MEMFAULT CPPINCFLAGS")
print(env['_CPPINCFLAGS'])

