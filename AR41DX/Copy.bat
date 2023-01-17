cd..
cd..
xcopy .\AR41Engine\Include\*.h .\Engine\Include\ /d /s /y
xcopy .\AR41Engine\Include\*.inl .\Engine\Include\ /d /s /y
xcopy .\AR41Engine\Include\*.hpp .\Engine\Include\ /d /s /y
xcopy .\AR41Engine\Bin\*.* .\Engine\Bin\ /d /s /y /exclude:Exclude.txt
xcopy .\AR41Engine\Bin\*.* .\AR41Editor\Bin\ /d /s /y /exclude:Exclude.txt
xcopy .\AR41Engine\Bin\*.* .\Client2D\Bin\ /d /s /y /exclude:Exclude.txt
xcopy .\AR41Engine\Bin\*.* .\Client3D\Bin\ /d /s /y /exclude:Exclude.txt