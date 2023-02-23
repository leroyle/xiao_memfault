Import("env")
import subprocess as sp

# fetch the GNU compiler generated build id from the elf file and save it

def get_build_id(source, target, env):

   print("************** Post build script saving GNU Build Id *************")

   cmd='readelf -n '
   program_path = target[0].get_abspath()
   filter=' | grep \"Build ID\"'
   bldIdCmd= cmd + program_path + filter

   #execute the fetch
   buildId = sp.getoutput(bldIdCmd)

   print("GNU Compiler generated Build ID: " + buildId)

   saveCmd = 'echo ' 
   program_dir = target[0].get_dir()
   redir = ' > ' 
   outfilePath = str(program_dir) +  '/GNU_BUILD_ID'
   saveBldIdCmd = saveCmd + buildId + redir + outfilePath

   # execute the save
   data = sp.getoutput(saveBldIdCmd)
   

env.AddPostAction("$PROGPATH", get_build_id)
