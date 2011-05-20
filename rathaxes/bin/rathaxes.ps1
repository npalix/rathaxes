 ##Script d'execution du compilateur Rathaxes
Write-Host "Rathaxes compilation"

$Env:OS="Windows"
$INSTALLPATH="D:\projets\SvnRathaxes\language\tags\rtx-prototype\"
$GENERATIONPATH="$INSTALLPATH\scripts\generation\"
$MISCPATH="$INSTALLPATH\scripts\misc\"
$PARSINGPATH="$INSTALLPATH\scripts\parsing\"
$CODEWORKEREXTENSION="$INSTALLPATH\scripts\CodeWorkerExtension\"
$SEMANTICSPATH="$INSTALLPATH\scripts\semantics\"
$COREPATH="$INSTALLPATH\scripts\core\"
$Env:CNORM_PATH="$INSTALLPATH\scripts\cnorm\"
$CNORM2PATH="$INSTALLPATH\scripts\cnorm\cnorm2c\"
$BLACKLIBRARYPATH="$INSTALLPATH\black_library\"

##$Env:CNORM_PATH="$INSTALLPATH\scripts\cnorm\"


if ($args.count -eq 0)
{
  Write-Host "Usage : Rathaxes *.rtx"
  exit 0
}

switch ($args[1])
{
	"clean" {
		rm -rf *.c
		rm -rf *.rtree
		}	
	default {		
		codeworker -nologo -stack 1000 -I $GENERATIONPATH -I $MISCPATH -I $PARSINGPATH -I $CODEWORKEREXTENSION -I $SEMANTICSPATH -I $COREPATH -I $Env:CNORM_PATH -I $CNORM2PATH -I $BLACKLIBRARYPATH -script $INSTALLPATH\scripts\rathaxes.cws -args $Env:OS $INSTALLPATH  $Env:OS $args
		}
}
