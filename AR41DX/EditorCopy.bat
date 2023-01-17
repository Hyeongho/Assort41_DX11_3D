cd..
cd..
xcopy .\AR41Editor\Include\GameObject\*.* .\Client2D\Include\GameObject\ /d /s /y /exclude:EditorExclude.txt
xcopy .\AR41Editor\Include\Scene\*.* .\Client2D\Include\Scene\ /d /s /y /exclude:EditorExclude.txt
xcopy .\AR41Editor\Include\UI\*.* .\Client2D\Include\UI\ /d /s /y /exclude:EditorExclude.txt
xcopy .\AR41Editor\Include\Setting\*.* .\Client2D\Include\Setting\ /d /s /y /exclude:EditorExclude.txt

xcopy .\AR41Editor\Bin\*.* .\Client2D\Bin\ /d /s /y /exclude:Exclude.txt