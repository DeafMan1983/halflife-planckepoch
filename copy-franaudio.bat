rem Put a copy command in here to copy your FranAudio.dll into your Half-Life executable directory
robocopy ".\projects\vs2019\Debug\FranAudio" "D:\SteamLibrary\steamapps\common\Half-Life" FranAudio.dll /njh /njs /ndl /nc /ns /np
robocopy ".\projects\vs2019\Debug\FranAudio" "D:\SteamLibrary\steamapps\common\Half-Life" FranAudio.pdb /njh /njs /ndl /nc /ns /np
