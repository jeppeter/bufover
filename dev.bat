makecert.exe -n "CN=hesfunddev" -iv root.pvk -ic root.cer -a sha512 -len 4096 -b 01/01/2015 -e 01/01/2025 -sky exchange -sv dev.pvk dev.cer    
pvk2pfx.exe -pvk dev.pvk -spc dev.cer -pfx dev.pfx 