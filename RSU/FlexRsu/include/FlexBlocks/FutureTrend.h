//SharedDigital64( startTime )//момент времени
SharedDigital32( durationSeconds )//сколько секунд перекрывают точки в кол-ве nPoints
SharedDigital32( nPoints )// кол-во актуальных точек из trendPoints, но не больше _countof(trendPoints)
SharedArrayAnalog32( trendPoints, 3600 )
SharedChars( Module, 256)
SharedChars( TagInModuleOrOPC, 256)