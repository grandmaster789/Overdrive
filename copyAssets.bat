@rem %1 ~> platform target (x64, win32)
@rem %2 ~> configuration (debug, release)
@rem %3 ~> project root
@rem %4 ~> target dir

@xcopy /Y /D /S /I %3assets %4\assets