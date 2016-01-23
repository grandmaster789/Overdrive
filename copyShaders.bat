@rem %1 ~> platform target (x64, win32)
@rem %2 ~> configuration (debug, release)
@rem %3 ~> project root
@rem %4 ~> target dir

@xcopy /Y /D /I %3Overdrive\Shaders %4\Shaders