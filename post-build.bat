cd 

xcopy "assets" "build/bin/assets" /h /i /c /k /e /r /y
IF %ERRORLEVEL% NEQ 0 (echo Error: %ERRORLEVEL% && exit)
popd
echo "Done."
PAUSE