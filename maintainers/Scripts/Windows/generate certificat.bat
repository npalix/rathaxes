MakeCert -r -pe -ss PrivateCertStore -n "CN=Rathaxes.org(Test)" rathaxes.cer
certmgr.exe -add Rathaxes.cer -s -r localMachine root 
certmgr.exe -add Rathaxes.cer -s -r localMachine trustedpublisher