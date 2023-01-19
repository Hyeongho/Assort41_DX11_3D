cd..
cd..
xcopy .\AR41Editor\Include\GameObject\*.* .\Client3D\Include\GameObject\ /d /s /y /exclude:EditorExclude.txt
xcopy .\AR41Editor\Include\Scene\*.* .\Client3D\Include\Scene\ /d /s /y /exclude:EditorExclude.txt
xcopy .\AR41Editor\Include\UI\*.* .\Client3D\Include\UI\ /d /s /y /exclude:EditorExclude.txt
xcopy .\AR41Editor\Include\Setting\*.* .\Client3D\Include\Setting\ /d /s /y /exclude:EditorExclude.txt

xcopy .\AR41Editor\Bin\*.* .\Client3D\Bin\ /d /s /y /exclude:Exclude.txt