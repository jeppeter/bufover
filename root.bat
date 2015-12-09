makecert.exe -n "CN=hesfund" -r -pe -a sha512 -len 4096 -cy authority -sv root.pvk root.cer
pvk2pfx.exe -pvk root.pvk -spc root.cer -pfx root.pfx 