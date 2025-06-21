import ctypes
import os.path as p
import pathlib as xz
import os
import shutil as i
def Main():
    if(p.exists("{}".format(os.getcwd() + "\\TowerOfFantasyInternalVersion-BypassAC.dll"))):
        i.copy2("{}".format(os.getcwd() + "\\TowerOfFantasyInternalVersion-BypassAC.dll"), "C:\\Program Files\\TowerOfFantasy_Global\\HottaLaunch\\version.dll")
        print("Successfully Copied to Tower Of Fantasy(Global Version) Folder Launcher!!!")
        os._exit(433)
    else:
        print("Not Founded TowerOfFantasyInternalVersion-BypassAC.dll... IF YOU DELETED THIS, PLS RESTORE THIS OR COPY AGAIN!!!")
        os._exit(433)

if __name__ == "__main__":
    Main()