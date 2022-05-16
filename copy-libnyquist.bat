rem Put a copy command in here to copy your FranAudio.dll into your Half-Life executable directory
robocopy ".\projects\vs2019\Debug\libnyquist" "F:\SteamLibrary\steamapps\common\Half-Life" libnyquist.dll /njh /njs /ndl /nc /ns /np
robocopy ".\projects\vs2019\Debug\libnyquist" "F:\SteamLibrary\steamapps\common\Half-Life" libnyquist.pdb /njh /njs /ndl /nc /ns /np
